#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Monster;

class Character {
    private:
        string name;
        int attack;
        int maxHealth;
        int maxMana;
        int defense;

        int currentHealth = maxHealth;
        int currentMana = maxMana;
        bool inParty = false;

        bool guarding = false;
        bool focusing = false;
        bool stunned = false;
        bool hitThisTurn = false;

        int overDamage = 0;
        int overTurns = 0;

        int bonusDefense = 0;

    protected:
        void increaseBaseStats(int amount);

        static void displaySkills(bool showIt) {
            if (showIt) {
                cout << "0. Cancel.\n\nChoose skill: ";
            } else {
                cout << "\n";
            }
        }

    public:
        Character(string name, int attack, int maxHealth, int maxMana, int defense);
        virtual ~Character();

        static Character* createCharacter();
        virtual void displayStats(ostream& os) const;

        virtual string getClassName() const {return "Unknown";}
        friend ostream& operator<<(ostream& os, const Character& character);
        bool operator==(const Character& other) const;

        //getters and setters
        string getName() const {return name;}
        int getHealth() const {return currentHealth;}
        int getMaxHealth() const {return maxHealth;}
        virtual int getAttack() const {return attack;}
        int getDefense() const {return defense + bonusDefense + (focusing ? 5 : 0);}
        int getMaxMana() const {return maxMana;}
        int getMana() const {return currentMana;}
        bool isAlive() const {return currentHealth > 0;}
        bool wasHitThisTurn() const {return hitThisTurn;}
        void clearHitStatus() {hitThisTurn = false;}
        bool isInParty() const {return inParty;}
        void setInParty(bool status) {inParty = status;}
        string ifActive() const {return inParty ? " [PARTY]" : "";}

        //stat changers
        void takeDamage(int dmg);
        void restoreHealth(int amountHealth);
        void setHealth(int amountHealth);
        void restoreMana(int amountMana);
        void restore();
        void heal();
        static int calculateDamage(double attack, double defense);

        //basic actions
        Monster* selectTarget(const vector<Monster*>& enemies) const;
        virtual void basicAttack(Character* target);
        void wait();

        void guard();
        void resetGuard(){guarding = false;}

        //skills
        virtual void onTurnStart() {}
        virtual void onWaveStart() {}
        virtual void showSkills(bool = true) const{}
        virtual bool useSkill(int actionSkill, vector<Character*>& activeParty, vector<Monster*>& enemies) {return false;}
        bool spendMana(int manaCost);

        void focus(){focusing = true;}
        void resetFocus(){focusing = false;}
        bool isFocusing() const {return focusing;}
        
        void getOverDamage(int dmgPerTurn, int turns);
        void applyOverDamage();

        void applyStun() {stunned = true; }
        bool isStunned() const {return stunned; }
        void clearStun() {stunned = false; }

        void halfDefense();

        virtual int getEvasion() const {return 0;}

        void addBonusDefense(int amount) {bonusDefense += amount;}
        void resetBonusDefense() {bonusDefense = 0;}
        Character* selectAlly(const vector<Character*>& activeParty) const;

        bool spendHealth(int healthCost);
        void reduceDefense(int amount);
        };

//------------------------------Class Stuff------------------------------

class Warrior : public Character {
    private:
        int rage;

    public:
        Warrior(string name, int rage = -1);
        string getClassName() const override {return "Warrior";}

        int getAttack() const override {return Character::getAttack() + (rage > 0 ? rage : 0);}

        void onTurnStart() override;
        void onWaveStart() override;
        void showSkills(bool showIt) const override;
        bool useSkill(int actionSkill, vector<Character*>& activeParty, vector<Monster*>& enemies) override;
};

class Mage : public Character {
    private:
        int manaRegen;

    public:
        Mage(string name, int manaRegen = 2);
        string getClassName() const override {return "Mage";}
        
        void onTurnStart() override;
        void showSkills(bool showIt) const override;
        bool useSkill(int actionSkill, vector<Character*>& activeParty, vector<Monster*>& enemies) override;
};

class Assassin : public Character {
    private:
        int evasion;
        
    public:
        Assassin(string name, int evasion = 25);
        string getClassName() const override {return "Assassin";}
        
        int getEvasion() const override;
        void onTurnStart() override;
        void onWaveStart() override;
        void showSkills(bool showIt) const override;
        bool useSkill(int actionSkill, vector<Character*>& activeParty, vector<Monster*>& enemies) override;
};

class Cleric : public Character {
    private:
        int divinePower;
        
    public:
        Cleric(string name, int divinePower = 2);
        string getClassName() const override {return "Cleric";}
        
        void onWaveStart() override;
        void showSkills(bool showIt) const override;
        bool useSkill(int actionSkill, vector<Character*>& activeParty, vector<Monster*>& enemies) override;
};

class Reaper : public Character {
    private:
        int lifeSteal;
        
    public:
        Reaper(string name, int lifeSteal = 0);
        string getClassName() const override {return "Reaper";}
        
        void onTurnStart() override;
        void basicAttack(Character* target) override;
        void showSkills(bool showIt) const override;
        bool useSkill(int actionSkill, vector<Character*>& activeParty, vector<Monster*>& enemies) override;
};