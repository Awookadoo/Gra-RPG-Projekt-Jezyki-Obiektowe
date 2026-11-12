#include <iostream>
#include "characters.h"
using namespace std;

int main(){
    Character* postac = Character::createCharacter();
    cout << *postac;
    delete postac;
    postac = nullptr;

    return 0;
}