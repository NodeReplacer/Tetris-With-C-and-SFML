//
// Created by user on 3/27/2023.
//

#include <iostream>
#include "Game.h"

void Game::initWindow() {
    this->windowDimensions.height = 480;
    this->windowDimensions.width = 320;
    this->window = new sf::RenderWindow(this->windowDimensions, "Tetris");
    this->window->setFramerateLimit(60);
}

void Game::initVariables() {
    this->timer = 0;
    this->delay = 0.3;
    srand(time(0));
}

Game::Game() {
    this->initVariables();
    this->initWindow();
}

Game::~Game() {
    
}

const bool Game::running() const {
    return this->window->isOpen(); // && this->gameEnded == false;
}

//Public functions

//Handle the game and window events.
//We'll include player events here as well.
//I am thoroughly regretting making these individual classes instead of just
//keeping them all in main.cpp.
void Game::PollEvents() {
    while (this->window->pollEvent(this->event))
    {
        switch (this->event.type)
        {
            // Window closed
            case sf::Event::Closed:
                this->window->close();
                break;

                // Key pressed
            case sf::Event::KeyPressed:
                // Which key was pressed?
                switch (this->event.key.code)
                {
                    case sf::Keyboard::Escape:
                        // Close the window.
                        this->window->close();
                        break;
                    case sf::Keyboard::Left:
                        this->block.dx = -1;
                        break;
                    case sf::Keyboard::Right:
                        this->block.dx = 1;
                        break;
                    case sf::Keyboard::Up:
                        this->block.rotateFlag = true;
                        break;
                    // Default case (unrecognized key was pressed).
                    default:
                        break;
                } // END of switch (event.key.code)

            default:
                break;

        } // END OF switch (event.type)
    }
}

void Game::Update() {
    //Make absolute certain that clock updates occur first.
    float time = this->clock.getElapsedTime().asSeconds();
    this->clock.restart();
    this->timer+=time;
    
    this->PollEvents();
    
    this->block.Move();
    this->block.RotateShape();
    if (timer>delay) {
        this->block.Tick(this->timer, this->delay);
        timer=0;
    }
}

void Game::Render() {
    /*
        - Clear old frame
        - Render objects (like the player)
        - Display frame in window
    */
    
    //Clear window, prep for drawing
    this->window->clear(sf::Color::White);
    
    //Render objects and other things here.
    this->block.Render(this->window);
    
    //Display the window
    this->window->display();
}
