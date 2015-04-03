#ifndef PLAYER
#define PLAYER

#include <SFML/Graphics.hpp>

using namespace sf;

class Player{
    static unsigned char playerCount;
    unsigned const char ID;
    Color playerColor;
    Color pickedColor;
    bool first;
    int hexCount;

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
