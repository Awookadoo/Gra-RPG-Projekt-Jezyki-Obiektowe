#include "characters.h"
#include "utility.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void mainMenu() {
    bool running = true;
    while (running) {
        clear();
        cout << "In order to start, you need to create a party of 3 characters.\n" <<
                    "Currently you have 0 characters.\n \n" <<
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
            if (choice == '1') {
                Character* postac = Character::createCharacter();
                clear();
                cout << *postac;
                pause();
                clear();
                delete postac;
            }
            else if (choice == '2') {
                cout << "Manage your current party (not implemented yet).\n";
                pause();
            }
            else if (choice == '3') {
                running = false;
            }  
    }
}

int main(){
    cout << "Welcome to - Not a very creative name - project\n" <<
            "Press Enter to start\n";
    cin.get();
    mainMenu();
    return 0;
}

//postac = nullptr;