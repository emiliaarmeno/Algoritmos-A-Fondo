
#include <iostream>
#include "biblioteca/funciones/millis.hpp"
#include "biblioteca/funciones/files.hpp"
#include "biblioteca/funciones/strings.hpp"
#include "biblioteca/funciones/tokens.hpp"
#include "biblioteca/tads/parte1/Fecha.hpp"
#include "biblioteca/tads/parte1/Coll.hpp"
#include "biblioteca/tads/parte2/Array.hpp"
#include "biblioteca/tads/parte2/List.hpp"
#include "biblioteca/tads/parte2/Map.hpp"
#include "biblioteca/tads/parte2/Queue.hpp"
#include "biblioteca/tads/parte2/Stack.hpp"
#include "principal.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;

int main()
{
    /* 
    ejemplo de iterar una coleccion con las funciones
    collReset(c);
    while(collHasNext(c)){
        int x = collNext(c, stringToInt);
        cout << x << endl;
    } 
    */

    Coll<int> c = coll<int>();
    collAdd(c, 10, intToString);
    collAdd(c, 20, intToString);
    collAdd(c, 30, intToString);

    collReset(c);
    //recorro usando la sobrecarga de collNext
    bool eoc = false;
    while(!eoc){
        int x = collNext(c, eoc, stringToInt);
        cout << x << endl;
    }
    // esperado: 10, 20, 30

    return 0;
}
