#ifndef __LOGIC_OPERANDI__
#define __LOGIC_OPERANDI__

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

enum class STATE{
    LOW = 0,
    HIGH,
    FLOATING
};

enum class LOGIC_TYPE {
    AND = 0,
    NOT,
    PIN,
    ANY,
    CTR
};

std::ostream& operator<<(std::ostream&,const STATE&);
std::ostream& operator<<(std::ostream&,const LOGIC_TYPE&);

struct logicOperandi{
    std::string name;
    std::string uid;
    LOGIC_TYPE type;

    STATE output;
    std::vector<logicOperandi*> inputs;

    unsigned int evaluation_cycle;
public:
    ~logicOperandi();
    logicOperandi(std::string,LOGIC_TYPE,STATE);
    logicOperandi(std::string,LOGIC_TYPE);
    logicOperandi();
    logicOperandi(const logicOperandi&);


    //logicOperandi(logicOperandi&&);
    //logicOperandi operator=(const logicOperandi&);
};

STATE LogicalAND(STATE,STATE);
STATE LogicalNOT(STATE);

#endif