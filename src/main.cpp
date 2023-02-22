#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <memory>

#include "Piece.hpp"
#include "PieceGrid.hpp"
#include "PlayerPiece.hpp"
#include "SpriteManager.hpp"
#include "UIManager.hpp"


void process_events(sf::RenderWindow& window, PlayerPiece& player_piece, PieceGrid& piece_grid);

void process_keyboard_events_(sf::Event& event, PlayerPiece& player_piece, PieceGrid& piece_grid);

void process_joystick_events_(sf::RenderWindow& window, sf::Event& event, PlayerPiece& player_piece, PieceGrid& piece_grid);


int main() {


    srand((unsigned)time(0));


    sf::RenderWindow window(sf::VideoMode({800, 600}), "Epic Tetris");
    window.setFramerateLimit(60);
    sf::Clock clock;


    sf::Image icon;
    if (!icon.loadFromFile("resources/icon.png"))
        return -1;
    window.setIcon(icon.getSize(), icon.getPixelsPtr());


    std::shared_ptr<SpriteManager> sprite_manager_sptr = std::make_shared<SpriteManager>(SpriteManager());

    UIManager ui_manager(sprite_manager_sptr);
    PieceGrid piece_grid(sf::Vector2i(275, 50), sprite_manager_sptr);
    PlayerPiece player_piece(piece_grid);


    if (!sprite_manager_sptr->load_sprites())
        return -1;

    if (!ui_manager.load_font())
        return -1;
    
    
    while (window.isOpen()) {

        float delta_time = clock.restart().asSeconds();

        process_events(window, player_piece, piece_grid);

        if (player_piece.is_game_over()) {

            piece_grid.empty_grid();
            player_piece.reset(piece_grid);

        }

        player_piece.update(delta_time, piece_grid);

        piece_grid.update(delta_time);

        window.clear();
        
        piece_grid.draw_grid(window);

        piece_grid.draw_piece_drop(window, player_piece);

        piece_grid.draw_player_piece(window, player_piece);

        ui_manager.draw_text(window, "Lines cleared", sf::Vector2f(550, 330), sf::Color(255, 255, 255), 30);

        ui_manager.draw_text(window,
            std::to_string(player_piece.get_lines_cleared()),
            sf::Vector2f(550, 365),
            sf::Color(255, 255, 255),
            30
        );

        ui_manager.draw_text(window, "Score", sf::Vector2f(550, 450), sf::Color(255, 255, 255), 30);

        ui_manager.draw_text(window,
            std::to_string(player_piece.get_score()),
            sf::Vector2f(550, 485),
            sf::Color(255, 255, 255),
            30
        );

        ui_manager.draw_piece_queue(window, sf::Vector2f(550, 70), player_piece.get_piece_queue());

        ui_manager.draw_hold_piece(window, sf::Vector2f(155, 70), player_piece.get_held_piece_type(), player_piece.can_swap_hold());

        window.display();

    }

    return 0;
}


void process_events(sf::RenderWindow& window, PlayerPiece& player_piece, PieceGrid& piece_grid) {

    sf::View view = window.getDefaultView();

    sf::Event event;
    while (window.pollEvent(event)) {

        if (event.type == sf::Event::Closed)
            window.close();
        
        if (event.type == sf::Event::Resized) {
            // resize my view
            view.setSize({
                    static_cast<float>(event.size.width),
                    static_cast<float>(event.size.height)
            });
            window.setView(view);
            // and align shape
        }
        
        process_keyboard_events_(event, player_piece, piece_grid);

        process_joystick_events_(window, event, player_piece, piece_grid);

    }

}


void process_keyboard_events_(sf::Event& event, PlayerPiece& player_piece, PieceGrid& piece_grid) {

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
            
            case (sf::Keyboard::Q):
                player_piece.swap_held_piece();
                break;

        }

    }

}


void process_joystick_events_(sf::RenderWindow& window, sf::Event& event, PlayerPiece& player_piece, PieceGrid& piece_grid) {

    if (event.type == sf::Event::JoystickButtonPressed) {

        switch (event.joystickButton.button) {

            case (0):
                player_piece.rotate(piece_grid, -1);
                break;
            
            case (1):
                player_piece.rotate(piece_grid, 1);
                break;
            
            case (4):
                player_piece.swap_held_piece();
                break;
            
            case (7):
                window.close();
                break;

        }

    }

    if (event.type == sf::Event::JoystickMoved) {

        sf::Joystick::Axis axis = event.joystickMove.axis;
        float position = event.joystickMove.position;

        if (axis == 7 && position > 0) {
            player_piece.hard_drop(piece_grid);
        }

        if (axis == 6 && position < 0) {
            player_piece.move_horizontal(piece_grid, PieceMove::Left);
        }

        if (axis == 6 && position > 0) {
            player_piece.move_horizontal(piece_grid, PieceMove::Right);
        }

    }

}