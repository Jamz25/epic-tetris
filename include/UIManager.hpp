#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <iostream>

#include "SpriteManager.hpp"
#include "Piece.hpp"

class UIManager {
private:
    static const char* font_path_;

    sf::Font font_;

    mutable sf::Text text_;

    std::shared_ptr<SpriteManager> sprite_manager_sptr_;

public:
    UIManager(std::shared_ptr<SpriteManager> sprite_manager_sptr);

    bool load_font();

    void draw_text(sf::RenderWindow& window, std::string string, sf::Vector2f position, sf::Color color, int size) const;

    void draw_piece_queue(sf::RenderWindow& window, sf::Vector2f position, std::array<PieceType, 3> piece_queue) const;

    void draw_hold_piece(sf::RenderWindow& window, sf::Vector2f position, PieceType piece_type, bool can_swap_hold) const;

private:
    void draw_ui_piece_(sf::RenderWindow& window, sf::Vector2f position, PieceType piece_type, int rotation, bool greyed = false) const;

};

#endif