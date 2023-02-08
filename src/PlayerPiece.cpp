#include "PlayerPiece.hpp"

PlayerPiece::PlayerPiece(PieceGrid const& piece_grid) {

    // Reset all values to default
    reset(piece_grid);

}

void PlayerPiece::reset(PieceGrid const& piece_grid) {

    lines_cleared_ = 0;
    score_ = 0;
    gameover_ = false;

    for (PieceType& piece_type : piece_queue_) {
        piece_type = get_random_type();
    }
    
    // "Respawn" piece
    respawn_(piece_grid);

}

void PlayerPiece::update(float delta_time, PieceGrid& piece_grid) {

    float move_down_tick_max = 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        move_down_tick_max = 0.06f;
    }

    move_down_tick_ += delta_time;

    if (move_down_tick_ >= move_down_tick_max) {

        move_down_tick_ = 0;
        if (piece_grid.can_move_piece_down(grid_pos_, piece_blocks_)) {
            grid_pos_.y++;
        }
        else {
            
            piece_grid.add_fallen_piece(grid_pos_, piece_blocks_, piece_type_);
            int lines_cleared = piece_grid.sweep_fallen_pieces();
            add_score_(lines_cleared);

            respawn_(piece_grid);

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

void PlayerPiece::rotate(PieceGrid const& piece_grid, int dir) {

    PieceRotateAttempt rotate_attempt = piece_grid.attempt_rotate(grid_pos_, get_blocks(piece_type_, (rotation_ + dir) % 4));
    if (rotate_attempt.success) {
        rotation_ += dir;
        if (rotation_ > 3) rotation_ = 0;
        if (rotation_ < 0) rotation_ = 3;
        piece_blocks_ = get_blocks(piece_type_, rotation_);
    }

    grid_pos_.x += rotate_attempt.x_push;

}

void PlayerPiece::hard_drop(PieceGrid& piece_grid) {

    piece_grid.add_fallen_piece(drop_position_, piece_blocks_, piece_type_);
    int lines_cleared = piece_grid.sweep_fallen_pieces();
    add_score_(lines_cleared);

    respawn_(piece_grid);

}

sf::Vector2i PlayerPiece::get_grid_pos() const {return grid_pos_;}

sf::Vector2i PlayerPiece::get_drop_pos() const {return drop_position_;}

PieceType PlayerPiece::get_piece_type() const {return piece_type_;}

PieceBlocks PlayerPiece::get_piece_blocks() const {return piece_blocks_;}

int PlayerPiece::get_lines_cleared() const {return lines_cleared_;}

int PlayerPiece::get_score() const {return score_;}

bool PlayerPiece::is_game_over() const {return gameover_;}

std::array<PieceType, 3> PlayerPiece::get_piece_queue() const {return piece_queue_;}


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

void PlayerPiece::respawn_(PieceGrid const& piece_grid) {

    grid_pos_ = sf::Vector2i(5, 2);
    
    piece_type_ = piece_queue_[0];
    piece_queue_[0] = piece_queue_[1];
    piece_queue_[1] = piece_queue_[2];
    piece_queue_[2] = get_random_type();

    rotation_ = 0;
    move_down_tick_ = 0;
    piece_blocks_ = get_blocks(piece_type_, rotation_);

    for (sf::Vector2i block : piece_blocks_) {

        int x_index = grid_pos_.x + block.x;
        int y_index = grid_pos_.y + block.y;

        if (piece_grid.get_grid_array().at(y_index).at(x_index) != PieceType::None) {
            gameover_ = true;
            break;
        }

    }

}

void PlayerPiece::add_score_(int lines_cleared) {

    if (lines_cleared == 0)
        return;

    lines_cleared_ += lines_cleared;

    score_ += lines_cleared * 5 + std::pow(5, lines_cleared);

}
