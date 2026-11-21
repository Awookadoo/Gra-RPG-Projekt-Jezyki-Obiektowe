#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Monster;

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

        bool guarding = false;
        bool focusing = false;

    public:
        Character(string name, int attack = 10, int maxHealth = 50, int maxMana = 10, int defense = 5);
        virtual ~Character();

        static Character* createCharacter();
        virtual void displayStats(ostream& os) const;

        virtual string getClassName() const {return "Unknown";}
        friend ostream& operator<<(ostream& os, const Character& character);

        //getters and setters
        string getName() const {return name;}
        int getHealth() const {return currentHealth;}
        int getMaxHealth() const {return maxHealth;}
        virtual int getAttack() const {return attack;}
        int getDefense() const {return defense;}
        int getMaxMana() const {return maxMana;}
        int getMana() const {return currentMana;}
        bool isAlive() const {return currentHealth > 0;}
        bool isInParty() const {return inParty;}
        void setInParty(bool status) {inParty = status;}
        string ifActive() const {return inParty ? " [PARTY]" : "";}

        //stat changers
        void takeDamage(int dmg);
        void restoreHealth(int amountHealth);
        void restoreMana(int amountMana);
        void restore();

        //basic actions
        Monster* selectTarget(const vector<Monster*>& enemies) const;
        void basicAttack(Character* target);
        void heal();
        void wait();

        void guard();
        void resetGuard(){guarding = false;}
        int isGuarding() const {return guarding;}
        int getTrueDEF() const;

        //skills
        virtual void onTurnStart() {}
        virtual void onTurnEnd() {}
        virtual void onWaveStart() {}
        virtual void showSkills() const{}
        virtual bool useSkill(int actionSkill, vector<Character*>& activeParty, vector<Monster*>& enemies) {return false;}

        void focus(){focusing = true;}
        void resetFocus(){focusing = false;}
        bool isFocusing() const {return focusing;}
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

        int getAttack() const override {return attack + rage;}

        void onTurnStart() override;
        void onWaveStart() override;
        void showSkills() const override;
        bool useSkill(int actionSkill, vector<Character*>& activeParty, vector<Monster*>& enemies) override;
};

class Mage : public Character {
    protected:
        int manaRegen;

    public:
        Mage(string name, int manaRegen = 0);
        void classDescription();
        void displayStats(ostream& os) const override;
        string getClassName() const override {return "Mage";}
        
        void showSkills() const override;
        bool useSkill(int actionSkill, vector<Character*>& activeParty, vector<Monster*>& enemies) override;
};

class Assassin : public Character {
    protected:
        int evasion;
        
    public:
        Assassin(string name, int evasion = 0);
        void classDescription();
        void displayStats(ostream& os) const override;
        string getClassName() const override {return "Assassin";}
        
        void showSkills() const override;
        bool useSkill(int actionSkill, vector<Character*>& activeParty, vector<Monster*>& enemies) override;
};