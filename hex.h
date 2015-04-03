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
   // static bool first[2];
    static bool any_picked;

    HexType type;
    Vector2f center; //srodek HEXa
    int size; //odleglosc wierzcholka od srodka
    Color color; //kolor hexa
    Color center_color; //kolor srodka hexa
    VertexArray shape; //tablica punktów
    VertexArray border; //ramka
    Vector2i coordinates; //wspolzedne w ukladzie osiowym
    bool enabled; //czy jest wyswietlany
    Vector2i neighbors[6]; //tablica ze wspolrzednymi sasiadow
    bool picked; //czy wybrany

    int strength; //sila pola
    Font* font; //czcionka hexa
    Text textToDisplay; //tekst na hexie

public:
    Hex(HexType=Hex::Flat, Vector2f= Vector2f(0,0), int = 30, Vector2i=Vector2i(0,0), Color = Color::White, Color = Color::White, bool = true);

    Vector2i getCoords() const; //zwraca koordynaty w ukladzie osiowym
    Vector2f getCenter() const; //zwraca x,y srodka

    void setColor(Color); //ustawia kolor hexa
    void setCenterColor(Color); //ustawia kolor srodka hexa
    Color getColor() const; //zwraca kolor hexa

    void enable(); //uaktywnij
    void disable(); //zdezaktywuj
    bool isEnabled() const; //czy jest aktywny

    bool isNeighbor(const Hex&) const; //czy sasiaduje
    Vector2i* getNeighbors(); //zwraca sasiadow

    void pick(); //zaznacz
    void unpick(); //odznacz
    bool isPicked(); //czy zaznaczony
    static void unpickAll(); //odznacz wszystko - chyba nieuzywane

    void increaseStrength(int=1); //zwieksz sile o (domyslnie 1)
    void decreaseStrength(int=1); //zmniejsz sile o (domyslnie 1)
    int getStrength() const; //pobierz sile
    void setStrength(int); //ustaw sile na

    void setFont(Font* const); //ustaw czcionke pola

private:
    virtual void draw(RenderTarget&, RenderStates) const; //sluzy do rysowania
    void changeText(std::string); //zmien tekst pola
    void changeText(int); //zmien tekst pola

};

#endif
