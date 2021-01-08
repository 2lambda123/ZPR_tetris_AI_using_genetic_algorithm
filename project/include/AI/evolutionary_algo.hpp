#ifndef GENETIC_TETRIS_EVOLUTIONARY_ALGO_HPP
#define GENETIC_TETRIS_EVOLUTIONARY_ALGO_HPP

#include <cassert>
#include <condition_variable>
#include <mutex>

#include "ai.hpp"
#include "genome.hpp"

namespace genetic_tetris {

/**
 * Evolutionary algorithm implementation being able to play Tetris
 *
 * Algorithm can be run in two modes:
 *  - PLAY load population from file and use specified genome to generate moves
 *  - EVOLVE evolve population to create better genomes
 */
class EvolutionaryAlgo : public AI, public Subject {
public:
    /// Specifies mode in which to run the algorithm
    enum class Mode {
        PLAY,
        EVOLVE,
    };
    /**
     * Generates best possible move taking into account tetris state and genome attributes
     * @param genome - genome used to calculate fitness function
     * @param tetris - tetris object for which function will generate the best move
     * @return best move generated for current state of tetris
     */
    static Move generateBestMove(const Genome& genome, Tetris& tetris);

    explicit EvolutionaryAlgo(Tetris& tetris) : AI(tetris) {}

    /**
     * Runs the algorithm
     * @param mode mode in which algorithm will be run
     */
    void operator()(Mode mode);

    void drop() override;
    void update(EventType e) override;
    void finish() override;
    /// Performs genetic_tetris::Tetris::tick()
    void tick();
    /// TODO
    bool isDroppingSmoothly() const;

    /**
     * Returns attributes of current generation of algorithm
     * @return string containing information like mean fitness, best score, best genome attributes
     */
    std::string getInfo() const;
    /// Returns current best genome
    Genome getBest() const;

    /// Saves genomes to file
    void save();

    /// Specifies generation number used to play against the player
    void setPlayingGeneration(int value);
    /// Returns the number of generations available in genome file
    int getAvailableGenerations() const { return generation_bests_.size(); }
    /**
     * Returns algorithm status for play() or evolve()
     * @return true if everything was ok, false e.g. number of available generations was less than playing generation
     */
    bool getSuccess() const { return success_; }

private:
    /// Specifies available states of the algorithm execution
    enum class State {
        STOP,
        START,
    };

    /// Population size
    const std::size_t POP_SIZE = 50;
    /// Rate at which genome attributes will be mutated
    const float MUTATION_RATE = 0.1f;
    /// Strength of the singular mutation
    const float MUTATION_STEP = 0.2f;
    /// Number of moves simulated in evaluation function
    const int MOVES_TO_SIMULATE = 400;

    /// File where genomes will be located
    const std::string GENOMES_FILE = "res/genomes.json";

    /// Saves given set of genomes to specified file
    static void saveToJSON(const std::string& file, std::vector<Genome>& genomes);
    /// Loads set of genomes from specified file
    static std::vector<Genome> loadFromJSON(const std::string& file);

    /// Runs algorithm in mode playing with the player
    void play();
    /// Runs algorithm in evolving mode
    void evolve();

    /// Generates next generation
    std::vector<Genome> nextGeneration(std::vector<Genome>& pop);
    /// Creates initial population
    std::vector<Genome> initialPop();
    /// Performs selection
    std::vector<Genome> selection(std::vector<Genome>& pop);
    /// Performs mutation on selected
    std::vector<Genome> mutation(std::vector<Genome>& selected);
    /// Evaluates the next population
    void evaluation(std::vector<Genome>& next_pop);

    /// Mutates one genome
    void mutate(Genome& genome);

    /// Current execution state
    State state_ = State::STOP;

    /// Execution status
    bool success_;

    /// Current best genome
    Genome best_;
    /// Best genomes from each generations (
    std::vector<Genome> generation_bests_;
    /// Mean fitness of last generation
    float mean_fitness_ = 0.0f;
    /// Generation count
    int t_ = 0;

    /// Mutex used to manage std::condition_variable
    std::mutex m_;
    /// Blocks algorithm if there is nothing for it to be done
    std::condition_variable drop_cond_;
    /// If true tells algorithm to drop current tetromino
    bool drop_;
    /// Tells whether tetromino should be dropped smoothly
    bool smooth_drop_;
    /// Tells whether algorithm is in the process of smoothly dropping a tetromino
    bool is_dropping_smoothly_;

    /// Generation playing againt the player. Specified in GUI.
    int playing_generation_;
    /// Generations available in loaded JSON file containing genomes
    int available_generations_;
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_EVOLUTIONARY_ALGO_HPP
