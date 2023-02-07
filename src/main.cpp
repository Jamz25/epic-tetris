#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include "Piece.hpp"

bool can_move_piece_down(sf::Vector2i pos, std::array<std::array<PieceType, 10>, 20> const& fallen_pieces, PieceType type, int rotation) {
    PieceBlocks blocks = get_blocks(type, rotation);
    for (sf::Vector2i block_pos : blocks) {
        int y_index = static_cast<int>((pos.y + block_pos.y * 25 - 50) / 25);
        int x_index = static_cast<int>((pos.x + block_pos.x * 25 - 275) / 25);
        if (y_index >= 19) {
            return false;
        }
        if (fallen_pieces.at(y_index + 1).at(x_index) != PieceType::None) {
            return false;
        }
    }
    return true;
}

bool can_move_piece_side(sf::Vector2i pos, std::array<std::array<PieceType, 10>, 20> const& fallen_pieces, PieceType type, int rotation, int dir) {
    PieceBlocks blocks = get_blocks(type, rotation);
    for (sf::Vector2i block_pos : blocks) {
        int y_index = static_cast<int>((pos.y + block_pos.y * 25 - 50) / 25);
        int x_index = static_cast<int>((pos.x + block_pos.x * 25 - 275) / 25);
        if (x_index + dir < 0 || x_index + dir > 9) {
            return false;
        }
        if (fallen_pieces.at(y_index).at(x_index + dir) != PieceType::None) {
            return false;
        }
    }
    return true;
}

void attempt_rotate(sf::Vector2i& pos, std::array<std::array<PieceType, 10>, 20> const& fallen_pieces, PieceType type, int& rotation) {
    PieceBlocks blocks = get_blocks(type, rotation + 1);
    int x_push = 0;
    for (sf::Vector2i block_pos : blocks) {
        int y_index = static_cast<int>((pos.y + block_pos.y * 25 - 50) / 25);
        int x_index = static_cast<int>((pos.x + block_pos.x * 25 - 275) / 25);
        if (x_index < 0) {
            x_push = std::max(x_push, x_index * -25);
            continue;
        }
        else if (x_index > 9) {
            x_push = std::min(x_push, (x_index - 9) * -25);
            continue;
        }
        if (fallen_pieces.at(y_index).at(x_index) != PieceType::None) {
            return;
        }
    }
    pos.x += x_push;
    rotation++;
}

void sweep_fallen_pieces(std::array<std::array<PieceType, 10>, 20>& fallen_pieces) {
    for (int row = 0; row < 20; row++) {
        bool full_row = true;
        for (int x = 0; x < 10; x++) {
            if (fallen_pieces.at(row).at(x) == PieceType::None) {
                full_row = false;
                break;
            }
        }
        if (!full_row)
            continue;
        fallen_pieces.at(row).fill(PieceType::None);
        for (int reverse = row; reverse > 0; reverse--) {
            fallen_pieces.at(reverse) = fallen_pieces.at(reverse - 1);
        }
    }
}

sf::Vector2i calculate_drop_position(sf::Vector2i pos, std::array<std::array<PieceType, 10>, 20> const& fallen_pieces, PieceType type, int rotation) {
    sf::Vector2i drop_position = pos;
    while (can_move_piece_down(drop_position, fallen_pieces, type, rotation)) {
        drop_position.y += 25;
    }
    return drop_position;
}

int main() {

    srand((unsigned)time(0));

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Game");
    sf::Clock clock;

    sf::VertexArray grid(sf::Lines, 2);
    grid.resize(2 * 9 + 2 * 19);
    for (int i = 0; i < 9; i++) {
        grid[i * 2].position = {i * 25 + 300, 50};
        grid[i * 2].color = {0, 0, 170};
        grid[i * 2 + 1].position = {i * 25 + 300, 550};
        grid[i * 2 + 1].color = {0, 0, 170};
    }
    for (int i = 0; i < 19; i++) {
        grid[2 * 9 + i * 2].position = {275, 75 + i * 25};
        grid[2 * 9 + i * 2].color = {0, 0, 170};
        grid[2 * 9 + i * 2 + 1].position = {525, 75 + i * 25};
        grid[2 * 9 + i * 2 + 1].color = {0, 0, 170};
    }

    sf::Vector2i piece_position = {425, 100};
    PieceType piece_type = get_random_type();
    int rotation = 0;
    float move_down_tick = 0;

    // Booleans storing if a piece has fallen
    std::array<std::array<PieceType, 10>, 20> fallen_pieces;
    for (int i = 0; i < 20; i++) {
        fallen_pieces.at(i).fill(PieceType::None);
    }

    while (window.isOpen()) {

        float delta_time = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    attempt_rotate(piece_position, fallen_pieces, piece_type, rotation);
                }
                if (event.key.code == sf::Keyboard::A) {
                    if (can_move_piece_side(piece_position, fallen_pieces, piece_type, rotation, -1)) {
                        piece_position.x -= 25;
                    }
                }
                if (event.key.code == sf::Keyboard::D) {
                    if (can_move_piece_side(piece_position, fallen_pieces, piece_type, rotation, 1)) {
                        piece_position.x += 25;
                    }
                }
                if (event.key.code == sf::Keyboard::W) {
                    sf::Vector2i calculated_drop = calculate_drop_position(piece_position, fallen_pieces, piece_type, rotation);
                    PieceBlocks blocks = get_blocks(piece_type, rotation);
                    for (sf::Vector2i block_pos : blocks) {
                        int y_index = static_cast<int>((calculated_drop.y + block_pos.y * 25 - 50) / 25);
                        int x_index = static_cast<int>((calculated_drop.x + block_pos.x * 25 - 275) / 25);
                        fallen_pieces.at(y_index).at(x_index) = piece_type;
                    }
                    piece_position = {425, 150};
                    piece_type = get_random_type();
                    sweep_fallen_pieces(fallen_pieces);
                }
            }

        }

        float move_down_tick_max = 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            move_down_tick_max = 0.03f;
        }

        move_down_tick += delta_time;
        if (move_down_tick >= move_down_tick_max) {
            move_down_tick = 0;
            if (can_move_piece_down(piece_position, fallen_pieces, piece_type, rotation)) {
                piece_position.y += 25;
            }
            else {
                PieceBlocks blocks = get_blocks(piece_type, rotation);
                for (sf::Vector2i block_pos : blocks) {
                    int y_index = static_cast<int>((piece_position.y + block_pos.y * 25 - 50) / 25);
                    int x_index = static_cast<int>((piece_position.x + block_pos.x * 25 - 275) / 25);
                    fallen_pieces.at(y_index).at(x_index) = piece_type;
                }
                piece_position = {425, 150};
                piece_type = get_random_type();
                sweep_fallen_pieces(fallen_pieces);
            }
        }

        window.clear();

        sf::RectangleShape border({100, 600});
        border.setFillColor({0, 0, 90});
        border.setPosition({175, 50});
        window.draw(border);
        border.setPosition({525, 50});
        window.draw(border);
        border.setSize({250, 100});
        border.setPosition({275, 550});
        window.draw(border);

        window.draw(grid);

        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 10; x++) {
                if (fallen_pieces.at(y).at(x) != PieceType::None) {
                    sf::RectangleShape rect({25, 25});
                    rect.setFillColor(PieceColorMap.at(fallen_pieces.at(y).at(x)));
                    rect.setOutlineThickness(1);
                    rect.setPosition({x * 25 + 275, y * 25 + 50});
                    window.draw(rect);
                }
            }
        }

        sf::Vector2i calculated_drop = calculate_drop_position(piece_position, fallen_pieces, piece_type, rotation);
        draw_calculated_drop(window, piece_type, rotation, calculated_drop, 25);

        draw_piece(window, piece_type, rotation, piece_position, 25);

        window.display();

    }

    return 0;
}