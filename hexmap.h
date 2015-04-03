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
    Vector2i size; //rozmiar mapy
    Vector2f coordinateCenter; //srodek hexa o koordynatach 0,0
    int radius; //"promien" dla typu mapy Hexagon
    int *first; //tablica wspolrzednych pierwszego wiersza/kolumny
    MapType type; //typ mapy
    Hex** map; //tablica hexow
    int hexSize; //rozmiar promienia hexow
    Font hexFont; //czcionka na hexach
    Hex blankHex; //"pusty" i nieaktywny hex

public:
    HexMap(MapType, Vector2i=Vector2i(0,0), Vector2f=Vector2f(0,0), int=30); //konstruktor
    HexMap(int, Vector2f=Vector2f(0,0), int=30); //konstruktor typu "hexagon"

    //ustaw hexa na danych koordynatach
    void setHex(int, int, Hex);
    void setHex(Vector2i, Hex);

    //zwroc referencje do hexa o danych koordynatach
    Hex& getHex(int, int);
    Hex& getHex(Vector2i);

    Vector2i findHex(Vector2i); //zwroc koordynaty hexa z danej pozycji myszki

    void randomize(); //losuje mape

    void unpickAllHexes(); //odznacza wszystkie hexy - chyba nieuzywane

private:
    virtual void draw(RenderTarget&, RenderStates) const;

    //do zwrotu koordynatow
    Vector2i roundHex(Vector2f);

};
#endif
