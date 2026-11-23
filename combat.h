#pragma once
#include "characters.h"
#include "monsters.h"
#include <vector>
using namespace std;

void displayColorName(const string& text, const Character* character, const Character* hasTurn, int colWidth);

class Combat {
    private:
        vector<Character*>& activeParty;
        vector<Monster*> enemies;

        void displayParty(Character* hasTurn) const;
        void displayEnemies(Character* hasTurn) const;
        void displayBattlefield(Character* hasTurn, bool clearScreen = true) const;
        void resetHitStatuses();

        void playerTurn();
        void enemyTurn();
        bool isPartyAlive() const;
        bool areEnemiesAlive() const;
        bool flee = false;
        int wave;

    public:
        //Combat(vector<Character*>& activeParty, vector<Monster*>& enemies);
        Combat(vector<Character*>& activeParty, vector<Monster*>& enemies, int currentWave);
        ~Combat();
        bool startCombat();
};