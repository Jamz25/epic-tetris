#ifndef PIECE_HPP
#define PIECE_HPP

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <array>

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

struct PieceBlocks {
    std::array<sf::Vector2i, 4> blocks;
};

// Rotations progress in a clockwise direction
struct PiecePositions {
    std::array<PieceBlocks, 4> rotations;
};

const std::unordered_map<PieceType, PiecePositions> Pieces {
    {PieceType::Square, PiecePositions{
        PieceBlocks{sf::Vector2i{-1, -1}, sf::Vector2i{0, -1}, sf::Vector2i{-1, 0}, sf::Vector2i{0, 0}},
        PieceBlocks{sf::Vector2i{-1, -1}, sf::Vector2i{0, -1}, sf::Vector2i{-1, 0}, sf::Vector2i{0, 0}},
        PieceBlocks{sf::Vector2i{-1, -1}, sf::Vector2i{0, -1}, sf::Vector2i{-1, 0}, sf::Vector2i{0, 0}},
        PieceBlocks{sf::Vector2i{-1, -1}, sf::Vector2i{0, -1}, sf::Vector2i{-1, 0}, sf::Vector2i{0, 0}}
    }},
    {PieceType::Line, PiecePositions{
        PieceBlocks{sf::Vector2i{-2, -1}, sf::Vector2i{-1, -1}, sf::Vector2i{0, -1}, sf::Vector2i{1, -1}},
        PieceBlocks{sf::Vector2i{0, -2}, sf::Vector2i{0, -1}, sf::Vector2i{0, 0}, sf::Vector2i{0, 1}},
        PieceBlocks{sf::Vector2i{-2, 0}, sf::Vector2i{-1, 0}, sf::Vector2i{0, 0}, sf::Vector2i{1, 0}},
        PieceBlocks{sf::Vector2i{-1, -2}, sf::Vector2i{-1, -1}, sf::Vector2i{-1, 0}, sf::Vector2i{-1, 1}}
    }},
    {PieceType::Pronged, PiecePositions{
        PieceBlocks{sf::Vector2i{0, 0}, sf::Vector2i{-1, 0}, sf::Vector2i{1, 0}, sf::Vector2i{0, 1}},
        PieceBlocks{sf::Vector2i{0, 0}, sf::Vector2i{-1, 0}, sf::Vector2i{0, -1}, sf::Vector2i{0, 1}},
        PieceBlocks{sf::Vector2i{0, 0}, sf::Vector2i{0, -1}, sf::Vector2i{-1, 0}, sf::Vector2i{1, 0}},
        PieceBlocks{sf::Vector2i{0, 0}, sf::Vector2i{1, 0}, sf::Vector2i{0, -1}, sf::Vector2i{0, 1}}
    }},
    {PieceType::LLeft, PiecePositions{
        PieceBlocks{sf::Vector2i{-1, -1}, sf::Vector2i{0, -1}, sf::Vector2i{0, 0}, sf::Vector2i{0, 1}},
        PieceBlocks{sf::Vector2i{0, -1}, sf::Vector2i{0, 0}, sf::Vector2i{-1, 0}, sf::Vector2i{-2, 0}},
        PieceBlocks{sf::Vector2i{0, 0}, sf::Vector2i{-1, 0}, sf::Vector2i{-1, -1}, sf::Vector2i{-1, -2}},
        PieceBlocks{sf::Vector2i{-1, 0}, sf::Vector2i{-1, -1}, sf::Vector2i{0, -1}, sf::Vector2i{1, -1}}
    }}
};

const std::unordered_map<PieceType, sf::Color> PieceColorMap {
    {PieceType::Square, sf::Color{251, 255, 95}},
    {PieceType::Line, sf::Color{67, 255, 250}},
    {PieceType::Pronged, sf::Color{130, 25, 74}},
    {PieceType::LLeft, sf::Color{255, 154, 0}}
};

PieceType get_random_type() {
    static const PieceType type_array[4] = {PieceType::Square, PieceType::Line, PieceType::Pronged, PieceType::LLeft};
    return type_array[rand() % 4];
}

PieceBlocks get_blocks(PieceType type, int rotation) {
    return Pieces.at(type).rotations.at(rotation % 4);
}

void draw_piece(sf::RenderWindow& window, PieceType type, int rotation, sf::Vector2i position, int scale = 1) {
    // Get defined piece with rotation
    PieceBlocks blocks = get_blocks(type, rotation);
    // Draw all rectangles
    for (sf::Vector2i offset : blocks.blocks) {
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
    for (sf::Vector2i offset : blocks.blocks) {
        sf::RectangleShape rect({scale, scale});
        rect.setPosition({position.x + offset.x * scale, position.y + offset.y * scale});
        rect.setFillColor({0, 0, 0});
        rect.setOutlineColor({255, 255, 255});
        rect.setOutlineThickness(1);
        window.draw(rect);
    }
}

#endif