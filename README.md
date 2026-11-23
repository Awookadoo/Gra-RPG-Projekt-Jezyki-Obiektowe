# RPG Game

A text-based RPG game written in C++17. Create characters, manage a party, and fight monsters in a terminal.

## Features

- Create characters and choose their class.
- Add characters to and manage the party.
- Fight monsters using character actions and class-specific skills.
- View party and monster statistics, with colors highlighting turn and hit status.

## Architecture

- `Character` is the polymorphic base for player classes and monsters. Its state is private and is accessed or changed through member functions.
- `Warrior`, `Mage`, `Assassin`, `Cleric`, and `Reaper` derive from `Character`. They override turn behavior and skill methods to provide class-specific abilities.
- `Monster` also derives from `Character` and is abstract. `Goblin`, `Skeleton`, `Slime`, `Ghoul`, and `Rat` implement their own attacks.
- `Game` manages the character roster, party selection, menus, and the sequence of waves. It owns the characters in its roster.
- `Combat` runs a battle using the selected party and the monsters for that wave. It borrows the party characters and owns the wave's monsters.
- `operator<<` prints a character's statistics. `Character::operator==` compares name and class and is used to prevent duplicate roster entries. The `Character` constructor throws `std::invalid_argument` when its name or base statistics are invalid; character creation catches and reports this error.

The headers declare the classes and their interfaces; the `.cpp` files implement the game, character, monster, and combat behavior.
See the [UML class diagram](./class-diagram.md) for all classes, members, and relationships.

## Example session

To start a game quickly with the example party:

1. Launch the program and press Enter.
2. Enter `5` to create and add the three example characters.
3. Enter `1` to start the game.
4. On a character's turn, enter `1` for basic actions, then `1` to attack, and select a living monster by its displayed number. Enter `2` on the action menu to view and use that character's skills.

To create your own character, enter `2` at the main menu, choose a name and class, confirm the class, and distribute the five stat points. Use option `3` to add the character to your party. Starting a game requires exactly three party members.

## Run the precompiled Windows program

The included `RPGGame.exe` is built for 64-bit Windows. Download it and double-click it, or run it from Command Prompt or PowerShell. No compiler is needed to run the executable.

Executables are operating-system and architecture specific. This Windows executable does not run natively on Linux or macOS; compile the source code on those systems using the instructions below.

## Compile from source

The source files require a C++17 compiler. The `.exe` is optional if compiling from source.

### Windows

Install MSYS2 and its UCRT64 compiler:

```powershell
winget install --id MSYS2.MSYS2
```

Open the **MSYS2 UCRT64** terminal and install the compiler:

```sh
pacman -Syu
pacman -S mingw-w64-ucrt-x86_64-gcc
```

From the project directory, compile and run:

```sh
g++ -std=c++17 *.cpp -o RPGGame.exe
./RPGGame.exe
```

### Linux

On Ubuntu/Debian, install g++:

```sh
sudo apt update
sudo apt install g++
```

Then, from the project directory:

```sh
g++ -std=c++17 *.cpp -o RPGGame
./RPGGame
```

### macOS

Install Apple's command-line developer tools:

```sh
xcode-select --install
```

Then, from the project directory:

```sh
clang++ -std=c++17 *.cpp -o RPGGame
./RPGGame
```
