#include "../header/ui/H-ScrollContainer.h"

void HScrollContainer::addRef(UIBASE* base){
    base->setOrigin(LEFTTOP);
    referenzes.push_back(base);
    if (referenzes.size() > 0){
        UIBASE* last = referenzes.at(referenzes.size()-1);
        base->setGlobalPosition(last->getGlobalPosition()+last->getSize());
    }else{
        base->setGlobalPosition(GlobalPosition);
    }
}

void HScrollContainer::scrollH(float scroll){
    this->scroll += scroll;
}