#include "../header/ui/UI-BASE.h"

UIBASE::UIBASE(sf::Vector2f position, sf::Vector2f size, sf::Color color){
    this->GlobalPosition = position;
    this->size = size;
    this->color = color;
}

UIBASE::~UIBASE(){

}

void UIBASE::draw(sf::RenderWindow& win,bool drawRect){
    if (drawRect)
        win.draw(rect);
}
void UIBASE::updateBody(){
    updateRect();
}
void UIBASE::updateRect(){
    rect.setPosition(GlobalPosition);
    rect.setSize(size);
    rect.setFillColor(color);
}

void UIBASE::update(){
    return;
}

void UIBASE::setGlobalPosition(sf::Vector2f position){
    this->GlobalPosition = position;
    updateBody();
}
sf::Vector2f UIBASE::getGlobalPosition(){return this->GlobalPosition;}
void UIBASE::setLocalPosition(sf::Vector2f position){
    this->LocalPosition = position;
    //updateBody();
}
sf::Vector2f UIBASE::getLocalPosition(){return this->LocalPosition;}


void UIBASE::setSize(sf::Vector2f size){
    this->size = size;
    updateBody();
}
sf::Vector2f UIBASE::getSize(){return this->size;}


void UIBASE::setColor(sf::Color color){
    this->color = color;
    updateBody();
}
sf::Color UIBASE::getColor(){return this->color;}

void* UIBASE::getBody(){
    return nullptr;
}

void UIBASE::setOrigin(UIBASE::piviot piv){
    setRectOrigin(piv);
    updateBody();
}
void UIBASE::setRectOrigin(UIBASE::piviot piv){
    if (piv == CENTER)
        rect.setOrigin(sf::Vector2f(size.x/2,size.y/2));
    else if (piv == LEFTTOP)
        rect.setOrigin(sf::Vector2f(0,0));
    else if (piv == RIGHTTOP)
        rect.setOrigin(sf::Vector2f(size.x,0));
    else if (piv == LEFTDOWN)
        rect.setOrigin(sf::Vector2f(0,size.y));
    else if (piv == RIGHTDOWN)
        rect.setOrigin(sf::Vector2f(size.x,size.y));
}

sf::RectangleShape* UIBASE::getRect(){return &this->rect;}
/*
void UIBASE::setRect(sf::RectangleShape r){
    this->rect = r;
}

sf::Vector2f UIBASE::GPTLP(const sf::Vector2f& globalPosition){
    return globalPosition - position;
}
sf::Vector2f UIBASE::getLPFO(const sf::Vector2f& offset){
    return position + offset;
}*/