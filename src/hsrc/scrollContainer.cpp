#include "../header/scrollContainer.h"

scrollContainer::scrollContainer(sf::Vector2f _position,sf::Vector2f _size){
    __position = _position;
    __size = _size;
    updateBody();
}
scrollContainer::scrollContainer(){
    return;
}

void scrollContainer::setColor(sf::Color _color){
    __color = _color;
    updateBody();
}

void scrollContainer::addLabel(std::string name,fs::path font){
    labelButton lb;
    lb.name = name;
    lb.labelFont.loadFromFile(font.generic_string());
    lb.labelName.setString(name);
    lb.labelName.setCharacterSize(CharSize);
    //lb.labelName.setFillColor(sf::Color::Black);
    sf::FloatRect textRect = lb.labelName.getLocalBounds();
    //lb.labelName.setOrigin(textRect.left + textRect.width/2.0f,
    //           textRect.top  + textRect.height/2.0f);
    sf::Vector2f tmp;
    sf::Vector2f tmpS;
    std::cout << "name: " << name << " lenght = " << name.length() << " size = " << name.size() << "\n";
    tmpS.x = textRect.width;
    tmpS.y = textRect.height;
    tmp = sf::Vector2f(name.length()*CharSize,__size.y*.9);
    std::cout << "rect size = " << tmp.x << "\n";
    if (__labels.size() == 0){
        lb.position = __position + sf::Vector2f(5,__size.y*.05);
    }else{
        tmp = __labels.at(__labels.size()-1).body.getSize();
        lb.position = __labels.at(__labels.size()-1).position + tmp + sf::Vector2f(5,-tmp.y);
    }
    tmp = sf::Vector2f(name.length()*CharSize,__size.y*.9);;
    lb.labelName.setPosition(lb.position+sf::Vector2f(tmp.x*0,tmp.y*.125));
    //lb.labelName.set
    lb.body.setPosition(lb.position);
    lb.body.setFillColor(sf::Color::Blue);
    lb.body.setSize(tmp);

    __labels.push_back(lb);
    //labelButton* lbr = &__labels.at(__labels.size()-1);
    //lbr->labelName.setFont(lbr->labelFont);
}

std::string scrollContainer::PressLabel(sf::Vector2f point){
    for (labelButton& LB : __labels){
        if (LB.body.getSize().x+(LB.position.x-__position_offset.x) > __position.x){
            sf::FloatRect rect = LB.body.getGlobalBounds();
            if (rect.contains(point)){
                return LB.name;
            }
        }
    }
    return "";
}

void scrollContainer::updateBody(){
    __body.setFillColor(__color);
    __body.setPosition(__position);
    __body.setSize(__size);
}

void scrollContainer::draw(sf::RenderWindow& win){
    win.draw(__body);
    for (labelButton& LB : __labels){
        if (LB.body.getSize().x+(LB.position.x-__position_offset.x) > __position.x && LB.position.x-__position_offset.x-LB.body.getSize().x < win.getSize().x){
            LB.labelName.setFont(LB.labelFont);
            LB.labelName.setPosition(sf::Vector2f(LB.labelName.getPosition().x-__position_offset.x,LB.position.y));
            LB.body.setPosition(sf::Vector2f(LB.position.x-__position_offset.x,LB.position.y));
            win.draw(LB.body);
            win.draw(LB.labelName);
        }
    }
}