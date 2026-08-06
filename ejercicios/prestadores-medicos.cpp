
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

#include "prestadores-medicos-tools.txt"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;

// Archivos: PRESTA19.dat y PRESTA20.dat
struct Presta
{
   int idPres; 
   int idPrac; 
   long long fechaHora; // aaaammddhhmm
   int minutos;
};

struct RPresta
{
    Presta p; 
    int anioPrestacion; //esto es para catalogar si trabajaron en ambos años (0), solo en 2019 (-1) o solo 2020 (1)
};

// Archivo: PRACTICAS.dat
struct Practica
{
   int idPrac; 
   char descr[50];
};

string rPrestaToString(RPresta rp);
RPresta rPrestaFromString(string s);

/*
    1. Un listado, ordenado decrecientemente por idPres, de los prestadores que se incorporaron en 2020.
    2. Para los prestadores que sí trabajaron durante 2019 y 2020, un listado de las prácticas que realizaron; ordenado decrecientemente por fechaHora.
    3. Para los prestadores que sólo trabajaron durante 2019, un listado indicando la cantidad de prácticas realizadas
*/

Coll<RPresta> presta19Subir(){
    Coll<RPresta> c19 = coll<RPresta>();
    FILE* f = fopen("PRESTA19.dat", "r+b");
    Presta p = read<Presta>(f);
    while(!feof(f)){
        RPresta rp = {p, -1};
        collAdd<RPresta>(c19, rp, rPrestaToString);
        p = read<Presta>(f);
    }
    return c19;
}

Coll<RPresta> presta20Subir(){
    Coll<RPresta> c20 = coll<RPresta>();
    FILE* f = fopen("PRESTA20.dat", "r+b");
    Presta p = read<Presta>(f);
    while(!feof(f)){
        RPresta rp = {p, 1};
        collAdd<RPresta>(c20, rp, rPrestaToString);
        p = read<Presta>(f);
    }
    return c20;
}

//leemos ambas colecciones simultaneamente 
//si el idPres19<idPres20 --> anioPres=-1 (solo trabajo en 2019)
//si el idPres19=idPres20 --> anioPres=0 (trabajo en ambios)
//si el idPres19>idPres20 --> anioPres=1 (solo en 2020)

//queda en cada coleccion destacado cuales exclusivamente participaron en ese año 
//para c19 el anioPrestacion queda en -1 y para el otro en 1 

//esto solo funciona porque los datos de los archivos estan ordenados ascendentemente por idPres
void prestadoresAparear(Coll<RPresta>& c19, Coll<RPresta>& c20){
    bool eof19 = false;
    bool eof20 = false; 

    collReset<RPresta>(c19);
    collReset<RPresta>(c20);

    int i = 0; 
    int j = 0;
    while(i<collSize<RPresta>(c19) || j<collSize<RPresta>(c20)){
        RPresta rp19 = collGetAt<RPresta>(c19, i, rPrestaFromString);
        RPresta rp20 = collGetAt<RPresta>(c20, j, rPrestaFromString);
        
        if(rp19.p.idPres<rp20.p.idPres){
            while(rp19.p.idPres<rp20.p.idPres){
                rp19 = collGetAt<RPresta>(c19, i, rPrestaFromString);
                i++;
            }
        }else if(rp19.p.idPres>rp20.p.idPres){
             while(rp19.p.idPres<rp20.p.idPres){
                rp20 = collGetAt<RPresta>(c20, i, rPrestaFromString);
                j++;
            }
        } 

        if(rp19.p.idPres==rp20.p.idPres){
            rp19.anioPrestacion=0;
            rp20.anioPrestacion=0;
            collSetAt<RPresta>(c19, rp19, i, rPrestaToString);
            collSetAt<RPresta>(c20, rp20, j, rPrestaToString);
            i++;
            j++;
        }
        
        i++;
        //como esta inicializado el campo en cero no hace falta cambiarlo para cuando aparece en ambos
    }

    if(eof19){
        //si se termino el de 19 todos los que quedan en el de 20 trabajaron exclusivamente en ese año 
        while(!eof20){
            RPresta rp20 = collNext<RPresta>(c20, eof20, rPrestaFromString);
            rp20.anioPrestacion=1;
            collSetAt<RPresta>(c20, rp20, i, rPrestaToString);
            i++;
        }
    }

    if(eof20){
        while(!eof19){
            RPresta rp19 = collNext<RPresta>(c19, eof19, rPrestaFromString);
            rp19.anioPrestacion=-1;
            collSetAt<RPresta>(c19, rp19, i, rPrestaToString);
            i++;
        }
    }
}

int main()
{
    Coll<RPresta> c19 = presta19Subir();
    Coll<RPresta> c20 = presta20Subir();
    //compara los prestadores de cada año y cambia el anioPrestacion
    prestadoresAparear(c19, c20);
    
    return 0;
}
