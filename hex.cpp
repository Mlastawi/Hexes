#include "hex.h"
#include <cmath>
#include <iostream>
#include "myHeader.h"

using namespace sf;

bool Hex::any_picked = false;

const Color LightRed( 255, 128, 128 );
const Color LightBlue( 128, 128, 255 );
const Color DarkBlue( 0, 0, 128 );

Hex::Hex(HexType type, Vector2f center, int size, Vector2i coordinates, Color color, Color center_color, bool enabled){
    shape.setPrimitiveType(TrianglesFan);
    shape.resize(8);
    border.setPrimitiveType(LinesStrip);
    border.resize(7);

    strength = 1;
    font = NULL;
    textToDisplay.setString(intToString(strength));
    textToDisplay.setCharacterSize(30);
    textToDisplay.setColor(Color::Black);

    this->type = type;
    this->setPosition(center);
    this->center = center;
    this->size = size;
    this->coordinates = coordinates;
    this->color = color;
    this->center_color = center_color;
    this->enabled = enabled;
    this->picked = false;

    shape[0].position=Vector2f(0,0);
    shape[0].color=center_color;

    for(unsigned int i=1; i<shape.getVertexCount(); i++){
        float angle_deg;
        if(type==Hex::Flat)
            angle_deg = 60 * (i-1);
        else if(type==Hex::Pointy)
            angle_deg = 60 * (i-1) + 90;
        float angle_rad = M_PI / 180 * angle_deg;

        shape[i].color=color;
        shape[i].position=Vector2f((float)size*cos(angle_rad), (float)size*sin(angle_rad));
        border[i-1].position=Vector2f((float)size*cos(angle_rad), (float)size*sin(angle_rad));
        border[i-1].color=Color::Black;
    }

    neighbors[0]=Vector2i(coordinates.x+1, coordinates.y);
    neighbors[1]=Vector2i(coordinates.x+1, coordinates.y-1);
    neighbors[2]=Vector2i(coordinates.x, coordinates.y-1);
    neighbors[3]=Vector2i(coordinates.x-1, coordinates.y);
    neighbors[4]=Vector2i(coordinates.x-1, coordinates.y+1);
    neighbors[5]=Vector2i(coordinates.x, coordinates.y+1);
}

Vector2i Hex::getCoords() const{
    return coordinates;
}
Vector2f Hex::getCenter() const{
    return center;
}
void Hex::draw( sf::RenderTarget& target, sf::RenderStates states ) const{
        states.transform *= getTransform();

        // ustawienie textury HEXa
        states.texture = NULL;

        // rysowanie
        target.draw(shape, states);
        target.draw(border, states);
        target.draw(textToDisplay, states);
    };

void Hex::setColor(Color color){
    this->color = color;
    for(unsigned int i = 1; i < shape.getVertexCount(); i++ ){
        shape[i].color=color;
    }
}
void Hex::setCenterColor(Color color){
    center_color = color;
    shape[0].color=center_color;
}
Color Hex::getColor() const{
    return color;
}

void Hex::enable(){
        enabled = true;
}
void Hex::disable(){
        enabled = false;
}
bool Hex::isEnabled() const{
    return enabled;
}

bool Hex::isNeighbor(const Hex& hex) const{
    for(int i = 0; i < 6; i++){
        if( hex.getCoords() == neighbors[i] )
            return true;
    }
    return false;
}
Vector2i* Hex::getNeighbors(){
    return neighbors;
}

void Hex::pick(){
    picked = true;
    any_picked = true;
    if( color == Color::Red )
        this->setColor( LightRed );
    else if( color == Color::Blue )
        this->setColor( LightBlue );

}
void Hex::unpick(){
    picked = false;
    if( color == LightRed )
        this->setColor( Color::Red );
    else if( color == LightBlue )
        this->setColor( Color::Blue);
}
void Hex::unpickAll(){
    any_picked = false;
}
bool Hex::isPicked(){
    return picked;
}

void Hex::increaseStrength(int amount){
    strength+=amount;

    changeText( strength );
}
void Hex::decreaseStrength(int amount){
    if(strength-amount>0)
        strength-=amount;
    else strength=0;

    changeText( strength );
}
int Hex::getStrength() const{
    return strength;
}
void Hex::setStrength(int var){
    strength = var;
    changeText( strength );
}

void Hex::setFont(Font* const toSet){
    font = toSet;
    if (font!=NULL)
        textToDisplay.setFont(*font);

    FloatRect textRect = textToDisplay.getLocalBounds();
    textToDisplay.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
}

void Hex::changeText(int a){
    textToDisplay.setString(intToString(a));
    FloatRect textRect = textToDisplay.getLocalBounds();
    textToDisplay.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
}
void Hex::changeText(std::string a){
    textToDisplay.setString(a);
    FloatRect textRect = textToDisplay.getLocalBounds();
    textToDisplay.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
}
