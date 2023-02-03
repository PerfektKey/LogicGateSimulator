#include <iostream>
#include <SFML/Graphics.hpp>

#include "header/Logic-Gate.h"
#include "header/logic-operandi.h"

#include "hsrc/test.cpp"


int main(){
    /*std::string path;
    std::cout << "give path to gate(json) or type ':q' to quit:\n";
    std::cin >> path;
    std::cout << path << "\n";
    LogicGate gate;
    std::cout << "IIIII\n";
    JsonToGate(path,gate);
    std::cout << "DDDDD\n";
    unsigned int d;
    gate.simulateOWN(d,true);
    std::cout << "EEEEE\n";
    return 1;*/
    std::string pathA = "C:\\Users\\Marco\\Documents\\code\\LogicGateSimulator\\jsonData\\and.json";
    std::string pathB = "C:\\Users\\Marco\\Documents\\code\\LogicGateSimulator\\jsonData\\not.json";

    LogicGate a;
    JsonToGate(pathA,a,"-0");
    LogicGate b;
    JsonToGate(pathB,b,"-1");

    LogicGate n1;
    JsonToGate(pathB,n1,"-2");
    LogicGate n2;
    JsonToGate(pathB, n2,"-3");

    a.connectGatesIO(n1,*a.getLOP("0-and-input-0-0"),*n1.getLOP("0-not-output-0-2"));
    a.connectGatesIO(n2,*a.getLOP("0-and-input-1-0"),*n2.getLOP("0-not-output-0-3"));

    b.connectGatesIO(a,*b.getLOP("0-not-input-0-1"),*a.getLOP("0-and-output-0-0"));
    //a.printInfo();
    //b.printInfo();
    unsigned int i = 0;
    std::cout << "Simulate from not gate Recursivly  \n";
    i = 0;
    b.simulateOWNR(i,true);

    system("pause");

    return 1;
}