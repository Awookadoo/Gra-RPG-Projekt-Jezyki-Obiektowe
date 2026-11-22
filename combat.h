#pragma once
#include "characters.h"
#include "monsters.h"
#include "utility.h"
#include "game.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;


class Combat {
    private:
        vector<Character*>& activeParty;
        vector<Monster*> enemies;

        void displayParty(Character* hasTurn) const;
        void displayEnemies(Character* hasTurn) const;
        void displayBattlefield(Character* hasTurn) const;

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