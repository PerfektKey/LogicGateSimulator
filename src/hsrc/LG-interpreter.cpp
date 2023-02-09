#include "../header/LG-Interpreter.h"


LGInterpreter::pin::~pin(){
    return;
}
LGInterpreter::pin::pin(){
    return;
}
LGInterpreter::pin::pin(const pin& other){
    body = other.body;
    name = other.name;
    connectedLOP = other.connectedLOP;
    //other.connectedLOP = nullptr;
}
/*LGInterpreter::pin& LGInterpreter::pin::operator=(pin&& other){
    if (this != &other){
        connectedLOP = other.connectedLOP;
        body = std::move(other.body);
        name = other.name;

        other.connectedLOP = nullptr;
        other.body = sf::CircleShape();
        other.name = std::string();
    }
    return *this;
}*/

LGInterpreter::LGInterpreter(fs::path path,sf::Vector2f pos,unsigned int ind){
    std::cout << path << "\n";
    Gpath = path;
    Gindex = ind;
    JsonToGate(path, *gate, "-"+std::to_string(ind));

    position = pos;

    setBody();
}
LGInterpreter::LGInterpreter(const LGInterpreter& other){
    position = other.position;

    
    Gpath = other.Gpath;
    Gindex = other.Gindex;

    std::cout << Gpath << "\n";

    JsonToGate(Gpath, *gate, "-"+std::to_string(Gindex));

    setBody();
}
LGInterpreter::LGInterpreter(LGInterpreter&& other){
    position = other.position;
    size = other.size;
    color = other.color;
    Gpath = other.Gpath;
    Gindex = other.Gindex;
    name = other.name;
    body = std::move(other.body);
    gate = std::move(other.gate);
    //pins = std::move(other.pins);
    pins = std::vector<pin>();


    other.position = sf::Vector2f();
    other.size = sf::Vector2f();
    other.color = sf::Color();
    other.Gpath = std::string();
    other.Gindex = int();
    other.name = std::string();
    other.body = sf::RectangleShape();
    other.gate = nullptr;
    other.pins = std::vector<pin>();

    setBody();
}


int LGInterpreter::max(int a, int b){
    return (a > b) ? a : b;
}

void LGInterpreter::setBody(){
    int m = max(gate->getInputs().size(),gate->getOutputs().size());
    int size_t = 2.5*pinSize + m*1.5*pinSize;
    this->size = sf::Vector2f(size_t,size_t);

    color = gate->getColor();

    setPins();
    updateBody();
}

void LGInterpreter::updateBody(){
    body.setFillColor(color);
    body.setPosition(position);
    body.setSize(size);
}

void LGInterpreter::setPins(){
    unsigned int i = 0;
    //float hypo = sqrt(2*(pinSize*pinSize));
    for (auto[uid,lop] : gate->getInputs()){
        pin p;
        p.connectedLOP = &lop;
        p.name = lop.name;
        p.body = sf::CircleShape();

        p.body.setFillColor(sf::Color::Black);
        p.body.setOrigin(sf::Vector2f(pinSize,pinSize));
        p.body.setPosition((position.x-pinSize),(position.y+1.5*pinSize)+i*2.5*pinSize);
        p.body.setRadius(pinSize);
        p.body.setOutlineThickness(pinSize/15);
        p.body.setOutlineColor(sf::Color::White);

        pins.push_back(p);
        i++;
    }
    i = 1;
    for (auto[uid,lop] : gate->getOutputs()){
        pin p;
        p.connectedLOP = &lop;
        p.name = lop.name;
        p.body = sf::CircleShape();

        p.body.setFillColor(sf::Color::Black);
        p.body.setOrigin(sf::Vector2f(pinSize,pinSize));
        if (gate->getOutputs().size() > gate->getInputs().size())
            p.body.setPosition((position.x+size.x+pinSize),(position.y+1.5*pinSize)+i*2.5*pinSize);
        else
            p.body.setPosition((position.x+size.x+pinSize),(position.y) + i*(size.y-(size.y/(gate->getOutputs().size()+1))));
        p.body.setRadius(pinSize);
        p.body.setOutlineThickness(pinSize/15);
        p.body.setOutlineColor(sf::Color::White);

        pins.push_back(p);
        i++;
    }
}

void LGInterpreter::draw(sf::RenderWindow& window){
    window.draw(body);
    for (pin& p : pins){

        if (p.connectedLOP->output == STATE::HIGH)
            p.body.setFillColor(sf::Color::Green);
        if (p.connectedLOP->output == STATE::LOW)
            p.body.setFillColor(sf::Color::Red);
        
        window.draw(p.body);
    }
}

LGInterpreter* LGInterpreter::pressed(sf::Vector2i pos){
    bool in = position.x < pos.x && position.x+size.x > pos.x && position.y < pos.y && position.y+size.y > pos.y;
    if (in){
        gate->setOutput(LogicalNOT(gate->getOutput()));
        return this;
    }
    return nullptr;
}
logicOperandi* LGInterpreter::pressedPin(sf::Vector2i pos){
    for (pin& p : pins){
        float dis = sqrt(pow(p.body.getPosition().x-pos.x,2)+pow(p.body.getPosition().y-pos.y,2));
        //std::cout << name << " dist from mouse: " << dis << "\n";
        if (dis < 2*pinSize){
            std::cout <<  p.connectedLOP->uid << "\n";
            return p.connectedLOP;
        }
    }
    return nullptr;
}

LogicGate* LGInterpreter::getGate(){
    return gate;
}