
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
    otra es hacerlo con eoc
    */
    /* // Crear el archivo de prueba
    FILE* f = fopen("archivo.txt", "w");
    fwrite("Este archivo es\nMUY-CHICO. Por eso, nadie lo quiere\ncompactar_mas. Ok?", sizeof(char), 71, f);
    fclose(f);

    // Leer con alsoAllowed = "-_"
    f = fopen("archivo.txt", "r+b");
    string w;
    bool hayMas = readWord(f, w, "-_");
    while(hayMas)
    {
        cout << "[" << w << "]" << endl;
        hayMas = readWord(f, w, "-_");
    }
    cout << "[" << w << "]" << endl; // ultima palabra, cuando hayMas ya es false
    fclose(f); */
    
    FILE* f = fopen("archivo.txt", "w+b");
    writeLine(f, "Esta es");
    writeLine(f, "una cadena");
    writeLine(f, "GENIAL!");
    fclose(f);

    f = fopen("archivo.txt", "r");
    string ln;
    bool hayMas = readLine(f, ln);
    while(hayMas){
        cout << "[" << ln << "]" << endl;
        hayMas = readLine(f, ln);
    }
    cout << "[" << ln << "]" << endl;
    fclose(f);

    return 0;
}
