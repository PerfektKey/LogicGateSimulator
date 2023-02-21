#ifndef __H_ScrollContainer__
#define __H_ScrollContainer__
#include "UI-BASE.h"
#include "base-container.h"
#include <vector>

class HScrollContainer : public BaseContainer{
private:
    float scroll;

    virtual void updateRefs();
public:
    HScrollContainer() = default;

    void scroll(float);
    void setScroll(float);

    virtual void draw(const sf::RenderWindow&,bool=true);
};

#endif