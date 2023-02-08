#include "PieceGrid.hpp"
#include "PlayerPiece.hpp"

PieceGrid::PieceGrid(sf::Vector2i root_pos, std::shared_ptr<SpriteManager> sprite_manager_sptr) {

    root_pos_ = root_pos;
    sprite_manager_sptr_ = sprite_manager_sptr;

    // Initialise grid
    empty_grid();

}

void PieceGrid::empty_grid() {

    for (int i = 0; i < ROWS; i++) {
        grid_.at(i).fill(PieceType::None);
    }

}

bool PieceGrid::can_move_piece_down(sf::Vector2i grid_index, PieceBlocks piece_blocks) const {

    for (sf::Vector2i block_pos : piece_blocks) {

        int x_index = grid_index.x + block_pos.x;
        int y_index = grid_index.y + block_pos.y;

        if (y_index >= ROWS - 1) {
            return false;
        }
        if (grid_.at(y_index + 1).at(x_index) != PieceType::None) {
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

        if (x_index + dir < 0 || x_index + dir > COLUMNS - 1) {
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
        else if (x_index > COLUMNS - 1) {
            piece_rotate_attempt.x_push = std::min(piece_rotate_attempt.x_push, (x_index - COLUMNS - 1));
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

int PieceGrid::sweep_fallen_pieces() {

    int lines_cleared = 0;

    for (int row = 0; row < 20; row++) {

        bool full_row = true;
        for (int x = 0; x < 10; x++) {

            if (grid_.at(row).at(x) == PieceType::None) {

                full_row = false;
                break;

            }

        }

        if (full_row) {

            lines_cleared++;

            grid_.at(row).fill(PieceType::None);

            for (int reverse = row; reverse > 0; reverse--) {
                grid_.at(reverse) = grid_.at(reverse - 1);
            }

        }

    }

    return lines_cleared;

}

void PieceGrid::draw_grid(sf::RenderWindow& window) const {

    // Draw each grid tile
    // Either filled in with a dropped piece or empty
    for (int y = 0; y < ROWS; y++) {

        draw_grid_row_(window, y);

    }

}

void PieceGrid::draw_player_piece(sf::RenderWindow& window, PlayerPiece const& player_piece) const {

    sf::Vector2i grid_index = player_piece.get_grid_pos();
    PieceBlocks piece_blocks = player_piece.get_piece_blocks();
    PieceType piece_type = player_piece.get_piece_type();

    sf::Sprite piece_sprite = sprite_manager_sptr_->get_piece_sprite(piece_type);

    for (sf::Vector2i block : piece_blocks) {

        float x_pos = grid_index.x * GRID_SIZE + block.x * GRID_SIZE + root_pos_.x;
        float y_pos = grid_index.y * GRID_SIZE + block.y * GRID_SIZE + root_pos_.y;
        
        piece_sprite.setPosition(sf::Vector2f(x_pos, y_pos));

        window.draw(piece_sprite);
    
    }

}

void PieceGrid::draw_piece_drop(sf::RenderWindow& window, PlayerPiece const& player_piece) const {

    sf::Vector2i grid_index = player_piece.get_drop_pos();
    PieceBlocks piece_blocks = player_piece.get_piece_blocks();

    for (sf::Vector2i block : piece_blocks) {

        sf::RectangleShape rect(sf::Vector2f(GRID_SIZE, GRID_SIZE));

        float x_pos = grid_index.x * GRID_SIZE + block.x * GRID_SIZE + root_pos_.x;
        float y_pos = grid_index.y * GRID_SIZE + block.y * GRID_SIZE + root_pos_.y;
        rect.setPosition(sf::Vector2f(x_pos, y_pos));

        rect.setFillColor({0, 0, 0});
        rect.setOutlineColor({255, 255, 255});
        rect.setOutlineThickness(1);
        
        window.draw(rect);
    
    }

}

GridArray const& PieceGrid::get_grid_array() const {
    return grid_;
}


//
//  PRIVATE FUNCTIONS
//

void PieceGrid::draw_grid_row_(sf::RenderWindow& window, int y) const {

    for (int x = 0; x < COLUMNS; x++) {

        sf::Vector2f screen_pos = sf::Vector2f(x * GRID_SIZE + root_pos_.x, y * GRID_SIZE + root_pos_.y);

        PieceType piece_type = grid_.at(y).at(x);
        if (piece_type != PieceType::None) {

            sf::Sprite piece_sprite = sprite_manager_sptr_->get_piece_sprite(piece_type);

            piece_sprite.setPosition(screen_pos);

            window.draw(piece_sprite);

        }
        else {

            sf::RectangleShape rect(sf::Vector2f(GRID_SIZE, GRID_SIZE));
            rect.setPosition(screen_pos);

            rect.setFillColor(sf::Color(0, 0, 0));
            rect.setOutlineColor(sf::Color(0, 0, 170));
            rect.setOutlineThickness(1);

            window.draw(rect);

        }

    }

}