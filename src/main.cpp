#include <iostream>
#include <SFML/Graphics.hpp>

#include "header/Logic-Gate.h"
#include "header/logic-operandi.h"


int main(){
    fs::path p = fs::current_path();
    p += "\\jsonData\\and.json";
    std::cout << p << "\n";
    system("pause");
    LogicGate gate;
    JsonToGate(p,gate);
    gate.printInfo();
    system("pause");

    return 1;
}