
#include <iostream>
//#include "biblioteca/funciones/millis.hpp"
#include "../biblioteca/funciones/files.hpp"
#include "../biblioteca/funciones/strings.hpp"
#include "biblioteca/funciones/tokens.hpp"
#include "biblioteca/tads/parte1/Fecha.hpp"
//#include "biblioteca/tads/parte1/Timer.hpp"
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

struct Caract
{
   int idCaract; 
   char descr[100];
};
struct Casa
{
   int idCasa;
   char direcc[50];
   int idDueno;
   int caractMask;
   int idUsr;
};

string casaToString(Casa c);
Casa casaFromString(string s);

struct RCasa
{
    Casa c;
    double concordancia; //el porcentaje de concordancia con la mask de la busqueda
};

string rCasaToString(RCasa c);
RCasa rCasaFromString(string s);

struct Busqueda
{
   int idUsr;
   int caractMask;
   int dias;
   double tolerancia;
};

/*
    CARACTERISTICAS.dat no se usa 
    CASAS.dat consultas, solo disponible si el idUsr esta en cero 
    cada busqueda es una novedad

    Por cada búsqueda, emitir un listado (ordenado decrecientemente por el porcentaje de concordancia) 
    de todas las casas disponibles cuya concordancia está por encima del valor tolerancia del registro de la búsqueda. 
    Por cada casa, se debe invocar a la función mostrarCasa, 
    que mostrará en la página Web las fotos, detalles y demás datos que resultarán de interés para el usuario.
*/

double concordancia(int idCasa,int mask);

// Retorna un registro con los parametros de busqueda indicados por el usuario
Busqueda leerBusqueda();  

// Indica si quedan mas busquedas por procesar
bool continuarOperando();

// Muestra por pantalla la casa especificada
void  mostrarCasa(int idCasa);

Coll<RCasa> casaSubir(){
    Coll<RCasa> cCas = coll<RCasa>();
    FILE* f = fopen("CASAS.dat", "r+b");
    Casa c = read<Casa>(f);
    while(!feof(f)){
        RCasa rc = {c, 0}; //seteo la concordancia en cero por ahora 
        collAdd<RCasa>(cCas, rc, rCasaToString);
        c = read<Casa>(f);
    }
    fclose(f);
    return cCas;
}

void casaAsigConcordancia(Busqueda bu, Coll<RCasa>& cCasas){
    //recorro la coleccion de casas y le agrego el campo concordancia segun la busqueda que se esta procesando
    int i=0;
    collReset<RCasa>(cCasas);
    while(collHasNext<RCasa>(cCasas)){
        RCasa rc = collNext<RCasa>(cCasas, rCasaFromString);
        double conc = concordancia(rc.c.idCasa, bu.caractMask);
        rc.concordancia=conc;
        collSetAt<RCasa>(cCasas, rc, i, rCasaToString);
        i++;
    }
}

int cmpConcordanciaDesc(RCasa a, RCasa b){
    // decreciente: la de MAYOR concordancia va primero
    if(a.concordancia > b.concordancia) return -1;
    if(a.concordancia < b.concordancia) return 1;
    return 0;
}

void casaMostrarConcordantes(Busqueda bu, Coll<RCasa> cCasas){
    //ordeno la coleccion para que se muestre en orden descendiente
    collSort<RCasa>(cCasas, cmpConcordanciaDesc, rCasaFromString, rCasaToString);

    collReset<RCasa>(cCasas);
    while(collHasNext<RCasa>(cCasas)){
        RCasa rc = collNext<RCasa>(cCasas, rCasaFromString);
        if(rc.c.idUsr==0 && rc.concordancia>bu.tolerancia){ //si la casa esta disponible y cumple tolerancia de busqueda 
            mostrarCasa(rc.c.idCasa);
        }
    }
}

int main()
{
    Coll<RCasa> cCasas = casaSubir();
    Busqueda bu; 
    
    while(continuarOperando()){
        bu = leerBusqueda(); //agarro la busqueda del usuario 
        casaAsigConcordancia(bu, cCasas); //asigna la concordancia al campo concordancia de todas segun la busqueda 
        casaMostrarConcordantes(bu, cCasas); //muestra segun toleracia de busqueda
    }

    return 0;
}
