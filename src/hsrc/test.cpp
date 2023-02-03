#include <iostream>
#include <SFML/Graphics.hpp>

#include "../header/Logic-Gate.h"
#include "../header/logic-operandi.h"

void nandTest(){
    LogicGate nand;
    unsigned int cycle = 1;

    nand.addInputPin("input-0");
    nand.addInputPin("input-1");

    nand.addOutputPin("output-0");

    nand.addLogicOP(logicOperandi("and-0",LOGIC_TYPE::AND));
    nand.addLogicOP(logicOperandi("not-0",LOGIC_TYPE::NOT));

    for (auto const&[key,val] : nand.getInputs()){
        std::cout << key << "\n";
    }
    for (auto const&[key,val] : nand.getOutputs()){
        std::cout << key << "\n";
    }
    for (auto const&[key,val] : nand.getLogicOP()){
        std::cout << key << "\n";
    }

    std::cout << "\n";

    nand.connectIO(nand.getLogicOP().at("not-0"),nand.getLogicOP().at("and-0"));

    nand.connectIO(nand.getLogicOP().at("and-0"),nand.getInputs().at("input-0"));
    nand.connectIO(nand.getLogicOP().at("and-0"),nand.getInputs().at("input-1"));

    nand.connectIO(nand.getOutputs().at("output-0"),nand.getLogicOP().at("not-0"));

    nand.getInputs().at("input-0").output = STATE::LOW;
    nand.getInputs().at("input-1").output = STATE::LOW;
    nand.simulate(cycle);
    nand.printState();
    cycle++;

    nand.getInputs().at("input-0").output = STATE::LOW;
    nand.getInputs().at("input-1").output = STATE::HIGH;
    nand.simulate(cycle);
    nand.printState();
    cycle++;

    nand.getInputs().at("input-0").output = STATE::HIGH;
    nand.getInputs().at("input-1").output = STATE::LOW;
    nand.simulate(cycle);
    nand.printState();
    cycle++;

    nand.getInputs().at("input-0").output = STATE::HIGH;
    nand.getInputs().at("input-1").output = STATE::HIGH;
    nand.simulate(cycle);
    nand.printState();
    cycle++;

    system("pause");
}


void test2(){
    std::string path;
    do {
        std::cout << "give path to gate(json) or type ':q' to quit:\n";
        std::cin >> path;
        std::cout << path << "\n";
        LogicGate gate;
        JsonToGate(path,gate,"0");
        unsigned int i;
        gate.simulateOWN(i,true);
    }while(path != ":q");
}