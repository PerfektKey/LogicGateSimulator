#include "../header/logic-operandi.h"

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
}