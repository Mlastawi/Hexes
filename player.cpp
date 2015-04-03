#include "player.h"
#include <SFML/Graphics.hpp>

using namespace sf;

unsigned char Player::playerCount = 0;

Player::Player( Color color ) : ID ( playerCount++ ), playerColor( color ){
    first = true;
    hexCount = 0;
}

unsigned char Player::getID() const{
    return ID;
}

Color Player::getColor() const{
    return playerColor;
}

void Player::setColor( Color color ){
    playerColor = color;
}

Color Player::getPickedColor() const{
    return pickedColor;
}

void Player::setPickedColor( Color color ){
    pickedColor = color;
}

bool Player::isFirst() const{
    return first;
}

void Player::setFirst( bool setter ){
    first = setter;
}

int Player::getHexCount() const{
    return hexCount;
}

void Player::increaseHexCount(  ){
    hexCount++;
}

void Player::decreaseHexCount(  ){
    if( hexCount > 0 )
        hexCount--;
}
