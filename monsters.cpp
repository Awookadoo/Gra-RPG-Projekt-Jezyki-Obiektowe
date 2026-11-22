#include "characters.h"
#include "monsters.h"
#include "utility.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
using namespace std;


Monster::Monster(string name, int attack, int maxHealth, int defense) : Character(name, attack, maxHealth, 0, defense){}

void Monster::takeTurn(vector<Character*>& playerParty){
    //szukanie focusing
    for (Character* hero : playerParty) {
        if (hero->isAlive() && hero->isFocusing()) {
            attackTarget(hero);
            return;
        }
    }
    //szukanie żywych postaci
    vector<Character*> aliveCharacters;
    for (Character* hero : playerParty){
        if (hero->isAlive()){
            aliveCharacters.push_back(hero);
        }
    }
    if (aliveCharacters.empty()){
        return;
    }

    //wybieranie losowej postaci do ataku
    int randomAttack = rand() % aliveCharacters.size();
    Character* target = aliveCharacters[randomAttack];

    int roll = rand() % 100;
    if (roll < target->getEvasion()) {
        cout << "\n" << target->getName() << " dodges the attack from " << name << "!\n";
        return;
    }

    attackTarget(target);
    if (!target->isAlive()) {
        cout << target->getName() << " has fallen!\n";}
}

void Monster::statUp(int loop){
    if (loop <= 0){
        return;
    }
    maxHealth += loop;
    currentHealth += loop;
    attack += loop;
    defense += loop;
}

//------------------------------Monster Types------------------------------

Goblin::Goblin() : Monster("Goblin", 10, 30, 2) {}

void Goblin::attackTarget(Character* target) {
    int dmg = Character::calculateDamage(attack, target->getTrueDEF());
    cout << "\n" << name << " stabbed " << target->getName() << " for " << dmg << " damage!\n";
    target->takeDamage(dmg);
}

Skeleton::Skeleton() : Monster("Skeleton", 13, 25, 4) {}

void Skeleton::attackTarget(Character* target) {
    int specialAttack = rand() % 100;
    if (specialAttack < 25){
        int dmg = Character::calculateDamage(attack, target->getTrueDEF() * 0.5);
        cout << "\n" << name << " shot " << target->getName() << " and dealt " << dmg << " while piercing through half of their DEF!\n";
        target->takeDamage(dmg);
    }
    else{
        int dmg = Character::calculateDamage(attack, target->getTrueDEF());
        cout << "\n" << name << " shot " << target->getName() << " and dealt " << dmg << " damage!\n";
        target->takeDamage(dmg);
    }
}

Slime::Slime() : Monster("Slime", 8, 30, 4) {}

void Slime::attackTarget(Character* target) {
    int specialAttack = rand() % 100;
    if (specialAttack < 40){
        int dmg = Character::calculateDamage(attack, target->getTrueDEF());
        cout << "\n" << name << " oozed over " << target->getName() << " to deal " << dmg << " damage and stun them for the next turn!\n";
        target->takeDamage(dmg);
        target->applyStun();
    }
    else{
        int dmg = Character::calculateDamage(attack, target->getTrueDEF());
        cout << "\n" << name << " oozed over " << target->getName() << " to deal " << dmg << " damage!\n";
        target->takeDamage(dmg);
    }
}

Ghoul::Ghoul() : Monster("Ghoul", 15, 40, 10) {}

void Ghoul::attackTarget(Character* target) {
    int specialAttack = rand() % 100;
    if (specialAttack < 50){
        int dmg = Character::calculateDamage(attack, target->getTrueDEF());
        cout << "\n" << name << " bit " << target->getName() << " and dealt " << dmg << " while healing himself for 5 health!\n";
        target->takeDamage(dmg);
        restoreHealth(5);
    }
    else{
        int dmg = Character::calculateDamage(attack, target->getTrueDEF());
        cout << "\n" << name << " bit " << target->getName() << " and dealt " << dmg << " damage!\n";
        target->takeDamage(dmg);
    }
}

Rat::Rat() : Monster("Rat", 1, 1, 0) {}

void Rat::attackTarget(Character* target) {
    int specialAttack = rand() % 100;
    if (specialAttack < 80){
        int dmg = target->getHealth();
        cout << "\n" << name << " infected " << target->getName() << " with rabies.\n";
        target->takeDamage(dmg);
        target->applyStun();
    }
    else{
        cout << "\n" << name << " was friendly :)\n";
    }
}