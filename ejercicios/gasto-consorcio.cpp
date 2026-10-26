
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

//GASTOS.dat (novedades) y CONSORCIOS.dat (consulta)
struct Gasto
{
   int idCons; // ver situaciones A y B
   int fecha;  // aaaammdd
   double importe;
   char categoria[20]; // ej. Limpieza, Sueldos, Servicios...
};
struct Consorcio
{
   int idCons;
   char direccion[50];
   int cantPisos;
   int dtosPorPiso;
};
struct RConsorcio{
    Consorcio c; 
    int impTotal; 
    List<Gasto> gastos;
};

double mtsPorcencuales(int idCons,int tipoDepto);



int main()
{
    cout<<"Hola Mundo!"<<endl;
    return 0;
}
