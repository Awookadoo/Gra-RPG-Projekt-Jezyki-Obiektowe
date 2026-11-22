#include "characters.h"
#include "utility.h"
#include "game.h"
#include "combat.h"
#include "monsters.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

Game::Game() {
    running = true;
}

Game::~Game() {
    for (int i = 0; i < party.size(); i++) {
        delete party[i];
    }
    party.clear();
    currentParty.clear();
}

//start programu
void Game::run(){
    cout << "Welcome to - 'Just a little project'\n" <<
            "Press Enter to start...\n";
    cin.get();
    mainMenu();
}

//wyświetlanie listy menu ------------------------------------------------------------------------------------------
void Game::displayMainMenu() const{
    cout << "In order to start, you need to have a party of 3 characters.\n" <<
                "Currently you have " << currentPartyCount() << " characters in your party.\n \n" <<
                "Input 1 to start the game if you have a party of 3 characters.\n" <<
                "Input 2 to start the character creator.\n" <<
                "Input 3 to manage your current party.\n" <<
                "Input 4 to add example characters.\n" <<
                "Input 5 to add example characters & add them to party.\n" <<
                "Input 0 to exit the program.\n \n";
}

void Game::mainMenu(){
    while (running) {
        clear();
        displayMainMenu();
        char choice; cin >> choice;
        clearInput();
        clear();
        //wybory w menu
        while (choice < '0' || choice > '5'){
            clearInput();
            cout << "Invalid option, try again. \n \n";
            displayMainMenu();
            cin >> choice;
            clear();
        }
        if (choice == '1' && party.size() < 3){
            cout << "In order to start, you need to have a party of 3 characters.\n" <<
                    "Currently you have " << currentPartyCount() << " characters in your party.\n \n";
                    pause();
        }
        else if (choice == '1' && currentPartyCount() == 3){
            cout << "You have created a party of 3 characters. You can now start the game.\n";
            pause();
            startGame();
        }
        else if (choice == '1' && currentPartyCount() > 3){
            cout << "You have too many characters in your party. Remove some.\n";
            pause();
        }
        //kreator postaci
        else if (choice == '2'){
            Character* postac = Character::createCharacter();
            party.push_back(postac);
            clear();
            cout << *postac;
            pause();
            clear();
        }
        //zarządzanie drużyną
        else if (choice == '3'){
            manageParty();
            pause();
        }
        //wyjście z programu
        else if (choice == '0'){
            running = false;
        }
        //przykładowe postacie
        else if (choice == '4'){
            addCharacter(new Warrior("Adam"));
            addCharacter(new Mage("Kai"));
            addCharacter(new Assassin("Mare"));
            addCharacter(new Cleric("Elizabeth"));
            addCharacter(new Reaper("Johann"));
        }
        //dodanie przykładowych do party
        else if (choice == '5'){
            addCharacter(new Warrior("Onir"));
            addCharacter(new Mage("Twon"));
            addCharacter(new Assassin("Thrista"));
            party[0]->setInParty(true);
            party[1]->setInParty(true);
            party[2]->setInParty(true);
        }
    }
}

//------------------------------Management Stuff------------------------------

//liczenie obecnie dodanych postaci do drużyny
int Game::currentPartyCount() const{
    int count = 0;
    for (int i = 0; i < party.size(); i++) {
        if (party[i]->isInParty()) {
            count++;
        }
    }
    return count;
}

//zarządzanie posiadanymi postaciami
void Game::manageParty(){
    while(true) {
        clear();
        if (party.empty()){
            cout << "Your party is empty.\n";
            //cin.get();
            return;
        }

        //wyświetlanie stworzonych postaci
        const int colWidth = 28;
        const int rowWidth = 3;
        cout << left;

        for (int start = 0; start < party.size(); start += rowWidth){
            int end = start + rowWidth;
            if (end > party.size()) {
                end = party.size();
            }
        
            //rząd 1 - nazwa postaci
            for (int i = start; i < end; i++){
                string showName = "[" + to_string(i + 1) + "] " + party[i]->getName() + party[i]->ifActive();
                cout << setw(colWidth) << showName;
            }
            cout << "\n";

            //rząd 2 - klasa postaci
            for (int i = start; i < end; i++){
                string showClass = "Class: " + party[i]->getClassName();
                cout << setw(colWidth) << showClass;
            }
            cout << "\n";

            //rząd 3 - HP & Mana
            for (int i = start; i < end; i++){
                string showHP = "HP: " + to_string(party[i]->getMaxHealth()) + " | Mana: " + to_string(party[i]->getMaxMana());
                cout << setw(colWidth) << showHP;
            }
            cout << "\n";

            //rząd 4 - ATK & DEF
            for (int i = start; i < end; i++){
                string showATKDEF = "ATK: " + to_string(party[i]->getAttack()) + " | DEF: " + to_string(party[i]->getDefense());
                cout << setw(colWidth) << showATKDEF;
            }
            cout << "\n \n";
        }
        
        cout << "Current party count: " << currentPartyCount() << "/3\n\n";
        cout << "Input 0 to go back to the main menu.\n";
        cout << "Input the number of the character to select it.\n\n";
        
        //akcje związane z postaciami
        int choice;
        while (true) {
            if (!(cin >> choice)){
                cin.clear();
                clearInput();
                cout << "Invalid input, try again. \n";
                continue;
            }
            if (choice == 0) {
                return;
            }
            if (choice < 1 || choice > party.size()){
                cout << "Invalid choice, try again. \n";
            }
            else {
                break;
            }
        }

        //wyświetlanie wybranej postaci oraz wybór akcji
        int selectedID = choice - 1;
        Character* selected = party[selectedID];
        clear();
        cout << "Name: " << selected->getName() << "\n";
        cout << "Class: " << selected->getClassName() << "\n";
        cout << "HP: " << selected->getMaxHealth() << "\n";
        cout << "Mana: " << selected->getMaxMana() << "\n";
        cout << "ATK: " << selected->getAttack() << "\n";
        cout << "DEF: " << selected->getDefense() << "\n";

        cout << "\n1 - " << (selected->isInParty() ? "Remove character from chosen party." : "Add character to chosen party.") << "\n";
        cout << "2 - Delete character permanently.\n";
        cout << "0 - Cancel.\n\n";

        int action; cin >> action;
        clearInput();

        //dodawanie/usuwanie z drużyny
        if(action==1){
            if(selected->isInParty()){
                selected->setInParty(false);
            }
            else{
                if(currentPartyCount()>=3){
                    cout << "\nCurrent party is already full. Remove a character to add another one.";
                }
                else{
                    selected->setInParty(true);
                    cout << "\n" << selected->getName() << " added to the party.";
                }
            }
            pause();
        }
        //usuwanie postaci
        else if(action==2){
            cout << "Are you sure you want to delete " << selected->getName() << "? (y/n) ";
            char confirm; cin >> confirm;
            clearInput();

            if(confirm=='y'){
                delete party[selectedID];
                party.erase(party.begin() + selectedID);
                cout << "\nCharacter deleted.";
                pause();
            }
        }
    }
}


//rozpoczęcie gry ------------------------------------------------------------------------------------------
void Game::startGame(){
    vector<Character*> combatParty;
    for (Character* c : party){
        if (c->isInParty()){
            combatParty.push_back(c);
        }
    }

    int wave = 1;
    bool survived = true;

    while (survived && running) {
        clear();
        cout << "-----------------------------\n" <<
                "           WAVE " << wave << "\n"
                "-----------------------------\n";
        pause();
        vector<Monster*> enemies = monsterWave(wave);

        Combat battle(combatParty, enemies, wave);
        survived = battle.startCombat();
        if(survived){
            clear();
            cout << "-----------------------------\n" <<
                    "       WAVE " << wave << " CLEARED!\n" <<
                    "-----------------------------\n";
            wave++;
            pause();
        }
    }
    clear();
    cout << "Game over! You reached wave " << wave << "!\n";
    for (Character* c : party) {
        c->restore();
    }
    pause();
}

//dodawanie fal przeciwników ------------------------------------------------------------------------------------------
vector<Monster*> Game::monsterWave(int wave) {
    vector<Monster*> enemies;
    int loop = (wave-1)/5;
    int loopWave = wave %5;

    if (loopWave == 0){
        enemies.push_back(new Slime());
        enemies.push_back(new Slime());
        enemies.push_back(new Slime());
    } 
    else {
        int count = 0;
        if (loopWave==1 || loopWave==2){
            count = (wave==1||wave==2) ? 2 : 3;
        }
        else if (loopWave == 3){
            count = (wave ==3) ? 3 : 4;
        }
        else if (loopWave == 4){
            count = 4;
        }

        //lista potworów
        enum MonsterType {GOBLIN, SKELETON, SLIME, GHOUL, RAT};
        vector<MonsterType> pool;

        if (loop == 0){
            pool = {GOBLIN, GOBLIN, GOBLIN, SKELETON, SLIME};
        } else if (loop == 1) {
            pool = {GOBLIN, GOBLIN, SKELETON, SKELETON, SLIME, SLIME, GHOUL};
        } else if (loop == 2) {
            pool = {GOBLIN, SKELETON, SLIME, GHOUL, RAT};
        } else{
            pool = {GOBLIN, SKELETON, SLIME, GHOUL, RAT, SKELETON, SLIME, GHOUL, GHOUL, RAT};
        }

        for (int i = 0; i < count; i++){
            MonsterType chosen = pool[rand() % pool.size()];
            if (chosen == GOBLIN){
                enemies.push_back(new Goblin());
            }
            else if (chosen == SKELETON){
                enemies.push_back(new Skeleton());
            }
            else if (chosen == SLIME){
                enemies.push_back(new Slime());
            }
            else if (chosen == GHOUL){
                enemies.push_back(new Ghoul());
            }
            else if (chosen == RAT){
                enemies.push_back(new Rat());
            }
        }
    }
    for (Monster* m : enemies){
        m->statUp(loop);
    }

    return enemies;
}