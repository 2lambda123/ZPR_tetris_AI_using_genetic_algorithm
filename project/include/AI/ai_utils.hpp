#ifndef GENETIC_TETRIS_AI_UTILS_HPP
#define GENETIC_TETRIS_AI_UTILS_HPP

#include <random>
#include <ctime>

namespace gentetris {

class RandomNumberGenerator {
public:
    static RandomNumberGenerator& getInstance();

    float random_0_1();

    template <int a, int b>
    float random() {
        static std::uniform_real_distribution<float> dis(a, b);
        return dis(generator_);
    }

private:
    RandomNumberGenerator();
    RandomNumberGenerator(const RandomNumberGenerator&) = delete;
    RandomNumberGenerator& operator=(const RandomNumberGenerator&) = delete;

    std::mt19937 generator_;
    std::uniform_real_distribution<float> dis_0_1;

};

}

#endif  // GENETIC_TETRIS_GUI_UTILS_HPP
