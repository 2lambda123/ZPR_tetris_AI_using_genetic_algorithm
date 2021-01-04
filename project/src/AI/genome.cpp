#include "AI/genome.hpp"

namespace gentetris {

RandomNumberGenerator& Genome::generator_ = RandomNumberGenerator::getInstance();
long Genome::next_id = 0;

}
