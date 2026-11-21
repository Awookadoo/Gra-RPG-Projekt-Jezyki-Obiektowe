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

    //atakowanie
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
    int dmg = attack - target->getDefense();
    target->takeDamage(dmg);
    cout << "\n" << name << " attacked " << target->getName() << " for " << dmg << " damage!\n";
}

Skeleton::Skeleton() : Monster("Skeleton", 15, 25, 2) {}

void Skeleton::attackTarget(Character* target) {
    //int specialAttack = target->getDefense() / 2; piercing 50% armor
    int dmg = attack - target->getDefense();
    target->takeDamage(dmg);
    cout << "\n" << name << " attacked " << target->getName() << " for " << dmg << " damage!\n";
}

Boss::Boss() : Monster("Boss", 12, 40, 5) {}

void Boss::attackTarget(Character* target) {
    //int specialAttack = target->getDefense() / 2; piercing 50% armor
    int dmg = attack - target->getDefense();
    target->takeDamage(dmg);
    cout << "\n" << name << " attacked " << target->getName() << " for " << dmg << " damage!\n";
}