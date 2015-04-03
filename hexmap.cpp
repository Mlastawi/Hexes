#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <string>
#include <algorithm>
#include <iostream>
#include "hex.h"
#include "hexmap.h"
#include <ctime>
#include <climits>

using namespace sf;

HexMap::HexMap(MapType type, Vector2i size, Vector2f coordinateCenter, int hexSize){
    this->type = type;
    this->size = size;
    this->coordinateCenter = coordinateCenter;
    this->hexSize = hexSize;
    Hex *temp;

    hexFont.loadFromFile("arial.ttf");

   if(type==HexMap::Odd_q || type==HexMap::Even_q){
        first = new int[size.x];
        for(int q=0;q<size.x;q++)
            if(type==HexMap::Odd_q) first[q]=-floor(q/2);
            else if(type==HexMap::Even_q) first[q]=-floor((q+1)/2);

        map = new Hex*[size.x];
        for(int q = 0; q < size.x; q++)
            map[q] = new Hex[size.y-first[size.x-1]];

        for(int q=0; q<size.x; q++) //q - kolumna
        for(int r=first[size.x-1]; r<size.y; r++){ //r - wiersz
            if(r-first[q]>=0 && r-first[q]<size.y){
                if(type==HexMap::Odd_q && q&1) temp = new Hex(Hex::Flat, coordinateCenter+Vector2f((hexSize*3/2)*q,hexSize*sqrt(3)/2+(hexSize*sqrt(3)/2)*(r-first[q])*2), hexSize, Vector2i(q,r));
                else if(type==HexMap::Even_q && (q&1)) temp = new Hex(Hex::Flat, coordinateCenter+Vector2f((hexSize*3/2)*q,-hexSize*sqrt(3)/2+(hexSize*sqrt(3)/2)*(r-first[q])*2), hexSize, Vector2i(q,r));
                else temp = new Hex(Hex::Flat, coordinateCenter+Vector2f((hexSize*3/2)*q,(hexSize*sqrt(3)/2)*(r-first[q])*2), hexSize, Vector2i(q,r));
                map[q][r-first[q]]=(*temp);
                map[q][r-first[q]].setFont(&hexFont);
            }
        }
   }
   else if(type==HexMap::Odd_r || type==HexMap::Even_r){
        first = new int[size.y];
        for(int r=0;r<size.y;r++)
            if(type==HexMap::Odd_r) first[r]=-floor(r/2);
            else if(type==HexMap::Even_r) first[r]=-floor((r+1)/2);

        map = new Hex*[size.y];
        for(int r = 0; r < size.y; r++)
            map[r] = new Hex[size.x-first[size.y-1]];

        for(int r=0; r<size.y; r++)
        for(int q=first[size.y-1]; q<size.x; q++){
            if(q-first[r]>=0 && q-first[r]<size.x){
                if(type==HexMap::Odd_r && r&1) temp = new Hex(Hex::Pointy, coordinateCenter+Vector2f(hexSize*sqrt(3)/2+(hexSize*sqrt(3)/2)*(q-first[r])*2, (hexSize*3/2)*r), hexSize, Vector2i(q,r));
                else if(type==HexMap::Even_r && r&1) temp = new Hex(Hex::Pointy, coordinateCenter+Vector2f(-hexSize*sqrt(3)/2+(hexSize*sqrt(3)/2)*(q-first[r])*2, (hexSize*3/2)*r), hexSize, Vector2i(q,r));
                else temp = new Hex(Hex::Pointy, coordinateCenter+Vector2f((hexSize*sqrt(3)/2)*(q-first[r])*2, (hexSize*3/2)*r), hexSize, Vector2i(q,r));
                map[r][q-first[r]]=(*temp);
                map[r][q-first[r]].setFont(&hexFont);
            }
        }
   }

    temp = new Hex(Hex::Pointy, Vector2f(0,0), 0 , Vector2i(INT_MAX,INT_MAX), Color::Black, Color::Black, false);
    blankHex = *temp;
    delete temp;
}

HexMap::HexMap(int radius, Vector2f coordinateCenter, int hexSize){
    this->type = HexMap::Hexagon;
    this->radius = radius;
    this->coordinateCenter = coordinateCenter;
    this->hexSize = hexSize;
    Hex *temp;

    hexFont.loadFromFile("arial.ttf");

    const int DIAMETER = 2*radius+1;

    first  = new int[DIAMETER];
    for(int r=(-radius); r<=radius; r++)
        first[r+radius] = -radius - std::min(0,r);

    map = new Hex*[DIAMETER];
    for(int q=0; q<DIAMETER; q++)
        map[q] = new Hex[DIAMETER];

    for(int q=(-radius); q<=radius; q++)
    for(int r=(-radius); r<=radius; r++){
        if((r+q>=(-radius)) && (r+q<=radius)){
            if(r&1) temp = new Hex(Hex::Pointy, coordinateCenter+Vector2f(hexSize*sqrt(3)/2+(hexSize*sqrt(3)/2)*(q-first[r+radius]-radius+abs(floor(r/2)))*2, (hexSize*3/2)*r), hexSize, Vector2i(q,r) );
            else temp = new Hex(Hex::Pointy, coordinateCenter+Vector2f((hexSize*sqrt(3)/2)*(q-first[r+radius]-radius+abs(floor(r/2)))*2, (hexSize*3/2)*r), hexSize, Vector2i(q,r));
            map[r+radius][q-first[r+radius]] = (*temp);
            map[r+radius][q-first[r+radius]].setFont(&hexFont);
        }
    }
}

void HexMap::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    states.transform *= getTransform();
    states.texture = NULL;

    Hex current;
    if(type==HexMap::Even_q||type==HexMap::Odd_q){
        for(int q=0; q<size.x; q++)
            for(int r=first[size.x-1]; r<size.y; r++){
                    if(r-first[q]>=0 && r-first[q]<size.y){
                        current = map[q][r-first[q]];
                        if(current.isEnabled())
                            target.draw( current,states );
                    }
            }
    }
    else if(type==HexMap::Even_r||type==HexMap::Odd_r){
        for(int r=0; r<size.y; r++)
            for(int q=first[size.y-1]; q<size.x; q++){
                    if(q-first[r]>=0 && q-first[r]<size.x){
                        current = map[r][q-first[r]];
                        if(current.isEnabled())
                            target.draw( current,states );
                    }
            }
    }
    else if(type==HexMap::Hexagon){
        for(int q=(-radius); q<=radius; q++)
        for(int r=(-radius); r<=radius; r++){
            if((r+q>=(-radius)) && (r+q<=radius)){
                current = map[r+radius][q-first[r+radius]];
                if(current.isEnabled())
                    target.draw( current,states );
            }
        }
    }
}


void HexMap::setHex(int q, int r, Hex hex){
    if(type==HexMap::Even_q||type==HexMap::Odd_q)
        map[q][r-first[q]]=hex;
    else if(type==HexMap::Even_r||type==HexMap::Odd_r)
        map[r][q-first[r]]=hex;
    else if(type==HexMap::Hexagon)
        map[r+radius][q-first[r+radius]]=hex;
}

void HexMap::setHex(Vector2i coords, Hex hex){
    if(type==HexMap::Even_q||type==HexMap::Odd_q)
        map[coords.x][coords.y-first[coords.x]]=hex;
    else if(type==HexMap::Even_r||type==HexMap::Odd_r)
        map[coords.y][coords.x-first[coords.y]]=hex;
    else if(type==HexMap::Hexagon)
        map[coords.y+radius][coords.x-first[coords.y+radius]]=hex;
}

Hex& HexMap::getHex(int q, int r){
    if(type==HexMap::Even_q||type==HexMap::Odd_q){
        if(r-first[q]>=0 && r-first[q]<size.y && q<size.x)
            return map[q][r-first[q]];
        else return blankHex;}
    else if(type==HexMap::Even_r||type==HexMap::Odd_r){
        if(q-first[r]>=0 && q-first[r]<size.x && r<size.y)
            return map[r][q-first[r]];
        else return blankHex;}
    else if(type==HexMap::Hexagon)
        return map[r+radius][q-first[r+radius]];
}

Hex& HexMap::getHex(Vector2i coords){
    if(type==HexMap::Even_q||type==HexMap::Odd_q){
        if(coords.y-first[coords.x]>=0 && coords.y-first[coords.x]<size.y && coords.x<size.x)
            return map[coords.x][coords.y-first[coords.x]];
        else return blankHex;}
    else if(type==HexMap::Even_r||type==HexMap::Odd_r){
        if(coords.x-first[coords.y]>=0 && coords.x-first[coords.y]<size.x && coords.y<size.y)
            return map[coords.y][coords.x-first[coords.y]];}
    else if(type==HexMap::Hexagon){
        if (coords.x+coords.y>=-radius && coords.x+coords.y<=radius && std::max(abs(coords.x),abs(coords.y))<= radius)
            return map[coords.y+radius][coords.x-first[coords.y+radius]];}
}

Vector2i HexMap::findHex(Vector2i position){
    float q,r;
    Vector2f coords;
    Vector2i return_coords;
    if(type==HexMap::Odd_q || type==HexMap::Even_q){
        q=(position.x-coordinateCenter.x) * 2/3 / hexSize;
        r=(-(position.x-coordinateCenter.x) / 3 + sqrt(3)/3 * (position.y-coordinateCenter.y)) / hexSize;
        coords=Vector2f(q,r);
        return_coords = roundHex(coords);

        if(!(return_coords.y-first[return_coords.x]>=0 && return_coords.y-first[return_coords.x]<size.y))
        return_coords=Vector2i(INT_MAX,INT_MAX);
    }
    else if(type==HexMap::Odd_r || type==HexMap::Even_r){
        q = ((position.x-coordinateCenter.x) * sqrt(3)/3 - (position.y-coordinateCenter.y) / 3) / hexSize;
        r = (position.y-coordinateCenter.y) * 2/3 / hexSize;
        coords=Vector2f(q,r);
        return_coords = roundHex(coords);

        if(!(return_coords.x-first[return_coords.y]>=0 && return_coords.x-first[return_coords.y]<size.x))
            return_coords=Vector2i(INT_MAX,INT_MAX);
    }
    else if(type==HexMap::Hexagon){
        q = ((position.x-coordinateCenter.x) * sqrt(3)/3 - (position.y-coordinateCenter.y) / 3) / hexSize;
        r = (position.y-coordinateCenter.y) * 2/3 / hexSize;
        coords=Vector2f(q,r);
        return_coords = roundHex(coords);

        if(!(return_coords.x+return_coords.y>=-radius && return_coords.x+return_coords.y<=radius && std::max(abs(return_coords.x),abs(return_coords.y))<= radius))
            return_coords=Vector2i(INT_MAX,INT_MAX);
    }
    return return_coords;
}

Vector2i HexMap::roundHex(Vector2f float_coords){
    Vector2i int_coords;
    float y = -float_coords.x-float_coords.y;
    int rx = roundf(float_coords.x);
    int ry = roundf(y);
    int rz = roundf(float_coords.y);

    float dx = fabs((float)rx - float_coords.x);
    float dy = fabs((float)ry - y);
    float dz = fabs((float)rz - float_coords.y);

    if(dx>dy && dx>dz)
        rx=-ry-rz;
    else if(dy>dz)
        ry=-rx-rz;
    else
        rz=-rx-ry;

    int_coords=Vector2i(rx,rz);

    return int_coords;
}

void HexMap::randomize(){
    srand(time(NULL));

    if(type==HexMap::Even_q||type==HexMap::Odd_q){
        for(int q=0; q<size.x; q++)
            for(int r=first[size.x-1]; r<size.y; r++){
                    if(r-first[q]>=0 && r-first[q]<size.y){
                        if(rand()%100<30) map[q][r-first[q]].disable();
                        else map[q][r-first[q]].enable();
                    }
            }
    }
    else if(type==HexMap::Even_r||type==HexMap::Odd_r){
        for(int r=0; r<size.y; r++)
            for(int q=first[size.y-1]; q<size.x; q++){
                    if(q-first[r]>=0 && q-first[r]<size.x){
                        if(rand()%100<30) map[r][q-first[r]].disable();
                        else map[r][q-first[r]].enable();
                    }
            }
    }
    else if(type==HexMap::Hexagon){
        for(int q=(-radius); q<=radius; q++)
        for(int r=(-radius); r<=radius; r++){
            if((r+q>=(-radius)) && (r+q<=radius)){
                if(rand()%100<30) map[r+radius][q-first[r+radius]].disable();
                else map[r+radius][q-first[r+radius]].enable();
            }
        }
    }
}

void HexMap::unpickAllHexes(){
    if(type==HexMap::Even_q||type==HexMap::Odd_q){
        for(int q=0; q<size.x; q++)
            for(int r=first[size.x-1]; r<size.y; r++){
                map[q][r-first[q]].unpick();
            }
    }
    else if(type==HexMap::Even_r||type==HexMap::Odd_r){
        for(int r=0; r<size.y; r++)
            for(int q=first[size.y-1]; q<size.x; q++){
                map[r][q-first[r]].unpick();
            }
    }
    else if(type==HexMap::Hexagon){
        for(int q=(-radius); q<=radius; q++)
        for(int r=(-radius); r<=radius; r++){
                map[r+radius][q-first[r+radius]].unpick();
        }
    }
}
