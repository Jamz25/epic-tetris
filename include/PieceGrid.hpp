#ifndef PIECE_GRID_HPP
#define PIECE_GRID_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include "Piece.hpp"
#include "PieceMove.hpp"

// Forward declaration to fix circular dependancy
class PlayerPiece;

struct PieceRotateAttempt {
    bool success = true;
    int x_push = 0;
};

class PieceGrid {
private:
    static const int ROWS_ = 20;
    static const int COLUMNS_ = 10;
    static const int GRID_SIZE_ = 25;

    std::array<std::array<PieceType, COLUMNS_>, ROWS_> grid_;

    sf::Vector2i root_pos_;

public:
    PieceGrid(sf::Vector2i root_pos);

    bool can_move_piece_down(sf::Vector2i grid_index, PieceBlocks piece_blocks) const;

    bool can_move_piece_side(sf::Vector2i grid_index, PieceBlocks piece_blocks, PieceMove piece_move) const;

    PieceRotateAttempt attempt_rotate(sf::Vector2i grid_index, PieceBlocks piece_blocks) const;

    void add_fallen_piece(sf::Vector2i grid_index, PieceBlocks piece_blocks, PieceType piece_type);

    void sweep_fallen_pieces();

    void draw_grid(sf::RenderWindow& window) const;

    void draw_player_piece(sf::RenderWindow& window, PlayerPiece const& player_piece);

    void draw_piece_drop(sf::RenderWindow& window, PlayerPiece const& player_piece);


private:
    void empty_grid_();

    void draw_grid_row_(sf::RenderWindow& window, int y) const;

};

#endif