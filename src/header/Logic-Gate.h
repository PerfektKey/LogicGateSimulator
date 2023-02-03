#ifndef __LOGIC_GATE__
#define __LOGIC_GATE__
#include "logic-operandi.h"

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;
#include <json/json.h>
#include <json/value.h>

class LogicGate{
private:
    std::map<std::string,logicOperandi> logics;
    std::map<std::string,std::map<std::string,logicOperandi>> pins;

    std::string name;
public:
    LogicGate();//constructor
    //LogicGate(const LogicGate&);//copy constructor
    //LogicGate(LogicGate&&);//move constructor

    void clear();

    void evaluate(logicOperandi&,unsigned int);
    void simulate(unsigned int);

    void printState();
    void printInfo();

    void addLogicOP(LOGIC_TYPE);
    void addLogicOP(LOGIC_TYPE,std::string);
    void addLogicOP(logicOperandi);

    logicOperandi* getLOP(std::string);

    std::map<std::string,logicOperandi>& getLogicOP();

    void addOutputPin(std::string);
    void addOutputPin(std::string,std::string);
    std::map<std::string,logicOperandi>& getOutputs();
    void addInputPin(std::string);
    void addInputPin(std::string,std::string);
    std::map<std::string,logicOperandi>& getInputs();

    void connectIO(std::string,std::string);
    void connectIO(logicOperandi&,logicOperandi&);

    void setName(std::string);
    std::string getName();
};

void JsonToGate(fs::path,LogicGate&);

#endif