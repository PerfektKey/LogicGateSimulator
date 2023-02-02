#ifndef __LOGIC_GATE__
#define __LOGIC_GATE__
#include "logic-operandi.h"

#include <map>
#include <vector>
#include <string>

class LogicGate{
private:
    std::map<std::string,logicOperandi> logics;
    std::map<std::string,std::map<std::string,logicOperandi>> pins;

    std::string name;
public:
    LogicGate();//constructor
    LogicGate(const LogicGate&);//copy constructor

    void evaluate(logicOperandi&,unsigned int);
    void simulate(unsigned int);
    void printState();

    void addLogicOP(LOGIC_TYPE);
    void addLogicOP(LOGIC_TYPE,std::string);
    void addLogicOP(logicOperandi);

    std::map<std::string,logicOperandi>& getLogicOP();

    void addOutputPin(std::string);
    std::map<std::string,logicOperandi>& getOutputs();
    void addInputPin(std::string);
    std::map<std::string,logicOperandi>& getInputs();

    void connectIO(std::string,std::string);
    void connectIO(logicOperandi&,logicOperandi&);
};

#endif