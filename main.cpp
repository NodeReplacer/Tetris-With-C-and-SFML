#include <iostream>
#include "Game.h"

//This program would have been easier to make procedural programming
//instead of object-oriented. Lesson learned.

int main() {
    Game game;
    
    //Initialize a random seed using time.
    srand(static_cast<unsigned>(time(0)));
    
    while (game.running()) {
        game.Update();
        game.Render();
    }
    return 0;
}
