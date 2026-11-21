#pragma once
#include <iostream>
#include <string>
using namespace std;

// funkcja do czyszczenia ekranu ------------------------------------------------------------------------------------------
inline void clear() {
    cout << string(50, '\n');
}

// funkcja do czyszczenia wejścia cin, aby uniknąć problemów ------------------------------------------------------------------------------------------
inline void clearInput() {
    cin.clear();
    cin.ignore(10000, '\n');
}

// funkcja do zatrzymania programu i oczekiwania na naciśnięcie Enter ------------------------------------------------------------------------------------------
inline void pause() {
    cout << "\nPress Enter to continue...";
    cin.get();
}