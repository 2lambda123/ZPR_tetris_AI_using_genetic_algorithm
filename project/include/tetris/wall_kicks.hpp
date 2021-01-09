/*
 * Author: Rafał Kulus
 */

#ifndef WALL_KICKS_HPP
#define WALL_KICKS_HPP

#include <vector>

namespace genetic_tetris::WallKicks {

std::vector<std::pair<int, int> > getGenericWallKicks(int from, int to);
std::vector<std::pair<int, int> > getITetrominoWallKicks(int from, int to);

}  // namespace genetic_tetris::WallKicks

#endif
