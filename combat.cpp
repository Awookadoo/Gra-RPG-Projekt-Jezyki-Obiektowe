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

//wspólne wyświetlanie
void Combat::displayBattlefield() const{
    clear();
    displayParty();
    cout << "--------------------------------------------------------------------------------\n\n";
    displayEnemies();
}

//tura gracza ------------------------------------------------------------------------------------------
void Combat::playerTurn(){
    for (Character* hero : activeParty){
        if (!hero->isAlive()) continue;
        if (!areEnemiesAlive()) break;
        hero->resetGuard();
        hero->resetFocus();
        hero->restoreMana(1);

        while(true){
            displayBattlefield();
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
            clearInput();
            if(action==1){
                displayBattlefield();
                cout << "\n--- " << hero->getName() << "'s Turn ---\n";
                cout << "\nChoose action: (not implemented yet, pick anything, always attacks)\n" <<
                    "1. Attack -> Deal damage to an enemy.\n" <<
                    "2. Guard -> Negate all damage until the next turn.\n" <<
                    "3. Heal -> Heal yourself for 5 HP.\n" <<
                    "4. Wait -> Gain back two more mana.\n" <<
                    "0. Cancel.\n\n";
                char actionBasic; cin >> actionBasic;
                clearInput();
                while (actionBasic < '0' || actionBasic > '4'){
                    cout << "Invalid option, try again. \n";
                    cin >> actionBasic;
                    clearInput();
                }
                if(actionBasic=='0'){
                    continue;
                }
                else if(actionBasic=='1'){
                    displayBattlefield();
                    cout << "------------------\n \n";
                    Monster* target = hero->selectTarget(enemies);
                    hero->basicAttack(target);
                    pause();
                    break;
                }
                else if(actionBasic=='2'){
                    hero->guard();
                    pause();
                    break;
                }
                else if(actionBasic=='3'){
                    hero->heal();
                    pause();
                    break;
                }
                else if(actionBasic=='4'){
                    hero->wait();
                    pause();
                    break;
                }
            }
            else if(action==2){
                displayBattlefield();
                cout << "\n--- " << hero->getName() << "'s Turn ---\n";
                hero->showSkills();
                int actionSkill;
                if(!(cin>>actionSkill)){
                    clearInput();
                    cout<<"Invalid skill choice, try again.\n";
                    pause();
                    continue;
                }
                clearInput();
                if(actionSkill==0){
                    continue;
                }
                if (!hero->useSkill(actionSkill, activeParty, enemies)) {
                    pause();
                    continue;
                }
                pause();
                break;
            }
            else if(action==0){
                flee = true;
                return;
            }
            else{
                cout << "Invalid choice, try again.\n";
                pause();
            }
            clearInput();
        }
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