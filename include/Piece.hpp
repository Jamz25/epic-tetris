#ifndef PIECE_HPP
#define PIECE_HPP

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <array>
#include <cmath>
#include <iostream>

enum PieceType {
    None,
    Square,
    Line,
    Pronged,
    LLeft,
    LRight,
    ZLeft,
    ZRight
};

// Stores blocks, positions relative to piece origin (rotation point)
typedef std::array<sf::Vector2i, 4> PieceBlocks;

// Stores each piece type's blocks, in unrotated (0 degrees) form
const std::unordered_map<PieceType, PieceBlocks> unrotated_piece_map {
    {PieceType::Square, PieceBlocks{sf::Vector2i{-1, -1}, sf::Vector2i{0, -1}, sf::Vector2i{-1, 0}, sf::Vector2i{0, 0}}},
    {PieceType::Line, PieceBlocks{sf::Vector2i{-2, 0}, sf::Vector2i{-1, 0}, sf::Vector2i{0, 0}, sf::Vector2i{1, 0}}},
    {PieceType::Pronged, PieceBlocks{sf::Vector2i{0, 0}, sf::Vector2i{-1, 0}, sf::Vector2i{1, 0}, sf::Vector2i{0, 1}}},
    {PieceType::LLeft, PieceBlocks{sf::Vector2i{-1, -1}, sf::Vector2i{0, -1}, sf::Vector2i{0, 0}, sf::Vector2i{0, 1}}},
    {PieceType::LRight, PieceBlocks{sf::Vector2i{1, -1}, sf::Vector2i{0, -1}, sf::Vector2i{0, 0}, sf::Vector2i{0, 1}}},
    {PieceType::ZLeft, PieceBlocks{sf::Vector2i{-1, 0}, sf::Vector2i{0, 0}, sf::Vector2i{0, 1}, sf::Vector2i{1, 1}}},
    {PieceType::ZRight, PieceBlocks{sf::Vector2i{1, 0}, sf::Vector2i{0, 0}, sf::Vector2i{0, 1}, sf::Vector2i{-1, 1}}}
};

const std::unordered_map<PieceType, sf::Color> PieceColorMap {
    {PieceType::Square, sf::Color{251, 255, 95}},
    {PieceType::Line, sf::Color{67, 255, 250}},
    {PieceType::Pronged, sf::Color{130, 25, 74}},
    {PieceType::LLeft, sf::Color{255, 154, 0}},
    {PieceType::LRight, sf::Color{50, 50, 230}},
    {PieceType::ZLeft, sf::Color{230, 50, 50}},
    {PieceType::ZRight, sf::Color{50, 230, 50}}
};

PieceType get_random_type() {
    static const PieceType type_array[7] = {
        PieceType::Square, PieceType::Line, PieceType::Pronged, PieceType::LLeft, PieceType::LRight, PieceType::ZLeft, PieceType::ZRight
        };
    PieceType type = type_array[rand() % 7];
    std::cout << type << "\n";
    return type;
}

PieceBlocks get_blocks(PieceType type, int rotation) {
    PieceBlocks rotated_blocks = unrotated_piece_map.at(type);
    if (type == PieceType::Square)
        return rotated_blocks;
    for (sf::Vector2i& block_offset : rotated_blocks) {
        for (int i = 0; i < rotation; i++) {
            int old_x = block_offset.x;
            block_offset.x = -block_offset.y;
            block_offset.y = old_x;
        }
    }
    return rotated_blocks;
}

void draw_piece(sf::RenderWindow& window, PieceType type, int rotation, sf::Vector2i position, int scale = 1) {
    // Get defined piece with rotation
    PieceBlocks blocks = get_blocks(type, rotation);
    // Draw all rectangles
    for (sf::Vector2i offset : blocks) {
        sf::RectangleShape rect({scale, scale});
        rect.setPosition({position.x + offset.x * scale, position.y + offset.y * scale});
        rect.setFillColor(PieceColorMap.at(type));
        rect.setOutlineThickness(1);
        window.draw(rect);
    }
}

void draw_calculated_drop(sf::RenderWindow& window, PieceType type, int rotation, sf::Vector2i position, int scale = 1) {
    // Get defined piece with rotation
    PieceBlocks blocks = get_blocks(type, rotation);
    // Draw all rectangles
    for (sf::Vector2i offset : blocks) {
        sf::RectangleShape rect({scale, scale});
        rect.setPosition({position.x + offset.x * scale, position.y + offset.y * scale});
        rect.setFillColor({0, 0, 0});
        rect.setOutlineColor({255, 255, 255});
        rect.setOutlineThickness(1);
        window.draw(rect);
    }
}

#endif