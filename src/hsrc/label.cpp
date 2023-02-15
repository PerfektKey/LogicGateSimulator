#include "../header/ui/label.h"

Label::Label(sf::Vector2f position, sf::Vector2f size, sf::Color color)
       :UIBASE(position,size,color) {
    updateBody();
}
Label::Label(sf::Vector2f position, sf::Vector2f size, sf::Color color, std::string str)
       :UIBASE(position,size,color) {
    setString(str);
}

void Label::draw(sf::RenderWindow& win,bool drawRect){
    if (drawRect)
        win.draw(rect);
    win.draw(text);
}

void Label::updateBody(){
    text.setFont(font);
    text.setString(string);
    text.setPosition(GlobalPosition);
    text.setCharacterSize(CharSize);

    updateRect();
}

void Label::loadFont(std::filesystem::path path){
    sf::Font dummy;
    if ( !dummy.loadFromFile( path.generic_string() ))
        std::cout << "failed loading from file " << path.generic_string() << " to " << this << " continue to use old font" << "\n"; 
    else
        font.loadFromFile(path.generic_string());
    updateBody();
}
void Label::setString(std::string str){
    string = str;
    updateBody();
}
std::string Label::getString(){
    return string;
}

void Label::setCharSize(unsigned int cs){
    CharSize = cs;
    updateBody();
}

sf::Text* Label::getText(){
    return &text;
}
sf::Font* Label::getFont(){
    return &font;
}

void Label::setOrigin(UIBASE::piviot piv){
    setRectOrigin(piv);
    sf::FloatRect textRect = text.getLocalBounds();
    if (piv == CENTER)
        text.setOrigin(sf::Vector2f( textRect.left + textRect.width/2, textRect.top + textRect.height/2 ));
    else if (piv == LEFTTOP)
        text.setOrigin(sf::Vector2f( textRect.left, textRect.top));
    else if (piv == RIGHTTOP)
        text.setOrigin(sf::Vector2f( textRect.left+textRect.width, textRect.top));
    else if (piv == LEFTDOWN)
        text.setOrigin(sf::Vector2f( textRect.left, textRect.top+textRect.height));
    else if (piv == RIGHTDOWN)
        text.setOrigin(sf::Vector2f( textRect.left + textRect.width, textRect.top + textRect.height ));
    updateBody();
}