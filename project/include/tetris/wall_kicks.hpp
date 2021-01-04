#ifndef WALL_KICKS_HPP
#define WALL_KICKS_HPP

#include <vector>

namespace WallKicks {

std::vector<std::pair<int, int> > getGenericWallKicks(int from, int to);
std::vector<std::pair<int, int> > getITetrominoWallKicks(int from, int to);

}  // namespace WallKicks

#endif
