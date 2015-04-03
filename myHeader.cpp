#include "myHeader.h"
#include <iostream>

std::string intToString(int a){
    std::string converted="", returned="";
    if (a<0){
        returned="-";
        a=-a;
    }
    do{
        converted += a%10 + 48;
        a -= a%10;
    }while(a/=10);
    for(int i=converted.size()-1; i>=0; i--)
        returned+=converted[i];
    return returned;
}
