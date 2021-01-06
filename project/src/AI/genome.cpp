#include "AI/genome.hpp"

namespace genetic_tetris {

RandomNumberGenerator& Genome::generator_ = RandomNumberGenerator::getInstance();
long Genome::next_id = 0;

}
