#include "../header/Logic-Gate.h"


LogicGate::LogicGate(){
    pins["input"] = std::map<std::string,logicOperandi>();
    pins["output"] = std::map<std::string,logicOperandi>();
    name = "";
}

/*
LogicGate::LogicGate(LogicGate&& other){
    logics = other.logics;
    pins = other.pins;


}*/


void LogicGate::addLogicOP(logicOperandi LOP){
    if (LOP.uid == "")
        return;
    /*if (logics.find(LOP.uid) == logics.end())
        return;*/
    logics[LOP.uid] = LOP;
}
void LogicGate::addLogicOP(LOGIC_TYPE type,std::string uid){
    logics[uid] = logicOperandi(uid,type,STATE::LOW);
}
std::map<std::string,logicOperandi>& LogicGate::getLogicOP(){
    return logics;
}

//set Add Input/Output and set get Input/Output
void LogicGate::addInputPin(std::string uid){
    pins["input"][uid] = logicOperandi(uid,LOGIC_TYPE::PIN,STATE::LOW);
}
void LogicGate::addInputPin(std::string uid,std::string name){
    logicOperandi lop(uid,LOGIC_TYPE::PIN,STATE::LOW);
    lop.name = name;
    pins["input"][uid] = lop;
}
std::map<std::string,logicOperandi>& LogicGate::getInputs(){
    return pins["input"];
}

void LogicGate::addOutputPin(std::string uid){
    pins["output"][uid] = logicOperandi(uid,LOGIC_TYPE::PIN,STATE::LOW);
}
void LogicGate::addOutputPin(std::string uid,std::string name){
    logicOperandi lop(uid,LOGIC_TYPE::PIN,STATE::LOW);
    lop.name = name;
    pins["output"][uid] = lop;
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

logicOperandi* LogicGate::getLOP(std::string uid){
    std::map<std::string,logicOperandi>::iterator search;

    search = logics.find(uid);
    if (search != logics.end())
        return &logics[uid];
    search = pins["input"].find(uid);
    if (search != pins["input"].end())
        return &pins["input"][uid];
    search = pins["output"].find(uid);
    if (search != pins["output"].end())
        return &pins["output"][uid];
    return nullptr;
}

void LogicGate::printState(){
    std::cout << "\n=########################################=\n" << name;
    for (auto&[uid, LOP] : pins["input"]){
        std::cout << LOP.output << " ";
    }
    std::cout << " | ";
    for (auto&[uid, LOP] : pins["output"]){
        std::cout << LOP.output << " ";
    }
    std::cout << "\n=########################################=\n";
}
void LogicGate::printInfo(){
    std::cout << "\n=########################################=\ngate name: " << name << "\n";
    auto CONlamda = [](logicOperandi* lop){for (logicOperandi* inp : lop->inputs){std::cout << inp->uid << ", ";}std::cout << "\n";};
    for (auto&[uid, LOP] : logics){
        std::cout << LOP.uid << " <- "; CONlamda(&LOP);
    }
    for (auto&[uid, LOP] : pins["input"]){
        std::cout << LOP.uid << "=" << LOP.name << " <- "; CONlamda(&LOP);
    }
    for (auto&[uid, LOP] : pins["output"]){
        std::cout << LOP.uid << "=" << LOP.name << " <- "; CONlamda(&LOP);
    }
    std::cout << "\n=########################################=\n";
}

void LogicGate::setName(std::string __name){
    name = __name;
}

void LogicGate::clear(){
    name = "";
    logics = std::map<std::string,logicOperandi>();
    pins = std::map<std::string,std::map<std::string,logicOperandi>>();
}

void JsonToGate(fs::path p, LogicGate& gate){
    std::ifstream file(p);

    Json::Reader reader;
    Json::Value value;

    reader.parse(file,value);

    gate.clear();

    gate.setName(value["name"].asString());

    Json::Value LOP = value["LOP"];
    Json::Value CON = value["CON"];

    for (unsigned int i = 0;i < LOP.size();i++){
        std::string type = LOP[i]["type"].asString();
        std::string name = LOP[i]["name"].asString();
        std::string uid = LOP[i]["uniqueID"].asString();



        std::cout << uid << "\n";

        if (type == "PIN"){
            if (LOP[i]["direction"].asString() == "input")
                gate.addInputPin(uid,name);
            if (LOP[i]["direction"].asString() == "output")
                gate.addOutputPin(uid,name);
        }
        if (type == "AND")
            gate.addLogicOP(LOGIC_TYPE::AND, uid);
        if (type == "NOT")
            gate.addLogicOP(LOGIC_TYPE::NOT, uid);
        if (type == "ANY")
            gate.addLogicOP(LOGIC_TYPE::ANY, uid);
    }
    //return;
    for (unsigned int i = 0;i < CON.size();i++){
        logicOperandi* to = gate.getLOP(CON[i]["uniqueID"].asString());
        if (to == nullptr)
            return;
        for (unsigned int j = 0;j < CON[i]["connects"].size();j++){
            logicOperandi* from = gate.getLOP(CON[i]["connects"][j].asString());
            if (from == nullptr)
                return;
            to->inputs.push_back(from);

        }
    }

}