#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "characters.h"
using namespace std;

class Game {
    private:
        //wektory drużyny
        vector<Character*> party;
        vector<Character*> currentParty;

        bool running;
        void mainMenu();
        void manageParty();

    public:
        Game();
        ~Game();
        void run();
};