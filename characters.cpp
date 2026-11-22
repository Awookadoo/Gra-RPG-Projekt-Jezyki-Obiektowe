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
        "3 - Assassin\n" <<
        "4 - Cleric\n" <<
        "5 - Reaper\n\n";

        if (!(cin >> classChoice)) {
            cout << "Invalid choice, try again. \n";
            clearInput();
            continue;
        }

        if (classChoice >= 1 && classChoice <= 5) {
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
            if (classChoice == 4) {
                Cleric tempCleric(name);
                tempCleric.classDescription();
            }
            if (classChoice == 5) {
                Reaper tempReaper(name);
                tempReaper.classDescription();
            }

            cout << "Is this your final choice? (y/n): "; cin >> confirmation;
            if (confirmation == 'y') {
                if (classChoice == 1) return new Warrior(name);
                if (classChoice == 2) return new Mage(name);
                if (classChoice == 3) return new Assassin(name);
                if (classChoice == 4) return new Cleric(name);
                if (classChoice == 5) return new Reaper(name);
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
    restoreHealth(10);
    cout << "\n" << name << " has healed themselves 10 HP.\n";
}

void Character::wait(){
    cout << "\n" << name << " skipped his turn.\n";
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

Character* Character::selectAlly(const vector<Character*>& activeParty) const{
    while (true){
        int target;
        cout << "Choose ally: "; cin >> target; target--;
        clearInput();
        if (target < 0 || target >= activeParty.size() || !activeParty[target]->isAlive()) {
            cout << "Invalid or dead ally! Try again.\n";
        }
        else {
            return activeParty[target];
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

void Character::halfDefense(){
    defense = defense * 0.5;
    if (defense < 0){
        defense = 0;
    }
}

void Character::setHealth(int amount){
    currentHealth = amount;
    if (currentHealth > maxHealth) currentHealth = maxHealth;
    if (currentHealth < 0) currentHealth = 0;
}

bool Character::spendHealth(int healthCost){
    if (currentHealth <= healthCost) {
        cout << "Not enough health!\n";
        return false;
    }
    currentHealth -= healthCost;
    return true;
}

void Character::reduceDefense(int amount) {
    defense -= amount;
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
            "3. Roar (Cost: 2 Mana) -> Enemies focus on you and you gain 5 DEF for the current turn.\n\n";
}

void Warrior::onTurnStart() {
    rage += 1;
}

void Warrior::onWaveStart() {
    rage = -1;
    attack = 10;
}

void Warrior::showSkills() const{
    cout << "\n--- Warrior Skills ---\n"
         << "Passive. Every turn gain 1 rage! [+1 ATK] - resets every wave.\n\n"
         << "1. Heavy Attack (Cost: 3 Mana) -> Deal 150% attack damage.\n"
         << "2. Enrage (Cost: 2 Mana) -> Rage goes up by 3.\n"
         << "3. Roar (Cost: 2 Mana) -> Enemies focus on you and you gain 5 DEF for the current turn.\n"
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
        if (!spendMana(2)) return false;
        focus();
        cout << "\n" << name << " used Roar to gain 5 DEF and enemy attention!\n";
        return true;
    }

    cout << "Invalid skill choice!\n";
    return false;
}

//------------------------------Mage Stuff------------------------------

Mage::Mage(string name_, int manaRegen_) : Character(name_, 10, 35, 12, 6){
    manaRegen = manaRegen_;
}

void Mage::displayStats(ostream& os) const{
    os << "Class: Mage\n";
    Character::displayStats(os);
    os << "Mana regen: " << manaRegen << "\n";
}

void Mage::classDescription(){
    cout << "Mage: Boosts mana regeneration. \n" <<
            "Every time you start a new turn you gain 2 more mana.\n\n" <<
            "Attack: 10 \n" <<
            "Health: 35 \n" <<
            "Mana: 15 \n" <<
            "Defense: 6 \n \n" <<
            "Skills: \n" <<
            "1. Fireball (Cost: 5 Mana) -> Deal 5 damage to all enemies and set them on fire for 2 additional turns (1 damage each).\n" <<
            "2. Ice Blast (Cost: 3 Mana) -> Freeze an enemy and disable their turn.\n" <<
            "3. Rust (Cost: 2 Mana) -> Weaken enemy DEF by 50%.\n\n";
}

void Mage::onTurnStart() {
    restoreMana(manaRegen);
}

void Mage::showSkills() const{
    cout << "\n--- Mage Skills ---\n"
         << "Passive. Every time you start a new turn you gain 2 more mana!\n\n"
         << "1. Fireball (Cost: 5 Mana) -> Deal 3 damage to all enemies and set them on fire for 2 additional turns (1 damage each).\n"
         << "2. Ice Blast (Cost: 3 Mana) -> Freeze an enemy and disable their turn.\n"
         << "3. Rust (Cost: 2 Mana) -> Weaken enemy DEF by 50% permamently.\n"
         << "0. Cancel.\n\n"
         << "Choose skill: ";
}

bool Mage::useSkill(int actionSkill, vector<Character*>& activeParty, vector<Monster*>& enemies){
    if (actionSkill == 1){
        if (!spendMana(5)) return false;
        for (Monster* enemy : enemies) {
            if (enemy->isAlive()) {
                enemy->takeDamage(5);
                enemy->getOverDamage(1, 2);
            }
        }
        cout << "\n" << name << " used fireball on the enemies and dealt 5 damage to each.\n";
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
        if (!spendMana(2)) return false;
        Monster* target = selectTarget(enemies);
        target->halfDefense();
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
         << "Passive. Have an ability to dodge attacks equal to evasion! [25% default]\n\n"
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
        int roll = rand() % 100; //
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

//------------------------------Cleric Stuff------------------------------

Cleric::Cleric(string name_, int divinePower_) : Character(name_, 8, 30, 10, 8){
    divinePower = divinePower_;
}

void Cleric::displayStats(ostream& os) const{
    os << "Class: Cleric\n";
    Character::displayStats(os);
    os << "Heal Power: " << divinePower << "\n";
}

void Cleric::classDescription(){
    cout << "Cleric: Adds a unique stat called Divine Power. \n" <<
            "Divine power increases the effectivness of your skill boosts.\n" <<
            "Resets every wave.\n\n" <<
            "Attack: 8 \n" <<
            "Health: 30 \n" <<
            "Mana: 10 \n" <<
            "Defense: 8 \n \n" <<
            "Skills: \n" <<
            "1. Team Heal (Cost: 3 Mana) -> Heal your entire team for 6 [+" << divinePower << "] health.\n" <<
            "2. Holy Barrier (Cost: 5 Mana) -> Increases the DEF of the chosen party member by " << divinePower << ". [Divine Power]\n" <<
            "3. Pray (Cost: 2 Mana) -> Divine Power increases by 1 for the current wave.\n\n";
}

void Cleric::showSkills() const{
    cout << "\n--- Cleric Skills ---\n"
         << "Passive. Divine power increases the effectivness of your skill boosts - resets back to 2 every wave.\n\n"
         << "1. Team Heal (Cost: 3 Mana) -> Heal your entire team for 6 [+" << divinePower << "] health.\n"
         << "2. Holy Barrier (Cost: 5 Mana) -> Increases the DEF of the chosen party member by " << divinePower << " [Divine Power] permanently.\n"
         << "3. Pray (Cost: 2 Mana) -> Divine Power increases by 1 for the current wave.\n"
         << "0. Cancel.\n\n"
         << "Choose skill: ";
}

void Cleric::onWaveStart() {
    divinePower = 2;
}

bool Cleric::useSkill(int actionSkill, vector<Character*>& activeParty, vector<Monster*>& enemies){
    if (actionSkill == 1){
        if (!spendMana(3)) return false;
        int healPower = 6 + divinePower;
        for (Character* hero : activeParty){
            if(hero->isAlive()){
                hero->restoreHealth(healPower);
            }
        }
        cout << "\n" << name << " heals the entire team for "<< healPower << " health!\n";
        return true;
    }
    else if (actionSkill == 2){
        if (!spendMana(5)) return false;
        Character* target = selectAlly(activeParty);
        target->addBonusDefense(divinePower);
        cout << "\n" << name << " casts protection on " << target->getName() << " and increases their defense by " << divinePower << "!\n";
        return true;
    }
    else if (actionSkill == 3){
        if (!spendMana(2)) return false;
        divinePower += 1;
        cout << "\n" << name << " prays and increases Divine Power to " << divinePower << "!\n";
        return true;
    }

    cout << "Invalid skill choice!\n";
    return false;
}

//------------------------------Reaper Stuff------------------------------

Reaper::Reaper(string name_, int lifeSteal_) : Character(name_, 9, 35, 10, 3){
    lifeSteal = lifeSteal_;
}

void Reaper::displayStats(ostream& os) const{
    os << "Class: Reaper\n";
    Character::displayStats(os);
    os << "Life steal: " << lifeSteal << "\n";
}

void Reaper::classDescription(){
    cout << "Reaper: Adds a unique stat called life steal. \n" <<
            "Every turn gain one life steal point.\n" <<
            "For every point you have, while using basic attack on the enemy you gain 1 HP.\n\n" <<
            "Attack: 9 \n" <<
            "Health: 35 \n" <<
            "Mana: 10 \n" <<
            "Defense: 3 \n \n" <<
            "Skills: \n" <<
            "1. Death Bomb (Cost: 5 Health) -> Deal 6 damage to every enemy and gain 2 life steal points.\n" <<
            "2. Drain Blade (Cost: 2 Health) -> Deal 3 damage to an enemy, reduce their DEF by 2 and gain 1 life steal point.\n" <<
            "3. Life Swap (Cost: 7 Mana) -> Swap the amount of health with an enemy (health cannot exceed maximum).\n\n";
}

void Reaper::basicAttack(Character* target) {
    Character::basicAttack(target);
    if (lifeSteal > 0) {
        restoreHealth(lifeSteal);
        cout << name << " stole " << lifeSteal << " HP from the 9 damage attack!\n";
    }
}

void Reaper::onTurnStart(){
    lifeSteal += 1;
}

void Reaper::showSkills() const{
    cout << "\n--- Reaper Skills (Life Steal: " << lifeSteal << ") ---\n"
         << "Passive. Every turn gain one life steal point, healing you each for 1 HP on basic attack.\n\n"
         << "1. Death Bomb (Cost: 5 Health) -> Deal 6 damage to every enemy and gain 2 life steal points.\n"
         << "2. Drain Blade (Cost: 2 Health) -> Deal 3 damage to an enemy, reduce their DEF by 2 and gain 1 life steal point.\n"
         << "3. Life Swap (Cost: 10 Mana) -> Swap the amount of health with an enemy (health cannot exceed maximum).\n"
         << "0. Cancel.\n\n"
         << "Choose skill: ";
}

bool Reaper::useSkill(int actionSkill, vector<Character*>& activeParty, vector<Monster*>& enemies){
    if (actionSkill == 1){
        if (!spendHealth(5)) return false;
        for (Monster* enemy : enemies){
            if(enemy->isAlive()){
                enemy->takeDamage(6);
            }
        }
        lifeSteal += 2;
        cout << "\n" << name << " uses Death Bomb to deal 6 damage to all enemies.\n";
        return true;
    }
    else if (actionSkill == 2){
        if (!spendHealth(2)) return false;
        Monster* target = selectTarget(enemies);
        target->takeDamage(3); target->reduceDefense(2);
        lifeSteal += 1;
        cout << "\n" << name << " drained 3 HP out of the enemy and reduced their DEF by 2!\n";
        return true;
    }
    else if (actionSkill == 3){
        if (!spendMana(7)) return false;
        Monster* target = selectTarget(enemies);
        int tmpHealth = target->getHealth();
        target->setHealth(getHealth());
        setHealth(tmpHealth);
        cout << "\n" << name << " swapped their health with " << target->getName() << "!\n";
        return true;
    }

    cout << "Invalid skill choice!\n";
    return false;
}