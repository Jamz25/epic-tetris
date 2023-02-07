#include "PieceGrid.hpp"

PieceGrid::PieceGrid() {

    // Initialise grid
    empty_grid_();

}

void PieceGrid::draw_grid(sf::RenderWindow& window) {

    // Draw each grid tile
    // Either filled in with a dropped piece or empty
    for (int y = 0; y < ROWS_; y++) {

        for (int x = 0; x < COLUMNS_; x++) {

            sf::RectangleShape rect(sf::Vector2f(GRID_SIZE_, GRID_SIZE_));
            rect.setOutlineThickness(1);
            rect.setPosition(sf::Vector2f(x * 25 + 275, y * 25 + 50));

            if (grid_.at(y).at(x) != PieceType::None) {

                rect.setFillColor(PieceColorMap.at(grid_.at(y).at(x)));
                window.draw(rect);

            }
            else {

                rect.setOutlineColor(sf::Color(0, 0, 170));
                rect.setFillColor(sf::Color(0, 0, 0));
                window.draw(rect);

            }

        }

    }

}

void PieceGrid::empty_grid_() {

    for (int i = 0; i < ROWS_; i++) {
        grid_.at(i).fill(PieceType::None);
    }

}