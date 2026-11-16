#include "characters.h"
#include "utility.h"
#include "game.h"
#include <iostream>
#include <string>
#include <vector>
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
void Game::run() {
    cout << "Welcome to - 'Just a little project'\n" <<
            "Press Enter to start...\n";
    cin.get();
    mainMenu();
}

void Game::mainMenu() {
    while (running) {
        clear();
        cout << "In order to start, you need to create a party of 3 characters.\n" <<
                    "Currently you have " << party.size() << " characters.\n \n" <<
                    "Input 1 to start the character creator.\n" <<
                    "Input 2 to manage your current party.\n" <<
                    "Input 3 to exit the program.\n \n";
            char choice; cin >> choice;
            clear();
            while (choice != '1' && choice != '2' && choice != '3') {
                cout << "Invalid option, try again. \n";
                clearInput();
                cin >> choice;
            }
            //kreator postaci
            if (choice == '1') {
                Character* postac = Character::createCharacter();
                party.push_back(postac);
                clear();
                cout << *postac;
                pause();
                clear();
            }
            //zarządzanie drużyną
            else if (choice == '2') {
                manageParty();
                pause();
            }
            //wyjście z programu
            else if (choice == '3') {
                running = false;
            }  
    }
}

//pokaz obecnie posiadanych postaci
void Game::manageParty() {
    clear();
    if (party.empty()) {
        cout << "Your party is empty.\n";
        return;
    }

    //wyświetlanie stworzonych postaci
    for (int i = 0; i < party.size(); i++) {
        cout << "(" << (i + 1) << ")\n" << *party[i] << "\n";
    }
}