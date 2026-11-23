#pragma once
#include <vector>
#include "characters.h"
#include "monsters.h"
using namespace std;

class Game {
    private:
        vector<Character*> party;
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
        void startGame();

        bool addCharacter(Character* c);
};