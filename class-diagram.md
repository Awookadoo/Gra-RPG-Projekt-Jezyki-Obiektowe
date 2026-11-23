# UML class diagram

The diagram uses Mermaid class-diagram syntax and renders on GitHub. In the member lists, `+` means public, `#` protected, and `-` private. `Character*[]` and `Monster*[]` represent vectors of pointers; `&` marks a reference. C++ `const` qualifiers are omitted for readability.

```mermaid
classDiagram
    direction LR

    class Character {
        -string name
        -int attack
        -int maxHealth
        -int maxMana
        -int defense
        -int currentHealth
        -int currentMana
        -bool inParty
        -bool guarding
        -bool focusing
        -bool stunned
        -bool hitThisTurn
        -int overDamage
        -int overTurns
        -int bonusDefense
        #increaseBaseStats(amount: int) void
        #displaySkills(showIt: bool) void
        +Character(name: string, attack: int, maxHealth: int, maxMana: int, defense: int)
        +virtual ~Character()
        +static createCharacter() Character*
        +virtual displayStats(os: ostream&) void
        +virtual getClassName() string
        +friend operator&lt;&lt;(os: ostream&, character: Character&) ostream&
        +operator==(other: Character) bool
        +getName() string
        +getHealth() int
        +getMaxHealth() int
        +virtual getAttack() int
        +getDefense() int
        +getMaxMana() int
        +getMana() int
        +isAlive() bool
        +wasHitThisTurn() bool
        +clearHitStatus() void
        +isInParty() bool
        +setInParty(status: bool) void
        +ifActive() string
        +takeDamage(dmg: int) void
        +restoreHealth(amountHealth: int) void
        +setHealth(amountHealth: int) void
        +restoreMana(amountMana: int) void
        +restore() void
        +heal() void
        +static calculateDamage(attack: double, defense: double) int
        +selectTarget(enemies: Monster*[]&) Monster*
        +virtual basicAttack(target: Character*) void
        +wait() void
        +guard() void
        +resetGuard() void
        +virtual onTurnStart() void
        +virtual onWaveStart() void
        +virtual showSkills(showIt: bool = true) void
        +virtual useSkill(actionSkill: int, activeParty: Character*[]&, enemies: Monster*[]&) bool
        +spendMana(manaCost: int) bool
        +focus() void
        +resetFocus() void
        +isFocusing() bool
        +getOverDamage(dmgPerTurn: int, turns: int) void
        +applyOverDamage() void
        +applyStun() void
        +isStunned() bool
        +clearStun() void
        +halfDefense() void
        +virtual getEvasion() int
        +addBonusDefense(amount: int) void
        +resetBonusDefense() void
        +selectAlly(activeParty: Character*[]&) Character*
        +spendHealth(healthCost: int) bool
        +reduceDefense(amount: int) void
    }

    class Warrior {
        -int rage
        +Warrior(name: string, rage: int = -1)
        +getClassName() string
        +getAttack() int
        +onTurnStart() void
        +onWaveStart() void
        +showSkills(showIt: bool) void
        +useSkill(actionSkill: int, activeParty: Character*[]&, enemies: Monster*[]&) bool
    }

    class Mage {
        -int manaRegen
        +Mage(name: string, manaRegen: int = 2)
        +getClassName() string
        +onTurnStart() void
        +showSkills(showIt: bool) void
        +useSkill(actionSkill: int, activeParty: Character*[]&, enemies: Monster*[]&) bool
    }

    class Assassin {
        -int evasion
        +Assassin(name: string, evasion: int = 25)
        +getClassName() string
        +getEvasion() int
        +onTurnStart() void
        +onWaveStart() void
        +showSkills(showIt: bool) void
        +useSkill(actionSkill: int, activeParty: Character*[]&, enemies: Monster*[]&) bool
    }

    class Cleric {
        -int divinePower
        +Cleric(name: string, divinePower: int = 2)
        +getClassName() string
        +onWaveStart() void
        +showSkills(showIt: bool) void
        +useSkill(actionSkill: int, activeParty: Character*[]&, enemies: Monster*[]&) bool
    }

    class Reaper {
        -int lifeSteal
        +Reaper(name: string, lifeSteal: int = 0)
        +getClassName() string
        +onTurnStart() void
        +basicAttack(target: Character*) void
        +showSkills(showIt: bool) void
        +useSkill(actionSkill: int, activeParty: Character*[]&, enemies: Monster*[]&) bool
    }

    class Monster {
        <<abstract>>
        #attackTarget(target: Character*) void
        +Monster(name: string, attack: int, maxHealth: int, defense: int)
        +virtual takeTurn(playerParty: Character*[]&) void
        +statUp(loop: int) void
    }

    class Goblin {
        #attackTarget(target: Character*) void
        +Goblin()
    }

    class Skeleton {
        #attackTarget(target: Character*) void
        +Skeleton()
    }

    class Slime {
        #attackTarget(target: Character*) void
        +Slime()
    }

    class Ghoul {
        #attackTarget(target: Character*) void
        +Ghoul()
    }

    class Rat {
        #attackTarget(target: Character*) void
        +Rat()
    }

    class Game {
        -Character*[] party
        -bool running
        -monsterWave(wave: int) Monster*[]
        -displayMainMenu() void
        -mainMenu() void
        -manageParty() void
        -currentPartyCount() int
        +Game()
        +~Game()
        +run() void
        +startGame() void
        +addCharacter(c: Character*) bool
    }

    class Combat {
        -Character*[]& activeParty
        -Monster*[] enemies
        -bool flee
        -int wave
        -displayParty(hasTurn: Character*) void
        -displayEnemies(hasTurn: Character*) void
        -displayBattlefield(hasTurn: Character*, clearScreen: bool = true) void
        -resetHitStatuses() void
        -playerTurn() void
        -enemyTurn() void
        -isPartyAlive() bool
        -areEnemiesAlive() bool
        +Combat(activeParty: Character*[]&, enemies: Monster*[]&, currentWave: int)
        +~Combat()
        +startCombat() bool
    }

    Character <|-- Warrior
    Character <|-- Mage
    Character <|-- Assassin
    Character <|-- Cleric
    Character <|-- Reaper
    Character <|-- Monster
    Monster <|-- Goblin
    Monster <|-- Skeleton
    Monster <|-- Slime
    Monster <|-- Ghoul
    Monster <|-- Rat

    Game "1" *-- "0..*" Character : owns roster
    Game ..> Combat : starts battles
    Game ..> Monster : creates waves
    Combat "1" *-- "0..*" Monster : owns enemies
    Combat ..> Character : borrows active party

    note for Monster "Abstract: attackTarget is pure virtual. Each concrete monster overrides it."
    note for Character "Character state and operations shown here match characters.h. Virtual methods dispatch polymorphically. Static methods: createCharacter, calculateDamage, and displaySkills."
```
