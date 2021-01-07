#ifndef GENETIC_TETRIS_EVOLUTIONARY_STRATEGY_HPP
#define GENETIC_TETRIS_EVOLUTIONARY_STRATEGY_HPP

#include <cassert>
#include <condition_variable>
#include <mutex>

#include "ai.hpp"
#include "genome.hpp"

namespace genetic_tetris {

class EvolutionaryStrategy : public AI {
public:
    enum class Mode {
        PLAY,
        EVOLVE,
    };

    explicit EvolutionaryStrategy(Tetris& tetris) : AI(tetris) {}

    void operator()(Mode mode);

    void drop() override {
        drop_ = true;
        drop_cond_.notify_one();
    }

    void update(EventType e) override {
        if (e == EventType::TETROMINO_DROPPED) {
            smooth_drop_ = true;
            drop();
        }
    }

    void finish() override;

    void tick();

    bool isDroppingSmoothly() const { return is_dropping_smoothly_; }

    std::string getInfo();

    Genome getBest() const {
        Genome tmp = best_;
        return tmp;
    }

    void save() {
        saveToJSON(BESTS_GAME, generation_bests_);
        EventManager::getInstance().addEvent(EventType::GENOMES_SAVED);
    }

    static Move generateBestMove(const Genome& genome, Tetris& tetris);

    void setGenerationNumber(int value) { generation_number_ = value; }

private:
    const std::size_t POP_SIZE = 50;
    const std::size_t SELECTED_TO_BREED = POP_SIZE / 2;
    const float MUTATION_RATE = 0.05f;
    const float MUTATION_STEP = 0.2f;
    const int MOVES_TO_SIMULATE = 40;

    const std::string BESTS_GAME = "res/bests_game.json";
    const std::string BESTS_GAME_DEFAULT = "res/default_bests_game.json";
    const std::string BESTS_EVOLVE = "res/bests_evolve.json";

    enum class State {
        STOP,
        START,
    } state_ = State::STOP;

    void play();
    void evolve();

    std::vector<Genome> next_generation(std::vector<Genome>& pop);
    std::vector<Genome> initialPop();
    std::vector<Genome> selection(std::vector<Genome>& pop);
    std::vector<Genome> crossoverAndMutation(const std::vector<Genome>& selected);
    void evaluation(std::vector<Genome>& next_pop);

    void mutate(Genome& genome);
    Genome breed(const std::vector<Genome>& selected);

    void saveToJSON(const std::string& file, std::vector<Genome>& genomes);
    std::vector<Genome> loadFromJSON(const std::string& file);

    Genome best_;
    std::vector<Genome> generation_bests_;
    float mean_fitness_ = 0.0f;
    float score_sum = 0.0f;
    int t = 0;

    std::mutex m_;
    std::condition_variable drop_cond_;
    bool drop_ = false;
    bool smooth_drop_ = false;
    bool is_dropping_smoothly_ = false;

    int generation_number_ = 0;
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_EVOLUTIONARY_STRATEGY_HPP
