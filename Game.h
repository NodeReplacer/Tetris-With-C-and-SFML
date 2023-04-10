//
// Created by user on 3/27/2023.
//

//I want to make the pieces/players/enemies include Game.h
//Not the other way around like last time because that was a confusing
//association to make.

#include "Block.h"

#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

class Game {
private:
    //Variables
    bool gameEnded;
    float timer = 0;
    float delay = 0.3;
    
    sf::Clock clock; //Tracks the time for block spawning and anything else time reliant.
    /*
    static const int fieldHeight = 20;
    static const int fieldWidth = 10;

    int field[20][10] = {0};
    */
    
    //Window
    //A pointer because the window will already exist. Save space by not copying the entire
    //thing.
    sf::RenderWindow* window;
    //Get events for the window game such as closing the window.
    //Player actions will be put elsewhere.
    sf::Event event;
    //The x,y dimensions of the window
    sf::VideoMode windowDimensions;

    //Game Objects
    Block block;
    sf::Sprite background;
    sf::Texture bgTexture;
    
    //Private functions. Technically constructors.
    void initWindow();
    void initVariables();
public:
    //Constructor/Destructor
    Game();
    virtual ~Game();
    
    //Accessors
    //Tell other programs that the game is running
    const bool running() const;
    
    //Functions - Handle backend.
    void PollEvents();
    //void MakeBlock(); //Function migrated to Block.h
    
    void Update();
    void Render();
};


#endif //TETRIS_GAME_H
