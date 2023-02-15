#ifndef __BASE_CONATINER_
#define __BASE_CONATINER_
#include "UI-BASE.h"

#include <vector>
#include <map>

class BaseContainer : public UIBASE{// a container is a position manager
protected:
    std::vector<UIBASE*> referenzes;
public:
    BaseContainer() = default;
    BaseContainer(sf::Vector2f,sf::Vector2f,sf::Color);

    virtual void draw(sf::RenderWindow&,bool=true);

    virtual void setLocalPositionOfUI(UIBASE&,sf::Vector2f,piviot=UIBASE::LEFTTOP);
    virtual void setRefsPosition();

    virtual void addRef(UIBASE*);
    
    virtual int findRef(UIBASE*);
    virtual void removeRef(unsigned int);
    virtual void removeRef(UIBASE*);

    virtual UIBASE* getUI(unsigned int);
    virtual UIBASE* getUI(UIBASE*);

    std::vector<UIBASE*> getRefs();

};

#endif