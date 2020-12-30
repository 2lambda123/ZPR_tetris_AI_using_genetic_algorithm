#ifndef GENETIC_TETRIS_GENOME_HPP
#define GENETIC_TETRIS_GENOME_HPP

#include <cstdlib>
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

    static float random_0_1() { return rand() / double(RAND_MAX); }
};

#endif  // GENETIC_TETRIS_GENOME_HPP
