#ifndef HEXMAP
#define HEXMAP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

using namespace sf;

class HexMap : public Drawable, public Transformable{
public:
    enum MapType {Odd_r, Even_r, Odd_q, Even_q, Triangle, Hexagon, Rhombus};

private:
    Vector2i size;
    Vector2f coordinateCenter;
    int radius;
    int *first;
    MapType type;
    Hex** map;
    int hexSize;
    std::string text;
    Font hexFont;
    Hex blankHex;

public:
    HexMap(MapType, Vector2i=Vector2i(0,0), Vector2f=Vector2f(0,0), int=30, std::string="");
    HexMap(int, Vector2f=Vector2f(0,0), int=30, std::string="");
    void setHex(int, int, Hex);
    void setHex(Vector2i, Hex);
    Hex& getHex(int, int);
    Hex& getHex(Vector2i);
    Vector2i findHex(Vector2i);
    Vector2i roundHex(Vector2f);

    void randomize();
    void unpickAllHexes();

private:
    virtual void draw(RenderTarget&, RenderStates) const;

};
#endif
