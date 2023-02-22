#ifndef __SCROLL_CONTAINER__
#define __SCROLL_CONTAINER__

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <filesystem>
namespace fs = std::filesystem;

class scrollContainer{
private:
    struct labelButton{
        sf::RectangleShape body;
        sf::Text labelName;
        std::string name;
        sf::Vector2f position;
    };

    sf::Font labelFont;

    const int CharSize = 48;

    std::vector<labelButton> __labels;

    //sf::Vector2f __position_offset;
    //sf::Vector2f __position_offset;

    unsigned long labelLenght;

    long double _scroll;
    long double _max_scroll;
    long double _min_scroll = 0;

    sf::RectangleShape __body;
    sf::Vector2f __position;
    sf::Vector2f __size;
    sf::Color __color;

    void updateBody();
    void updateLabels();

public:
    scrollContainer();
    scrollContainer(sf::Vector2f,sf::Vector2f);

    void setColor(sf::Color);
    void setFont(fs::path);

    void setPosition(float,float);
    void setPosition(sf::Vector2f);
    sf::Vector2f getPosition();

    void addLabel(std::string);
    std::vector<labelButton>& getLabels();

    std::string PressLabel(sf::Vector2f);
    bool inside(sf::Vector2f);

    void draw(sf::RenderWindow&);

    void scroll(long double);
    void setScroll(long double=0);
};

#endif