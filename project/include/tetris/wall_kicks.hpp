/*
 * Author: Rafal Kulus
 */

#ifndef WALL_KICKS_HPP
#define WALL_KICKS_HPP

#include <vector>

/**
 * https://tetris.fandom.com/wiki/SRS#Wall_Kicks
 */
namespace genetic_tetris::WallKicks {

/// Wall kicks for every tetromino except for O and I. from and to should be in range [0, 3]
std::vector<std::pair<int, int> > getGenericWallKicks(int from, int to);
std::vector<std::pair<int, int> > getITetrominoWallKicks(int from, int to);

}  // namespace genetic_tetris::WallKicks

#endif
