#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "characters.h"
using namespace std;

class Monster : public Character{
protected:
    virtual void attackTarget(Character* target) = 0;

public:
    Monster(string name, int attack, int maxHealth, int defense);
    void takeTurn(vector<Character*>& playerParty);
    void statUp(int loop);
};

class Goblin : public Monster{
protected:
    void attackTarget(Character* target) override;
public:
    Goblin();
};

class Skeleton : public Monster{
protected:
    void attackTarget(Character* target) override;
public:
    Skeleton();
};