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
        sf::Font labelFont;
        sf::Text labelName;
        std::string name;
        sf::Vector2f position;
    };

    const int CharSize = 48;

    std::vector<labelButton> __labels;

    sf::Vector2f __position_offset;

    float _scroll;

    sf::RectangleShape __body;
    sf::Vector2f __position;
    sf::Vector2f __size;
    sf::Color __color;

    void updateBody();

public:
    scrollContainer();
    scrollContainer(sf::Vector2f,sf::Vector2f);

    void setColor(sf::Color);

    void addLabel(std::string,fs::path);

    std::string PressLabel(sf::Vector2f);

    void draw(sf::RenderWindow&);
};

#endif