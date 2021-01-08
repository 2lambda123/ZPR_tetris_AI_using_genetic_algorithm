#ifndef GENETIC_TETRIS_RANDOM_NUMBER_GENERATOR_HPP
#define GENETIC_TETRIS_RANDOM_NUMBER_GENERATOR_HPP

#include <ctime>
#include <random>

namespace genetic_tetris {

/**
 * Random number generator using Mersenne Twister and std::uniform_real_distribution
 */
class RandomNumberGenerator {
public:
    static RandomNumberGenerator& getInstance();

    RandomNumberGenerator(const RandomNumberGenerator&) = delete;
    RandomNumberGenerator& operator=(const RandomNumberGenerator&) = delete;

    float random_0_1();

    /**
     * Return random value from given range
     * @tparam a min value
     * @tparam b max value
     * @return random number
     */
    template <int a, int b>
    float random() {
        static std::uniform_real_distribution<float> dis(a, b);
        return dis(generator_);
    }

private:
    RandomNumberGenerator();

    std::mt19937 generator_;
    std::uniform_real_distribution<float> dis_0_1;
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_GUI_UTILS_HPP
