#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "characters.h"
#include "monsters.h"
using namespace std;

class Game {
    private:
        vector<Character*> party;
        vector<Character*> currentParty;
        vector<Monster*> monsterWave(int wave);

        bool running;
        void displayMainMenu() const;
        void mainMenu();
        void manageParty();
        int currentPartyCount() const;

    public:
        Game();
        ~Game();
        void run();
        void exit();
        void startGame();

        void addCharacter(Character* c) { party.push_back(c); }
};