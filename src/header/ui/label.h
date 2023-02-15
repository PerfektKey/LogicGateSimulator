#ifndef __LABEL__
#define __LABEL__
#include <string>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include "UI-BASE.h"

class Label : public UIBASE {
private:
    sf::Text text;
    sf::Font font;
    unsigned int CharSize;
    std::string string;
protected:
    virtual void updateBody();
public:
    Label() = default;
    Label(sf::Vector2f,sf::Vector2f,sf::Color);
    Label(sf::Vector2f,sf::Vector2f,sf::Color,std::string);

    virtual void draw(sf::RenderWindow&,bool=false);

    sf::Text* getText();
    sf::Font* getFont();

    void loadFont(std::filesystem::path);
    void setString(std::string);
    void setCharSize(unsigned int);

    virtual void setOrigin(piviot);

    std::string getString();
};

#endif