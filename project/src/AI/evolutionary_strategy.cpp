#include "AI/evolutionary_strategy.hpp"

long EvolutionaryStrategy::Genome::next_id = 0;

void EvolutionaryStrategy::operator()() {
    std::thread evolution(&EvolutionaryStrategy::evolve, this);
    while (!finish_) {
        drop_ = false;
        //t = 0;
        //auto pop = initialPop();

        while (!drop_) {}
        Genome best_cpy = best;
        Move move = generateBestMove(best_cpy, tetris_);
        move.apply(tetris_);
        if (tetris_.isFinished()) {
            finish_ = true;
            evolution.join();
        }
    }
}

void EvolutionaryStrategy::evolve() {
    t = 0;
    auto pop = initialPop();
    while (!finish_) {
        auto selected = selection(pop);
        auto next_pop = crossoverAndMutation(selected);
        evaluation(next_pop);
        pop = next_pop;
        displayState();
        t++;
    }
}

std::vector<EvolutionaryStrategy::Genome> EvolutionaryStrategy::initialPop() {
    std::vector<Genome> initial_pop(POP_SIZE);
    evaluation(initial_pop);
    return initial_pop;
}

std::vector<EvolutionaryStrategy::Genome> EvolutionaryStrategy::selection(std::vector<Genome>& pop) {
    std::vector<Genome> selected;
    selected.reserve(POP_SIZE);
    selected.push_back(best);
    while (selected.size() < SELECTED_TO_CROSS_AND_MUTATE) {
        selected.push_back(rouletteSelection(pop));
    }
    return selected;
}

std::vector<EvolutionaryStrategy::Genome> EvolutionaryStrategy::crossoverAndMutation(const std::vector<Genome> selected) {
    std::vector<Genome> next_pop(selected);
    while (next_pop.size() < POP_SIZE - 1) {
        float p = random_0_1();
        if (p < PROB_CROSSOVER) {
            auto father = rouletteSelection(selected);
            auto mother = rouletteSelection(selected);
            Genome child;
            child.max_height = (father.max_height + mother.max_height) / 2.0f;
            next_pop.push_back(child);
        } else {
            float dx = std::clamp(random_0_1() - 0.5f, -MUTATION_STRENGTH, MUTATION_STRENGTH);
            auto mutant = rouletteSelection(selected);
            mutant.max_height += dx;
        }
    }
    next_pop.push_back(best);
    return next_pop;
}


void EvolutionaryStrategy::evaluation(std::vector<Genome>& next_pop) {
    score_sum = 0.0f;
    for (auto& c : next_pop) {
        Tetris tmp(tetris_);
        for (int i = 0; i < 2; i++) {
            auto best_move = generateBestMove(c, tmp);
            best_move.apply(tmp);
        }
        float max_height = getMaxHeight(tmp);
        c.score = 20 - max_height;
        score_sum += c.score;

    }
    float prev_ps = 0.0f;
    for (auto& c : next_pop) {
        c.ps = prev_ps + c.score / score_sum;
        prev_ps = c.ps;
    }

    // find best
    best = next_pop[0];
    for (auto e : next_pop) {
        if (e.score > best.score)
            best = e;
    }
    std::cout << best.score;
}

Move EvolutionaryStrategy::generateBestMove(const Genome& genome, Tetris& tetris) {
    Move best_move;
    float best_fitness;
    for (int mx = Move::MIN_MOVE; mx <= Move::MAX_MOVE; mx++) {
        for (int rot = Move::MIN_ROT; rot <= Move::MAX_ROT; rot++) {
            Tetris tmp(tetris);
            Move move(mx, rot);
            move.apply(tmp);
            int max_height = getMaxHeight(tmp);
            float fitness = 20 + genome.max_height * max_height;
            if (fitness > best_fitness) {
                best_fitness = fitness;
                best_move = move;
            }
        }
    }
    return best_move;
}

void EvolutionaryStrategy::displayState() {
    mean_fitness_ = score_sum / POP_SIZE;
    std::cout << "Generation " << t << ": " << std::endl;
    std::cout << "\tmean fitness: " << mean_fitness_ << std::endl;
    printf("\tbest: (move_x=%d, rot=%d, fitness=%f)", best.best_move.getMoveX(), best.best_move.getRotation(), best.score);
}
