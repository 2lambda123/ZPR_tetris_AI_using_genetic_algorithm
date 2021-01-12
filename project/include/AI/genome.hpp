/*
 * Author: Damian Kolaska
 */

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
    Genome(float rowsCleared, float maxHeight, float cumulativeHeight, float relativeHeight,
           float holes, float roughness)
        : rows_cleared(rowsCleared),
          max_height(maxHeight),
          cumulative_height(cumulativeHeight),
          relative_height(relativeHeight),
          holes(holes),
          roughness(roughness) {
        id = next_id++;
    }
    bool operator==(const Genome& rhs) const {
        return rows_cleared == rhs.rows_cleared && max_height == rhs.max_height &&
               cumulative_height == rhs.cumulative_height &&
               relative_height == rhs.relative_height && holes == rhs.holes &&
               roughness == rhs.roughness;
    }
    bool operator!=(const Genome& rhs) const { return !(rhs == *this); }
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
