#include "characters.h"
#include "monsters.h"
#include "utility.h"
#include "game.h"
#include "combat.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
using namespace std;

Combat::Combat(vector<Character*>& activeParty, vector<Monster*>& enemies) : activeParty(activeParty), enemies(enemies) {}

Combat::~Combat() {
    for (Monster* enemy : enemies) {
        delete enemy;
    }
}

//wyświetlanie drużyny ------------------------------------------------------------------------------------------
void Combat::displayParty() const{
    const int colWidth = 28;
    int count = 3;
    cout << left;

    //rząd 1 - nazwa postaci
    for (int i = 0; i < count; i++) {
        string status = activeParty[i]->isAlive() ? "" : " [DEAD]";
        string showName = "[" + to_string(i + 1) + "] " + activeParty[i]->getName() + status;
        cout << setw(colWidth) << showName;
    }
    cout << "\n";

    //rząd 2 - klasa postaci
    for (int i = 0; i < count; i++) {
        string showClass = "Class: " + activeParty[i]->getClassName();
        cout << setw(colWidth) << showClass;
    }
    cout << "\n";

    //rząd 3 - aktualne HP & Mana
    for (int i = 0; i < count; i++) {
        string showHPMana = "HP: " + to_string(activeParty[i]->getHealth()) + "/" + to_string(activeParty[i]->getMaxHealth()) + " | Mana: " + to_string(activeParty[i]->getMana());
        cout << setw(colWidth) << showHPMana;
    }
    cout << "\n";

    //rząd 4 - ATK & DEF
    for (int i = 0; i < count; i++) {
        string showATKDEF = "ATK: " + to_string(activeParty[i]->getAttack()) + " | DEF: " + to_string(activeParty[i]->getDefense());
        cout << setw(colWidth) << showATKDEF;
    }
    cout << "\n\n";
}

//wyświetlanie przeciwników ------------------------------------------------------------------------------------------
void Combat::displayEnemies() const {
    const int colWidth = 28;
    int count = enemies.size();
    cout << left;

    //rząd 1 - nazwa potwora
    for (int i = 0; i < count; i++) {
        string status = enemies[i]->isAlive() ? "" : " [DEAD]";
        string showName = "[" + to_string(i + 1) + "] " + enemies[i]->getName() + status;
        cout << setw(colWidth) << showName;
    }
    cout << "\n";

    //rząd 2 - aktualne HP
    for (int i = 0; i < count; i++) {
        string showHP = "HP: " + to_string(enemies[i]->getHealth()) + "/" + to_string(enemies[i]->getMaxHealth());
        cout << setw(colWidth) << showHP;
    }
    cout << "\n";

    //rząd - ATK & DEF
    for (int i = 0; i < count; i++) {
        string showATKDEF = "ATK: " + to_string(enemies[i]->getAttack()) + " | DEF: " + to_string(enemies[i]->getDefense());
        cout << setw(colWidth) << showATKDEF;
    }
    cout << "\n\n";
}

//tura gracza ------------------------------------------------------------------------------------------
void Combat::playerTurn(){
    for (Character* hero : activeParty){
        if (!hero->isAlive()) continue;
        if (!areEnemiesAlive()) break;
        while(true){
            clear();
            displayParty();
            cout << "--------------------------------------------------------------------------------\n\n";
            displayEnemies();
            cout << "\n--- " << hero->getName() << "'s Turn ---\n";
            
            //wybieranie akcji ------------------------------------------------------------------------------------------
            cout << "\nChoose action:\n" <<
                    "1. Basic actions.\n" <<
                    "2. Special skills.\n" <<
                    "0. Flee and end the game.\n\n";

            int action;
            if (!(cin >> action)) {
                clearInput();
                cout << "Invalid choice, try again.\n";
                pause();
                continue;
            }
            else if(action==1){
                cout << "\nChoose action: (not implemented yet, pick anything, always attacks)\n" <<
                    "1. Attack.\n" <<
                    "2. Guard.\n" <<
                    "3. Heal.\n" <<
                    "4. Wait.\n" <<
                    "0. Cancel.\n\n";
                char actionBasic; cin >> actionBasic;
                while (actionBasic < '0' || actionBasic > '4'){
                    clearInput();
                    cout << "Invalid option, try again. \n";
                    cin >> actionBasic;
                }
                break;
            }
            else if(action==2){
                cout << "\nChoose action: (always attacks)\n" <<
                    "1. Not.\n" <<
                    "2. Implemented.\n" <<
                    "3. Yet.\n" <<
                    "4. Lol.\n" <<
                    "0. Cancel.\n\n";
                char actionSkill; cin >> actionSkill;
                while (actionSkill < '0' || actionSkill > '4'){
                    clearInput();
                    cout << "Invalid option, try again. \n";
                    cin >> actionSkill;
                }
                break;
            }
            else if(action==0){
                flee = true;
                return;
            }
            clearInput();
        }

        //wybieranie przeciwnika ------------------------------------------------------------------------------------------
        clear();
        displayParty();
        cout << "--------------------------------------------------------------------------------\n\n";
        displayEnemies();
        cout << "------------------\n \n";
        // for (int i = 0; i < enemies.size(); i++) {
        //     cout << i + 1 << ". " << enemies[i]->getName() << " (" << enemies[i]->getHealth() << "/" << enemies[i]->getMaxHealth() << " HP)" << (enemies[i]->isAlive() ? "" : " [DEAD]") << "\n";
        //     cout << "[ATK] " << enemies[i]->getAttack() << "   [DEF] " << enemies[i]->getDefense() << "\n \n";
        // }

        while(true){
            int target;
            cout << "Choose target: "; cin >> target; target--;
            if (target < 0 || target >= enemies.size() || !enemies[target]->isAlive()) {
                cout << "Invalid or dead target! Try again.\n";
                clearInput();
            }
            else {
                Monster* targetMonster = enemies[target];
                int dmg = hero->getAttack() - targetMonster->getDefense();
                targetMonster->takeDamage(dmg);

                cout << "\n" << hero->getName() << " hits " << targetMonster->getName() << " for " << dmg << " damage!\n";
                if (!targetMonster->isAlive()) {
                    cout << targetMonster->getName() << " is defeated!\n";
                }
                clearInput();
                break;
            }
        }
        pause();
    }
}

//tura przeciwników ------------------------------------------------------------------------------------------
void Combat::enemyTurn(){
    cout << "\n--- Enemy Turn ---\n";
    for (Monster* enemy : enemies){
        if (!enemy->isAlive()) continue;
        if (!isPartyAlive()) break;
        enemy->takeTurn(activeParty);
        pause();
    }
}

//warunki do gry
bool Combat::isPartyAlive() const{
    for (const Character * hero : activeParty){
        if (hero->isAlive()){
            return true;
        }
    }
    return false;
}

bool Combat::areEnemiesAlive() const{
    for (const Monster * enemy : enemies){
        if (enemy->isAlive()){
            return true;
        }
    }
    return false;
}

//rozpoczęcie walki
bool Combat::startCombat(){
    while(isPartyAlive() && areEnemiesAlive() && !flee){
        playerTurn();

        if(flee || !areEnemiesAlive()){
            break;
        }

        enemyTurn();
    }
    return !flee & isPartyAlive();
}