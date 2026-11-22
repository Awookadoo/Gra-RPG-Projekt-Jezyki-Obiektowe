#include "characters.h"
#include "utility.h"
#include "monsters.h"
#include "combat.h"
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
        clear();
        cout << "Choose a class:\n" <<
        "1 - Warrior\n" <<
        "2 - Mage\n" <<
        "3 - Assassin\n\n";

        if (!(cin >> classChoice)) {
            cout << "Invalid choice, try again. \n";
            clearInput();
            continue;
        }

        if (classChoice >= 0 && classChoice <= 2) {
            clear();
            if (classChoice == 1) {
                Warrior tempWarrior(name);
                tempWarrior.classDescription();
            }

            if (classChoice == 2) {
                Mage tempMage(name);
                tempMage.classDescription();
            }
            if (classChoice == 3) {
                Assassin tempAssassin(name);
                tempAssassin.classDescription();
            }

            cout << "Is this your final choice? (y/n): "; cin >> confirmation;
            if (confirmation == 'y') {
                if (classChoice == 1) return new Warrior(name);
                if (classChoice == 2) return new Mage(name);
                if (classChoice == 3) return new Assassin(name);
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

//funkcja do wyświetlania statystyk postaci (przeciążenie operatora <<)
ostream& operator<<(ostream& os, const Character& character) {
    character.displayStats(os);
    return os;
}

//------------------------------Combat Stuff------------------------------

void Character::takeDamage(int dmg){
    if (guarding){
        cout << name << " guarded and negated the damage!\n";
        return;
    }
    if (dmg < 0) return;
    currentHealth -= dmg;
    if(currentHealth < 0){
        currentHealth = 0;
    }
}

void Character::restoreMana(int amount){
    currentMana += amount;
    if (currentMana > maxMana){
        currentMana = maxMana;
    }
}

void Character::restoreHealth(int amount){
    currentHealth += amount;
    if (currentHealth > maxHealth){
        currentHealth = maxHealth;
    }
}

void Character::restore(){
    currentHealth = maxHealth;
    currentMana = maxMana;
}

//------------------------------Basic Actions------------------------------

void Character::basicAttack(Character* target){
    int dmg = getAttack() - target->getDefense();
    target->takeDamage(dmg);

    cout << "\n" << name << " hits " << target->getName() << " for " << dmg << " damage!\n";
    if (!target->isAlive()){
        cout << target->getName() << " is defeated!\n";
    }
}

void Character::guard(){
    guarding = true;
    cout << "\n" << name << " is holding a guard this turn.";
}

int Character::getTrueDEF() const{
    return focusing ? defense + 2 : defense;
}

void Character::heal(){
    restoreHealth(5);
    cout << "\n" << name << " has healed themselves.\n";
}

void Character::wait(){
    restoreMana(2);
    cout << "\n" << name << " waited and regenerated some mana.\n";
}

Monster* Character::selectTarget(const vector<Monster*>& enemies) const{
    while (true){
        int target;
        cout << "Choose target: "; cin >> target; target--;
        clearInput();
        if (target < 0 || target >= enemies.size() || !enemies[target]->isAlive()) {
            cout << "Invalid or dead target! Try again.\n";
        }
        else {
            return enemies[target];
        }
    }
}

bool Character::spendMana(int manaCost) {
    if (currentMana < manaCost) {
        cout << "Not enough mana!\n";
        return false;
    }
    currentMana -= manaCost;
    return true;
}

//------------------------------Skills Stuff------------------------------

void Character::getOverDamage(int dmgPerTurn, int turns) {
    overDamage = dmgPerTurn;
    overTurns = turns;
}

void Character::applyOverDamage() {
    if (overTurns > 0 && isAlive()) {
        takeDamage(overDamage);
        overTurns--;
        cout << "\n" << name << " takes " << overDamage << " damage over time! ("
                << overTurns << " turns left)\n";
        if (!isAlive()) {
            cout << name << " burned to death!\n";
        }
    }
}

void Character::reduceDefense() {
    defense = defense * 0.5;
    if (defense < 0){
        defense = 0;
    }
}

//------------------------------Warrior Stuff------------------------------

Warrior::Warrior(string name_, int rage_) : Character(name_, 10, 40, 8, 5){
    rage = rage_;
}

void Warrior::displayStats(ostream& os) const{
    os << "Class: Warrior\n";
    Character::displayStats(os);
    os << "Rage: " << rage << "\n";
}

void Warrior::classDescription(){
    cout << "Warrior: Adds a unique stat called Rage. \n" <<
            "Every turn gain one rage [+1 ATK].\n" <<
            "Resets every wave.\n\n" <<
            "Attack: 10 \n" <<
            "Health: 40 \n" <<
            "Mana: 8 \n" <<
            "Defense: 5 \n \n" <<
            "Skills: \n" <<
            "1. Heavy Attack (Cost: 3 Mana) -> Deal 150% attack damage.\n" <<
            "2. Enrage (Cost: 2 Mana) -> Rage goes up by 3.\n" <<
            "3. Roar (Cost: 1 Mana) -> Enemies focus on you and you gain 2 DEF for the current turn.\n\n";
}

void Warrior::onTurnStart() {
    rage += 1;
}

void Warrior::onWaveStart() {
    rage = -1;
}

void Warrior::showSkills() const{
    cout << "\n--- Warrior Skills ---\n"
         << "Passive. Every turn gain 1 rage! [+1 ATK]\n"
         << "1. Heavy Attack (Cost: 3 Mana) -> Deal 150% attack damage.\n"
         << "2. Enrage (Cost: 2 Mana) -> Rage goes up by 3.\n"
         << "3. Roar (Cost: 1 Mana) -> Enemies focus on you and you gain 2 DEF for the current turn.\n"
         << "0. Cancel.\n\n"
         << "Choose skill: ";
}

bool Warrior::useSkill(int actionSkill, vector<Character*>& activeParty, vector<Monster*>& enemies){
    if (actionSkill == 1){
        if (!spendMana(3)) return false;
        Monster* target = selectTarget(enemies);
        int dmg = (getAttack() * 1.5) - target->getDefense();
        target->takeDamage(dmg);
        cout << "\n" << name << " uses Heavy Attack on " << target->getName() << " for " << dmg << " damage!\n";
        return true;
    }
    else if (actionSkill == 2){
        if (!spendMana(2)) return false;
        rage += 3;
        cout << "\n" << name << " used Enrage to gain 2 ATK!\n";
        return true;
    }
    else if (actionSkill == 3){
        if (!spendMana(1)) return false;
        focus();
        cout << "\n" << name << " used Roar to gain 2 DEF and enemy attention!\n";
        return true;
    }

    cout << "Invalid skill choice!\n";
    return false;
}

//------------------------------Mage Stuff------------------------------

Mage::Mage(string name_, int manaRegen_) : Character(name_, 10, 35, 15, 6){
    manaRegen = manaRegen_;
}

void Mage::displayStats(ostream& os) const{
    os << "Class: Mage\n";
    Character::displayStats(os);
    os << "Mana regen: " << manaRegen << "\n";
}

void Mage::classDescription(){
    cout << "Mage: Boosts mana regeneration. \n" <<
            "Every time you start a new turn or wait, you gain 2 more mana.\n\n" <<
            "Attack: 10 \n" <<
            "Health: 35 \n" <<
            "Mana: 15 \n" <<
            "Defense: 6 \n \n" <<
            "Skills: \n" <<
            "1. Fireball (Cost: 5 Mana) -> Deal 3 damage to all enemies and set them on fire for 2 additional turns (1 damage each).\n" <<
            "2. Ice Blast (Cost: 3 Mana) -> Freeze an enemy and disable their turn.\n" <<
            "3. Rust (Cost: 3 Mana) -> Weaken enemy DEF by 50%.\n\n";
}

void Mage::onTurnStart() {
    restoreMana(manaRegen);
}

void Mage::showSkills() const{
    cout << "\n--- Mage Skills ---\n"
         << "Passive. Every time you start a new turn or wait, you gain 2 more mana!\n"
         << "1. Fireball (Cost: 5 Mana) -> Deal 3 damage to all enemies and set them on fire for 2 additional turns (1 damage each).\n"
         << "2. Ice Blast (Cost: 3 Mana) -> Freeze an enemy and disable their turn.\n"
         << "3. Rust (Cost: 3 Mana) -> Weaken enemy DEF by 50%.\n"
         << "0. Cancel.\n\n"
         << "Choose skill: ";
}

bool Mage::useSkill(int actionSkill, vector<Character*>& activeParty, vector<Monster*>& enemies){
    if (actionSkill == 1){
        if (!spendMana(5)) return false;
        for (Monster* enemy : enemies) {
            if (enemy->isAlive()) {
                enemy->takeDamage(3);
                enemy->getOverDamage(1, 2);
            }
        }
        cout << "\n" << name << " used fireball on the enemies and dealt 3 damage to each.\n";
        return true;
    }
    else if (actionSkill == 2){
        if (!spendMana(3)) return false;
        Monster* target = selectTarget(enemies);
        target->applyStun();
        cout << "\n" << name << " froze " << target->getName() << " for the next turn!\n";
        return true;
    }
    else if (actionSkill == 3){
        if (!spendMana(3)) return false;
        Monster* target = selectTarget(enemies);
        target->reduceDefense();
        cout << "\n" << name << " has rusted " << target->getName() << " armor and cut their DEF in half!\n";
        return true;
    }

    cout << "Invalid skill choice!\n";
    return false;
}

//------------------------------Assassin Stuff------------------------------

Assassin::Assassin(string name_, int evasion_) : Character(name_, 15, 25, 10, 4){
    evasion = evasion_;
}

void Assassin::displayStats(ostream& os) const{
    os << "Class: Assassin\n";
    Character::displayStats(os);
    os << "Evasion: " << evasion << "\n";
}

void Assassin::classDescription(){
    cout << "Assassin: Adds a unique stat called Evasion. \n" <<
            "You gain the ability to dodge attacks (default 25%)\n\n" <<
            "Attack: 15 \n" <<
            "Health: 25 \n" <<
            "Mana: 10 \n" <<
            "Defense: 4 \n \n" <<
            "Skills: \n" <<
            "1. Throwing Knife (Cost: 2 Mana) -> Deal 9 damage to an enemy and gain +25% evasion for the current turn.\n" <<
            "2. Cloak (Cost: 3 Mana) -> Grants 100% evasion for this and next turn.\n" <<
            "3. Surprise Attack (Cost: 3 Mana) -> Sneak up on an enemy to deal 25 damage (success rate equal to evasion).\n\n";
}

void Assassin::showSkills() const{
    cout << "\n--- Assassin Skills ---\n"
         << "Passive. Have an ability to dodge attacks equal to evasion! [25% default]\n"
         << "1. Throwing Knife (Cost: 2 Mana) -> Deal 9 damage to an enemy and gain +25% evasion for the current turn.\n"
         << "2. Cloak (Cost: 3 Mana) -> Grants 100% evasion for this and next turn.\n"
         << "3. Surprise Attack (Cost: 3 Mana) -> Sneak up on an enemy to deal 25 damage (success rate equal to evasion).\n"
         << "0. Cancel.\n\n"
         << "Choose skill: ";
}

int Assassin::getEvasion() const {
    return evasion;
}

void Assassin::onTurnStart() {
    if (evasion > 100) {
        evasion = 100;
    } else {
        evasion = 25;
    }
}

void Assassin::onWaveStart() {
    evasion = 25;
}

bool Assassin::useSkill(int actionSkill, vector<Character*>& activeParty, vector<Monster*>& enemies){
    if (actionSkill == 1){
        if (!spendMana(2)) return false;
        Monster* target = selectTarget(enemies);
        target->takeDamage(9);
        evasion += 25;
        cout << "\n" << name << " has damaged " << target->getName() << " with a knife and gained 25% evasion for this turn!\n";
        return true;
    }
    else if (actionSkill == 2){
        if (!spendMana(3)) return false;
        evasion = 125;
        cout << "\n" << name << " has become invisible until the end of next turn!\n";
        return true;
    }
    else if (actionSkill == 3){
        if (!spendMana(3)) return false;
        Monster* target = selectTarget(enemies);
        int roll = rand() % 100; // Generates 0..99
        if (roll < getEvasion()) {
            target->takeDamage(25);
            cout << "\n" << name << " sneaks up on " << target->getName() << " and hits them for 25 damage!\n";
            return true;
        }
        cout << "\n" << target->getName() << " has forseen the surprise attack from " << name << ".\n";
        return true;
    }
    cout << "Invalid skill choice!\n";
    return false;
}