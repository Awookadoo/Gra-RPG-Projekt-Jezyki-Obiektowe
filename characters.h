#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum class CharacterClass {
    Warrior,
    Mage,
    Archer
};

class Character {
    private:
        string name;
        CharacterClass characterClass;
        int attack = 10;
        int maxHealth = 50;
        int mana = 10;
        int defense = 5;

    public:
        Character(string name, CharacterClass characterClass, int attack = 10, int maxHealth = 50, int mana = 10, int defense = 5);
        virtual ~Character();

        static Character* createCharacter();
        virtual void displayStats(ostream& os) const;

        string getClassName() const;
        friend ostream& operator<<(ostream& os, const Character& character);
};



//------------------------------Warrior Stuff------------------------------

class Warrior : public Character {
    private:
        int rage;

    public:
        Warrior(string name, int rage);
        void classDescription();
        void displayStats(ostream& os) const override;
};

class Mage : public Character {
    private:
        int spellPower;
};

class Archer : public Character {
    private:
        int agility;
};