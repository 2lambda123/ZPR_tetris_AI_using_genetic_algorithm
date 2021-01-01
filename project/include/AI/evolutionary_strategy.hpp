#ifndef GENETIC_TETRIS_EVOLUTIONARY_STRATEGY_HPP
#define GENETIC_TETRIS_EVOLUTIONARY_STRATEGY_HPP

#include <cassert>
#include <mutex>

#include "ai.hpp"
#include "genome.hpp"

class EvolutionaryStrategy : public AI {
public:
    EvolutionaryStrategy(Tetris& tetris) : AI(tetris) {}

    void operator()() override;
    void operator()(const std::string& input_json, const std::string & output_json);

    void controlLoop();

    void drop() override { drop_mutex_.unlock(); }
    void update(GenTetrisEvent e) override {
        if (e == GenTetrisEvent::TETROMINO_DROPPED)
            drop_mutex_.unlock();
    }
    void finish() override;
    void saveToJSON(const std::string& file, std::vector<Genome>& genomes);
    std::vector<Genome> loadFromJSON(const std::string& file);

private:
    const std::size_t POP_SIZE = 40;
    const std::size_t SELECTED_TO_CROSS_AND_MUTATE = 30;
    const float MUTATION_STRENGTH = 0.05f;
    const float PROB_CROSSOVER = 0.9f;
    const int MOVES_TO_SIMULATE = 40;

    enum class State {
        STOP,
        START,
    } state_ = State::STOP;

    void evolve();
    void evolve(const std::string& input_json, const std::string& output_json);
    std::vector<Genome> next_generation(std::vector<Genome>& pop);
    std::vector<Genome> initialPop();
    std::vector<Genome> selection(std::vector<Genome>& pop);

    Genome rouletteSelection(const std::vector<Genome>& pop) {
        while (true) {
            float p = random_0_1();
            for (const auto& c : pop) {
                assert(c.ps >= 0.0f);
                if (p < c.ps) {
                    return c;
                }
            }
        }
    }

    std::vector<Genome> crossoverAndMutation(const std::vector<Genome> selected);
    void evaluation(std::vector<Genome>& next_pop);

    Move generateBestMove(const Genome& genome, Tetris& tetris);

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
