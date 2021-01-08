#ifndef GENETIC_TETRIS_GENOME_HPP
#define GENETIC_TETRIS_GENOME_HPP

#include <cstdlib>

#include "random_number_generator.hpp"

namespace genetic_tetris {

/**
 * Genome used by evolutionary algorithm.
 * It contains several attributes being the weights algorithm's fitness function.
 * Fitness_function is a sum of attribute_weight * attribute_value for all attributes.
 */
class Genome {
public:
    /// Constructs genome with randomly generated attributes
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
    /// Next genome id
    inline static long next_id = 0;

    /// Genome id
    long id;
    /// Weight for rows cleared in the last move
    float rows_cleared;
    /// Weight for maximum column height
    float max_height;
    /// Weight for sum of heights of all columns
    float cumulative_height;
    /// Difference between highest and lowest column
    float relative_height;
    /// Sum of holes
    float holes;
    /// Sum of height differences of all adjacent columns
    float roughness;

    /// Last genome score
    float score;
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_GENOME_HPP
