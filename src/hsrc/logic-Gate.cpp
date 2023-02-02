#include "../header/Logic-Gate.h"


LogicGate::LogicGate(){
    pins["input"] = std::map<std::string,logicOperandi>();
    pins["output"] = std::map<std::string,logicOperandi>();
}


void LogicGate::addLogicOP(logicOperandi LOP){
    if (LOP.uid == "")
        return;
    /*if (logics.find(LOP.uid) == logics.end())
        return;*/
    logics[LOP.uid] = LOP;
}
std::map<std::string,logicOperandi>& LogicGate::getLogicOP(){
    return logics;
}

//set Add Input/Output and set get Input/Output
void LogicGate::addInputPin(std::string uid){
    pins["input"][uid] = logicOperandi(uid,LOGIC_TYPE::PIN,STATE::LOW);
}
std::map<std::string,logicOperandi>& LogicGate::getInputs(){
    return pins["input"];
}

void LogicGate::addOutputPin(std::string uid){
    pins["output"][uid] = logicOperandi(uid,LOGIC_TYPE::PIN,STATE::LOW);
}
std::map<std::string,logicOperandi>& LogicGate::getOutputs(){
    return pins["output"];
}


void LogicGate::connectIO(std::string to,std::string from){//conect by searching for the LOP in the maps

}
void LogicGate::connectIO(logicOperandi& to,logicOperandi& from){//conect by reference from two LOP's
    to.inputs.push_back(&from);
}

void LogicGate::evaluate(logicOperandi& LOP, unsigned int current_cycle){
    if (LOP.evaluation_cycle == current_cycle)
        return;
    LOP.evaluation_cycle = current_cycle;
    switch (LOP.type)
    {
    case LOGIC_TYPE::AND://if both inputs are HIGH output HIGH
        if (LOP.inputs.size() != 2)
            break;
        evaluate(*LOP.inputs.at(0), current_cycle);
        evaluate(*LOP.inputs.at(1), current_cycle);
        LOP.output = LogicalAND(LOP.inputs.at(0)->output, LOP.inputs.at(1)->output);
        break;
    case LOGIC_TYPE::NOT://if input is HIGH output LOW (REVERSE the input)
        if (LOP.inputs.size() != 1)
            break;
        evaluate(*LOP.inputs.at(0), current_cycle);
        LOP.output = LogicalNOT(LOP.inputs.at(0)->output);
        break;
    case LOGIC_TYPE::PIN://if any input is HIGH the LOP's output is HIGH
        if (LOP.inputs.size() == 0)
            break;
        for (const logicOperandi* lop : LOP.inputs){
            LOP.output = STATE::LOW;
            if (lop->output == STATE::HIGH){
                LOP.output = STATE::HIGH;
                break;
            }
        }
        break;
    case LOGIC_TYPE::CTR://still not decided maybe just no operation
        break;
        
    default:
        break;
    }
}

void LogicGate::simulate(unsigned int current_cycle){
    for (auto&[uid,LOP] : logics){
        evaluate(LOP, current_cycle);
    }
    for (auto&[uid, LOP] : pins["output"]){
        evaluate(LOP, current_cycle);
    }
}
void LogicGate::printState(){
    std::cout << "\n------------------------------------------------------\n";
    for (auto&[uid, LOP] : pins["input"]){
        std::cout << LOP.output << " ";
    }
    std::cout << " | ";
    for (auto&[uid, LOP] : pins["output"]){
        std::cout << LOP.output << " ";
    }
    std::cout << "\n------------------------------------------------------\n";
}


LogicGate JsonToGate(fs::path p){
    std::ifstream file(p);

    Json::Reader reader;
    Json::Value value;

    reader.parse(file,value);
}