#pragma once
#include <iostream>
#include <string>
using namespace std;

inline void clear() {
    cout << string(50, '\n');
}

inline void clearInput() {
    cin.clear();
    cin.ignore(10000, '\n');
}

inline void pause() {
    cout << "Press Enter to continue...";
    cin.ignore(10000, '\n');
    cin.get();
}