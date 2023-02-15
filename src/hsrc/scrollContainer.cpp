#include "../header/ui/scrollContainer.h"

scrollContainer::scrollContainer(sf::Vector2f _position,sf::Vector2f _size){
    __position = _position;
    __size = _size;
    _scroll = 0;
    updateBody();
}
scrollContainer::scrollContainer(){
    _scroll = 0;
    return;
}

void scrollContainer::setColor(sf::Color _color){
    __color = _color;
    updateBody();
}
void scrollContainer::setFont(fs::path path){
    if ( !labelFont.loadFromFile(path.generic_string()) )
        std::cout << "unable to load font for scrollContainer( " << this << " ) from: " << path << "\n";
}

void scrollContainer::addLabel(std::string name){
    labelButton lb;
    lb.name = name;
    //lb.labelFont.loadFromFile(font.generic_string());
    lb.labelName.setFont(labelFont);
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
    //std::cout << "rect size = " << tmp.x << "\n";
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
    //if (lb.body.getSize().x > 0)
    //_max_scroll += lb.body.getSize().x;
    labelLenght += lb.body.getSize().x;
    if (__size.x < labelLenght)
        _max_scroll = labelLenght-__size.x;

    __labels.push_back(std::move(lb));
    //labelButton* lbr = &__labels.at(__labels.size()-1);
    //lbr->labelName.setFont(lbr->labelFont);
}

std::vector<scrollContainer::labelButton>& scrollContainer::getLabels(){
    return __labels;
}

std::string scrollContainer::PressLabel(sf::Vector2f point){
    for (labelButton& LB : __labels){
        if (LB.body.getSize().x+(LB.position.x-_scroll) > __position.x){
            sf::FloatRect rect = LB.body.getGlobalBounds();
            if (rect.contains(point)){
                return LB.name;
            }
        }
    }
    return "";
}

void scrollContainer::scroll(long double amm){
    if (_scroll+amm > _min_scroll-10 && _scroll+amm < _max_scroll+10)
        _scroll += amm;
    //if (_scroll+amm > _min_scroll && _scroll-amm < _max_scroll)
    //    _scroll += amm;
}
void scrollContainer::setScroll(long double __scroll){
    _scroll = __scroll;
}

void scrollContainer::updateBody(){
    __body.setFillColor(__color);
    __body.setPosition(__position);
    __body.setSize(__size);
}

void scrollContainer::draw(sf::RenderWindow& win){
    win.draw(__body);
    for (labelButton& LB : __labels){
        if (LB.body.getSize().x+(LB.position.x-_scroll) > __position.x && LB.position.x-_scroll-LB.body.getSize().x < win.getSize().x){
            ///std::cout << LB.name << " " << (std::string)LB.labelName.getString() << std::endl;
            //LB.labelName.setFont(labelFont);
            //LB.labelName.setPosition(sf::Vector2f(LB.labelName.getPosition().x-_scroll,LB.position.y));
            LB.body.setPosition(sf::Vector2f(LB.position.x-_scroll,LB.position.y));
            LB.labelName.setPosition(LB.body.getPosition());
            LB.labelName.setString(LB.name);
            win.draw(LB.body);
            win.draw(LB.labelName);
        }
    }
}