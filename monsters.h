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
        virtual void takeTurn(vector<Character*>& playerParty);
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

class Slime : public Monster{
    protected:
        void attackTarget(Character* target) override;
    public:
        Slime();
};

class Ghoul : public Monster{
    protected:
        void attackTarget(Character* target) override;
    public:
        Ghoul();
};

class Rat : public Monster{
    protected:
        void attackTarget(Character* target) override;
    public:
        Rat();
};