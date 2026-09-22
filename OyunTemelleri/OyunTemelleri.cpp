#include "GameManager.h"
#include <cstdlib>
#include <ctime>

int main() {

    srand(static_cast<unsigned int>(time(nullptr)));
    GameManager game;
    game.startGame();
    
    return 0;
}