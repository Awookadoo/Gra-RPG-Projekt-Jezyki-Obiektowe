#include "characters.h"
#include "monsters.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
using namespace std;


Monster::Monster(string name, int attack, int maxHealth, int defense) : Character(name, attack, maxHealth, 0, defense){}

void Monster::takeTurn(vector<Character*>& playerParty){
    Character* target = nullptr;
    for (Character* hero : playerParty) {
        if (hero->isAlive() && hero->isFocusing()) {
            target = hero;
            break;
        }
    }

    if (target == nullptr) {
        vector<Character*> aliveCharacters;
        for (Character* hero : playerParty){
            if (hero->isAlive()){
                aliveCharacters.push_back(hero);
            }
        }
        if (aliveCharacters.empty()){
            return;
        }

        int randomAttack = rand() % aliveCharacters.size();
        target = aliveCharacters[randomAttack];
    }

    int roll = rand() % 100;
    if (roll < target->getEvasion()) {
        cout << "\n" << target->getName() << " dodges the attack from " << getName() << "!\n";
        return;
    }

    attackTarget(target);
    if (!target->isAlive()) {
        cout << target->getName() << " has fallen!\n";
    }
}

void Monster::statUp(int loop){
    if (loop <= 0){
        return;
    }
    increaseBaseStats(loop);
}

//------------------------------Monster Types------------------------------

Goblin::Goblin() : Monster("Goblin", 10, 30, 2) {}

void Goblin::attackTarget(Character* target) {
    int dmg = Character::calculateDamage(getAttack(), target->getDefense());
    cout << "\n" << getName() << " stabbed " << target->getName() << " for " << dmg << " damage!\n";
    target->takeDamage(dmg);
}

Skeleton::Skeleton() : Monster("Skeleton", 13, 25, 4) {}

void Skeleton::attackTarget(Character* target) {
    int specialAttack = rand() % 100;
    if (specialAttack < 25){
        int dmg = Character::calculateDamage(getAttack(), target->getDefense() * 0.5);
        cout << "\n" << getName() << " shot " << target->getName() << " and dealt " << dmg << " while piercing through half of their DEF!\n";
        target->takeDamage(dmg);
    }
    else{
        int dmg = Character::calculateDamage(getAttack(), target->getDefense());
        cout << "\n" << getName() << " shot " << target->getName() << " and dealt " << dmg << " damage!\n";
        target->takeDamage(dmg);
    }
}

Slime::Slime() : Monster("Slime", 8, 30, 4) {}

void Slime::attackTarget(Character* target) {
    int specialAttack = rand() % 100;
    if (specialAttack < 40){
        int dmg = Character::calculateDamage(getAttack(), target->getDefense());
        cout << "\n" << getName() << " oozed over " << target->getName() << " to deal " << dmg << " damage and stun them for the next turn!\n";
        target->takeDamage(dmg);
        target->applyStun();
    }
    else{
        int dmg = Character::calculateDamage(getAttack(), target->getDefense());
        cout << "\n" << getName() << " oozed over " << target->getName() << " to deal " << dmg << " damage!\n";
        target->takeDamage(dmg);
    }
}

Ghoul::Ghoul() : Monster("Ghoul", 15, 40, 10) {}

void Ghoul::attackTarget(Character* target) {
    int specialAttack = rand() % 100;
    if (specialAttack < 50){
        int dmg = Character::calculateDamage(getAttack(), target->getDefense());
        cout << "\n" << getName() << " bit " << target->getName() << " and dealt " << dmg << " while healing himself for 5 health!\n";
        target->takeDamage(dmg);
        restoreHealth(5);
    }
    else{
        int dmg = Character::calculateDamage(getAttack(), target->getDefense());
        cout << "\n" << getName() << " bit " << target->getName() << " and dealt " << dmg << " damage!\n";
        target->takeDamage(dmg);
    }
}

Rat::Rat() : Monster("Rat", 1, 1, 0) {}

void Rat::attackTarget(Character* target) {
    int specialAttack = rand() % 100;
    if (specialAttack < 80){
        int dmg = target->getHealth();
        cout << "\n" << getName() << " infected " << target->getName() << " with rabies.\n";
        target->takeDamage(dmg);
        target->applyStun();
    }
    else{
        cout << "\n" << getName() << " was friendly :)\n";
    }
}