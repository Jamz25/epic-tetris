#include "PieceGrid.hpp"
#include "PlayerPiece.hpp"

PieceGrid::PieceGrid(sf::Vector2i root_pos) {

    root_pos_ = root_pos;

    // Initialise grid
    empty_grid_();

}

bool PieceGrid::can_move_piece_down(sf::Vector2i grid_index, PieceBlocks piece_blocks) const {

    for (sf::Vector2i block_pos : piece_blocks) {

        int x_index = grid_index.x + block_pos.x;
        int y_index = grid_index.y + block_pos.y;

        if (grid_index.y >= 19) {
            return false;
        }
        if (grid_.at(grid_index.y + 1).at(grid_index.x) != PieceType::None) {
            return false;
        }

    }

    return true;

}

bool PieceGrid::can_move_piece_side(sf::Vector2i grid_index, PieceBlocks piece_blocks, PieceMove piece_move) const {

    int dir = (piece_move == PieceMove::Right) ? 1 : -1;

    for (sf::Vector2i block_pos : piece_blocks) {

        int x_index = grid_index.x + block_pos.x;
        int y_index = grid_index.y + block_pos.y;

        if (x_index + dir < 0 || x_index + dir > 9) {
            return false;
        }
        if (grid_.at(y_index).at(x_index + dir) != PieceType::None) {
            return false;
        }

    }

    return true;

}

PieceRotateAttempt PieceGrid::attempt_rotate(sf::Vector2i grid_index, PieceBlocks piece_blocks) const {

    // Stores data about the attempted rotation, so that it can be returned
    PieceRotateAttempt piece_rotate_attempt;

    for (sf::Vector2i block_pos : piece_blocks) {

        int x_index = grid_index.x + block_pos.x;
        int y_index = grid_index.y + block_pos.y;

        if (x_index < 0) {
            piece_rotate_attempt.x_push = std::max(piece_rotate_attempt.x_push, x_index * -1);
            continue;
        }
        else if (x_index > 9) {
            piece_rotate_attempt.x_push = std::min(piece_rotate_attempt.x_push, (x_index - 9) * -1);
            continue;
        }

        if (grid_.at(y_index).at(x_index) != PieceType::None) {
            piece_rotate_attempt.success = false;
            return piece_rotate_attempt;
        }

    }

    return piece_rotate_attempt;

}

void PieceGrid::add_fallen_piece(sf::Vector2i grid_index, PieceBlocks piece_blocks, PieceType piece_type) {

    for (sf::Vector2i block_pos : piece_blocks) {
        int x_index = grid_index.x + block_pos.x;
        int y_index = grid_index.y + block_pos.y;
        grid_.at(y_index).at(x_index) = piece_type;
    }

}

void PieceGrid::sweep_fallen_pieces() {

    for (int row = 0; row < 20; row++) {

        bool full_row = true;
        for (int x = 0; x < 10; x++) {

            if (grid_.at(row).at(x) == PieceType::None) {

                full_row = false;
                break;

            }

        }

        if (full_row) {

            grid_.at(row).fill(PieceType::None);

            for (int reverse = row; reverse > 0; reverse--) {
                grid_.at(reverse) = grid_.at(reverse - 1);
            }

        }

    }

}

void PieceGrid::draw_grid(sf::RenderWindow& window) const {

    // Draw each grid tile
    // Either filled in with a dropped piece or empty
    for (int y = 0; y < ROWS_; y++) {

        draw_grid_row_(window, y);

    }

}

void PieceGrid::draw_player_piece(sf::RenderWindow& window, PlayerPiece const& player_piece) {

    sf::Vector2i grid_index = player_piece.get_grid_pos();
    PieceBlocks piece_blocks = player_piece.get_piece_blocks();
    PieceType piece_type = player_piece.get_piece_type();

    for (sf::Vector2i block : piece_blocks) {

        sf::RectangleShape rect(sf::Vector2f(GRID_SIZE_, GRID_SIZE_));

        float x_pos = grid_index.x * GRID_SIZE_ + block.x * GRID_SIZE_ + root_pos_.x;
        float y_pos = grid_index.y * GRID_SIZE_ + block.y * GRID_SIZE_ + root_pos_.y;
        rect.setPosition(sf::Vector2f(x_pos, y_pos));

        rect.setFillColor(PieceColorMap.at(piece_type));
        rect.setOutlineThickness(1);

        window.draw(rect);
    
    }

}

void PieceGrid::draw_piece_drop(sf::RenderWindow& window, PlayerPiece const& player_piece) {

    sf::Vector2i grid_index = player_piece.get_drop_pos();
    PieceBlocks piece_blocks = player_piece.get_piece_blocks();

    for (sf::Vector2i block : piece_blocks) {

        sf::RectangleShape rect(sf::Vector2f(GRID_SIZE_, GRID_SIZE_));

        float x_pos = grid_index.x * GRID_SIZE_ + block.x * GRID_SIZE_ + root_pos_.x;
        float y_pos = grid_index.y * GRID_SIZE_ + block.y * GRID_SIZE_ + root_pos_.y;
        rect.setPosition(sf::Vector2f(x_pos, y_pos));

        rect.setFillColor({0, 0, 0});
        rect.setOutlineColor({255, 255, 255});
        rect.setOutlineThickness(1);
        
        window.draw(rect);
    
    }

}


//
//  PRIVATE FUNCTIONS
//

void PieceGrid::draw_grid_row_(sf::RenderWindow& window, int y) const {

    for (int x = 0; x < COLUMNS_; x++) {

        sf::RectangleShape rect(sf::Vector2f(GRID_SIZE_, GRID_SIZE_));
        rect.setOutlineThickness(1);
        rect.setPosition(sf::Vector2f(x * GRID_SIZE_ + root_pos_.x, y * GRID_SIZE_ + root_pos_.y));

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

void PieceGrid::empty_grid_() {

    for (int i = 0; i < ROWS_; i++) {
        grid_.at(i).fill(PieceType::None);
    }

}