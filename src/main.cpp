#include <iostream>
#include <SFML/Graphics.hpp>

#include "header/Logic-Gate.h"
#include "header/logic-operandi.h"
#include "header/DrawGate.h"
#include "header/scrollContainer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>


std::vector<LogicGate*> HiddenGates;
std::vector<DrawGate> gates;
std::vector<DrawGate> inputs;
std::vector<DrawGate> outputs;

std::vector<DrawGate*> showDG;
std::vector<LogicGate> showLG;
std::vector<fs::path> showPaths;


fs::path Datapath;
std::string gateName;

fs::path fontFullpath;

/*
crashing if you place a gate and then place anything else
*/

unsigned long long i = 1;

void sim();
int findchar(const std::string&,char,unsigned int);

int main(){
    fs::path p;
    p = fs::current_path();
    p += "\\jsonData\\and.json";

    Datapath = fs::current_path();
    Datapath += "\\jsonData";

    fontFullpath = fs::current_path();
    fontFullpath += "\\ARCADECLASSIC.TTF";


    std::map<std::string,unsigned int> GateId;

    sf::RenderWindow window(sf::VideoMode(1000,800),"LIFE ENGINE");
    sf::RectangleShape bg;
    bg.setSize(sf::Vector2f(window.getSize().x,window.getSize().y));
    bg.setFillColor(sf::Color(100,100,100));

    scrollContainer SC(sf::Vector2f(0,window.getSize().y*.9),sf::Vector2f(window.getSize().x,window.getSize().y*.1));
    SC.setColor(sf::Color(50,50,50));

    std::cout << fontFullpath << "\n";
    SC.setFont(fontFullpath);
    for(const auto& dirEntry : fs::recursive_directory_iterator(Datapath)){
        SC.addLabel(dirEntry.path().stem().generic_string());
        GateId[dirEntry.path().stem().generic_string()] = 0;
    }


    logicOperandi* fromLOP = nullptr;
    logicOperandi* toLOP = nullptr;

    while(window.isOpen()){


        sf::Event event;
        while(window.pollEvent(event)){

            if (event.type == sf::Event::Closed)    window.close();
            if (event.type == sf::Event::MouseButtonPressed){
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    logicOperandi* lop = nullptr;
                    LogicGate* pressedLG = nullptr;
                    for (DrawGate& dg : inputs){
                        if (pressedLG != nullptr && lop != nullptr)
                            break;
                        if (lop == nullptr || showDG.size() == 0)
                            lop = dg.pressedPin(sf::Mouse::getPosition(window));
                        if (lop != nullptr)
                            fromLOP = lop;
                        if (pressedLG == nullptr)
                            pressedLG = dg.pressedGate(sf::Vector2f(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y));
                    }
                    for (DrawGate& dg : outputs){
                        if (lop != nullptr || showDG.size() > 0)
                            break;
                        lop = dg.pressedPin(sf::Mouse::getPosition(window));
                        if (lop != nullptr)
                            toLOP = lop;
                    }
                    for (DrawGate& dg : gates){
                        if (lop != nullptr || showDG.size() > 0)
                            break;
                        lop = dg.pressedPin(sf::Mouse::getPosition(window));
                        //continue;
                        //this breaks it
                        if (lop == nullptr)
                            continue;
                        int start = findchar(lop->uid,'-',1) + 1;
                        int lenght = findchar(lop->uid,'-',2) - findchar(lop->uid,'-',1) - 1;
                        if (start == -1 || lenght < 0)
                            continue;
                        std::string tmpStr = lop->uid.substr(start,lenght);
                        if (tmpStr == "input")
                            toLOP = lop;
                        else if (tmpStr == "output")
                            fromLOP = lop;
                        else
                            std::cout << "[error] cant decide if input or output: " << lop->uid << " found: " << tmpStr << "\n";
                    }
                    gateName = SC.PressLabel(sf::Vector2f(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y));
                    if (pressedLG != nullptr){
                        for (auto&[uid,lop] : pressedLG->getOutputs()){
                            lop.output = LogicalNOT(lop.output);
                            //sim();
                            break;
                        }
                        //this throws a logic error IDK why
                        //pressedLG->getOutputs().at(0).output = (LogicalNOT(pressedLG->getOutputs().at(0).output));
                    }
                    std::cout << fromLOP << " -> " << toLOP << std::endl;
                    if (fromLOP != nullptr && toLOP != nullptr){
                        std::cout << "connecting\n";
                        if (std::find(toLOP->inputs.begin(),toLOP->inputs.end(),fromLOP) == toLOP->inputs.end())
                            toLOP->inputs.push_back(fromLOP);
                        for (logicOperandi* lop : toLOP->inputs){
                            std::cout << toLOP->uid << " connects to: " << lop->uid << "\n";
                        }
                        fromLOP = nullptr;
                        toLOP = nullptr;
                    }
                    std::cout << gateName << std::endl;
                    if (lop == nullptr && gateName != ""){
                        LogicGate* tmp = new LogicGate();
                        fs::path tmpFullPath = Datapath;tmpFullPath.concat("\\");tmpFullPath.concat(gateName);tmpFullPath.concat(".json");
                        //std::cout << Datapath << "\n" << gateName << "\n" << tmpFullPath << "\n";
                        JsonToGate(tmpFullPath,*tmp,"-"+std::to_string(GateId[gateName]));
                        std::cout << GateId[gateName] << std::endl;
 
                        if (gateName == "Input"){
                            inputs.push_back(std::move(DrawGate(tmp)));
                            showDG.push_back(&inputs.at(inputs.size()-1));
                        }else
                        if (gateName == "Output"){
                            outputs.push_back(std::move(DrawGate(tmp)));
                            showDG.push_back(&outputs.at(outputs.size()-1));
                        }else{
                            gates.push_back(DrawGate(tmp));
                            showDG.push_back(&gates.at(gates.size()-1));
                        }
                        HiddenGates.push_back(tmp);

                        GateId[gateName] += 1;
                    }
                    if (lop == nullptr && gateName == ""){
                        showDG.clear();
                    }

                }
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){
                    fromLOP = nullptr;
                    toLOP = nullptr;
                }
            }
            if(event.type == sf::Event::MouseWheelScrolled){
                if (event.mouseWheelScroll.delta == 1)
                    SC.scroll(25);
                else if (event.mouseWheelScroll.delta == -1)
                    SC.scroll(-25);
            }
        }

        sim();

        window.clear();
        window.draw(bg);
        SC.draw(window);
        sf::Vector2f positionAdd = sf::Vector2f(0,0);
        for (DrawGate* dg : showDG){
            //std::cout << dg->getGate()->get->uid << std::endl;
            dg->setPosition(sf::Vector2f(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y) + positionAdd);
            //dg->draw(window,true);
            positionAdd += dg->getSize();
            positionAdd.y += 5;
            positionAdd.x = 0;
        }
        for (DrawGate& dg : inputs){
            dg.draw(window);
        }
        for (DrawGate& dg : outputs){
            dg.draw(window);
        }
        for (DrawGate& dg : gates){
            dg.draw(window);
        }
        window.display();


    }

    std::cin.get();

}

void sim(){
    for (DrawGate& DR : outputs){
        //std::cout << "sim\n";
        DR.getGate()->simulateI(i);
    }
    if (outputs.size() == 0)
        for (DrawGate& DR : gates){
            //std::cout << "sim\n";
            DR.getGate()->simulateI(i);
        }
    i++;
}

int findchar(const std::string& string,char toFind,unsigned int occ){
    unsigned int found = 0;
    for (int i = 0;i < string.size();i++){
        if (string.at(i) != toFind)
            continue;
        found++;
        if (found == occ)
            return i;
    }
    return -1;
}