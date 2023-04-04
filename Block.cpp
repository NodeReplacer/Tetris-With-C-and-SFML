//
// Created by user on 3/27/2023.
//

#include <iostream>
#include "Block.h"

void Block::InitVariables() {
    //Initialize a random seed using time.
    srand(static_cast<unsigned>(time(0)));
    this->currPos[0].x = 0;
    this->FirstPiece();
}

void Block::InitShape() {
    this->blockPic.loadFromFile("images/tiles.png");
    this->blockSprite.setTexture(blockPic);
    this->blockSprite.setTextureRect(sf::IntRect(0,0,18,18));
}

Block::Block() {
    this->InitVariables();
    this->InitShape();
}

Block::~Block() {
    
}

void Block::MakeShape(sf::RenderTarget* targetWindow) {
    //Given the current position, we want to stamp the sprite we are currently using
    //four times to create one whole tetris block.
    
    /*
    //This n will be the first dimension of the 2D shape array (shape[7][4] if that helps).
    //The first dimension tells us which piece we want: I, Z, S, T, L, J, or O.
    //I don't know if an enum can handle this requirement.
    int n=3; //We call it n, but a more descriptive name is 'shapeID'.
    
    //Now that we have snagged our shapeID (n)
    //We have to figure out the coordinate of each cube in the tetris block.
    //We are filling it in one square at a time, like it's pixel art.
    if (this->currPos[0].x==0)
    for (int i=0;i<4;i++)
    {
        //So why is currPos[i]'s value for 'x' %2?
        //According to the way the original tetris did it:
        //Each block is a maximum of 2 squares wide. 0 is left, 1 is right.
        //%2 gives the remainder which is either 0 or 1.
        //To save space we (well this is how THEY saved space) 
        //divide the number at the exact same index in shape[7][4] by 2.
        //maximum is 4 blocks so you won't see any number in the 2D shape array
        //greater than 8.
        this->currPos[i].x = this->shape[n][i] % 2;
        this->currPos[i].y = this->shape[n][i] / 2;
    }
    */
    
    //Clean those movement variables immediately after we don't need them anymore.
    
    this->dx=0;
    this->rotateFlag = false;
    
    //Draw the blocks that have fallen and settled on the ground
    for (int i=0;i<fieldHeight;++i) {
        for (int j=0;j<fieldWidth;++j) {
            if (field[i][j]==0) {
                continue;
            }
            this->blockSprite.setPosition(j*18,i*18);
            targetWindow->draw(this->blockSprite);
        }
    }
    
    //Draw the current falling block.
    for (int i=0;i<4;i++) {
        //Okay now that we have the coordinates of each square
        //to make up one shape we actually have to convert our pseudo coordinates (which
        //are tiny) to onscreen coordinates. Our individual squares are 18*18
        //pixels big so that's our multiplication.
        //I don't know how to stretch the size of these images, but
        //I would like to. This is tiny.
        this->blockSprite.setPosition(this->currPos[i].x*18,this->currPos[i].y*18);
        targetWindow->draw(this->blockSprite);
    }
    
}

void Block::Move() {
    for (int i=0;i<4;++i) {
        //If you remember that our true x position is currPos[i].x * 18
        //then you can see that all we need to do is modify x on the ground
        //level to move our piece instead of multiplying dx by anything.
        this->prevPos[i] = currPos[i];
        this->currPos[i].x += this->dx;
        //We're out of bounds. Revert to previous position.
    }
    if (!this->Check()) {
        for (int i=0;i<4;++i) {
            currPos[i] = prevPos[i];
        }
    }
}

void Block::RotateShape() {
    if (rotateFlag) {
        Point rotationPoint = this->currPos[1]; //The center of our rotation
        //We'll be changing currPos[1] but still need its old values to rotate
        //the rest of the block so it's best to store them safely.
        for (int i = 0; i < 4; ++i) {
            //clockwise rotation
            /*
            Usually rotations require cos(theta) and sin(theta) but because
            we rotate around 90 degrees those will be easy ignorable whole numbers.
            Therefore to rotate 90 degrees clockwise:
            Let (p_x,p_y) be the coordinates of the center of rotation
            Let (x,y) be the block cells relative to the center of rotation.
            x' = p_x - (y - p_y)
            y' = p_y + (x - p_x)
            
            The true formula looks like this:
            x' = (p_x - x) * cos(theta) - (p_y - y) * sin(theta) + x
            y' = (p_x - x) * sin(theta) + (p_y - y) * cos(theta) + y
             
            but cos(90 degrees) is 0. sin(90 degrees) is 1. So sub them in above
            and entire sections of that formula vanish.
            */
            int x = this->currPos[i].y - rotationPoint.y;
            int y = this->currPos[i].x - rotationPoint.x;
            this->currPos[i].x = rotationPoint.x - x;
            this->currPos[i].y = rotationPoint.y + y;
        }
        
    }
}

bool Block::Check() {
    for (int i=0;i<4;++i) {
        if (this->currPos[i].x<0 || this->currPos[i].x>=fieldWidth || this->currPos[i].y>=fieldHeight)
            return false;
        else if (field[this->currPos[i].y][this->currPos[i].x])
            return false;
    }
    return true;
}

void Block::FirstPiece() {
    if (!this->Check()) {
        this->colorNum = 1+std::rand()%7;
        int n = std::rand()%7;
        for (int i=0;i<4;++i) {
            currPos[i].x = shape[n][i] % 2;
            currPos[i].y = shape[n][i] / 2;
        }
    }
}

void Block::Update() {
    this->InitVariables();
    this->InitShape();
}

void Block::Render(sf::RenderTarget* targetWindow) {
    this->MakeShape(targetWindow);
}

void Block::Tick(float timer, float delay) {
    /*
        Handle the timer tick for blocks here.
        timer and delay will be passed in from Game.cpp's Game::Update()
        function.
    */
    //delay default is set to 0.3. Timer is measured in seconds so every 0.3 seconds.
    if (timer>delay) {
        //Move all parts of a piece down 1 "square".
        for (int i=0; i<4; ++i) {
            this->prevPos[i] = this->currPos[i];
            ++this->currPos[i].y;
        }
        //If the piece is out of bounds.
        if (!this->Check()) {
            for (int i=0;i<4;++i){
                field[this->prevPos[i].y][this->prevPos[i].x] = this->colorNum;
            }
            
            this->colorNum = 1+std::rand()%7;
            int n = std::rand()%7;
            for (int i=0;i<4;++i) {
                currPos[i].x = shape[n][i] % 2;
                currPos[i].y = shape[n][i] / 2;
            }
        }
        timer = 0;
    }
}