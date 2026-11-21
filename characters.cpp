#include "characters.h"
#include "utility.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

Character::Character(string name_, int attack_, int maxHealth_, int maxMana_, int defense_) {
    name = name_;
    attack = attack_;
    maxHealth = maxHealth_;
    maxMana = maxMana_;
    defense = defense_;
    currentHealth = maxHealth_;
    currentMana = maxMana_;
}

Character::~Character() {}

//tworzenie postaci ------------------------------------------------------------------------------------------
Character* Character::createCharacter() {

    const int maxNameLength = 13;
    string name;
    int classChoice;
    char confirmation;

    while (true) {
        cout << "Choose a name (max " << maxNameLength << " characters): "; getline(cin, name);
        if (name.empty()){
            cout << "Name cannot be empty. Choose a name: ";
        } else if (name.length() > maxNameLength){
            cout << "Name is too long. Please keep it to " << maxNameLength << " or fewer characters.\n";
        } else {
            break;
        }
    }

    while (true) {
        cout << "Choose a class (0 - Warrior, 1 - Mage, 2 - Assassin): ";

        if (!(cin >> classChoice)) {
            cout << "Invalid choice, try again. \n";
            clearInput();
            continue;
        }

        if (classChoice >= 0 && classChoice <= 2) {
            clear();
            if (classChoice == 0) {
                Warrior tempWarrior(name, 100);
                tempWarrior.classDescription();
            }

            if (classChoice == 1) {
                Mage tempMage(name, 100);
                tempMage.classDescription();
            }
            if (classChoice == 2) {
                Assassin tempAssassin(name, 100);
                tempAssassin.classDescription();
            }

            cout << "Is this your final choice? (y/n): "; cin >> confirmation;
            if (confirmation == 'y') {
                if (classChoice == 0) return new Warrior(name, 100);
                if (classChoice == 1) return new Mage(name, 100);
                if (classChoice == 2) return new Assassin(name, 100);
            }
            cout << "\n";
        }
        else {
            cout << "Invalid choice, try again. \n \n";
        }
    }
}

void Character::displayStats(ostream& os) const {
    os << "Name: " << name << "\n"
       << "Attack: " << attack << "\n"
       << "Health: " << maxHealth << "\n"
       << "Mana: " << maxMana << "\n"
       << "Defense: " << defense << "\n \n";
}

//funkcja do wyświetlania statystyk postaci (przeciążenie operatora <<) ------------------------------------------------------------------------------------------
ostream& operator<<(ostream& os, const Character& character) {
    character.displayStats(os);
    return os;
}

//------------------------------Warrior Stuff------------------------------

Warrior::Warrior(string name_, int rage_) : Character(name_, 13, 40, 8, 5){
    rage = rage_;
}

void Warrior::displayStats(ostream& os) const {
    os << "Class: Warrior\n";
    Character::displayStats(os);
    os << "Rage: " << rage << "\n";
}

void Warrior::classDescription() {
    cout << "Warrior: Adds a unique stat called Rage. \n \n";
    cout << "Attack: 10 -> 13 \n";
    cout << "Health: 30 -> 40 \n";
    cout << "Mana: 10 -> 8 \n";
    cout << "Defense: 5 -> 5 \n \n";
}

//------------------------------Mage Stuff------------------------------

Mage::Mage(string name_, int manaRegen_) : Character(name_, 10, 35, 15, 3){
    manaRegen = manaRegen_;
}

void Mage::displayStats(ostream& os) const {
    os << "Class: Mage\n";
    Character::displayStats(os);
    os << "Mana regen: " << manaRegen << "\n";
}

void Mage::classDescription() {
    cout << "Mage: Adds a unique stat called Mana Regen. \n \n";
    cout << "Attack: 10 -> 10 \n";
    cout << "Health: 30 -> 35 \n";
    cout << "Mana: 10 -> 15 \n";
    cout << "Defense: 5 -> 3 \n \n";
}

//------------------------------Assassin Stuff------------------------------

Assassin::Assassin(string name_, int evasion_) : Character(name_, 20, 25, 10, 4){
    evasion = evasion_;
}

void Assassin::displayStats(ostream& os) const {
    os << "Class: Assassin\n";
    Character::displayStats(os);
    os << "Evasion: " << evasion << "\n";
}

void Assassin::classDescription() {
    cout << "Assassin: Adds a unique stat called Evasion. \n \n";
    cout << "Attack: 10 -> 20 \n";
    cout << "Health: 30 -> 25 \n";
    cout << "Mana: 10 -> 10 \n";
    cout << "Defense: 5 -> 4 \n \n";
}

//------------------------------Combat Stuff------------------------------

void Character::takeDamage(int dmg){
    if (dmg < 0) return;
    currentHealth -= dmg;
    if(currentHealth < 0){
        currentHealth = 0;
    }
}

void Character::restore() {
    currentHealth = maxHealth;
    currentMana = maxMana;

}