#include <iostream>
#include <SFML/Graphics.hpp>

#include "header/Logic-Gate.h"
#include "header/logic-operandi.h"

#include "hsrc/test.cpp"


#include <iostream>
#include <fstream>
#include <string>

int main(){

    std::string pathA = "C:\\Users\\Marco\\Documents\\code\\LogicGateSimulator\\jsonData\\nand.json";
    LogicGate nand;
    JsonToGate(pathA,nand,"-0");
    //nand.printInfo();
    unsigned int k = 0;
    nand.simulateOWN(k,true);
    system("pause");

    std::string pathB = "C:\\Users\\Marco\\Documents\\code\\LogicGateSimulator\\jsonData\\or.json";
    LogicGate OR;
    JsonToGate(pathB,OR,"-0");
    //OR.printInfo();
    k = 0;
    OR.simulateOWN(k,true);
    system("pause");

    return 1;
}
int maiSn(){
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

    //a.connectGatesIO(n1,*a.getLOP("0-and-input-0-0"),*n1.getLOP("0-not-output-0-2"));
    //a.connectGatesIO(n2,*a.getLOP("0-and-input-1-0"),*n2.getLOP("0-not-output-0-3"));

    b.connectGatesIO(a,*b.getLOP("0-not-input-0-1"),*a.getLOP("0-and-output-0-0"));
    //a.printInfo();
    //b.printInfo();
    fs::path p = fs::current_path();
    p += "\\jsonData\\nand.json";

    unsigned int j = 0;
    //b.simulateOWNR(j,true);
    system("pause");

    GatesToJson(b,p,"nand");
    system("pause");
    return 1;

    unsigned int i = 0;
    std::cout << "Simulate from not gate Recursivly  \n";
    i = 0;
    b.simulateOWNR(i,true);

    system("pause");

    return 1;
}