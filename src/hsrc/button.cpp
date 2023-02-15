#include "../header/ui/button.h"

Button::Button(sf::Vector2f position, sf::Vector2f size, sf::Color color)
       :UIBASE(position,size,color) {
    updateBody();
}

void Button::draw(sf::RenderWindow& win,bool drawRect){
    if (drawRect)
        win.draw(rect);
}
/*
void Button::updateBody(){
    rect.setPosition(this->GlobalPosition);
    rect.setSize(this->size);
    rect.setFillColor(this->color);
}
*/

void* Button::getBody(){
    return &this->rect;
}

bool Button::isPressed(sf::Vector2f point,const sf::Event& event){
    if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
        return rect.getGlobalBounds().contains(point);
    return false;
}
Button* Button::isPressedC(sf::Vector2f point,const sf::Event& event){
    if (isPressed(point,event))
        return this;
    else
        return nullptr;
}