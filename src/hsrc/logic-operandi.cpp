#include "../header/logic-operandi.h"

std::ostream& operator<<(std::ostream& stream,const STATE& other){
    stream << ((other== STATE::HIGH) ? 1 : 0);
    return stream;
}

STATE LogicalAND(STATE a, STATE b){
    return (a == STATE::FLOATING || b == STATE::FLOATING) ? STATE::FLOATING : (a == STATE::HIGH && b == STATE::HIGH) ? STATE::HIGH : STATE::LOW;
}
STATE LogicalNOT(STATE a){
    return (a == STATE::FLOATING) ? STATE::FLOATING : (a == STATE::HIGH) ? STATE::LOW : STATE::HIGH;
}

logicOperandi::logicOperandi(std::string UID,LOGIC_TYPE TYPE,STATE OUTPUT){
    uid = UID;
    type = TYPE;
    output = OUTPUT;
    evaluation_cycle = 0;
    name = "";
}
logicOperandi::logicOperandi(std::string UID,LOGIC_TYPE TYPE){
    uid = UID;
    type = TYPE;
    output = STATE::LOW;
    evaluation_cycle = 0;
    name = "";
}
logicOperandi::logicOperandi(){
    evaluation_cycle = 0;
    name = "";
}
logicOperandi::logicOperandi(const logicOperandi& other){
    uid = other.uid;
    type = other.type;
    name = other.name;

    evaluation_cycle = 0;
}

logicOperandi::~logicOperandi(){
    inputs = std::vector<logicOperandi*>();
}
/*
logicOperandi::logicOperandi(logicOperandi&& other){
    std::cout << "Moved: " << other.uid << "\n";
    name = other.name;
    uid = other.uid;
    type = other.type;
    output = other.output;
    evaluation_cycle = other.evaluation_cycle;

    inputs = other.inputs;

    other.inputs = std::vector<logicOperandi*>();
}

logicOperandi logicOperandi::operator=(const logicOperandi& other){
    name = other.name;
    uid = other.uid;
    type = other.type;
    output = other.output;
    evaluation_cycle = other.evaluation_cycle;
    return *this;
}*/