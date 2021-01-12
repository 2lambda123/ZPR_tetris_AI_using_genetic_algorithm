#ifdef __linux__
#define BOOST_TEST_DYN_LINK
#endif

#define BOOST_TEST_MODULE TestAI
#include <boost/test/unit_test.hpp>
#define private public
#include "AI/ai.hpp"
#include "AI/evolutionary_algo.hpp"
#include "AI/genome.hpp"

using namespace genetic_tetris;

std::ostream& operator<<(std::ostream& os, const Tetris::Grid& grid) {
    for (int i = Tetris::GRID_FULL_HEIGHT - 1; i >= 0; i--) {
        for (const auto& block : grid[i]) {
            if (block == Tetromino::Color::EMPTY) {
                os << "0";
            }
            else {
                os << "1";
            }
        }
        os << "\n";
    }
    return os;
}

BOOST_AUTO_TEST_CASE(test_genome_serialization) {
    std::cout << "Test genome serialization" << std::endl;
    std::vector<Genome> genomes;
    genomes.push_back(Genome(1, 1, 1, 1, 1, 1));
    std::vector<Genome> load_genomes;
    EvolutionaryAlgo::saveToJSON("test.json", genomes);
    load_genomes = EvolutionaryAlgo::loadFromJSON("test.json");
    BOOST_REQUIRE(load_genomes[0] == genomes[0] && load_genomes[0].id == genomes[0].id);
}

BOOST_AUTO_TEST_CASE(test_move) {
    std::cout << "Test move" << std::endl;
    Move move;
    std::vector<Tetromino::Color> empty_row(Tetris::GRID_WIDTH, Tetromino::Color::EMPTY);
    Tetris::Grid grid(Tetris::GRID_FULL_HEIGHT, empty_row);
    std::cout << grid;
    move.calculateGridProperties(grid);
    BOOST_REQUIRE(move.getRelativeHeight() == 0);
    BOOST_REQUIRE(move.getMaxHeight() == 0);
    BOOST_REQUIRE(move.getCumulativeHeight() == 0);
    BOOST_REQUIRE(move.getRoughness() == 0);
    BOOST_REQUIRE(move.getHoles() == 0);

    grid[0][0] = Tetromino::Color::CYAN;
    grid[0][1] = Tetromino::Color::CYAN;
    grid[0][2] = Tetromino::Color::CYAN;
    grid[1][0] = Tetromino::Color::CYAN;
    grid[1][2] = Tetromino::Color::CYAN;
    grid[2][0] = Tetromino::Color::CYAN;
    grid[2][1] = Tetromino::Color::CYAN;
    grid[2][2] = Tetromino::Color::CYAN;
    grid[3][1] = Tetromino::Color::CYAN;
    std::cout << grid;
    move.calculateGridProperties(grid);
    BOOST_REQUIRE(move.getRelativeHeight() == 4);
    BOOST_REQUIRE(move.getMaxHeight() == 4);
    BOOST_REQUIRE(move.getCumulativeHeight() == 10);
    BOOST_REQUIRE(move.getRoughness() == 5);
    BOOST_REQUIRE(move.getHoles() == 1);

}
