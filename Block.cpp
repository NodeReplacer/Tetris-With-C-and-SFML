//
// Created by user on 3/27/2023.
//

#include "Block.h"

void Block::InitVariables() {
    //On my system random_device doesn't create true randomness between separate program runs. So I'm relying on this instead.
    //std::random_device randDevice;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    
    this->randomGenerator = std::mt19937(seed);
    this->randDistribution = std::uniform_int_distribution<int>(0,6);
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

bool Block::Check() {
    /*
        Check if a block is still in the field. CheckLine() might seem like it overlaps. But CheckLine() checks
        for a completed line.
    */
    for (int i=0;i<4;++i) {
        if (this->currPos[i].x<0 || this->currPos[i].x>=fieldWidth || this->currPos[i].y>=fieldHeight)
            return false;
        else if (field[this->currPos[i].y][this->currPos[i].x])
            return false;
    }
    return true;
}

void Block::RenderField(sf::RenderTarget *targetWindow) {
    //Draw the blocks that have fallen and settled on the ground
    for (int i=0;i<fieldHeight;++i) {
        for (int j=0;j<fieldWidth;++j) {
            if (field[i][j]==0) {
                continue;
            }
            this->blockSprite.setTextureRect(sf::IntRect(this->field[i][j]*18,0,18,18));
            this->blockSprite.setPosition(j*18,i*18);
            this->blockSprite.move(28,31); // Offset to align with background.
            targetWindow->draw(this->blockSprite);
        }
    }
#if 0
    // Print the array with indices
    std::cout << "   ";
    for (int j = 0; j < fieldWidth; j++) {
        std::cout << j << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < fieldHeight; i++) {
        std::cout << i << ": ";
        for (int j = 0; j < fieldWidth; j++) {
            std::cout << this->field[i][j] << " ";
        }
        std::cout << std::endl;
    }
#endif
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
    
    //Draw the current falling block.
    for (int i=0;i<4;i++) {
        //Okay now that we have the coordinates of each square
        //to make up one shape we actually have to convert our pseudo coordinates (which
        //are tiny) to onscreen coordinates. Our individual squares are 18*18
        //pixels big so that's our multiplication.
        //I don't know how to stretch the size of these images, but
        //I would like to. This is tiny.
        this->blockSprite.setTextureRect(sf::IntRect(this->colorNum*18,0,18,18));
        this->blockSprite.setPosition(this->currPos[i].x*18,this->currPos[i].y*18);
        this->blockSprite.move(28,31); // Offset to align with background.
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

float Block::Tick(float timer, float delay) {
    /*
        Handle the timer tick for blocks here.
        timer and delay will be passed in from Game.cpp's Game::Update()
        function.
        
        Returns the timer. If timer>delay (the requirement for a tick to happen)
        is true then the timer is set to 0 first.
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

            int n = this->randDistribution(this->randomGenerator);
            //this->colorNum = 1+std::rand()%7;
            this->colorNum = 1+n;
            for (int i=0;i<4;++i) {
                currPos[i].x = shape[n][i] % 2;
                currPos[i].y = shape[n][i] / 2;
            }
        }
        timer = 0;
    }
    return timer;
}

void Block::CheckLine() {
    //Starting from the bottom of the field.
    //WARNING: SFML does y positions upside down. So increasing lineContainer will actually move downwards and
    //subtracting will move up.
    int lineContainer = this->fieldHeight-1;
    for (int i=this->fieldHeight-1;i>0;--i) {
        //Count if the index at field[i][j] is filled or not.
        int count = 0; 
        for (int j=0;j<this->fieldWidth;++j) {
            if (this->field[i][j]) 
                ++count;
            //Copy the current space to the row indicated by lineContainer.
            //If lineContainer is below i then this moves the row we are looking at down.
            this->field[lineContainer][j] = this->field[i][j];
        }
        if (count<this->fieldWidth)
            //Then LineContainer stays behind on the row below i.
            --lineContainer;
    }
}

void Block::FirstPiece() {
    if (!this->Check()) {
        int n = this->randDistribution(this->randomGenerator);
        //this->colorNum = 1+std::rand()%7;
        this->colorNum = 1+n;
        for (int i=0;i<4;++i) {
            currPos[i].x = shape[n][i] % 2;
            currPos[i].y = shape[n][i] / 2;
        }
    }
}

void Block::Update() {
    
}

void Block::Render(sf::RenderTarget* targetWindow) {
    this->RenderField(targetWindow);
    this->MakeShape(targetWindow);
}