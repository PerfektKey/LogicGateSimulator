#include "../header/DrawGate.h"

DrawGate::DrawGate(LogicGate* pg){
    pgate = pg;
    setBody();
    updateBody();
    setPins();
}

DrawGate::DrawGate(DrawGate&& other){
    pgate = other.pgate;
    //body = other.body;
    position = other.position;
    size = other.size;
    color = other.color;
    //labelName = other.labelName;
    //labelFont = other.labelFont;
    //pins = other.pins;

    //other.pins.clear();
    other.pgate = nullptr;

    setBody();
    updateBody();
    setPins();
}


int DrawGate::max(int a, int b){
    return (a > b) ? a : b;
}

void DrawGate::setBody(){

    color = pgate->getColor();
    name = pgate->getName();

    //std::cout << "inputs size: " << pgate->getInputs().size() << "\n";

    int m = max(pgate->getInputs().size(),pgate->getOutputs().size());
    int sizeGate = 2.5*pinSize + m*1.5*pinSize;
    this->size = sf::Vector2f(CharSize*name.size()*1,sizeGate);

    std::string lfp = fs::current_path().generic_string();
    lfp += "\\ARCADECLASSIC.TTF";
    labelFont.loadFromFile(lfp);

    labelName.setFont(labelFont);
    labelName.setString(name);
    labelName.setCharacterSize(CharSize);
    sf::FloatRect textRect = labelName.getLocalBounds();
    labelName.setOrigin(textRect.left + textRect.width/2.0f,
               textRect.top  + textRect.height/2.0f);
    labelName.setPosition(sf::Vector2f(position.x+size.x/2,position.y+size.y/2));
}

void DrawGate::updateBody(){
    body.setFillColor(color);
    body.setSize(size);
    body.setPosition(position);

}
void DrawGate::setPins(){
    pins.clear();
    unsigned int i = 0;
    //std::cout << pgate->getInputs().size() << " : " << pgate->getOutputs().size() << std::endl;
    for (auto&[uid,lop] : pgate->getInputs()){
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
        p.offset = p.body.getPosition() - position;

        pins.push_back(p);
        i++;
    }
    i = 0;
    for (auto&[uid,lop] : pgate->getOutputs()){
        pin p;
        p.connectedLOP = &lop;
        p.name = lop.name;
        p.body = sf::CircleShape();

        p.body.setFillColor(sf::Color::Black);
        p.body.setOrigin(sf::Vector2f(pinSize,pinSize));
        if (pgate->getOutputs().size() > pgate->getInputs().size())
            p.body.setPosition((position.x+size.x+pinSize),(position.y+1.5*pinSize)+i*2.5*pinSize);
        else
            p.body.setPosition((position.x+size.x+pinSize),(position.y) + (i+1)*(size.y/(pgate->getOutputs().size()+1)));
        p.body.setRadius(pinSize);
        p.body.setOutlineThickness(pinSize/15);
        p.body.setOutlineColor(sf::Color::White);
        p.offset = p.body.getPosition() - position;

        pins.push_back(p);
        i++;
    }
}

void DrawGate::updatePins(){
    for (pin& p : pins){
        p.body.setPosition(position + p.offset);
    }
}

sf::Vector2f DrawGate::getSize(){
    return size;
}
sf::Vector2f DrawGate::getPosition(){
    return position;
}
void DrawGate::setPosition(sf::Vector2f position){
    this->position = position;
    body.setPosition(position);
    labelName.setPosition(sf::Vector2f(position.x+size.x/2,position.y+size.y/2));
    updatePins();
}

std::string DrawGate::getName(){
    return name;
}
LogicGate* DrawGate::getGate(){
    return pgate;
}

DrawGate::pin* DrawGate::pressedPin(sf::Vector2f pos){
    for (pin& p : pins){
        float dis = sqrt(pow(p.body.getPosition().x-pos.x,2)+pow(p.body.getPosition().y-pos.y,2));
        if (dis < 2*pinSize){
            return &p;
        }
    }
    return nullptr;
}
DrawGate* DrawGate::pressedDrawGate(sf::Vector2f pos){
    //sf::FloatRect rect = body.getLocalBounds();
    if (position.x < pos.x && position.x+size.x > pos.x && position.y < pos.y && position.y+size.y > pos.y)
        return this;
    return nullptr;
}

void DrawGate::draw(sf::RenderWindow& win,bool update){
    if (update){
        updateBody();
    }
    win.draw(body);
    for (pin& p : pins){
        p.body.setFillColor(sf::Color::Black);
        if (p.connectedLOP->output == STATE::HIGH)
            p.body.setFillColor(sf::Color::Green);
        if (p.connectedLOP->output == STATE::LOW)
            p.body.setFillColor(sf::Color::Red);
        win.draw(p.body);
    }

    win.draw(labelName);
}