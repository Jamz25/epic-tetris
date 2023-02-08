#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>

#include "Piece.hpp"
#include "PieceGrid.hpp"
#include "PlayerPiece.hpp"


int main() {

    srand((unsigned)time(0));

    sf::RenderWindow window(sf::VideoMode({650, 600}), "Tetris");
    sf::Clock clock;

    sf::Font font;
    if (!font.loadFromFile("resources/LLPIXEL3.ttf"))
        return -1;
    sf::Text line_text;
    line_text.setFont(font);
    line_text.setFillColor(sf::Color(255, 255, 255));
    line_text.setPosition(sf::Vector2f(340, 150));

    PieceGrid piece_grid(sf::Vector2i(50, 50));
    PlayerPiece player_piece(piece_grid);

    if (!piece_grid.load_textures())
        return -1;
    
    
    while (window.isOpen()) {

        float delta_time = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed) {

                if (event.key.code == sf::Keyboard::J) {
                    player_piece.rotate(piece_grid, -1);
                }

                if (event.key.code == sf::Keyboard::K) {
                    player_piece.rotate(piece_grid, 1);
                }

                if (event.key.code == sf::Keyboard::A) {
                    player_piece.move_horizontal(piece_grid, PieceMove::Left);
                }

                if (event.key.code == sf::Keyboard::D) {
                    player_piece.move_horizontal(piece_grid, PieceMove::Right);
                }

                if (event.key.code == sf::Keyboard::W) {
                    player_piece.hard_drop(piece_grid);
                }

            }

        }

        if (player_piece.is_game_over()) {

            piece_grid.empty_grid();
            player_piece.reset(piece_grid);

        }

        player_piece.update(delta_time, piece_grid);

        line_text.setString("Lines cleared: " + std::to_string(player_piece.get_lines_cleared()));

        window.clear();
        
        piece_grid.draw_grid(window);

        piece_grid.draw_piece_drop(window, player_piece);

        piece_grid.draw_player_piece(window, player_piece);

        window.draw(line_text);

        window.display();

    }

    return 0;
}