/*
 * Author: Rafał Kulus
 */

#include "tetris/wall_kicks.hpp"

#include <vector>

namespace genetic_tetris::WallKicks {

std::vector<std::pair<int, int> > getGenericWallKicks(int from, int to) {
    switch (from) {
        case 0:
            if (to == 1) {
                return {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}};
            } else if (to == 3) {
                return {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}};
            }
            break;
        case 1:
            if (to == 2 || to == 0) {
                return {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}};
            }
            break;
        case 2:
            if (to == 3) {
                return {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}};
            } else if (to == 1) {
                return {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}};
            }
            break;
        case 3:
            if (to == 0 || to == 2) {
                return {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}};
            }
            break;
        default:
            break;
    }
    return {};
}

std::vector<std::pair<int, int> > getITetrominoWallKicks(int from, int to) {
    switch (from) {
        case 0:
            if (to == 1) {
                return {{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}};
            } else if (to == 3) {
                return {{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}};
            }
            break;
        case 1:
            if (to == 2) {
                return {{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}};
            } else if (to == 0) {
                return {{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}};
            }
            break;
        case 2:
            if (to == 3) {
                return {{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}};
            } else if (to == 1) {
                return {{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}};
            }
            break;
        case 3:
            if (to == 0) {
                return {{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}};
            } else if (to == 2) {
                return {{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}};
            }
            break;
        default:
            break;
    }
    return {};
}

}  // namespace genetic_tetris::WallKicks