#ifndef SPRITE_MANAGER_HPP
#define SPRITE_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "Piece.hpp"
#include "Constants.hpp"

class SpriteManager {
private:
    static const char* sprite_path_;

    static const std::unordered_map<PieceType, int> piece_texture_offsets_;

    sf::Texture piece_texture_;

    std::unordered_map<PieceType, sf::Sprite> piece_sprites_;

public:
    SpriteManager() = default;

    bool load_sprites();

    sf::Sprite get_piece_sprite(PieceType piece_type);

};

#endif