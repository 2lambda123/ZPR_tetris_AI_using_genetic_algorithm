/*
 * Author: Damian Kolaska
 */

#include "AI/evolutionary_algo.hpp"

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

#include <boost/format.hpp>
#include <fstream>
#include <sstream>

#include "exception.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"

namespace genetic_tetris {

Move EvolutionaryAlgo::generateBestMove(const Genome& genome, Tetris& tetris) {
    Move best_move;
    float initial_best = -10000000.0f;
    float best_fitness = initial_best;
    for (int mx = Move::MIN_MOVE; mx <= Move::MAX_MOVE; mx++) {
        for (int rot = Move::MIN_ROT; rot <= Move::MAX_ROT; rot++) {
            Tetris tmp(tetris);
            Move move(mx, rot);
            move.apply(tmp);
            if (tmp.isFinished()) continue;
            float fitness = genome.max_height * (float)move.getMaxHeight() +
                            genome.cumulative_height * (float)move.getCumulativeHeight() +
                            genome.relative_height * (float)move.getRelativeHeight() +
                            genome.holes * (float)move.getHoles() +
                            genome.roughness * (float)move.getRoughness() +
                            genome.rows_cleared * (float)tmp.getLastTickClearedRowsCount();
            assert(initial_best < fitness);
            if (fitness > best_fitness) {
                best_fitness = fitness;
                best_move = move;
            }
        }
    }
    return best_move;
}

void EvolutionaryAlgo::operator()(EvolutionaryAlgo::Mode mode) {
    finish_ = false;
    if (mode == Mode::EVOLVE) {
        evolve();
    } else if (mode == Mode::PLAY) {
        play();
    }
}

void EvolutionaryAlgo::drop() {
    drop_ = true;
    drop_cond_.notify_one();
}

void EvolutionaryAlgo::update(EventType e) {
    if (e == EventType::TETROMINO_DROPPED) {
        smooth_drop_ = true;
        drop();
    }
}

void EvolutionaryAlgo::finish() {
    if (finish_) {
        return;
    }
    finish_ = true;
    if (state_ == State::START) {
        if (!tetris_.isFinished()) {
            drop_cond_.notify_one();
        }
    }
    state_ = State::STOP;
}

void EvolutionaryAlgo::tick() {
    if (is_dropping_smoothly_) {
        bool has_dropped = tetris_.tick(true);
        if (has_dropped) {
            is_dropping_smoothly_ = false;
            if (tetris_.isFinished()) {
                finish();
            }
        }
    }
}

bool EvolutionaryAlgo::isDroppingSmoothly() const { return is_dropping_smoothly_; }

std::string EvolutionaryAlgo::getInfo() const {
    std::stringstream string_stream;
    string_stream << "Generation " << t_ << ": " << std::endl;
    string_stream << "\tmean fitness: " << mean_fitness_ << std::endl;
    string_stream
        << boost::format(
               "\tbest: "
               "{\n\t\tid=%8%\n\t\tscore=%1%\n\t\tmax_h=%2%\n\t\trows_cleared=%3%\n\t\tcumulative_h=%4%\n\t\t"
               "relative_h=%5%\n\t\tholes=%6%\n\t\troughness=%7%)\n\t}") %
               best_.score % best_.max_height % best_.rows_cleared % best_.cumulative_height %
               best_.relative_height % best_.holes % best_.roughness % best_.id;
    return string_stream.str();
}

Genome EvolutionaryAlgo::getBest() const { return best_; }

void EvolutionaryAlgo::save() {
    saveToJSON(GENOMES_FILE, generation_bests_);
    EventManager::getInstance().addEvent(EventType::GENOMES_SAVED);
}

void EvolutionaryAlgo::setPlayingGeneration(int value) { playing_generation_ = value; }

void EvolutionaryAlgo::saveToJSON(const std::string& file, std::vector<Genome>& genomes) {
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
        Value score(g.score);
        g_json.AddMember("id", id, d.GetAllocator());
        g_json.AddMember("rows_cleared", rows_cleared, d.GetAllocator());
        g_json.AddMember("max_height", max_height, d.GetAllocator());
        g_json.AddMember("cumulative_height", cumulative_height, d.GetAllocator());
        g_json.AddMember("relative_height", relative_height, d.GetAllocator());
        g_json.AddMember("holes", holes, d.GetAllocator());
        g_json.AddMember("roughness", roughness, d.GetAllocator());
        g_json.AddMember("score", score, d.GetAllocator());
        genomes_json.PushBack(g_json, d.GetAllocator());
    }
    std::ofstream ofs(file);
    OStreamWrapper osw(ofs);
    Writer<OStreamWrapper> writer(osw);
    genomes_json.Accept(writer);
}

std::vector<Genome> EvolutionaryAlgo::loadFromJSON(const std::string& file) {
    using namespace rapidjson;
    std::cout << "Loading genomes from JSON: " << file << std::endl;
    std::vector<Genome> pop;
    std::ifstream ifs(file);
    IStreamWrapper isw(ifs);
    Document d;
    d.ParseStream(isw);
    if (!d.IsArray()) throw GenomeFileNotFoundException();
    for (Value::ConstValueIterator itr = d.Begin(); itr != d.End(); ++itr) {
        Genome g;
        auto g_json = itr->GetObject();
        g.id = (long)g_json["id"].GetDouble();
        g.rows_cleared = (float)g_json["rows_cleared"].GetDouble();
        g.max_height = (float)g_json["max_height"].GetDouble();
        g.cumulative_height = (float)g_json["cumulative_height"].GetDouble();
        g.relative_height = (float)g_json["relative_height"].GetDouble();
        g.holes = (float)g_json["holes"].GetDouble();
        g.roughness = (float)g_json["roughness"].GetDouble();
        pop.push_back(g);
    }
    return pop;
}

void EvolutionaryAlgo::play() {
    state_ = State::START;
    finish_ = drop_ = smooth_drop_ = false;

    try {
        generation_bests_ = loadFromJSON(GENOMES_FILE);
    }
    catch (GenomeFileNotFoundException& e) {
        available_generations_ = 0;
    }
    available_generations_ = generation_bests_.size();
    if (available_generations_ <= playing_generation_) {
        notifyObservers(EventType::GAME_START_FAILED);
        EventManager::getInstance().addEvent(EventType::GENERATION_OUT_OF_BOUNDS);
        return;
    }

    while (!finish_) {
        std::unique_lock<std::mutex> lk(m_);
        drop_cond_.wait(lk, [this]() { return (drop_ && !is_dropping_smoothly_) || finish_; });
        if (finish_) {
            lk.unlock();
            return;
        }
        if (drop_) {
            Genome genome = generation_bests_[playing_generation_];
            Move move = generateBestMove(genome, tetris_);
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

void EvolutionaryAlgo::evolve() {
    t_ = 0;
    auto pop = initialPop();
    while (!finish_) {
        pop = nextGeneration(pop);
    }
}

std::vector<Genome> EvolutionaryAlgo::nextGeneration(std::vector<Genome>& pop) {
    auto selected = selection(pop);
    auto next_pop = mutation(selected);
    evaluation(next_pop);
    std::cout << getInfo() << std::endl;
    t_++;
    return next_pop;
}

std::vector<Genome> EvolutionaryAlgo::initialPop() {
    std::vector<Genome> initial_pop(POP_SIZE);
    evaluation(initial_pop);
    return initial_pop;
}

std::vector<Genome> EvolutionaryAlgo::selection(std::vector<Genome>& pop) {
    std::vector<Genome> selected;
    selected.reserve(POP_SIZE);
    best_ = *std::max_element(pop.begin(), pop.end(), [](const Genome& a, const Genome& b) {
                return a.score < b.score;
             });
    generation_bests_.push_back(best_);
    while (selected.size() < POP_SIZE - 1) {
        std::vector<Genome> fighters;
        std::sample(pop.begin(), pop.end(), std::back_inserter(fighters), 2,
                    std::mt19937{std::random_device{}()});
        if (fighters[0].score > fighters[1].score) {
            selected.push_back(fighters[0]);
        }
        else {
            selected.push_back(fighters[1]);
        }

    }
    return selected;
}

std::vector<Genome> EvolutionaryAlgo::mutation(std::vector<Genome>& selected) {
    for (auto& genome : selected) {
        mutate(genome);
    }
    selected.push_back(best_);
    assert(selected.size() == POP_SIZE);
    return selected;
}

void EvolutionaryAlgo::evaluation(std::vector<Genome>& next_pop) {
    float score_sum = 0.0f;
    for (auto& c : next_pop) {
        Tetris tmp;
        Move best_move;
        for (int i = 0; i < MOVES_TO_SIMULATE; i++) {
            if (finish_) return;
            best_move = generateBestMove(c, tmp);
            best_move.apply(tmp);
            if (tmp.isFinished()) {
                break;
            }
        }
        c.score = (float)tmp.getScore();
        score_sum += c.score;
    }
    mean_fitness_ = score_sum / POP_SIZE;
}

void EvolutionaryAlgo::mutate(Genome& genome) {
    auto mutate_gene = [this](float gene) {
        if (generator_.random_0_1() < MUTATION_RATE) {
            return gene + generator_.random<-1,1>() * MUTATION_STEP;
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

}  // namespace genetic_tetris
