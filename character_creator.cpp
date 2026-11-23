#include "characters.h"
#include "utility.h"
#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

Character* Character::createCharacter() {
    const int maxNameLength = 13;
    string name;
    int classChoice;
    char confirmation;

    while (true) {
        cout << "Choose a name (max " << maxNameLength << " characters): ";
        getline(cin, name);
        if (name.empty()) {
            cout << "Name cannot be empty. Choose a name: ";
        } else if (name.length() > maxNameLength) {
            cout << "Name is too long. Please keep it to " << maxNameLength << " or fewer characters.\n";
        } else {
            break;
        }
    }

    while (true) {
        clear();
        cout << "Choose a class:\n"
             << "1 - Warrior\n"
             << "2 - Mage\n"
             << "3 - Assassin\n"
             << "4 - Cleric\n"
             << "5 - Reaper\n\n";

        if (!(cin >> classChoice)) {
            cout << "Invalid choice, try again.\n";
            clearInput();
            continue;
        }

        if (classChoice < 1 || classChoice > 5) {
            cout << "Invalid choice, try again.\n\n";
            continue;
        }

        Character* character = nullptr;
        try{
            switch (classChoice){
                case 1: character = new Warrior(name); break;
                case 2: character = new Mage(name); break;
                case 3: character = new Assassin(name); break;
                case 4: character = new Cleric(name); break;
                case 5: character = new Reaper(name); break;
            }
        }
        catch (const invalid_argument& error){
            cout << "Could not create character: " << error.what() << "\n";
            continue;
        }

        clear();
        cout << *character;
        character->showSkills(false);
        cout << "Do you want to choose this class? (y/n): ";
        cin >> confirmation;
        clearInput();

        if (confirmation == 'y') {
            int pointsRemaining = 5;
            while (pointsRemaining > 0) {
                clear();
                cout << *character;
                cout << "\nDistribute your stat points:\n"
                     << "Points remaining: " << pointsRemaining << "\n"
                     << "1. Max Health (+3)\n"
                     << "2. Attack (+1)\n"
                     << "3. Defense (+1)\n"
                     << "Choose a stat: ";

                int statChoice;
                if (!(cin >> statChoice)) {
                    clearInput();
                    cout << "Invalid choice, try again.\n";
                    pause();
                    continue;
                }
                clearInput();

                if (statChoice == 1) {
                    character->maxHealth += 3;
                    character->currentHealth += 3;
                } else if (statChoice == 2) {
                    character->attack += 1;
                } else if (statChoice == 3) {
                    character->defense += 1;
                } else {
                    cout << "Invalid choice, try again.\n";
                    pause();
                    continue;
                }
                pointsRemaining--;
            }

            clear();
            cout << *character;
            return character;
        }

        delete character;
        cout << "\n";
    }
}
