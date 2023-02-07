#ifndef PIECE_GRID_HPP
#define PIECE_GRID_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include "Piece.hpp"

class PieceGrid {
private:
    static const int ROWS_ = 20;
    static const int COLUMNS_ = 10;
    static const int GRID_SIZE_ = 25;

    std::array<std::array<PieceType, COLUMNS_>, ROWS_> grid_;

public:
    PieceGrid();

    void draw_grid(sf::RenderWindow& window);

private:
    void empty_grid_();

};

#endif