#ifndef GENETIC_TETRIS_GENOME_HPP
#define GENETIC_TETRIS_GENOME_HPP

#include <cstdlib>

#include "ai_utils.hpp"

namespace genetic_tetris {

class Genome {
public:
    Genome() {
        id = next_id++;
        rows_cleared = generator_.random<-1, 1>();
        max_height = generator_.random<-1, 1>();
        cumulative_height = generator_.random<-1, 1>();
        relative_height = generator_.random<-1, 1>();
        holes = generator_.random<-1, 1>();
        roughness = generator_.random<-1, 1>();
    }
    static long next_id;
    long id;
    float rows_cleared;
    float max_height;
    float cumulative_height;
    float relative_height;
    float holes;
    float roughness;

    float score;
private:

    static RandomNumberGenerator& generator_;
};

}

#endif  // GENETIC_TETRIS_GENOME_HPP
