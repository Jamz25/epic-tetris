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

void UIManager::draw_text(sf::RenderWindow& window, std::string string, sf::Vector2f position, sf::Color color, int size) {

    text_.setString(string);
    text_.setPosition(position);
    text_.setFillColor(color);
    text_.setCharacterSize(size);

    window.draw(text_);

}

void UIManager::draw_piece_queue(sf::RenderWindow& window, sf::Vector2f position, std::array<PieceType, 3> piece_queue) {

    float piece_scale = 2.0 / 3.0;
    int scaled_grid_size = GRID_SIZE * piece_scale;

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

        PieceBlocks piece_blocks = get_blocks(piece_type, 0);
        int piece_y_origin = position.y + (y + 1) * scaled_grid_size + y * scaled_grid_size * 4 + scaled_grid_size;

        for (sf::Vector2i piece_pos : piece_blocks) {

            int x_pos = piece_x_origin + piece_pos.x * scaled_grid_size;
            int y_pos = piece_y_origin + piece_pos.y * scaled_grid_size;

            sf::Sprite piece_sprite = sprite_manager_sptr_->get_piece_sprite(piece_type);
            sf::FloatRect sprite_size = piece_sprite.getGlobalBounds();
            piece_sprite.scale(sf::Vector2f(piece_scale, piece_scale));
            piece_sprite.setPosition(sf::Vector2f(x_pos, y_pos));

            window.draw(piece_sprite);

        }

    }

}