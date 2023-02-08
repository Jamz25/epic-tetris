#include "SpriteManager.hpp"

const char* SpriteManager::sprite_path_ = "resources/pieces.png";

const std::unordered_map<PieceType, int> SpriteManager::piece_texture_offsets_ {
    {PieceType::Square, 288},
    {PieceType::Line, 240},
    {PieceType::Pronged, 144},
    {PieceType::LLeft, 96},
    {PieceType::LRight, 0},
    {PieceType::ZLeft, 192},
    {PieceType::ZRight, 48}
};

bool SpriteManager::load_sprites() {

    if (!piece_texture_.loadFromFile(sprite_path_))
        return false;

    for (std::pair<PieceType, int> piece_type_pair : piece_texture_offsets_) {

        sf::Sprite sprite;
        sprite.setTexture(piece_texture_);

        int x_offset = piece_texture_offsets_.at(piece_type_pair.first);
        sf::IntRect offset_rect(sf::Vector2i(x_offset, 0), sf::Vector2i(48, 48));
        sprite.setTextureRect(offset_rect);

        sf::FloatRect sprite_size = sprite.getGlobalBounds();
        sprite.setScale(sf::Vector2f(GRID_SIZE / sprite_size.width, GRID_SIZE / sprite_size.height));

        piece_sprites_.insert(std::pair<PieceType, sf::Sprite>(piece_type_pair.first, sprite));

    }

    return true;

}

sf::Sprite SpriteManager::get_piece_sprite(PieceType piece_type) {

    return piece_sprites_.at(piece_type);

}