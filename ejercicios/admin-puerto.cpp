
#include <iostream>
// #include "biblioteca/funciones/millis.hpp"
#include "../biblioteca/funciones/files.hpp"
#include "../biblioteca/funciones/strings.hpp"
#include "biblioteca/funciones/tokens.hpp"
#include "biblioteca/tads/parte1/Fecha.hpp"
// #include "biblioteca/tads/parte1/Timer.hpp"
#include "biblioteca/tads/parte1/Coll.hpp"
#include "biblioteca/tads/parte2/Array.hpp"
#include "biblioteca/tads/parte2/List.hpp"
#include "biblioteca/tads/parte2/Map.hpp"
#include "biblioteca/tads/parte2/Queue.hpp"
#include "biblioteca/tads/parte2/Stack.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;


struct Buque
{
   int idBuque;
   char nombre[50];
   int darsena;
   int grua; 
   int cantContainers;
   Fecha fecha; // llegada o salida
   int cteFlotacion;
   char oriODest[100];
};
struct RBuque{
    Buque b;
    Map<int, Container> mCont; //
};
struct Container
{
   int idContainer;
   int peso;
   int longitud; 
   int idBuque;
};

bool estable(int peso,int longitud,int cteFlotacion);

int main()
{
    cout<<"Hola Mundo!"<<endl;
    return 0;
}
