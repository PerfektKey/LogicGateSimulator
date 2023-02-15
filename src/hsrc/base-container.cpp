#include "../header/ui/base-container.h"


BaseContainer::BaseContainer(sf::Vector2f position, sf::Vector2f size, sf::Color color)
       :UIBASE(position,size,color) {
    updateBody();
}

void BaseContainer::draw(sf::RenderWindow& win,bool drawRect){
    if (drawRect)
        win.draw(rect);
    for (UIBASE* uib : referenzes)
        uib->draw(win);
    
}

void BaseContainer::setLocalPositionOfUI(UIBASE& uib,sf::Vector2f localP,piviot align){
    uib.setGlobalPosition(GlobalPosition-rect.getOrigin()+localP);
}
void BaseContainer::setRefsPosition(){
    return;//null here
}

void BaseContainer::addRef(UIBASE* element){
    referenzes.push_back(element);
    updateBody();
}

int BaseContainer::findRef(UIBASE* element){
    for (int i = 0;i < referenzes.size();i++)
        if (referenzes.at(i) == element)
            return i;
    
    return -1;
}
void BaseContainer::removeRef(unsigned int index){
    if (index > referenzes.size())
        return;
    referenzes.erase( referenzes.begin() + index);
    updateBody();
}
void BaseContainer::removeRef(UIBASE* element){
    int i =  findRef(element);
    if ( i != -1)
        removeRef(i);
    updateBody();
}

UIBASE* BaseContainer::getUI(unsigned int index){
    if (index > referenzes.size())
        return nullptr;
    return referenzes.at(index);
}
UIBASE* BaseContainer::getUI(UIBASE* element){
    unsigned int index = findRef(element);
    if (index > referenzes.size() || index < 0)
        return nullptr;
    return referenzes.at(index);
}

std::vector<UIBASE*> BaseContainer::getRefs(){
    return referenzes;
}