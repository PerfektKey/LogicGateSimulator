#ifndef __LOGIC-OPERANDI__
#define __LOGIC-OPERANDI__

#include <iostream>
#include <string>
#include <vector>

enum class STATE{
    LOW = 0,
    HIGH,
    FLOATING
};

enum class LOGIC_TYPE {
    AND = 0,
    NOT,
    PIN,
    CTR
};

struct logicOperandi{
    std::string uid;
    LOGIC_TYPE type;

    STATE output;
    std::vector<logicOperandi*> inputs;

    unsigned int evaluation_cycle;

    logicOperandi(std::string,LOGIC_TYPE,STATE);
    
};

STATE LogicalAND(STATE,STATE);
STATE LogicalNOT(STATE,STATE);

#endif