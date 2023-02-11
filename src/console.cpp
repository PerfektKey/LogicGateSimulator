#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <map>

#include "header/Logic-Gate.h"//src\header\Logic-Gate.h
#include "header/logic-operandi.h"



namespace fs = std::filesystem;
using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

int main(){//not working 

    std::map<std::string,unsigned int> GateCount;
    
    std::vector<LogicGate> gates;
    std::vector<LogicGate> inputs;
    std::vector<LogicGate> outputs;
    unsigned int ecycle = 0;
    std::string command;
    fs::path chipPath = fs::current_path().concat("\\jsonData\\");


    do{
        GameLoop:
            std::cout << "what command to use?(type 'help' for help)\n";
            std::cin >> command;

            if (command == "help"){
                std::cout << "quit to quit the application\n";
                std::cout << "help => this fenster\n";
                std::cout << "ag => prints all saved gates\n";
                std::cout << "sg => saves the current chip with a name\n";
                std::cout << "ng => adds new gate\n";
                std::cout << "ni => adds new input\n";
                std::cout << "no => adds new output\n";
                std::cout << "pg  => prints all gates and their connections\n";
                std::cout << "pi  => prints all inputs\n";
                std::cout << "po  => prints all outputs\n";
                std::cout << "si => sets a input to a certain state(High or Low)\n";
                //std::cout << "pgp  => prints all gates and their pins\n";
                std::cout << "conn  => connects outputs to inputs from gates\n";
            }else
            if (command == "ag"){
                for(const auto& dirEntry : recursive_directory_iterator(chipPath)){
                    std::cout << dirEntry << "\n";
                }
            }else
            if (command == "ng"){
                std::cout << "what is the chips name: ";
                std::string name;
                std::cin >> name;
                if (name == ""){
                    std::cout << "you need to give the name of a gate\n";
                }
                fs::path tmp = chipPath;
                tmp += name + ".json";

                if (!fs::exists(tmp)){
                    std::cout << "file " << tmp << "does not exit\n";
                    goto GameLoop;
                }
                LogicGate gate;
                JsonToGate(tmp,gate, "-"+std::to_string(GateCount[name]));
                gates.push_back(std::move(gate));
            }else
            if (command == "ni"){
                fs::path tmp = chipPath;
                tmp += "InputCTR.json";
                LogicGate gate;
                JsonToGate(tmp,gate,"-"+std::to_string(GateCount["input"]));
                inputs.push_back(std::move(gate));
            }else
            if (command == "no"){
                fs::path tmp = chipPath;
                tmp += "OutputCTR.json";
                LogicGate gate;
                JsonToGate(tmp,gate,"-"+std::to_string(GateCount["output"]));
                outputs.push_back(std::move(gate));
            }else
            if(command == "pg"){
                for (LogicGate& g : gates)
                    g.printInfo();
            }else
            if(command == "pi"){
                for (LogicGate& g : inputs)
                    g.printInfo();
            }else 
            if(command == "po"){
                for (LogicGate& g : outputs)
                    g.printInfo();
            }else 
            if (command == "pgp"){
                for (LogicGate& g : gates)
                for (auto&[uid,lop] : g.getInputs()){
                    std::cout << uid << " = " << &lop << "\n";
                }
            }else
            if(command == "conn"){
                std::string from_str;
                std::string to_str;
                LogicGate* from_g;
                logicOperandi* from = nullptr;
                logicOperandi* to = nullptr;
                std::cout << "from(name of pin): ";std::cin >> from_str;std::cout << "\n";
                std::cout << "to(name of pin): ";std::cin >> to_str;std::cout << "\n";

                for (LogicGate& g : gates){
                    if (from == nullptr){
                        from = g.getLOP(from_str);
                        from_g = &g;
                    }
                    if (to == nullptr)
                        to = g.getLOP(to_str);
                    if (from != nullptr && to != nullptr)
                        goto GameLoop;
                }
                for (LogicGate& g : inputs){
                    if (from == nullptr){
                        from = g.getLOP(from_str);
                        from_g = &g;
                    }
                    if (to == nullptr)
                        to = g.getLOP(to_str);
                    if (from != nullptr && to != nullptr)
                        goto GameLoop;
                }
                for (LogicGate& g : outputs){
                    if (from == nullptr){
                        from = g.getLOP(from_str);
                        from_g = &g;
                    }
                    if (to == nullptr)
                        to = g.getLOP(to_str);
                    if (from != nullptr && to != nullptr)
                        goto GameLoop;
                }
                if (from == nullptr || to == nullptr){
                    std::cout << "error \n";
                    goto GameLoop;
                }

                from_g->connectIO(*to, *from);

            }else
            if (command == "si"){
                std::string input_str;
                logicOperandi* input;
                std::cout << "name of input: ";std::cin >> input_str;std::cout << "\n";
                for (LogicGate& g : inputs){
                    if (input == nullptr){
                        input = g.getLOP(input_str);
                    }
                    if (input != nullptr)
                        goto GameLoop;
                }
                if (input == nullptr){
                    std::cout << "input " << input_str << " not found\n";
                    goto GameLoop;
                }
                int state = -1;
                do{
                    std::cout << "set the state(1 or 0): ";std::cout << "\n";
                }while(!(std::cin >> state));
                std::cout << "state set to: " << state << "\n";
                if (state == 0){
                    input->output = STATE::LOW;
                }else if (state == 1){
                    input->output = STATE::HIGH;
                    std::cout << "Win";
                }else{
                    std::cout << "state " << state << " is invalid\n";
                    goto GameLoop;
                }
                for (LogicGate& g : outputs){
                    g.simulate(ecycle);
                }
                for (LogicGate& g : inputs){
                    std::cout << g.getOutput() << " ";
                }
                std::cout << " | ";
                for (LogicGate& g : outputs){
                    std::cout << g.getOutput() << " ";
                }
                std::cout << "\n";
            }

    }while(command != "quit");

    return 1;
}

/*
compiled ready
test with:
    1. not gate
    1. output
    1. input
    
*/