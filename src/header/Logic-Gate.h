#ifndef __LOGIC-GATE__
#define __LOGIC-GATE__
#include "logic-operandi.h"

#include <map>
#include <vector>
#include <string>

class LogicGate{
private:
    std::map<std::string,logicOperandi> logics;
    std::map<std::string,std::map<std::string,logicOperandi>> pins;
};

#endif