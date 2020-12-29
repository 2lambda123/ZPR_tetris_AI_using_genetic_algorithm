#ifndef GENETIC_TETRIS_CLASSIC_GENETIC_ALGO_HPP
#define GENETIC_TETRIS_CLASSIC_GENETIC_ALGO_HPP

#include "ai.hpp"
#include <algorithm>
#include <cassert>

class ClassicGeneticAlgo : public AI {
    struct Chromosome {
        Move move;
        float fitness = 0.0f;
        float ps;
    };

public:
    ClassicGeneticAlgo(Tetris& tetris): AI(tetris) {}
    void operator()() override {
        while (!finish_) {
            drop_ = false;
            t = 0;
            auto pop = initialPop();
            while (!drop_) {
                auto selected = selection(pop);
                auto next_pop = crossoverAndMutation(selected);
                evaluation(next_pop);
                pop = next_pop;
                displayState();
                t++;
            }
            std::sort(pop.begin(), pop.end(), [](const Chromosome& a, const Chromosome& b){
                return a.fitness > b.fitness;
            });
            best_move_ = pop[0].move;
            move(tetris_, best_move_);
            if (tetris_.isFinished()) {
                finish_ = true;
            }
        }
    }

private:
    const int POP_SIZE = 50;
    const int SELECTED_TO_CROSS_AND_MUTATE = 30;
    const float PROB_CROSSOVER = 0.95f;

    std::vector<Chromosome> initialPop() {
        std::vector<Chromosome> initial_pop(POP_SIZE);
        evaluation(initial_pop);
        return initial_pop;
    }

    std::vector<Chromosome> selection(std::vector<Chromosome>& pop) {
        std::vector<Chromosome> selected;
        selected.reserve(POP_SIZE);
        while (selected.size() < SELECTED_TO_CROSS_AND_MUTATE) {
            selected.push_back(rouletteSelection(pop));
        }
        return selected;
    }

    Chromosome rouletteSelection(const std::vector<Chromosome>& pop) {
        while (true) {
            float p = rand() / double(RAND_MAX);
            for (const auto& c : pop) {
                assert(c.fitness > 0.0f);
                if (p < c.ps) {
                    return c;
                }
            }
        }
    }

    std::vector<Chromosome> crossoverAndMutation(const std::vector<Chromosome> selected) {
        std::vector<Chromosome> next_pop(selected);
        while (next_pop.size() < POP_SIZE) {
            float p = rand() / double(RAND_MAX);
            if (p < PROB_CROSSOVER) {
                auto father = rouletteSelection(selected);
                auto mother = rouletteSelection(selected);
                Chromosome child;
                assert(father.move.move_x_ <= 9);
                child.move.move_x_ = father.move.move_x_;
                child.move.rotations_ = mother.move.rotations_;
                next_pop.push_back(child);
            }
            else {
                auto mutant = rouletteSelection(selected);
                p = rand() / double(RAND_MAX);
                if (p < 0.5f) {
                    mutant.move.move_x_ = std::rand() % (Tetris::GRID_WIDTH + 1) - 1;
                }
                else {
                    mutant.move.rotations_ = std::rand() % 4;
                }
                next_pop.push_back(mutant);
            }
        }
        return next_pop;
    }

    void evaluation(std::vector<Chromosome>& next_pop) {
        fitness_sum_ = 0.0f;
        float current_max_height = getMaxHeight(tetris_);
        for (auto& c : next_pop) {
            Tetris tmp(tetris_);
            move(tmp, c.move);
            float max_height = getMaxHeight(tmp);
            c.fitness = 4 - (max_height - current_max_height);
            fitness_sum_ += c.fitness;

        }
        float prev_ps = 0.0f;
        for (auto& c : next_pop) {
            c.ps = prev_ps + c.fitness / fitness_sum_;
            prev_ps = c.ps;
        }
    }

    float getMaxHeight(Tetris& tetris) {
        int max_height = 0;
        const auto& grid = tetris.getGrid();
        int rows = grid.size();
        int cols = grid[0].size();
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                if (grid[y][x] != Tetromino::EMPTY && y + 1 > max_height)
                    max_height = y + 1;
            }
        }
        return max_height;
    }

    void displayState() {
        mean_fitness_ = fitness_sum_ / POP_SIZE;
        std::cout << "Generation " << t << ": " << std::endl;
        std::cout << "\tmean fitness: " << mean_fitness_ << std::endl;
    }

    float mean_fitness_ = 0.0f;
    float fitness_sum_ = 0.0f;
    int t = 0;
};

#endif //GENETIC_TETRIS_CLASSIC_GENETIC_ALGO_HPP
