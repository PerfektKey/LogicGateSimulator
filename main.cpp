#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <json/value.h>
#include <json/json.h>


//#define INSRC 1
#ifdef INSRC
#include "header/Logic-Gate.h"
#include "header/logic-operandi.h"
#include "header/DrawGate.h"
#include "header/scrollContainer.h"
#else
#include "src/header/Logic-Gate.h"
#include "src/header/logic-operandi.h"
#include "src/header/DrawGate.h"
#include "src/header/scrollContainer.h"
#endif
#include <iostream>
#include <fstream>
#include <string>
#include <map>

class sfLine : public sf::Drawable//https://github.com/SFML/SFML/wiki/Source:-Line-segment-with-thickness
{
public:
    sfLine(){}
    sfLine(const sf::Vector2f& point1, const sf::Vector2f& point2,float __thickness){
        thickness = __thickness;
        sf::Vector2f direction = point2 - point1;
        sf::Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

        sf::Vector2f offset = (thickness/2.f)*unitPerpendicular;

        vertices[0].position = point1 + offset;
        vertices[1].position = point2 + offset;
        vertices[2].position = point2 - offset;
        vertices[3].position = point1 - offset;
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(vertices,4,sf::Quads);
    }

    void setFillColor(sf::Color __color){
        color = __color;
        for (int i=0; i<4; ++i)
            vertices[i].color = color;
    }
    void setThickness(float __thickness){
        thickness = __thickness;
    }
    void setPosition(const sf::Vector2f& point1, const sf::Vector2f& point2){
        sf::Vector2f direction = point2 - point1;
        sf::Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

        sf::Vector2f offset = (thickness/2.f)*unitPerpendicular;

        vertices[0].position = point1 + offset;
        vertices[1].position = point2 + offset;
        vertices[2].position = point2 - offset;
        vertices[3].position = point1 - offset;
    }

private:
    sf::Vertex vertices[4];
    float thickness;
    sf::Color color;
};


struct cable{//struct for the cable that connects gates
    sfLine line = sfLine();
    logicOperandi* from;
    logicOperandi* to;
public:
    cable(sf::Vector2f fromPos, sf::Vector2f toPos, unsigned int thickness=5){
        line = sfLine(fromPos,toPos,thickness);
    }
};


std::vector<cable> cables;

std::vector<LogicGate> HiddenGates;
std::vector<DrawGate*> gates;
std::vector<DrawGate*> inputs;
std::vector<DrawGate*> outputs;

//std::vector<DrawGate*> ;
std::vector<DrawGate*> moveDG;


fs::path Datapath;

fs::path fontFullpath;

void GatesToJson(fs::path,std::string,int[3]);
void getALL(std::vector<logicOperandi*>&,logicOperandi*);

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
    SC.setScroll();

    std::cout << fontFullpath << "\n";
    SC.setFont(fontFullpath);
    for(const auto& dirEntry : fs::recursive_directory_iterator(Datapath)){
        //std::cout << dirEntry << std::endl;
        SC.addLabel(dirEntry.path().stem().generic_string());
        GateId[dirEntry.path().stem().generic_string()] = 0;
    }


    logicOperandi* fromLOP = nullptr;
    sf::Vector2f fromPos;
    logicOperandi* toLOP = nullptr;
    sf::Vector2f toPos;

    while(window.isOpen()){


        sf::Event event;
        while( window.pollEvent( event ) ){

            if (event.type == sf::Event::Closed)    window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)){
                int i[3]{125,125,125};
                GatesToJson(Datapath,"test XOR", i );
                std::cout << "key N pressed\n";
            }
            if (event.type == sf::Event::MouseButtonPressed){
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    DrawGate* pressedDRP = nullptr;
                    //DrawGate* pressedDRInput = nullptr;
                    bool pressedDR = false;
                    bool pressedDRInput = false;
                    bool pressedFromPin = fromLOP != nullptr;
                    bool pressedToPin = toLOP != nullptr;
                    sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                    
                    std::string GateName = SC.PressLabel(mouse_pos);
                    if (GateName != ""){

                        LogicGate* tmp = new LogicGate();
                        HiddenGates.push_back(LogicGate());
                        fs::path tmpFullPath = Datapath;tmpFullPath.concat("\\");tmpFullPath.concat(GateName);tmpFullPath.concat(".json");
                        JsonToGate(tmpFullPath,*tmp,"-"+std::to_string(GateId[GateName]));

                        DrawGate* ndrg = new DrawGate(tmp);
    
                        if (GateName == "Input"){
                            inputs.push_back(ndrg);
                            moveDG.push_back(ndrg);
                        }else
                        if (GateName == "Output"){
                            outputs.push_back(ndrg);
                            moveDG.push_back(ndrg);                    
                            }else{
                            gates.push_back(ndrg);
                            moveDG.push_back(ndrg);
                        }
                        GateId[GateName]++;
                        goto endLeftClick;
                    }
                    if (moveDG.size() > 0)
                        goto clearMoveDR;
                    
                    for (DrawGate* dr : inputs){
                        if (!pressedFromPin){
                            DrawGate::pin* tmp = dr->pressedPin(mouse_pos);
                            if ( tmp != nullptr ){
                                pressedFromPin = true;
                                fromLOP = tmp->connectedLOP;
                                fromPos = tmp->body.getPosition();
                            }
                        }
                        if (!pressedDRInput){
                            DrawGate* tmp = dr->pressedDrawGate(mouse_pos);
                            if (tmp != nullptr){
                                pressedDRP = tmp;
                                pressedDRInput = true;
                                pressedDR = true;
                            }
                        }
                    }
                    for (DrawGate* dr : outputs){
                        if (!pressedToPin){
                            DrawGate::pin* tmp = dr->pressedPin(mouse_pos);
                            if ( tmp != nullptr ){
                                pressedToPin = true;
                                toLOP = tmp->connectedLOP;
                                toPos = tmp->body.getPosition();
                            }
                        }
                        if (!pressedDR){
                            DrawGate* tmp = dr->pressedDrawGate(mouse_pos);
                            if (tmp != nullptr){
                                pressedDRP = tmp;
                                pressedDR = true;
                            }
                        }
                    }


                    for (DrawGate* dr : gates){
                        if (!pressedFromPin || !pressedToPin){
                            DrawGate::pin* tmp = dr->pressedPin(mouse_pos);
                            if (tmp != nullptr){
                                logicOperandi* lop = tmp->connectedLOP;
                                int start = findchar(lop->uid,'-',1) + 1;
                                int lenght = findchar(lop->uid,'-',2) - findchar(lop->uid,'-',1) - 1;
                                if (start == 0 || lenght < 0)
                                    continue;
                                std::string tmpStr = lop->uid.substr(start,lenght);
                                if (tmpStr == "input" && !pressedToPin){
                                    toLOP = lop;
                                    toPos = tmp->body.getPosition();
                                    pressedToPin = true;
                                }else if (tmpStr == "output" && !pressedFromPin){
                                    fromLOP = lop;
                                    fromPos = tmp->body.getPosition();
                                    pressedFromPin = true;
                                }else
                                    std::cout << "[error] cant decide if input or output: " << lop->uid << " found: " << tmpStr << "\n";
                            }
                        }
                        if (!pressedDR){
                            DrawGate* tmp = dr->pressedDrawGate(mouse_pos);
                            if (tmp != nullptr){
                                pressedDRP = tmp;
                                pressedDR = true;
                            }
                        }
                    }


                    if (pressedDR){
                        if (pressedDRInput){
                            for (auto&[uid,lop] : pressedDRP->getGate()->getOutputs()){
                                lop.output = LogicalNOT(lop.output);
                                sim();
                                //break;
                            }
                        }
                    }
                    std::cout << pressedDR << pressedDRInput << pressedFromPin << pressedToPin << std::endl;
                    if (pressedFromPin && pressedToPin){
                        if (std::find(toLOP->inputs.begin(),toLOP->inputs.end(),fromLOP) == toLOP->inputs.end()){
                            std::cout << "connecting " << fromLOP->uid << " to " << toLOP->uid << std::endl;
                            toLOP->inputs.push_back(fromLOP);
                            cable cl(fromPos,toPos);
                            cl.from = fromLOP;
                            cl.to = toLOP;
                            cables.push_back(cl);
                        }else
                            std::cout << fromLOP->uid << " is already connected to " << toLOP->uid << std::endl;
                        toLOP = nullptr;fromLOP = nullptr;
                    }
                clearMoveDR:
                    if (pressedDR == false,pressedDRInput == false, pressedFromPin == false, pressedToPin == false){
                        moveDG.clear();
                    }
                }
        endLeftClick:
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

        //sim();

        window.clear();
        window.draw(bg);
        SC.draw(window);
        sf::Vector2f positionAdd = sf::Vector2f(0,0);
        for (DrawGate* dg : moveDG){
            dg->setPosition(sf::Vector2f(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y) + positionAdd);
            positionAdd += dg->getSize();
            positionAdd.y += 5;
            positionAdd.x = 0;
        }
        for (DrawGate* dg : inputs){
            dg->draw(window);
        }
        for (DrawGate* dg : outputs){
            dg->draw(window);
        }
        for (DrawGate* dg : gates){
            dg->draw(window);
        }
        for (cable& cl : cables){
            cl.line.setFillColor(sf::Color::Black);
            if (cl.from->output == STATE::HIGH)
                cl.line.setFillColor(sf::Color::Green);
            else if (cl.from->output == STATE::LOW)
                cl.line.setFillColor(sf::Color::Red);
            window.draw(cl.line);
        }
        window.display();


    }

    std::cin.get();

}

void sim(){
    for (DrawGate* DR : outputs){
        //std::cout << "sim\n";
        DR->getGate()->simulateI(i);
    }
    if (outputs.size() == 0)
        for (DrawGate* DR : gates){
            //std::cout << "sim\n";
            DR->getGate()->simulate(i);
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

std::string getNewId(const std::map<std::string,std::string>& map, std::string id){
    std::string nid = id;
    if (map.find(id) != map.end())
        nid = getNewId(map, map.at(id));
    return nid;
}

void getALL(std::vector<logicOperandi*>& all,logicOperandi* LOP){
    for (logicOperandi* lop : LOP->inputs){
        all.push_back(lop);
        getALL(all,lop);
    }
}

void GatesToJson(fs::path path, std::string name,int color[3]){
    std::cout << "new gate\n";

    Json::Value root;
    Json::Value data;

    root["name"] = name;
    for (int i = 0;i < 3;i++)
        root["color"][i] = color[i];

    std::vector<logicOperandi*> all;

    std::map<std::string,std::string> Newids;
    std::vector<logicOperandi*> placedLOPS;


    //here the code
    unsigned int index = 0;
    for (DrawGate* DG : outputs)
    for (auto&[uid,lop] : DG->getGate()->getInputs()){
        getALL(all,&lop);
        data.clear();
        data["name"] = lop.name;
        std::string id = name + "-output-" + std::to_string(index);
        Newids[lop.uid] = id;
        data["uniqueID"] = id;
        data["type"] = "PIN";
        data["direction"] = "output";
        root["LOP"][index] = data;
        index++;
    }


    std::map<LOGIC_TYPE,int> amm;
    amm[LOGIC_TYPE::AND] = 0;
    amm[LOGIC_TYPE::NOT] = 0;
    amm[LOGIC_TYPE::PIN] = 0;
    amm[LOGIC_TYPE::ANY] = 0;
    for (logicOperandi* lop : all){
        data.clear();
       if (lop->type == LOGIC_TYPE::AND){
            data["uniqueID"] = name+"-and-"+std::to_string(amm.at(LOGIC_TYPE::AND));
            data["type"] = "AND";
            Newids[lop->uid] = name+"-and-"+std::to_string(amm.at(LOGIC_TYPE::AND));
            amm[LOGIC_TYPE::AND]++;
        }
        if (lop->type == LOGIC_TYPE::NOT){
            data["uniqueID"] = name+"-not-"+std::to_string(amm.at(LOGIC_TYPE::NOT));
            data["type"] = "NOT";
            Newids[lop->uid] = name+"-not-"+std::to_string(amm.at(LOGIC_TYPE::NOT));
            amm[LOGIC_TYPE::NOT]++;
        }
        if (lop->type == LOGIC_TYPE::ANY || lop->type == LOGIC_TYPE::PIN){
            if (lop->inputs.size() == 1){
                //can skip this any
                Newids[lop->uid] = lop->inputs.at(0)->uid;
                if (lop->type == LOGIC_TYPE::ANY)
                    continue;
                goto pinTEST;
            }
            data["uniqueID"] = name+"-any-"+std::to_string(amm.at(LOGIC_TYPE::ANY));
            data["type"] = "ANY";
            Newids[lop->uid] = name+"-any-"+std::to_string(amm.at(LOGIC_TYPE::ANY));
            amm[LOGIC_TYPE::ANY]++;
        }
        std::cout << "\t" << lop->uid << " : " << Newids.at(lop->uid) << std::endl;
    pinTEST:
        if (lop->type == LOGIC_TYPE::PIN){
            if (lop->inputs.size() != 0) {
                //Newids[lop->uid] = 
                continue;
            }
            data["name"] = lop->name;
            data["uniqueID"] = name+"-input-"+std::to_string(amm.at(LOGIC_TYPE::PIN));
            data["type"] = "PIN";
            data["direction"] = "input";
            Newids[lop->uid] = name+"-input-"+std::to_string(amm.at(LOGIC_TYPE::PIN));
            amm[LOGIC_TYPE::PIN]++;
        }
        placedLOPS.push_back(lop);
        std::cout << "\t" << lop->uid << " : " << Newids.at(lop->uid) << std::endl;
        root["LOP"][index] = data;
        index++;
    }
    std::cout << "\n";
    for (auto&[key,val] : Newids)
       std::cout << "\t" << key << " : " << val << std::endl;
    std::cout << "\n";
    
    index = 0;
    for (DrawGate* DG : outputs)
    for (auto&[uid,lop] : DG->getGate()->getInputs()){
        //std::cout << uid << std::endl;
        if (lop.inputs.size() == 0)
            continue;
        data.clear();
        data["uniqueID"] = Newids.at(lop.uid);
        unsigned int j = 0;
        for (logicOperandi* lopInp : lop.inputs){
            if ( Newids.find( Newids.at( lopInp->uid ) ) != Newids.end() )
                data["connects"][j] = Newids.at(Newids.at(lopInp->uid));
            else
                data["connects"][j] = Newids.at(lopInp->uid);
            j++;
        }
        root["CON"][index] = data;
        index++;
    }
    for (logicOperandi* lop : placedLOPS){
        if (lop->inputs.size() == 0)
            continue;
        data.clear();
        data["uniqueID"] = Newids.at(lop->uid);
        unsigned int j = 0;
        for (logicOperandi* lopInp : lop->inputs){
            data["connects"][j] = getNewId(Newids,lopInp->uid);
            j++;
        }
        root["CON"][index] = data;
        index++;
    }



    std::string json_file;

    Json::StreamWriterBuilder builder;
    json_file = Json::writeString(builder, root);
    //std::cout << json_file << std::endl;


    std::ofstream file;
    file.open(path.concat("\\"+name+".json"), std::ios::trunc);
    file << json_file;
    //file.write()
    file.close();

}