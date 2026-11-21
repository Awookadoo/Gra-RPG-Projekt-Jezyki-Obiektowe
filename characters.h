#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Character {
    protected:
        string name;
        int attack = 10;
        int maxHealth = 30;
        int maxMana = 10;
        int defense = 5;

        int currentHealth = maxHealth;
        int currentMana = maxMana;
        bool inParty = false;

    public:
        Character(string name, int attack = 10, int maxHealth = 50, int mana = 10, int defense = 5);
        virtual ~Character();

        static Character* createCharacter();
        virtual void displayStats(ostream& os) const;

        virtual string getClassName() const {return "Unknown";};
        friend ostream& operator<<(ostream& os, const Character& character);

        string getName() const {return name;}
        int getHealth() const {return currentHealth;}
        int getMaxHealth() const {return maxHealth;}
        int getAttack() const {return attack;}
        int getDefense() const {return defense;}
        int getMaxMana() const {return maxMana;}
        int getMana() const {return currentMana;}
        bool isAlive() const {return currentHealth > 0;}
        bool isInParty() const {return inParty;}
        void setInParty(bool status) {inParty = status;}
        string ifActive() const {return inParty ? " [PARTY]" : "";}

        void takeDamage(int dmg);

        void restore();
};

//------------------------------Class Stuff------------------------------

class Warrior : public Character {
    protected:
        int rage;

    public:
        Warrior(string name, int rage = 0);
        void classDescription();
        void displayStats(ostream& os) const override;
        string getClassName() const override {return "Warrior";}
};

class Mage : public Character {
    protected:
        int manaRegen;

    public:
        Mage(string name, int manaRegen = 0);
        void classDescription();
        void displayStats(ostream& os) const override;
        string getClassName() const override {return "Mage";}
};

class Assassin : public Character {
    protected:
        int evasion;
        
    public:
        Assassin(string name, int evasion = 0);
        void classDescription();
        void displayStats(ostream& os) const override;
        string getClassName() const override {return "Assassin";}
};