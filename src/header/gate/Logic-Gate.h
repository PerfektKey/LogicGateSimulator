#ifndef __LOGIC_GATE__
#define __LOGIC_GATE__
#include "logic-operandi.h"

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>

#include <SFML/Graphics.hpp>

class LogicGate{
private:
    std::map<std::string,logicOperandi> logics;
    std::map<std::string,std::map<std::string,logicOperandi>> pins;

    std::map<std::string,logicOperandi*> outsideInputs;

    bool recursive = false;

    sf::Color color;
    std::string name;
public:
    LogicGate();//constructor
    ~LogicGate();
    LogicGate(LogicGate&&);//move constructor
    LogicGate& operator=(LogicGate&&);
    //LogicGate(const LogicGate&);//copy constructor

    void clear();

    void evaluate(logicOperandi&,unsigned int);
    void simulate(unsigned int);
    void simulateI(unsigned int);
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

    void setOutput(STATE);
    STATE getOutput();

    std::map<std::string,logicOperandi*>& getOInputs();

    void connectIO(std::string,std::string);
    void connectIO(logicOperandi&,logicOperandi&);

    void connectGatesIO(logicOperandi&,logicOperandi&);

    void setName(std::string);
    std::string getName();

    void setColor(int[3]);
    sf::Color getColor();
};

void JsonToGate(fs::path,LogicGate&,std::string);
//void GatesToJson(LogicGate&,fs::path,std::string);

//std::vector<logicOperandi*> getALL(std::vector<logicOperandi*>);
//void getALL(std::vector<logicOperandi*>&,logicOperandi*);

#endif