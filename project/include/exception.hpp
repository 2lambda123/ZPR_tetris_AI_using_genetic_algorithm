/*
 * Author: Damian Kolaska
 */

#ifndef GENETIC_TETRIS_EXCEPTION_HPP
#define GENETIC_TETRIS_EXCEPTION_HPP

#include <exception>

namespace genetic_tetris {

class GenomeFileNotFoundException : public std::exception {

};

}

#endif  // GENETIC_TETRIS_EXCEPTION_HPP
