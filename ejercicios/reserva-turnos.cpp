
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

struct Reserva //novedades 
{
   int idRes;
   int idPac;
   int idPra;
   Fecha fPra; // TAD
   Fecha hPra;  // TAD
};
struct Practica //consulta
{
   int idPra;
   char desc[100];
   int nTurnos; // cant. requerida para hacerla (n*20 min)
   int nProf;   // cant. prof.
};
struct RPractica
{
    Practica p; 
    //dias del mes, horas del dia (0 libre, 1 tomado)
    Map<int, bool[37]> horarios; //entre las 8 y las 20 hay 37 turnos de 20min
};



int main()
{
    //bajo a memoria las practicas 
    //recorro el archivo de reservas 
    //por cada una compruebo que: existe la practica y tiene profesionales, 
    return 0;
}
