#ifndef GENETIC_TETRIS_EVOLUTIONARY_STRATEGY_HPP
#define GENETIC_TETRIS_EVOLUTIONARY_STRATEGY_HPP

#include <mutex>

#include "ai.hpp"

class EvolutionaryStrategy : public AI {
public:
    EvolutionaryStrategy(Tetris& tetris) : AI(tetris) {}

    void operator()() override;
    void drop() override { drop_mutex_.unlock(); }
    void update() override { drop_mutex_.unlock(); }
    void finish() override;

private:
    struct Genome {
        Genome() {
            id = next_id++;
            rows_cleared = random_0_1() - 0.5f;
            max_height = random_0_1() - 0.5f;
            cumulative_height = random_0_1() - 0.5f;
            relative_height = random_0_1() - 0.5f;
            holes = random_0_1() - 0.5f;
            roughness = random_0_1() - 0.5f;
        }
        static long next_id;
        long id;
        float rows_cleared;
        float max_height;
        float cumulative_height;
        float relative_height;
        float holes;
        float roughness;

        // may be moved somewhere else
        float score;
        float ps;
        Move best_move;
    };

    const std::size_t POP_SIZE = 50;
    const std::size_t SELECTED_TO_CROSS_AND_MUTATE = 30;
    const float MUTATION_STRENGTH = 0.05f;
    const float PROB_CROSSOVER = 0.9f;
    const int MOVES_TO_SIMULATE = 5;

    void evolve();
    std::vector<Genome> initialPop();
    std::vector<Genome> selection(std::vector<Genome>& pop);

    Genome rouletteSelection(const std::vector<Genome>& pop) {
        while (true) {
            float p = random_0_1();
            for (const auto& c : pop) {
                if (p < c.ps) {
                    return c;
                }
            }
        }
    }

    std::vector<Genome> crossoverAndMutation(const std::vector<Genome> selected);
    void evaluation(std::vector<Genome>& next_pop);

    Move generateBestMove(const Genome& genome, Tetris& tetris);

    int getMaxHeight(Tetris& tetris) {
        int max_height = 0;
        const auto& grid = tetris.getGrid();
        int rows = grid.size();
        int cols = grid[0].size();
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                if (grid[y][x] != Tetromino::EMPTY && y + 1 > max_height) max_height = y + 1;
            }
        }
        return max_height;
    }

    static float random_0_1() { return rand() / double(RAND_MAX); }
    void displayState();

    Genome best;
    float mean_fitness_ = 0.0f;
    float score_sum = 0.0f;
    int t = 0;

    std::thread evolution_thread;
    std::mutex drop_mutex_;
};

#endif  // GENETIC_TETRIS_EVOLUTIONARY_STRATEGY_HPP
