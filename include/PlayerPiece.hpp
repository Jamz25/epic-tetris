#ifndef PLAYER_PIECE_HPP
#define PLAYER_PIECE_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>

#include "Piece.hpp"
#include "PieceGrid.hpp"
#include "PieceMove.hpp"

class PlayerPiece {
private:
    static const sf::Vector2i spawn_pos_;

    sf::Vector2i grid_pos_, drop_position_;
    PieceType piece_type_;
    PieceBlocks piece_blocks_;
    int rotation_, lines_cleared_;
    long score_;
    float move_down_tick_;
    bool gameover_, can_swap_hold_;

    std::array<PieceType, 3> piece_queue_;
    PieceType held_piece_type_;

public:
    PlayerPiece(PieceGrid const& piece_grid);

    void reset(PieceGrid const& piece_grid);

    void update(float delta_time, PieceGrid& piece_grid);

    void move_horizontal(PieceGrid const& piece_grid, PieceMove piece_move);

    void rotate(PieceGrid const& piece_grid, int dir);

    void hard_drop(PieceGrid& piece_grid);

    void swap_held_piece();

    sf::Vector2i get_grid_pos() const;

    sf::Vector2i get_drop_pos() const;

    PieceType get_piece_type() const;

    PieceBlocks get_piece_blocks() const;

    int get_lines_cleared() const;

    int get_score() const;

    bool is_game_over() const;

    std::array<PieceType, 3> get_piece_queue() const;

    PieceType get_held_piece_type() const;
    
    bool can_swap_hold() const;

private:
    void respawn_(PieceGrid const& piece_grid);

    void rotate_piece_queue_();
    
    void calculate_drop_position_(PieceGrid const& piece_grid);

    void add_score_(int lines_cleared);

};

#endif