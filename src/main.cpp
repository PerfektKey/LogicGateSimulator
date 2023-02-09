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


std::vector<LogicGate> HiddenGates;
std::vector<DrawGate> gates;
std::vector<DrawGate> inputs;
std::vector<DrawGate> outputs;

std::vector<DrawGate> showDG;
std::vector<LogicGate> showLG;
std::vector<fs::path> showPaths;


fs::path fullpath;
fs::path Datapath;
std::string gateName;

fs::path fontFullpath;

/*
connecting gates and simulating
*/

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

    for(const auto& dirEntry : fs::recursive_directory_iterator(Datapath)){
            SC.addLabel(dirEntry.path().stem().generic_string(),fontFullpath);
        }

    while(window.isOpen()){


        sf::Event event;
        while(window.pollEvent(event)){

            if (event.type == sf::Event::Closed)    window.close();
            if (event.type == sf::Event::MouseButtonPressed){
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    logicOperandi* lop = nullptr;
                    for (DrawGate& dg : inputs){
                        if (lop != nullptr)
                            break;
                        lop = dg.pressedPin(sf::Mouse::getPosition(window));
                    }
                    for (DrawGate& dg : outputs){
                        if (lop != nullptr)
                            break;
                        lop = dg.pressedPin(sf::Mouse::getPosition(window));
                    }
                    for (DrawGate& dg : gates){
                        if (lop != nullptr)
                            break;
                        lop = dg.pressedPin(sf::Mouse::getPosition(window)); 
                    }
                    gateName = SC.PressLabel(sf::Vector2f(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y));
                    if (lop != nullptr){
                        std::cout << lop->uid << "\n";
                    }
                    if (lop == nullptr && gateName != ""){
                        LogicGate tmp;
                        fullpath = Datapath.concat("\\"+gateName+".json");
                        JsonToGate(fullpath,tmp,"-");
                        showDG.push_back(DrawGate(&tmp));
                        showLG.push_back(std::move(tmp));
                        showPaths.push_back(fullpath);
                    }
                    if (lop == nullptr && gateName == ""){
                        for (fs::path& p : showPaths){
                            LogicGate tmp;
                            JsonToGate(p,tmp,"-"+std::to_string(GateId[gateName]));
                            if (p.stem().generic_string() == "Input"){
                                inputs.push_back(DrawGate(&tmp));
                                inputs.at(inputs.size()-1).setPosition(sf::Vector2f(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y));
                            }else
                            if (p.stem().generic_string() == "Output"){
                                outputs.push_back(DrawGate(&tmp));
                                outputs.at(outputs.size()-1).setPosition(sf::Vector2f(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y));
                            }else{
                                gates.push_back(DrawGate(&tmp));
                                gates.at(gates.size()-1).setPosition(sf::Vector2f(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y));
                            }
                            
                            HiddenGates.push_back(std::move(tmp));
                            showPaths.clear();
                            showDG.clear();
                            showLG.clear();
                            GateId[gateName]++;
                        }
                    }

                }
            }
        }

        window.clear();
        window.draw(bg);
        SC.draw(window);
        for (DrawGate& dg : showDG){
            dg.setPosition(sf::Vector2f(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y));
            dg.draw(window);
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

void AddLabels(){
    for(const auto& dirEntry : fs::recursive_directory_iterator(Datapath)){
        std::cout << dirEntry.path().stem() << "\n";
        continue;
    }
}