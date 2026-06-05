
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
    Coll<int> c = coll<int>();
    collAdd(c, 10, intToString);
    collAdd(c, 20, intToString);
    collAdd(c, 30, intToString);

    // elemento que existe
    cout << collFind(c, 20, cmpInt, stringToInt) << endl; // esperado: 1

    // primer elemento
    cout << collFind(c, 10, cmpInt, stringToInt) << endl; // esperado: 0

    // ultimo elemento
    cout << collFind(c, 30, cmpInt, stringToInt) << endl; // esperado: 2

    // elemento que no existe
    cout << collFind(c, 99, cmpInt, stringToInt) << endl; // esperado: -1

    return 0;
}
