#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <memory>

#include "Piece.hpp"
#include "PieceGrid.hpp"
#include "PlayerPiece.hpp"
#include "SpriteManager.hpp"


int main() {

    srand((unsigned)time(0));

    sf::RenderWindow window(sf::VideoMode({650, 600}), "Tetris");
    window.setFramerateLimit(60);
    sf::Clock clock;



    sf::Image icon;
    if (!icon.loadFromFile("resources/icon.png"))
        return -1;
    window.setIcon(icon.getSize(), icon.getPixelsPtr());

    sf::Font font;
    if (!font.loadFromFile("resources/LLPIXEL3.ttf"))
        return -1;
    sf::Text line_text;
    line_text.setFont(font);
    line_text.setFillColor(sf::Color(255, 255, 255));



    std::shared_ptr<SpriteManager> sprite_manager_sptr = std::make_shared<SpriteManager>(SpriteManager());
    if (!sprite_manager_sptr->load_sprites(GRID_SIZE))
        return -1;

    PieceGrid piece_grid(sf::Vector2i(50, 50), sprite_manager_sptr);
    PlayerPiece player_piece(piece_grid);
    
    
    while (window.isOpen()) {

        float delta_time = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed) {

                switch (event.key.code) {

                    case (sf::Keyboard::J):
                        player_piece.rotate(piece_grid, -1);
                        break;
                    
                    case (sf::Keyboard::K):
                        player_piece.rotate(piece_grid, 1);
                        break;
                    
                    case (sf::Keyboard::A):
                        player_piece.move_horizontal(piece_grid, PieceMove::Left);
                        break;
                    
                    case (sf::Keyboard::D):
                        player_piece.move_horizontal(piece_grid, PieceMove::Right);
                        break;
                    
                    case (sf::Keyboard::W):
                        player_piece.hard_drop(piece_grid);
                        break;

                }

            }

        }

        if (player_piece.is_game_over()) {

            piece_grid.empty_grid();
            player_piece.reset(piece_grid);

        }

        player_piece.update(delta_time, piece_grid);

        line_text.setPosition(sf::Vector2f(340, 150));
        line_text.setString("Lines cleared: " + std::to_string(player_piece.get_lines_cleared()));

        window.clear();
        
        piece_grid.draw_grid(window);

        piece_grid.draw_piece_drop(window, player_piece);

        piece_grid.draw_player_piece(window, player_piece);

        window.draw(line_text);

        line_text.setPosition(sf::Vector2f(10, 10));
        line_text.setString(std::to_string(static_cast<int>(1 / delta_time)) + " fps");
        window.draw(line_text);

        window.display();

    }

    return 0;
}