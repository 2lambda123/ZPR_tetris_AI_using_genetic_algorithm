#ifndef GENETIC_TETRIS_CLASSIC_GENETIC_ALGO_HPP
#define GENETIC_TETRIS_CLASSIC_GENETIC_ALGO_HPP

#include "ai.hpp"
#include <algorithm>
#include <cassert>

class ClassicGeneticAlgo : public AI {
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
            //move(tetris_, best_move_);
            best_move_.apply(tetris_);
            if (tetris_.isFinished()) {
                finish_ = true;
            }
        }
    }

private:
    struct Chromosome {
        Move move;
        float fitness = 0.0f;
        float ps;
    };

    const int POP_SIZE = 50;
    const int SELECTED_TO_CROSS_AND_MUTATE = 30;
    const float PROB_CROSSOVER = 0.9f;

    std::vector<Chromosome> initialPop() {
        std::vector<Chromosome> initial_pop(POP_SIZE);
        evaluation(initial_pop);
        return initial_pop;
    }

    std::vector<Chromosome> selection(std::vector<Chromosome>& pop) {
        std::vector<Chromosome> selected;
        selected.reserve(POP_SIZE);
        selected.push_back(best);
        while (selected.size() < SELECTED_TO_CROSS_AND_MUTATE) {
            selected.push_back(rouletteSelection(pop));
        }
        return selected;
    }

    Chromosome rouletteSelection(const std::vector<Chromosome>& pop) {
        while (true) {
            float p = random_0_1();
            for (const auto& c : pop) {
                if (p < c.ps) {
                    return c;
                }
            }
        }
    }

    std::vector<Chromosome> crossoverAndMutation(const std::vector<Chromosome> selected) {
        std::vector<Chromosome> next_pop(selected);
        while (next_pop.size() < POP_SIZE - 1) {
            float p = random_0_1();
            if (p < PROB_CROSSOVER) {
                /*
                auto father = rouletteSelection(selected);
                auto mother = rouletteSelection(selected);
                Chromosome child;
                child.move.setMoveX(father.move.getMoveX());
                child.move.setRotation(mother.move.getRotation());
                next_pop.push_back(child);
                 */
                auto child = rouletteSelection(selected);
                next_pop.push_back(child);
            }
            else {
                auto mutant = rouletteSelection(selected);
                p = random_0_1();
                if (p < 0.5f) {
                    p = random_0_1();
                    p < 0.5f ? mutant.move.incrementMoveX() : mutant.move.decrementMoveX();
                }
                else {
                    auto val = rand() % 4;
                    mutant.move.setRotation(2);
                    /*
                    p = random_0_1();
                    p < 0.5f ? mutant.move.incrementRotation() : mutant.move.decrementRotation();
                     */
                }
                next_pop.push_back(mutant);
            }
        }
        next_pop.push_back(best);
        return next_pop;
    }

    float random_0_1() {
        return rand() / double(RAND_MAX);
    }

    void evaluation(std::vector<Chromosome>& next_pop) {
        fitness_sum_ = 0.0f;
        float current_max_height = getMaxHeight(tetris_);
        for (auto& c : next_pop) {
            Tetris tmp(tetris_);
            c.move.apply(tmp);
            float max_height = getMaxHeight(tmp);
            c.fitness = 4 - (max_height - current_max_height);
            fitness_sum_ += c.fitness;

        }
        float prev_ps = 0.0f;
        for (auto& c : next_pop) {
            c.ps = prev_ps + c.fitness / fitness_sum_;
            prev_ps = c.ps;
        }
        /*
        auto best = *std::max_element(next_pop.begin(), next_pop.end(), [](Chromosome a, Chromosome b) {
            return a.fitness > b.fitness;
        });
        std::cout << best.fitness << std::endl;
         */
        best = next_pop[0];
        for (auto e : next_pop) {
            if (e.fitness > best.fitness)
                best = e;
        }
        std::cout << best.fitness;
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
        printf("\tbest: (move_x=%d, rot=%d, fitness=%f)", best.move.getMoveX(), best.move.getRotation(), best.fitness);
    }

    Chromosome best;
    float mean_fitness_ = 0.0f;
    float fitness_sum_ = 0.0f;
    int t = 0;
};

#endif //GENETIC_TETRIS_CLASSIC_GENETIC_ALGO_HPP
