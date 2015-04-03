#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#include "myHeader.h"
#include "hex.h"
#include "hexmap.h"
#include <climits>
#include "player.h"
#include <ctime>

using namespace sf;

int main(){
    enum GameMode{Display, Orders, Regroup};

    const Color LightRed( 255, 128, 128 );
    const Color LightBlue( 128, 128, 255 );
    const Color DarkBlue( 0, 0, 128 );

    srand( time( NULL ) );
//-------------------------------------------------//
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16; //wygladzanie

    RenderWindow window(VideoMode( 1024, 720 ), "Hex test", Style::Close, settings);
    window.setFramerateLimit( 30 );
    Event event;

    Font font;
    if(!(font.loadFromFile( "arial.ttf" )))
        return 1;
    Text display_text( "", font, 20 );
    display_text.setColor( Color::White );

    int hexSize = 30;
    Vector2i mapSize( 20, 11 );
    Vector2f ffirst( 80, 80 );
//------------------------------------------------//
    HexMap *map;
    int a=0;
    a=1;
    //std::cin >> a;
    if( a == 1 )
        map = new HexMap( HexMap::Odd_q, mapSize, ffirst, hexSize) ;
    else if( a == 2 )
        map = new HexMap( HexMap::Even_q, mapSize, ffirst, hexSize );
    else if( a == 3 )
        map = new HexMap( HexMap::Odd_r, mapSize, ffirst, hexSize );
    else if( a == 4 )
        map = new HexMap( HexMap::Even_r, mapSize, ffirst, hexSize );
    else if( a == 5 )
        map = new HexMap( 3, Vector2f(512,360), hexSize );
    else
        return 1;

    map -> randomize();

    GameMode mode = Display;

    unsigned char playerCount = 2;

    Player players[2];
    players[0].setColor( Color::Red );
    players[0].setPickedColor( LightRed );
    players[1].setColor( Color::Blue );
    players[1].setPickedColor( LightBlue );

    Player* currentPlayer = players;

    int avaiablePoints = 10;

    Hex* picked = NULL;

    while(window.isOpen())
    {
        while(window.pollEvent(event)){
            if( event.type == Event::Closed )
                window.close();
            if( event.type == Event::KeyPressed ){
                switch( event.key.code ){
                    case Keyboard::Escape : window.close();
                    break;
                    case Keyboard::Num1 :
                        mode = Display;
                        if( picked != NULL ){
                            picked -> unpick();
                            picked = NULL;
                        }
                        avaiablePoints = currentPlayer -> getHexCount();
                        if( currentPlayer -> isFirst() )
                            avaiablePoints = 10;
                    break;
                    case Keyboard::Num2 :
                        mode = Orders;
                        if( picked != NULL ){
                            picked -> unpick();
                            picked = NULL;
                        }
                    break;
                    case Keyboard::Num3 :
                        mode = Regroup;
                        if( picked != NULL ){
                            picked -> unpick();
                            picked = NULL;
                        }
                    break;
                    case Keyboard::R :
                        map -> randomize();
                    break;
                    case Keyboard::Return:
                        if( mode != 2 )
                            mode = GameMode((int)mode + 1);
                        else {
                                mode = (GameMode) 0;
                                if( currentPlayer -> getID() != playerCount - 1 )
                                    currentPlayer++;
                                else currentPlayer = players;
                        }
                        if( currentPlayer -> isFirst() )
                            avaiablePoints = 10;
                        else avaiablePoints = currentPlayer -> getHexCount();
                        if( picked != NULL ){
                            picked -> unpick();
                            picked = NULL;
                        }
                    break;
                    default:
                    break;
                }
            }
            if(event.type == Event::MouseButtonPressed && (event.mouseButton.button == Mouse::Left || event.mouseButton.button == Mouse::Right)){
                Vector2i coords=map->findHex(Mouse::getPosition(window));
                if(!(coords==Vector2i(INT_MAX, INT_MAX))){
                    Hex& current = map->getHex(coords);

                if( mode == Display ){
                    if( event.mouseButton.button == Mouse::Left ){
                        if( current.getColor() == currentPlayer -> getColor() && avaiablePoints > 0 ){
                                    current.increaseStrength();
                                    avaiablePoints--;
                        }
                        else if( currentPlayer -> isFirst() && current.isEnabled() && current.getColor() == Color::White ) {
                                    currentPlayer -> increaseHexCount();
                                    current.setColor( currentPlayer -> getColor() );
                                    current.setStrength( 1 );
                                    currentPlayer -> setFirst( false );
                                    avaiablePoints--;
                            }
                        }
                    if( avaiablePoints == 0 )
                        mode = Orders;
                }
                else if ( mode == Orders && current.isEnabled() ){
                    if( event.mouseButton.button == Mouse::Left && current.getColor() == currentPlayer -> getColor() ){
                        if(picked!=NULL) picked -> unpick();
                        current.pick();
                        picked = &current;
                    }
                    else if( ( event.mouseButton.button == Mouse::Left &&  current.isPicked() ) || event.mouseButton.button == Mouse::Right ){
                        if( picked!=NULL ) picked -> unpick();
                        picked = NULL;
                    }
                    else if( event.mouseButton.button == Mouse::Left && picked!=NULL && currentPlayer -> getPickedColor() == picked -> getColor() && picked -> isNeighbor( current ) ){
                        if ( picked -> getStrength() > 1 ){
                            while( picked -> getStrength() > 1 && current.getStrength() > 0 ){
                                int attackerLoses = 0;
                                for( int i = 0; i < picked -> getStrength() - 1 && current.getStrength() > 0; i++){
                                        if( picked -> getStrength() - attackerLoses > 1 ){
                                            if( rand() % 2 )
                                                current.decreaseStrength();
                                            else
                                                attackerLoses++;
                                        }
                                    }
                                picked -> setStrength( picked -> getStrength() - attackerLoses );
                            }
                            if( picked -> getStrength() > 1){
                                if( current.getColor() != Color::White )
                                    for( unsigned int i = 0; i < playerCount; i++ )
                                    if( players[i].getColor() == current.getColor() ){
                                        players[i].decreaseHexCount();
                                        break;
                                    }
                                currentPlayer -> increaseHexCount();
                                current.setStrength( picked -> getStrength() - 1 );
                                picked -> setStrength( 1 );
                                current.setColor( picked -> getColor() );
                                picked -> unpick();
                                current.pick();
                                picked = &current;
                            }
                        }
                        else{
                         picked -> unpick();
                         picked = NULL;
                        }
                    }
                }
                else if( mode == Regroup ){
                    if( event.mouseButton.button == Mouse::Left && current.getColor() == currentPlayer -> getColor() ){
                        if( picked!=NULL && picked -> getStrength() > 1 ){
                            current.setStrength( picked -> getStrength() );
                            picked -> setStrength( 1 );
                            picked -> unpick();
                            picked = NULL;
                        }
                        else{
                            current.pick();
                            picked = &current;
                        }
                    }
                    else if( ( event.mouseButton.button == Mouse::Left &&  current.isPicked() ) || event.mouseButton.button == Mouse::Right ){
                        if( picked!=NULL ) picked -> unpick();
                        picked = NULL;
                    }
                }
            }
        }
        }

        window.clear(Color::Black);

        display_text.setString("Player " + intToString( currentPlayer -> getID() + 1 ));
        display_text.setColor( currentPlayer -> getColor() );
        display_text.setPosition( 0, 0 );
        window.draw( display_text );

        if( mode == Display ){
            display_text.setString("Pozostalo do rozstawienia " + intToString( avaiablePoints ));
            display_text.setColor( Color::White );
            display_text.setPosition( 150, 0 );
            window.draw( display_text );
        }
        else {
            display_text.setString("Nacisnij ENTER aby przejsc dalej");
            display_text.setColor( Color::White );
            display_text.setPosition( 150, 0 );
            window.draw( display_text );
        }

        window.draw(( *map ));
        window.display();
    }
return 0;
}
