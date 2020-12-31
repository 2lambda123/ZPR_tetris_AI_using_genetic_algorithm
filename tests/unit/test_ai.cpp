#ifdef __linux__
#define BOOST_TEST_DYN_LINK
#endif

#define BOOST_TEST_MODULE TestAI
#include <boost/test/unit_test.hpp>
#include "AI/ai.hpp"

int add(int i, int j) { return i + j; }

BOOST_AUTO_TEST_CASE(test_move) {
    Move move;
    move.setMoveX(-3);
    BOOST_REQUIRE(move.getMoveX() == Move::MIN_MOVE);
    move.decrementMoveX();
    BOOST_REQUIRE(move.getMoveX() == Move::MAX_MOVE);
    move.decrementMoveX();
    BOOST_REQUIRE(move.getMoveX() == Move::MAX_MOVE - 1);
    move.setMoveX(99);
    BOOST_REQUIRE(move.getMoveX() == Move::MAX_MOVE);
    move.incrementMoveX();
    BOOST_REQUIRE(move.getMoveX() == Move::MIN_MOVE);
    move.incrementMoveX();
    BOOST_REQUIRE(move.getMoveX() == Move::MIN_MOVE + 1);

    move.setRotation(-3);
    BOOST_REQUIRE(move.getRotation() == Move::MIN_ROT);
    move.decrementRotation();
    BOOST_REQUIRE(move.getRotation() == Move::MAX_ROT);
    move.decrementRotation();
    BOOST_REQUIRE(move.getRotation() == Move::MAX_ROT - 1);
    move.setRotation(10);
    BOOST_REQUIRE(move.getRotation() == Move::MAX_ROT);
    move.incrementRotation();
    BOOST_REQUIRE(move.getRotation() == Move::MIN_ROT);
    move.incrementRotation();
    BOOST_REQUIRE(move.getRotation() == Move::MIN_ROT + 1);
}
