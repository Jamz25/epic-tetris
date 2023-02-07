#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>

#include "Piece.hpp"
#include "PieceGrid.hpp"
#include "PlayerPiece.hpp"


int main() {

    srand((unsigned)time(0));

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Game");
    sf::Clock clock;

    PlayerPiece player_piece;
    PieceGrid piece_grid(sf::Vector2i(50, 50));

    while (window.isOpen()) {

        float delta_time = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed) {

                if (event.key.code == sf::Keyboard::Space) {
                    player_piece.rotate(piece_grid);
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

        player_piece.update(delta_time, piece_grid);

        std::cout << player_piece.get_grid_pos().x << ", " << player_piece.get_grid_pos().y << "\n";

        window.clear();
        
        piece_grid.draw_grid(window);

        piece_grid.draw_piece_drop(window, player_piece);

        piece_grid.draw_player_piece(window, player_piece);

        window.display();

    }

    return 0;
}