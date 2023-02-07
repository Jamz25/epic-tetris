#include "PlayerPiece.hpp"

PlayerPiece::PlayerPiece() {

    // Reset member variables
    reset_();

}

void PlayerPiece::update(float delta_time, PieceGrid& piece_grid) {

    float move_down_tick_max = 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        move_down_tick_max = 0.03f;
    }

    move_down_tick_ += delta_time;

    if (move_down_tick_ >= move_down_tick_max) {

        move_down_tick_ = 0;
        if (piece_grid.can_move_piece_down(grid_pos_, piece_blocks_)) {
            grid_pos_.y++;
        }
        else {
            
            piece_grid.add_fallen_piece(grid_pos_, piece_blocks_, piece_type_);
            piece_grid.sweep_fallen_pieces();

        }

    }

    calculate_drop_position_(piece_grid);

}

void PlayerPiece::move_horizontal(PieceGrid const& piece_grid, PieceMove piece_move) {

    if (piece_grid.can_move_piece_side(grid_pos_, piece_blocks_, piece_move)) {

        int dir = (piece_move == PieceMove::Right) ? 1 : -1;
        grid_pos_.x += dir;

    }

}

void PlayerPiece::rotate(PieceGrid const& piece_grid) {

    PieceRotateAttempt rotate_attempt = piece_grid.attempt_rotate(grid_pos_, piece_blocks_);
    if (rotate_attempt.success) {
        rotation_++;
        piece_blocks_ = get_blocks(piece_type_, rotation_);
    }

    grid_pos_.x += rotate_attempt.x_push;

}

void PlayerPiece::hard_drop(PieceGrid& piece_grid) {

    piece_grid.add_fallen_piece(drop_position_, piece_blocks_, piece_type_);
    piece_grid.sweep_fallen_pieces();

    reset_();

}

sf::Vector2i PlayerPiece::get_grid_pos() const {return grid_pos_;}

sf::Vector2i PlayerPiece::get_drop_pos() const {return drop_position_;}

PieceType PlayerPiece::get_piece_type() const {return piece_type_;}

PieceBlocks PlayerPiece::get_piece_blocks() const {return piece_blocks_;}


//
//  PRIVATE FUNCTIONS
//

void PlayerPiece::calculate_drop_position_(PieceGrid const& piece_grid) {

    sf::Vector2i drop_position = grid_pos_;
    while (piece_grid.can_move_piece_down(drop_position, piece_blocks_)) {
        drop_position.y++;
    }

    drop_position_ = drop_position;

}

void PlayerPiece::reset_() {

    grid_pos_ = sf::Vector2i(5, 3);
    piece_type_ = get_random_type();
    rotation_ = 0;
    move_down_tick_ = 0;
    piece_blocks_ = get_blocks(piece_type_, rotation_);

}
