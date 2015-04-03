#ifndef HEX
#define HEX

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

using namespace sf;

class Hex : public Drawable, public Transformable{
public:
    enum HexType{Flat, Pointy};
    enum HexColor{Red=0, Blue=1};

private:
    static bool first[2];
    static int count[2];
    static bool any_picked;

    HexType type;
    Vector2f center; //srodek HEXa
    int size; //odleglosc wierzcholka od srodka
    Color color;
    Color center_color;
    VertexArray shape; //tablica punktów
    VertexArray border; //ramka
    Vector2i coordinates; //wspolzedne w ukladzie osiowym
    bool enabled; //czy jest wyswietlany
    Vector2i neighbors[6]; //tablica ze wspolrzednymi sasiadow
    bool picked; //czy wybrany

    int strength;
    Font* font;
    Text textToDisplay;

public:
    Hex(HexType=Hex::Flat, Vector2f= Vector2f(0,0), int = 30, Vector2i=Vector2i(0,0), Color = Color::White, Color = Color::White, bool = true);

    Vector2i getCoords() const; //zwraca koordynaty w ukladzie osiowym
    Vector2f getCenter() const; //zwraca x,y srodka
    void setColor(Color);
    void setCenterColor(Color);
    Color getColor() const;
    void enable();
    void disable();
    bool isEnabled() const;
    bool isNeighbor(const Hex&) const;
    Vector2i* getNeighbors();
    void pick();
    void unpick();
    bool isPicked();

    void increaseStrength(int=1);
    void decreaseStrength(int=1);
    int getStrength() const;
    void setStrength(int);

    void setFont(Font* const);

    static void unpickAll();
    static bool isFirst(HexColor);
    static void setFirst(bool, HexColor);
    static void increaseCount(HexColor);
    static void decreaseCount(HexColor);
    static int getCount(HexColor);

private:
    virtual void draw(RenderTarget&, RenderStates) const; //sluzy do rysowania
    void changeText(std::string);
    void changeText(int);

};

#endif
