#ifndef GENETIC_TETRIS_GENOME_HPP
#define GENETIC_TETRIS_GENOME_HPP

#include <cstdlib>

#include "ai_utils.hpp"

namespace genetic_tetris {

class Genome {
public:
    Genome() : score(0) {
        RandomNumberGenerator& generator = RandomNumberGenerator::getInstance();
        id = next_id++;
        rows_cleared = generator.random<-1, 1>();
        max_height = generator.random<-1, 1>();
        cumulative_height = generator.random<-1, 1>();
        relative_height = generator.random<-1, 1>();
        holes = generator.random<-1, 1>();
        roughness = generator.random<-1, 1>();
    }

    inline static long next_id = 0;

    long id;
    float rows_cleared;
    float max_height;
    float cumulative_height;
    float relative_height;
    float holes;
    float roughness;

    float score;
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_GENOME_HPP
