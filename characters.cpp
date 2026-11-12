#include "characters.h"
#include <iostream>
using namespace std;

Character::Character(string name_, CharacterClass characterClass_, int attack_, int maxHealth_, int mana_, int defense_) {
    name = name_;
    characterClass = characterClass_;
    attack = attack_;
    maxHealth = maxHealth_;
    mana = mana_;
    defense = defense_;
}

Character::~Character() {
    cout << "\nCharacter " << name << " is gone now. \n";
}

Character* Character::createCharacter() {
    string name; cout << "Choose a name: "; cin >> name;
    int classChoice;
    char confirmation;

    while (true) {
        cout << "Choose a class (0 - Warrior, 1 - Mage, 2 - Archer): ";

        if (!(cin >> classChoice)) {
            cout << "Invalid choice, try again. \n \n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (classChoice >= 0 && classChoice <= 2) {
            if (classChoice == 0) {
                Warrior tempWarrior(name, 100);
                tempWarrior.classDescription();
            }
            cout << "Is this your final choice? (y/n):"; cin >> confirmation;
            if (confirmation == 'y') {
                if (classChoice == 0) return new Warrior(name, 100);
            }
            cout << "\n";
        }
        else {
            cout << "Invalid choice, try again. \n \n";
        }
    }
}

string Character::getClassName() const {
    switch (characterClass) {
        case CharacterClass::Warrior:
            return "Warrior";
        case CharacterClass::Mage:
            return "Mage";
        case CharacterClass::Archer:
            return "Archer";
        default:
            return "Unknown";
    }
}

void Character::displayStats(ostream& os) const {
    os << "\nName: " << name << "\n"
       << "Attack: " << attack << "\n"
       << "Health: " << maxHealth << "\n"
       << "Mana: " << mana << "\n"
       << "Defense: " << defense << "\n";
}

ostream& operator<<(ostream& os, const Character& character) {
    character.displayStats(os);
    return os;
}



//------------------------------Warrior Stuff------------------------------

Warrior::Warrior(string name_, int rage_) : Character(name_, CharacterClass::Warrior, 15, 60, 5, 5){
    rage = rage_;
}

void Warrior::displayStats(ostream& os) const {
    os << "\nClass: Warrior\n";
    Character::displayStats(os);
    os << "Rage: " << rage << "\n";
}

void Warrior::classDescription() {
    cout << "\nWarrior: Adds a unique stat called Rage. \n \n";
    cout << "Attack: 10 -> 15 \n";
    cout << "Health: 50 -> 60 \n";
    cout << "Mana: 10 -> 5 \n";
    cout << "Defense: 5 -> 5 \n \n";
}