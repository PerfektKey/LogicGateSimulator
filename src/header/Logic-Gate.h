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

    std::map<std::string,logicOperandi*> outsideInputs;

    std::string name;
public:
    LogicGate();//constructor
    ~LogicGate();
    //LogicGate(const LogicGate&);//copy constructor
    //LogicGate(LogicGate&&);//move constructor

    void clear();

    void evaluate(logicOperandi&,unsigned int);
    void simulate(unsigned int);
    void simulateR(unsigned int);
    void simulateOWN(unsigned int&,bool);
    void simulateOWNR(unsigned int&,bool);

    void printState(bool=true);
    void printStateR(bool=true);
    void printInfo();

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

    std::map<std::string,logicOperandi*>& getOInputs();

    void connectIO(std::string,std::string);
    void connectIO(logicOperandi&,logicOperandi&);

    void connectGatesIO(LogicGate&,logicOperandi&,logicOperandi&);

    void setName(std::string);
    std::string getName();
};

void JsonToGate(fs::path,LogicGate&,std::string);

#endif