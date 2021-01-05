#ifndef GENETIC_TETRIS_EVOLUTIONARY_STRATEGY_HPP
#define GENETIC_TETRIS_EVOLUTIONARY_STRATEGY_HPP

#include <cassert>
#include <condition_variable>
#include <mutex>

#include "ai.hpp"
#include "genome.hpp"

namespace gentetris {

class EvolutionaryStrategy : public AI {
public:
    EvolutionaryStrategy(Tetris& tetris) : AI(tetris) {}

    void operator()() override;
    void operator()(const std::string& input_json, const std::string & output_json);

    void controlLoop();

    void drop() override {
        drop_ = true;
        drop_cond_.notify_one();
    }
    void update(GenTetrisEvent e) override {
        if (e == GenTetrisEvent::TETROMINO_DROPPED) {
            drop();
        }
    }
    void finish() override;
    void saveToJSON(const std::string& file, std::vector<Genome>& genomes);
    std::vector<Genome> loadFromJSON(const std::string& file);

private:
    const std::size_t POP_SIZE = 50;
    const std::size_t SELECTED_TO_BREED = POP_SIZE / 2;
    const float MUTATION_RATE = 0.05f;
    const float MUTATION_STEP = 0.2f;
    const int MOVES_TO_SIMULATE = 40;

    const std::string BESTS_FILE = "res/bests.json";

    enum class State {
        STOP,
        START,
    } state_ = State::STOP;

    void evolve();
    void evolve(const std::string& input_json, const std::string& output_json);

    std::vector<Genome> next_generation(std::vector<Genome>& pop);
    std::vector<Genome> initialPop();
    std::vector<Genome> selection(std::vector<Genome>& pop);
    std::vector<Genome> crossoverAndMutation(const std::vector<Genome> selected);
    void evaluation(std::vector<Genome>& next_pop);

    void mutate(Genome& genome);
    Genome breed(const std::vector<Genome>& selected);

    Move generateBestMove(const Genome& genome, Tetris& tetris);

    void displayState();

    Genome best_;
    std::vector<Genome> generation_bests_;
    float mean_fitness_ = 0.0f;
    float score_sum = 0.0f;
    int t = 0;

    std::thread evolution_thread_;
    std::mutex m_;
    std::condition_variable drop_cond_;
    bool drop_ = false;
};

}

#endif  // GENETIC_TETRIS_EVOLUTIONARY_STRATEGY_HPP
