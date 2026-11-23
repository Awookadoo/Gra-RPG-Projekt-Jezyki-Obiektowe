#include "characters.h"
#include "utility.h"
#include "monsters.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

Character::Character(string name_, int attack_, int maxHealth_, int maxMana_, int defense_)
    : name(name_), attack(attack_), maxHealth(maxHealth_), maxMana(maxMana_),
    defense(defense_), currentHealth(maxHealth_), currentMana(maxMana_) {
    if (name.empty() || name.length() > 13) {
        throw invalid_argument("Character name must contain 1 to 13 characters.");
    }
    if (attack < 0 || maxHealth <= 0 || maxMana < 0 || defense < 0) {
        throw invalid_argument("Character stats must be non-negative and max health must be positive.");
    }
}

Character::~Character() {}

//funkcja do wyświetlania statystyk postaci (przeciążenie operatora <<)
void Character::displayStats(ostream& os) const {
    os << "Name: " << name << "\n"
       << "Class: " << getClassName() << "\n"
       << "Attack: " << getAttack() << "\n"
       << "Health: " << maxHealth << "\n"
       << "Mana: " << maxMana << "\n"
       << "Defense: " << getDefense() << "\n \n";
}

ostream& operator<<(ostream& os, const Character& character) {
    character.displayStats(os);
    return os;
}


//porównywanie imion i klas postaci do uniknięcia powtórek (przeciążenie operatora ==)
bool Character::operator==(const Character& other) const {
    return name == other.name && getClassName() == other.getClassName();
}

//------------------------------Combat Stuff------------------------------

void Character::increaseBaseStats(int amount) {
    if (amount <= 0) {
        return;
    }
    maxHealth += amount;
    currentHealth += amount;
    attack += amount;
    defense += amount;
}

void Character::takeDamage(int dmg){
    if (guarding){
        cout << name << " guarded and negated the damage!\n";
        return;
    }
    if (dmg < 0) return;
    currentHealth -= dmg;
    hitThisTurn = true;
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
    guarding = false;
    focusing = false;
    stunned = false;
    hitThisTurn = false;
    overDamage = 0;
    overTurns = 0;
}

int Character::calculateDamage(double attack_, double defense_) {
    double damage = attack_ - defense_;
    return (damage > 1.0) ? damage : 1;
}

//------------------------------Basic Actions------------------------------

void Character::basicAttack(Character* target){
    int dmg = calculateDamage(getAttack(), target->getDefense());
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

void Character::heal(){
    restoreHealth(8);
    cout << "\n" << name << " has healed themselves 8 HP.\n";
}

void Character::wait(){
    cout << "\n" << name << " skipped their turn.\n";
}

Monster* Character::selectTarget(const vector<Monster*>& enemies) const{
    while (true){
        int target;
        cout << "Choose target: ";
        if (!(cin >> target)) {
            clearInput();
            cout << "Invalid target, try again.\n";
            continue;
        }
        clearInput();
        if (target < 1 || static_cast<size_t>(target) > enemies.size() || !enemies[target - 1]->isAlive()) {
            cout << "Invalid or dead target! Try again.\n";
        }
        else {
            return enemies[target - 1];
        }
    }
}

Character* Character::selectAlly(const vector<Character*>& activeParty) const{
    while (true){
        int target;
        cout << "Choose ally: ";
        if (!(cin >> target)) {
            clearInput();
            cout << "Invalid ally, try again.\n";
            continue;
        }
        clearInput();
        if (target < 1 || static_cast<size_t>(target) > activeParty.size() || !activeParty[target - 1]->isAlive()) {
            cout << "Invalid or dead ally! Try again.\n";
        }
        else {
            return activeParty[target - 1];
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
        cout << "\n" << name << " takes " << overDamage << " damage over time! (" << overTurns << " turns left)\n";
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

void Warrior::onTurnStart() {
    rage += 1;
}

void Warrior::onWaveStart() {
    rage = -1;
}

void Warrior::showSkills(bool showIt) const{
    cout << "\n--- Warrior Skills ---\n"
         << "Passive. Every turn gain 1 rage! [+1 ATK] - resets every wave.\n\n"
         << "1. Heavy Attack (Cost: 3 Mana) -> Deal 150% attack damage.\n"
         << "2. Enrage (Cost: 2 Mana) -> Rage goes up by 3.\n"
         << "3. Roar (Cost: 2 Mana) -> Enemies focus on you and you gain 5 DEF for the current turn.\n";
    displaySkills(showIt);
}

bool Warrior::useSkill(int actionSkill, vector<Character*>& activeParty, vector<Monster*>& enemies){
    if (actionSkill == 1){
        if (!spendMana(3)) return false;
        Monster* target = selectTarget(enemies);
        int dmg = calculateDamage(getAttack() * 1.5, target->getDefense());
        target->takeDamage(dmg);
        cout << "\n" << getName() << " uses Heavy Attack on " << target->getName() << " for " << dmg << " damage!\n";
        return true;
    }
    else if (actionSkill == 2){
        if (!spendMana(2)) return false;
        rage += 3;
        cout << "\n" << getName() << " used Enrage to gain 3 ATK!\n";
        return true;
    }
    else if (actionSkill == 3){
        if (!spendMana(2)) return false;
        focus();
        cout << "\n" << getName() << " used Roar to gain 5 DEF and enemy attention!\n";
        return true;
    }

    cout << "Invalid skill choice!\n";
    return false;
}

//------------------------------Mage Stuff------------------------------

Mage::Mage(string name_, int manaRegen_) : Character(name_, 10, 35, 12, 6){
    manaRegen = manaRegen_;
}

void Mage::onTurnStart() {
    restoreMana(manaRegen);
}

void Mage::showSkills(bool showIt) const{
    cout << "\n--- Mage Skills ---\n"
         << "Passive. Every time you start a new turn you gain 2 more mana!\n\n"
         << "1. Fireball (Cost: 5 Mana) -> Deal 5 damage to all enemies and set them on fire for 2 additional turns (1 damage each).\n"
         << "2. Ice Blast (Cost: 3 Mana) -> Freeze an enemy and disable their turn.\n"
         << "3. Rust (Cost: 2 Mana) -> Weaken enemy DEF by 50% permamently.\n";
    displaySkills(showIt);
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
        cout << "\n" << getName() << " used fireball on the enemies and dealt 5 damage to each.\n";
        return true;
    }
    else if (actionSkill == 2){
        if (!spendMana(3)) return false;
        Monster* target = selectTarget(enemies);
        target->applyStun();
        cout << "\n" << getName() << " froze " << target->getName() << " for the next turn!\n";
        return true;
    }
    else if (actionSkill == 3){
        if (!spendMana(2)) return false;
        Monster* target = selectTarget(enemies);
        target->halfDefense();
        cout << "\n" << getName() << " has rusted " << target->getName() << " armor and cut their DEF in half!\n";
        return true;
    }

    cout << "Invalid skill choice!\n";
    return false;
}

//------------------------------Assassin Stuff------------------------------

Assassin::Assassin(string name_, int evasion_) : Character(name_, 15, 25, 10, 4){
    evasion = evasion_;
}

void Assassin::showSkills(bool showIt) const{
    cout << "\n--- Assassin Skills ---\n"
         << "Passive. Have an ability to dodge attacks equal to evasion! [25% default]\n\n"
         << "1. Throwing Knife (Cost: 2 Mana) -> Deal 9 damage to an enemy and gain +25% evasion for the current turn.\n"
         << "2. Cloak (Cost: 3 Mana) -> Grants 100% evasion for this and next turn.\n"
         << "3. Surprise Attack (Cost: 3 Mana) -> Sneak up on an enemy to deal 25 damage (success rate equal to evasion).\n";
    displaySkills(showIt);
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
        cout << "\n" << getName() << " has damaged " << target->getName() << " with a knife and gained 25% evasion for this turn!\n";
        return true;
    }
    else if (actionSkill == 2){
        if (!spendMana(3)) return false;
        evasion = 125;
        cout << "\n" << getName() << " has become invisible until the end of next turn!\n";
        return true;
    }
    else if (actionSkill == 3){
        if (!spendMana(3)) return false;
        Monster* target = selectTarget(enemies);
        int roll = rand() % 100; //
        if (roll < getEvasion()) {
            target->takeDamage(25);
            cout << "\n" << getName() << " sneaks up on " << target->getName() << " and hits them for 25 damage!\n";
            return true;
        }
        cout << "\n" << target->getName() << " has forseen the surprise attack from " << getName() << ".\n";
        return true;
    }
    cout << "Invalid skill choice!\n";
    return false;
}

//------------------------------Cleric Stuff------------------------------

Cleric::Cleric(string name_, int divinePower_) : Character(name_, 7, 30, 9, 5){
    divinePower = divinePower_;
}

void Cleric::showSkills(bool showIt) const{
    cout << "\n--- Cleric Skills ---\n"
         << "Passive. Divine power increases the effectivness of your skill boosts - resets back to 2 every wave.\n\n"
         << "1. Team Heal (Cost: 3 Mana) -> Heal your entire team for 6 +" << divinePower << " (divine power) health.\n"
         << "2. Holy Barrier (Cost: 5 Mana) -> Increases the DEF of the chosen party member by " << divinePower << " (divine power) permanently.\n"
         << "3. Pray (Cost: 2 Mana) -> Divine Power increases by 1 for the current wave.\n";
    displaySkills(showIt);
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
        cout << "\n" << getName() << " heals the entire team for "<< healPower << " health!\n";
        return true;
    }
    else if (actionSkill == 2){
        if (!spendMana(5)) return false;
        Character* target = selectAlly(activeParty);
        target->addBonusDefense(divinePower);
        cout << "\n" << getName() << " casts protection on " << target->getName() << " and increases their defense by " << divinePower << "!\n";
        return true;
    }
    else if (actionSkill == 3){
        if (!spendMana(2)) return false;
        divinePower += 1;
        cout << "\n" << getName() << " prays and increases Divine Power to " << divinePower << "!\n";
        return true;
    }

    cout << "Invalid skill choice!\n";
    return false;
}

//------------------------------Reaper Stuff------------------------------

Reaper::Reaper(string name_, int lifeSteal_) : Character(name_, 9, 30, 10, 2){
    lifeSteal = lifeSteal_;
}

void Reaper::basicAttack(Character* target) {
    int healthBeforeAttack = target->getHealth();
    Character::basicAttack(target);
    int damageDealt = healthBeforeAttack - target->getHealth();
    int healthStolen = (lifeSteal < damageDealt) ? lifeSteal : damageDealt;
    if (healthStolen > 0) {
        restoreHealth(healthStolen);
        lifeSteal -= healthStolen;
        cout << getName() << " stole " << healthStolen << " HP from the attack!\n";
    }
}

void Reaper::onTurnStart(){
    lifeSteal += 1;
}

void Reaper::showSkills(bool showIt) const{
    cout << "\n--- Reaper Skills ---\n"
         << "Passive. Every turn gain one life steal point, healing you each for 1 HP on basic attack.\n\n"
         << "1. Death Bomb (Cost: 7 Health) -> Deal 6 damage to every enemy and gain 2 life steal points.\n"
         << "2. Drain Blade (Cost: 3 Health) -> Deal 3 damage to an enemy, reduce their DEF by 2 and gain 1 life steal point.\n"
         << "3. Life Swap (Cost: 8 Mana) -> Swap the amount of health with an enemy (health cannot exceed maximum).\n";
    displaySkills(showIt);
}

bool Reaper::useSkill(int actionSkill, vector<Character*>& activeParty, vector<Monster*>& enemies){
    if (actionSkill == 1){
        if (!spendHealth(7)) return false;
        for (Monster* enemy : enemies){
            if(enemy->isAlive()){
                enemy->takeDamage(6);
            }
        }
        lifeSteal += 2;
        cout << "\n" << getName() << " uses Death Bomb to deal 6 damage to all enemies.\n";
        return true;
    }
    else if (actionSkill == 2){
        if (!spendHealth(3)) return false;
        Monster* target = selectTarget(enemies);
        target->takeDamage(3); target->reduceDefense(2);
        lifeSteal += 1;
        cout << "\n" << getName() << " drained 3 HP out of the enemy and reduced their DEF by 2!\n";
        return true;
    }
    else if (actionSkill == 3){
        if (!spendMana(8)) return false;
        Monster* target = selectTarget(enemies);
        int tmpHealth = target->getHealth();
        target->setHealth(getHealth());
        setHealth(tmpHealth);
        cout << "\n" << getName() << " swapped their health with " << target->getName() << "!\n";
        return true;
    }

    cout << "Invalid skill choice!\n";
    return false;
}