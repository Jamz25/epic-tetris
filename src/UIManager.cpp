#include "UIManager.hpp"

const char* UIManager::font_path_ = "resources/LLPIXEL3.ttf";

UIManager::UIManager(std::shared_ptr<SpriteManager> sprite_manager_sptr) {

    sprite_manager_sptr_ = sprite_manager_sptr;

}

bool UIManager::load_font() {

    if (!font_.loadFromFile(font_path_))
        return false;
    
    text_.setFont(font_);

    return true;

}

void UIManager::draw_text(sf::RenderWindow& window, std::string string, sf::Vector2f position, sf::Color color, int size) const {

    text_.setString(string);
    text_.setPosition(position);
    text_.setFillColor(color);
    text_.setCharacterSize(size);

    window.draw(text_);

}

void UIManager::draw_piece_queue(sf::RenderWindow& window, sf::Vector2f position, std::array<PieceType, 3> piece_queue) const {

    int scaled_grid_size = GRID_SIZE * UI_PIECE_SCALE;

    float border_y_size = piece_queue.size() * 4 * scaled_grid_size + 3 * scaled_grid_size;
    sf::RectangleShape border(sf::Vector2f(scaled_grid_size * 6, border_y_size));
    border.setPosition(position);
    border.setOutlineColor(sf::Color(40, 40, 170));
    border.setOutlineThickness(2);
    border.setFillColor(sf::Color(0, 0, 0));

    window.draw(border);

    int piece_x_origin = position.x + scaled_grid_size * 5 / 2;
    for (int y = 0; y < piece_queue.size(); y++) {

        PieceType piece_type = piece_queue.at(y);

        int piece_y_origin = position.y + (y + 1) * scaled_grid_size + y * scaled_grid_size * 4 + scaled_grid_size;

        draw_ui_piece_(window, sf::Vector2f(piece_x_origin, piece_y_origin), piece_type, 0);

    }

}

void UIManager::draw_hold_piece(sf::RenderWindow& window, sf::Vector2f position, PieceType piece_type, bool can_swap_hold) const {

    int scaled_grid_size = GRID_SIZE * UI_PIECE_SCALE;

    sf::RectangleShape border(sf::Vector2f(6 * scaled_grid_size, 6 * scaled_grid_size));
    border.setPosition(position);
    border.setOutlineColor(sf::Color(40, 40, 170));
    border.setOutlineThickness(2);
    border.setFillColor(sf::Color(0, 0, 0));

    window.draw(border);

    if (piece_type == PieceType::None)
        return;

    int piece_x_origin = position.x + scaled_grid_size * 6 / 2;
    int piece_y_origin = position.y + scaled_grid_size * 6 / 2;
    
    draw_ui_piece_(window, sf::Vector2f(piece_x_origin, piece_y_origin), piece_type, 0, !can_swap_hold);

}


//
//  PRIVATE FUNCTIONS
//

void UIManager::draw_ui_piece_(sf::RenderWindow& window, sf::Vector2f position, PieceType piece_type, int rotation, bool greyed) const {

    PieceBlocks piece_blocks = get_blocks(piece_type, rotation);

    for (sf::Vector2i piece_pos : piece_blocks) {

        int x_pos = position.x + piece_pos.x * GRID_SIZE * UI_PIECE_SCALE;
        int y_pos = position.y + piece_pos.y * GRID_SIZE * UI_PIECE_SCALE;

        sf::Sprite piece_sprite = sprite_manager_sptr_->get_piece_sprite(piece_type);
        sf::FloatRect sprite_size = piece_sprite.getGlobalBounds();
        piece_sprite.scale(sf::Vector2f(UI_PIECE_SCALE, UI_PIECE_SCALE));
        piece_sprite.setPosition(sf::Vector2f(x_pos, y_pos));

        if (greyed) {
            piece_sprite.setColor(sf::Color(100, 100, 100));
        }

        window.draw(piece_sprite);

    }

}