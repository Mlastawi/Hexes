#ifndef PLAYER
#define PLAYER

#include <SFML/Graphics.hpp>

using namespace sf;

class Player{
    static unsigned char playerCount; //ilosc graczy
    unsigned const char ID; //id gracza
    Color playerColor; //kolor gracza
    Color pickedColor; //kolor wybranego gracza
    bool first; //czy postawiono juz pierwszego hexa
    int hexCount; //ilosc posiadanych hexow

public:
    Player( Color = Color::Transparent );

    unsigned char getID() const;

    Color getColor() const;
    void setColor( Color );
    Color getPickedColor() const;
    void setPickedColor( Color );

    bool isFirst() const;
    void setFirst( bool );

    int getHexCount() const;
    void increaseHexCount(  );
    void decreaseHexCount(  );

};

#endif // PLAYER
