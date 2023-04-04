//
// Created by user on 3/27/2023.
//

#ifndef TETRIS_BLOCK_H
#define TETRIS_BLOCK_H

#include <SFML/Graphics.hpp>
#include <cstdlib>

class Block {
private:
    sf::Texture blockPic;
    sf::Sprite blockSprite;
    
    //Variables
    static const int fieldHeight = 20;
    static const int fieldWidth = 10;

    int field[20][10] = {0};
    
    void InitVariables();
    void InitShape(); //Get shape of block and its sprite.
public:
    Block();
    virtual ~Block();
    
    //Variables
    int dx = 0; //Direction of x, determines move direction of block. -1 (left) or 1 (right).
    bool rotateFlag = false; //Signal if this block should rotate. (Clockwise 90 degrees)
    int colorNum = 1; //
    
    //Point represents the four points of a shape in Tetris.
    //The position of the shape. We'll need to check this to make sure we stay in bounds.
    struct Point{
        int x,y;
    } currPos[4], prevPos[4];
    
    //An array made to create the shapes for each tetris block.
    //There are 7 shapes and 4 tiles per shape.
    //A for loop will only need to mess with the 2nd dimension
    //of the array. The first dimension tells us which shape we want.
    //So this works cleverly:
    //currPos[whatever].x and currPos[whatever].y
    //both use the same number to find the final position.
    //currPos[i].x = shape[shapeID][i] % 2; results in either 0 or 1
    //currPos[i].y = shape[shapeID][i] / 2; results in 0->literally anything
    //
    //So if x can only be 0 or 1 then that means the size is 2 squares wide max
    //(in default position). When we move the block over to the right, x will become
    //bigger than 1 because our new position is determined in multiples of 18
    //(the size of one square). We can't smoothly transition between those two positions
    //or we'll get into some serious trouble fitting pieces together.
    // 
    // IN SUMMARY
    // In the array below, odd numbers result in x = 1, even numbers result inx = 0
    // y coordinates are determined by how big the number is.
    // Usually maxes out at y = 4 (so numbers in the array won't go above 8)
    // Read above for a more full explanation
    int shape[7][4] =
    {
        1,3,5,7, // I
        2,4,5,7, // Z
        3,5,4,6, // S
        3,5,4,7, // T
        2,3,5,7, // L
        3,5,7,6, // J
        2,3,4,5, // O
    };
    
    void MakeShape(sf::RenderTarget* targetWindow);

    void Move();
    void RotateShape();
    void Tick(float timer, float delay); 
    bool Check(); //Check if piece is out of bounds.
    void FirstPiece(); //Make the first piece before anything starts.
    void Update();
    void Render(sf::RenderTarget* targetWindow);
};


#endif //TETRIS_BLOCK_H
