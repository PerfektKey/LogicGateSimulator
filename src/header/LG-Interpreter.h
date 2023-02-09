#ifndef __LG_INTERPRETER__
#define  __LG_INTERPRETER__

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Logic-Gate.h"
#include "logic-operandi.h"
#include <math.h>

class LGInterpreter{
private:
    const unsigned int pinSize = 15;

    enum class BLOCK_TYPE {PIN,GATE};
    LogicGate* gate;
    std::string name;
    BLOCK_TYPE bt;

    sf::RectangleShape body;
    sf::Vector2f position;
    sf::Vector2f size;
    sf::Color color;

    fs::path Gpath;
    unsigned int Gindex;

    struct pin{
        logicOperandi* connectedLOP;
        sf::CircleShape body;
        std::string name;
    public:
        pin();
        ~pin();
        pin(const pin&);
        pin(pin&&);
        pin& operator=(pin&&) = delete;
    };

    std::vector<pin> pins;

    int max(int a,int b);

    void setBody();
    void setPins();

public:
    LGInterpreter(fs::path,sf::Vector2f,unsigned int);
    LGInterpreter(const LGInterpreter&);
    LGInterpreter(LGInterpreter&&);
    
    LogicGate* getGate();

    void updateBody();

    void draw(sf::RenderWindow&);

    LGInterpreter* pressed(sf::Vector2i);
    logicOperandi* pressedPin(sf::Vector2i);
};


#endif