#include "characters.h"
#include "monsters.h"
#include "utility.h"
#include "game.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;

int main(){
    srand(static_cast<unsigned int>(time(0)));
    Game game;
    game.run();
    return 0;
}