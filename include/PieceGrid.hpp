#ifndef PIECE_GRID_HPP
#define PIECE_GRID_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <memory>

#include "Piece.hpp"
#include "PieceMove.hpp"
#include "SpriteManager.hpp"

// Forward declaration to fix circular dependancy
class PlayerPiece;

struct PieceRotateAttempt {
    bool success = true;
    int x_push = 0;
};

const int ROWS = 20;
const int COLUMNS = 10;
const int GRID_SIZE = 25;

typedef std::array<std::array<PieceType, COLUMNS>, ROWS> GridArray;

class PieceGrid {
private:

    GridArray grid_;

    sf::Vector2i root_pos_;

    std::shared_ptr<SpriteManager> sprite_manager_sptr_;

public:
    PieceGrid(sf::Vector2i root_pos, std::shared_ptr<SpriteManager> sprite_manager_sptr);

    void empty_grid();

    bool can_move_piece_down(sf::Vector2i grid_index, PieceBlocks piece_blocks) const;

    bool can_move_piece_side(sf::Vector2i grid_index, PieceBlocks piece_blocks, PieceMove piece_move) const;

    PieceRotateAttempt attempt_rotate(sf::Vector2i grid_index, PieceBlocks piece_blocks) const;

    void add_fallen_piece(sf::Vector2i grid_index, PieceBlocks piece_blocks, PieceType piece_type);

    int sweep_fallen_pieces();

    void draw_grid(sf::RenderWindow& window) const;

    void draw_player_piece(sf::RenderWindow& window, PlayerPiece const& player_piece) const;

    void draw_piece_drop(sf::RenderWindow& window, PlayerPiece const& player_piece) const;

    GridArray const& get_grid_array() const;


private:

    void draw_grid_row_(sf::RenderWindow& window, int y) const;

};

#endif