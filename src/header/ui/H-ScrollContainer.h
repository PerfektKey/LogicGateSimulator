#ifndef __H_ScrollContainer__
#define __H_ScrollContainer__
#include "UI-BASE.h"
#include "base-container.h"
#include <vector>

class HScrollContainer : public BaseContainer{
private:
    float scroll;
    float min_scroll;
    float max_scroll;

    virtual void updateRefs();
public:
    HScrollContainer() = default;

    //virtual void draw(const sf::RenderWindow&,bool=true);

    virtual void addRef(UIBASE*);

    virtual void setLocalPositionOfUI(UIBASE&,sf::Vector2f,piviot=UIBASE::LEFTTOP);
    virtual void setRefsPosition();

    void scrollH(float);
    void setScroll(float);

};

#endif