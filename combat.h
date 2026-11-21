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

    void displayParty() const;
    void displayEnemies() const;
    void displayBattlefield() const;

    void playerTurn();
    void enemyTurn();
    bool isPartyAlive() const;
    bool areEnemiesAlive() const;
    bool flee = false;

public:
    Combat(vector<Character*>& activeParty, vector<Monster*>& enemies);
    ~Combat();
    bool startCombat();
};