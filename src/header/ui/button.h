#ifndef __BUTTON__
#define __BUTTON__
#include "UI-BASE.h"
#include <SFML/Graphics.hpp>

class Button : public UIBASE {// Do i really need a new class for this?
public:
    Button() = default;
    Button(sf::Vector2f,sf::Vector2f,sf::Color);

    virtual void* getBody();

    bool isPressed(sf::Vector2f,const sf::Event&);
    Button* isPressedC(sf::Vector2f,const sf::Event&);

    virtual void draw(sf::RenderWindow&,bool=true);

    //Button* isPressed(sf::Vector2f);
};


#endif