#include "characters.h"
#include "monsters.h"
#include "utility.h"
#include "combat.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#ifdef _WIN32
#define byte win_byte_override
#include <windows.h>
#undef byte
#endif
using namespace std;

void displayColorName(const string& text, const Character* character, const Character* hasTurn, int colWidth) {
    #ifdef _WIN32
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(console, &info);
    WORD originalColor = info.wAttributes;

    if (character == hasTurn && character->isAlive()) {
        SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    } else if (character->wasHitThisTurn()) {
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
    }
    cout << text;
    SetConsoleTextAttribute(console, originalColor);
    #else
    if (character == hasTurn && character->isAlive()) {
        cout << "\033[32m";
    } else if (character->wasHitThisTurn()) {
        cout << "\033[31m";
    }
    cout << text;
    if ((character == hasTurn && character->isAlive()) || character->wasHitThisTurn()) {
        cout << "\033[0m";
    }
    #endif

    if (text.size() < static_cast<size_t>(colWidth)) {
        cout << string(colWidth - text.size(), ' ');
    }
}

Combat::Combat(vector<Character*>& activeParty, vector<Monster*>& enemies, int currentWave) : activeParty(activeParty), enemies(enemies), wave(currentWave) {}

Combat::~Combat() {
    for (Monster* enemy : enemies) {
        delete enemy;
    }
}

//wyświetlanie drużyny ------------------------------------------------------------------------------------------
void Combat::displayParty(Character* hasTurn) const{
    const int colWidth = 28;
    int count = activeParty.size();
    cout << left;

    //rząd 1 - nazwa postaci
    for (int i = 0; i < count; i++) {
        string status = "";
        if (!activeParty[i]->isAlive()) {
            status = " [DEAD]";
        } else if (activeParty[i] == hasTurn) {
            status = " [TURN]";
        }
        string showName = "[" + to_string(i + 1) + "] " + activeParty[i]->getName() + status;
        displayColorName(showName, activeParty[i], hasTurn, colWidth);
    }
    cout << "\n";

    //rząd 2 - klasa postaci
    for (int i = 0; i < count; i++) {
        string showClass = "Class: " + activeParty[i]->getClassName();
        cout << setw(colWidth) << showClass;
    }
    cout << "\n";

    //rząd 3 - ATK & HP
    for (int i = 0; i < count; i++) {
        string showHPMana = "HP: " + to_string(activeParty[i]->getHealth()) + "/" + to_string(activeParty[i]->getMaxHealth()) + " | ATK: " + to_string(activeParty[i]->getAttack());
        cout << setw(colWidth) << showHPMana;
    }
    cout << "\n";

    //rząd 4 - DEF & Mana
    for (int i = 0; i < count; i++) {
        string showATKDEF =  "Mana: " + to_string(activeParty[i]->getMana()) + "/" + to_string(activeParty[i]->getMaxMana()) + " | DEF: " + to_string(activeParty[i]->getDefense());
        cout << setw(colWidth) << showATKDEF;
    }
    cout << "\n\n";
}

//wyświetlanie przeciwników ------------------------------------------------------------------------------------------
void Combat::displayEnemies(Character* hasTurn) const {
    const int colWidth = 28;
    int count = enemies.size();
    cout << left;

    //rząd 1 - nazwa potwora
    for (int i = 0; i < count; i++) {
        string status = "";
        if (!enemies[i]->isAlive()) {
            status = " [DEAD]";
        } else if (enemies[i] == hasTurn) {
            status = " [TURN]";
        }
        string showName = "[" + to_string(i + 1) + "] " + enemies[i]->getName() + status;
        displayColorName(showName, enemies[i], hasTurn, colWidth);
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
void Combat::displayBattlefield(Character* hasTurn, bool clearScreen) const{
    if (clearScreen) {
        clear();
    }
    cout << "-----------------------------\n" <<
                "           WAVE " << wave << "\n"
                "-----------------------------\n\n";
    displayParty(hasTurn);
    cout << "--------------------------------------------------------------------------------\n\n";
    displayEnemies(hasTurn);
}

void Combat::resetHitStatuses() {
    for (Character* hero : activeParty) {
        hero->clearHitStatus();
    }
    for (Monster* enemy : enemies) {
        enemy->clearHitStatus();
    }
}

//tura gracza ------------------------------------------------------------------------------------------
void Combat::playerTurn(){
    for (Character* hero : activeParty){
        if (!hero->isAlive()) continue;
        if (!areEnemiesAlive()) break;
        resetHitStatuses();
        hero->onTurnStart();
        hero->resetGuard();
        hero->resetFocus();
        hero->restoreMana(1);

        while(true){
            
            if (hero->isStunned()) {
                displayBattlefield(hero);
                cout << "\n" << hero->getName() << " is stuck from the Slime attack and can't move!\n";
                hero->clearStun();
                pause();
                break;
            }
            else{
                displayBattlefield(hero);
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
                    displayBattlefield(hero);
                    cout << "\n--- " << hero->getName() << "'s Turn ---\n";
                    cout << "\nChoose action:\n" <<
                        "1. Attack -> Deal damage to an enemy.\n" <<
                        "2. Guard -> Negate all damage until the next turn.\n" <<
                        "3. Heal -> Heal yourself for 8 HP.\n" <<
                        "4. Wait -> Skip your turn.\n" <<
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
                        displayBattlefield(hero);
                        cout << "------------------\n \n";
                        Monster* target = hero->selectTarget(enemies);
                        hero->basicAttack(target);
                        displayBattlefield(hero, false);
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
                    displayBattlefield(hero);
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
                    displayBattlefield(hero, false);
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
}

//tura przeciwników ------------------------------------------------------------------------------------------
void Combat::enemyTurn(){
    for (Monster* enemy : enemies){
        resetHitStatuses();
        cout << "\n--- Enemy Turn ---\n";
        if (!enemy->isAlive()) {
            displayBattlefield(enemy);
            continue;
        }
        enemy->applyOverDamage();
        if (!enemy->isAlive()) {
            displayBattlefield(enemy);
            continue;
        }
        displayBattlefield(enemy);

        if (enemy->isStunned()) {
        cout << "\n" << enemy->getName() << " is frozen and skips their turn!\n";
        enemy->clearStun();
        
        pause();
        continue;
    }

        if (!isPartyAlive()) break;
        enemy->takeTurn(activeParty);
        displayBattlefield(enemy, false);
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
    for (Character* hero : activeParty){
        hero->onWaveStart();
    }
    
    while(isPartyAlive() && areEnemiesAlive() && !flee){
        playerTurn();

        if(flee || !areEnemiesAlive()){
            break;
        }

        enemyTurn();
    }
    return !flee && isPartyAlive();
}