#include "AI/evolutionary_strategy.hpp"

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

#include <boost/format.hpp>
#include <fstream>
#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"

namespace gentetris {

void EvolutionaryStrategy::operator()(EvolutionaryStrategy::Mode mode) {
    finish_ = false;
    if (mode == Mode::EVOLVE) {
        evolve();
    } else if (mode == Mode::PLAY) {
        play();
    }
}

void EvolutionaryStrategy::play() {
    // TODO: are the assignments below necessary?
    finish_ = false;
    drop_ = false;
    smooth_drop_ = false;
    state_ = State::START;
    generation_bests_ = loadFromJSON(BESTS_GAME);
    if (generation_bests_.size() == 0)
        throw std::runtime_error(BESTS_GAME + " does not contain genomes");
    while (!finish_) {
        std::unique_lock<std::mutex> lk(m_);
        drop_cond_.wait(lk, [this]() { return (drop_ || finish_) && !is_dropping_smoothly_; });
        if (finish_) return;
        if (drop_) {
            Genome best_cpy = generation_bests_[0];
            Move move = generateBestMove(best_cpy, tetris_);
            move.apply(tetris_, !smooth_drop_);
            if (smooth_drop_) {
                is_dropping_smoothly_ = true;
            }
            drop_ = false;
            smooth_drop_ = false;
        }
        lk.unlock();
        if (tetris_.isFinished()) {
            finish();
        }
    }
}

void EvolutionaryStrategy::finish() {
    if (finish_) return;
    finish_ = true;
    if (state_ == State::START) {
        if (!tetris_.isFinished()) drop_cond_.notify_one();
    }
    state_ = State::STOP;
}

void EvolutionaryStrategy::tick() {
    if (is_dropping_smoothly_) {
        bool has_dropped = tetris_.tick();
        if (has_dropped) {
            is_dropping_smoothly_ = false;
            if (tetris_.isFinished()) {
                finish();
            }
        }
    }
}

void EvolutionaryStrategy::saveToJSON(const std::string& file, std::vector<Genome>& genomes) {
    using namespace rapidjson;
    std::cout << "Saving genomes to JSON: " << file << std::endl;
    Document d;
    Value genomes_json(kArrayType);
    for (const auto& g : genomes) {
        Value g_json(kObjectType);
        Value id(g.id);
        Value rows_cleared(g.rows_cleared);
        Value max_height(g.max_height);
        Value cumulative_height(g.cumulative_height);
        Value relative_height(g.relative_height);
        Value holes(g.holes);
        Value roughness(g.roughness);
        g_json.AddMember("id", id, d.GetAllocator());
        g_json.AddMember("rows_cleared", rows_cleared, d.GetAllocator());
        g_json.AddMember("max_height", max_height, d.GetAllocator());
        g_json.AddMember("cumulative_height", cumulative_height, d.GetAllocator());
        g_json.AddMember("relative_height", relative_height, d.GetAllocator());
        g_json.AddMember("holes", holes, d.GetAllocator());
        g_json.AddMember("roughness", roughness, d.GetAllocator());
        genomes_json.PushBack(g_json, d.GetAllocator());
    }
    std::ofstream ofs(file);
    OStreamWrapper osw(ofs);
    Writer<OStreamWrapper> writer(osw);
    genomes_json.Accept(writer);
}

std::vector<Genome> EvolutionaryStrategy::loadFromJSON(const std::string& file) {
    using namespace rapidjson;
    std::cout << "Loading genomes from JSON: " << file << std::endl;
    std::vector<Genome> pop;
    std::ifstream ifs(file);
    IStreamWrapper isw(ifs);
    Document d;
    d.ParseStream(isw);
    assert(d.IsArray());
    for (Value::ConstValueIterator itr = d.Begin(); itr != d.End(); ++itr) {
        Genome g;
        auto g_json = itr->GetObject();
        g.id = g_json["id"].GetDouble();
        g.rows_cleared = g_json["rows_cleared"].GetDouble();
        g.max_height = g_json["max_height"].GetDouble();
        g.cumulative_height = g_json["cumulative_height"].GetDouble();
        g.relative_height = g_json["relative_height"].GetDouble();
        g.holes = g_json["holes"].GetDouble();
        g.roughness = g_json["roughness"].GetDouble();
        pop.push_back(g);
    }
    return pop;
}

void EvolutionaryStrategy::evolve() {
    t = 0;
    auto pop = initialPop();
    while (!finish_) {
        pop = next_generation(pop);
    }
    saveToJSON(BESTS_EVOLVE, generation_bests_);
}

void EvolutionaryStrategy::evolve(const std::string& input_json, const std::string& output_json) {
    t = 0;
    std::vector<Genome> pop;
    if (input_json != "") {
        pop = loadFromJSON(input_json);
        evaluation(pop);
    } else
        pop = initialPop();
    while (!finish_) {
        pop = next_generation(pop);
    }
    if (output_json != "") saveToJSON(output_json, pop);
}

std::vector<Genome> EvolutionaryStrategy::next_generation(std::vector<Genome>& pop) {
    auto selected = selection(pop);
    auto next_pop = crossoverAndMutation(selected);
    evaluation(next_pop);
    std::cout << getInfo() << std::endl;
    t++;
    return next_pop;
}

std::vector<Genome> EvolutionaryStrategy::initialPop() {
    std::vector<Genome> initial_pop(POP_SIZE);
    evaluation(initial_pop);
    return initial_pop;
}

std::vector<Genome> EvolutionaryStrategy::selection(std::vector<Genome>& pop) {
    std::vector<Genome> selected;
    selected.reserve(SELECTED_TO_BREED);
    std::sort(pop.begin(), pop.end(),
              [](const Genome& a, const Genome& b) { return a.score > b.score; });
    selected.push_back(pop[0]);
    best_ = pop[0];
    generation_bests_.push_back(best_);
    for (std::size_t i = 0; i < SELECTED_TO_BREED; i++) {
        selected.push_back(pop[i]);
    }
    return selected;
}

std::vector<Genome> EvolutionaryStrategy::crossoverAndMutation(const std::vector<Genome> selected) {
    std::vector<Genome> next_pop(selected);
    std::vector<Genome> children;
    while (children.size() + SELECTED_TO_BREED < POP_SIZE - 1) {
        children.push_back(breed(selected));
    }
    for (auto& child : children) {
        mutate(child);
        next_pop.push_back(child);
    }
    assert(next_pop.size() == POP_SIZE);
    return next_pop;
}

void EvolutionaryStrategy::mutate(Genome& genome) {
    auto mutate_gene = [this](float gene) {
        if (generator_.random_0_1() < MUTATION_RATE) {
            return gene + generator_.random_0_1() * MUTATION_STEP * 2 - MUTATION_STEP;
        }
        return gene;
    };
    genome.rows_cleared = mutate_gene(genome.rows_cleared);
    genome.max_height = mutate_gene(genome.max_height);
    genome.cumulative_height = mutate_gene(genome.cumulative_height);
    genome.relative_height = mutate_gene(genome.relative_height);
    genome.holes = mutate_gene(genome.holes);
    genome.roughness = mutate_gene(genome.roughness);
}

Genome EvolutionaryStrategy::breed(const std::vector<Genome>& selected) {
    Genome child;
    std::vector<Genome> parents;
    std::sample(selected.begin(), selected.end(), std::back_inserter(parents), 2,
                std::mt19937{std::random_device{}()});
    auto gene_picker = [](float gene_father, float gene_mother) {
        float p = generator_.random<-1, 1>();
        if (p >= 0.0f) return gene_father;
        return gene_mother;
    };
    child.rows_cleared = gene_picker(parents[0].rows_cleared, parents[1].rows_cleared);
    child.max_height = gene_picker(parents[0].max_height, parents[1].max_height);
    child.cumulative_height =
        gene_picker(parents[0].cumulative_height, parents[1].cumulative_height);
    child.relative_height = gene_picker(parents[0].relative_height, parents[1].relative_height);
    child.holes = gene_picker(parents[0].holes, parents[1].holes);
    child.roughness = gene_picker(parents[0].roughness, parents[1].roughness);

    return child;
}

void EvolutionaryStrategy::evaluation(std::vector<Genome>& next_pop) {
    score_sum = 0.0f;
    for (auto& c : next_pop) {
        Tetris tmp(true);
        Move best_move;
        for (int i = 0; i < MOVES_TO_SIMULATE; i++) {
            best_move = generateBestMove(c, tmp);
            best_move.apply(tmp);
            if (tmp.isFinished()) {
                break;
            }
        }
        if (tmp.isFinished()) {
            c.score = 0.0f;
        } else {
            c.score = tmp.getScore();
        }
        assert(c.score >= 0.0f);
        score_sum += c.score;
    }
    mean_fitness_ = score_sum / POP_SIZE;
}

Move EvolutionaryStrategy::generateBestMove(const Genome& genome, Tetris& tetris) {
    Move best_move;
    float initial_best = -10000000.0f;
    float best_fitness = initial_best;
    for (int mx = Move::MIN_MOVE; mx <= Move::MAX_MOVE; mx++) {
        for (int rot = Move::MIN_ROT; rot <= Move::MAX_ROT; rot++) {
            Tetris tmp(tetris);
            Move move(mx, rot);
            move.apply(tmp);
            if (tmp.isFinished()) continue;
            float fitness = genome.max_height * move.getMaxHeight() +
                            genome.cumulative_height * move.getCumulativeHeight() +
                            genome.relative_height * move.getRelativeHeight() +
                            genome.holes * move.getHoles() + genome.roughness * move.getRoughness();
            assert(initial_best < fitness);
            if (fitness > best_fitness) {
                best_fitness = fitness;
                best_move = move;
            }
        }
    }
    return best_move;
}

std::string EvolutionaryStrategy::getInfo() {
    std::stringstream string_stream;
    string_stream << "Generation " << t << ": " << std::endl;
    string_stream << "\tmean fitness: " << mean_fitness_ << std::endl;
    string_stream
        << boost::format(
               "\tbest: "
               "{\n\t\tscore=%1%\n\t\tmax_h=%2%\n\t\trows_cleared=%3%\n\t\tcumulative_h=%4%\n\t\t"
               "relative_h=%5%\n\t\tholes=%6%\n\t\troughness=%7%)\n\t}") %
               best_.score % best_.max_height % best_.rows_cleared % best_.cumulative_height %
               best_.relative_height % best_.holes % best_.roughness;
    return string_stream.str();
}

}  // namespace gentetris
