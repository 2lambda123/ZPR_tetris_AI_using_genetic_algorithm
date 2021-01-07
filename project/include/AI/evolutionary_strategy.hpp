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

    static Move generateBestMove(const Genome& genome, Tetris& tetris);

    explicit EvolutionaryStrategy(Tetris& tetris) : AI(tetris) {}

    void operator()(Mode mode);

    void drop() override;
    void update(EventType e) override;
    void finish() override;
    void tick();

    bool isDroppingSmoothly() const;
    std::string getInfo() const;
    Genome getBest() const;

    void save();

    void setGenerationNumber(int value);

private:
    enum class State {
        STOP,
        START,
    };

    const std::size_t POP_SIZE = 50;
    const std::size_t SELECTED_TO_BREED = POP_SIZE / 2;
    const float MUTATION_RATE = 0.05f;
    const float MUTATION_STEP = 0.2f;
    const int MOVES_TO_SIMULATE = 40;

    const std::string BESTS_GAME = "res/bests_game.json";
    const std::string BESTS_GAME_DEFAULT = "res/default_bests_game.json";
    const std::string BESTS_EVOLVE = "res/bests_evolve.json";

    static void saveToJSON(const std::string& file, std::vector<Genome>& genomes);
    static std::vector<Genome> loadFromJSON(const std::string& file);

    void play();
    void evolve();

    std::vector<Genome> nextGeneration(std::vector<Genome>& pop);
    std::vector<Genome> initialPop();
    std::vector<Genome> selection(std::vector<Genome>& pop);
    std::vector<Genome> crossoverAndMutation(const std::vector<Genome>& selected);
    void evaluation(std::vector<Genome>& next_pop);

    void mutate(Genome& genome);
    Genome breed(const std::vector<Genome>& selected);

    State state_ = State::STOP;

    Genome best_;
    std::vector<Genome> generation_bests_;
    float mean_fitness_ = 0.0f;
    float score_sum_ = 0.0f;
    int t_ = 0;

    std::mutex m_;
    std::condition_variable drop_cond_;
    bool drop_ = false;
    bool smooth_drop_ = false;
    bool is_dropping_smoothly_ = false;

    int generation_number_ = 0;
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_EVOLUTIONARY_STRATEGY_HPP
