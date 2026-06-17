
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

#include "torneo de futbol tools.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;


Coll<REquipo> equiposSubir(){
    Coll<REquipo> ret = coll<REquipo>();

    FILE* f = fopen("EQUIPOS.dat", "r+b");
    Equipo e = read<Equipo>(f);
    while( !feof(f) )
    {
        REquipo x = rEquipo(e, 0);        
        collAdd<REquipo>(ret,x,rEquipoToString);

        e = read<Equipo>(f);
    }

    fclose(f);
    return ret;
}

int cmpREquipoId(REquipo re, int id){
    return re.e.idEq-id;
}

void punto1Procesar(Resultado r,Coll<REquipo>& cEq)
{
    //busco los requipos
    int id1 = r.idEq1;
    int pos1 = collFind<REquipo,int>(cEq, id1, cmpREquipoId, rEquipoFromString);
    REquipo re1 = collGetAt<REquipo>(cEq, pos1, rEquipoFromString);
    int id2 = r.idEq2;
    int pos2 = collFind<REquipo,int>(cEq, id2, cmpREquipoId, rEquipoFromString);
    REquipo re2 = collGetAt<REquipo>(cEq, pos2, rEquipoFromString);

    // calculo cuantos puntos le tocan a cada equipo
    if(r.codRes==0){ //empate 
        re1.acum+=1;
        re2.acum+=1;
    }else if(r.codRes<0){ //gana equipo 1 
        re1.acum+=3;
    }else{ //gana equipo 2 
        re2.acum+=3;
    }

    //los seteo en la coleccion porque solo modifique las copias
    collSetAt<REquipo>(cEq, re1, pos1, rEquipoToString);
    collSetAt<REquipo>(cEq, re2, pos2, rEquipoToString);
}

int cmpREstadioNom(REstadio est, string nom){
    return cmpString(est.est, nom);
}

int descubrirEstadio(Coll<REstadio> cEs, string nombre){
    //lo busco en la coleccion 
     int pos = collFind<REstadio, string>(cEs, nombre, cmpREstadioNom, rEstadioFromString);
     //si no estaba en la coleccion lo agrego
     if(pos<0){
        REstadio est = rEstadio(nombre, 0, 0);
        pos = collAdd<REstadio>(cEs, est, rEstadioToString);
     }
     return pos; 
}

void punto2Procesar(Resultado r,Coll<REstadio>& cEs)
{
    // descubro el estadio
    string es = r.estadio; //porque es un char[]
    int pos = descubrirEstadio(cEs, es);
    REstadio rEst = collGetAt<REstadio>(cEs, pos, rEstadioFromString);
    // sumo 1 al cpj
    rEst.cpj+=1;
    // si corresponde sumo 1 al cpe
    if(r.codRes==0){ //solo sumo si empataron 
        rEst.cpe+=1;
    }
    collSetAt<REstadio>(cEs, rEst, pos, rEquipoToString); 
}

int cmpREqPuntos(REquipo re1, REquipo re2){
    int ptos1 = re1.e.puntos+re1.acum;
    int ptos2 = re2.e.puntos+re2.acum;
    return ptos2-ptos1; //orden decreciente 
}

void punto1Mostrar(Coll<REquipo> cEq)
{
    // ordenar cEq descr. por puntos+acum
    collSort(cEq, cmpREqPuntos, rEquipoFromString, rEquipoToString);
    // recorrer y mostrar
    for(int i=0; i<collSize(cEq); i++){
        REquipo re = collGetAt(cEq, i, rEquipoFromString);
        int rePtos = re.acum+re.e.puntos;
        cout<<re.e.nombre<<": "<< rePtos <<endl;
    }
}
void punto2Mostrar(Coll<REstadio> cEs)
{
    // recorrer y mostrar cpj y cpe
    for(int i=0; i<collSize(cEs); i++){
        REstadio rest = collGetAt(cEs, i, rEstadioFromString);
        cout<<"Cantidad de partidos en " << rest.est << " jugados: " << rest.cpj << " empatados: " << rest.cpe <<endl;
    }
}

/* 
    RESULTADOS.dat archivo de novedades 
    EQUIPOS.dat archivo de consulta

    subir a memoria en una coleccion equipos wrappeada con un campo nuevo acumulador (un acumulador por equipo)
    voy a tener que recorrer el archivo de resultados, por cada uno ver el resultado y sumar los puntos correspondientes al acumulador
    busco por cada resultado el equ1 y el eq2 y el cod del resultado 

    1. informar la tabla de posiciones (listado de los equipos ordenado de mayor a menor por los puntos)
    2. para cada estadio cuantos (contadores) los partidos jugados y los que fueron empates (vamos a hacer la estructura estadio aunque no wrappee a nadie)
    tengo que hacer descubrimiento para tener la coleccion de estadios, leo resultado veo estadio si no esta en la coleccion lo agrego y sea como sea sumo acumuladores
    
    */

int main()
{
    // colecciones a memoria
    Coll<REquipo> cEq = equiposSubir();
    Coll<REstadio> cEs = coll<REstadio>();

    // barro archivo de movimientos
    FILE* f = fopen("RESULTADOS.dat","r+b");
    Resultado r = read<Resultado>(f);
    while( !feof(f) )
    {
        punto1Procesar(r,cEq);
        punto2Procesar(r,cEs);

        r = read<Resultado>(f);
    }

    punto1Mostrar(cEq);
    punto2Mostrar(cEs);

    fclose(f);
    return 0;
}
