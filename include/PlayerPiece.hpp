#ifndef PLAYER_PIECE_HPP
#define PLAYER_PIECE_HPP

#include <SFML/Graphics.hpp>
#include "Piece.hpp"
#include "PieceGrid.hpp"
#include "PieceMove.hpp"

class PlayerPiece {
private:

    sf::Vector2i grid_pos_, drop_position_;
    PieceType piece_type_;
    PieceBlocks piece_blocks_;
    int rotation_;
    float move_down_tick_;

public:
    PlayerPiece();

    void update(float delta_time, PieceGrid& piece_grid);

    void move_horizontal(PieceGrid const& piece_grid, PieceMove piece_move);

    void rotate(PieceGrid const& piece_grid);

    void hard_drop(PieceGrid& piece_grid);

    sf::Vector2i get_grid_pos() const;

    sf::Vector2i get_drop_pos() const;

    PieceType get_piece_type() const;

    PieceBlocks get_piece_blocks() const;

private:
    void reset_();
    
    void calculate_drop_position_(PieceGrid const& piece_grid);

};

#endif