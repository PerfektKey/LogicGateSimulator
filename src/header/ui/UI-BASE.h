#ifndef __UI_BASE__
#define __UI_BASE__
#include <iostream>
#include <SFML/Graphics.hpp>

class UIBASE{
public:
    enum piviot{CENTER = 0,LEFTTOP,RIGHTTOP,LEFTDOWN,RIGHTDOWN};
private:
protected:
    sf::Vector2f GlobalPosition;
    sf::Vector2f LocalPosition;
    sf::Vector2f size;
    sf::Color color;

    sf::RectangleShape rect;

    virtual void updateBody();
    void updateRect();

    void setRectOrigin(piviot);
public:
    UIBASE() = default;
    UIBASE(sf::Vector2f,sf::Vector2f,sf::Color);
    virtual ~UIBASE();

    virtual void update();

    virtual void* getBody();

    virtual void setGlobalPosition(sf::Vector2f);
    virtual void setLocalPosition(sf::Vector2f);
    virtual void setSize(sf::Vector2f);
    virtual void setColor(sf::Color);
    //void setRect(sf::RectangleShape);
    virtual void setOrigin(piviot);

    sf::RectangleShape* getRect();
    virtual sf::Vector2f getGlobalPosition();
    virtual sf::Vector2f getLocalPosition();
    virtual sf::Vector2f getSize();
    virtual sf::Color getColor();

    virtual void draw(sf::RenderWindow&,bool=true);

    //virtual sf::Vector2f GPTLP(const sf::Vector2f&);// Global position to local position
    //virtual sf::Vector2f getLPFO(const sf::Vector2f&);// get Local Position from offset 
};
#endif