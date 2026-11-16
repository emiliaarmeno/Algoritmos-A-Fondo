
#include <iostream>
#include <cassert>

#include "biblioteca/funciones/files.hpp"
#include "biblioteca/funciones/strings.hpp"
#include "biblioteca/tads/parte1/Fecha.hpp"
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

struct Camion
{
   int idCam;
   char conductor[20];
   int volCargaMax;
   int prioridad; // 0=>max
};
struct RCamion
{
    Camion c; 
    int volAclanzado; 
};
struct Paquete
{
   int idPaq;
   char dirDest[20];
   int cpDest; // cod. post
   int alto;
   int ancho;
   int prof;
};

string cod2dest(int cp){
    return "";
}

int cmpCamPriori(RCamion a, RCamion b){
    return a.c.prioridad-b.c.prioridad;  
}

RCamion rCamion(Camion cam){
    RCamion rc;
    rc.c=cam;
    rc.volAclanzado=0;
    return rc; 
}

//ordenados por prioridad
List<RCamion> camionesSubir(FILE* f){
    List<RCamion> ret = list<RCamion>();

    unsigned char cantCamiones = read<unsigned char>(f);

    for(int i=0; i<cantCamiones; i++){
        Camion c = read<Camion>(f);
        RCamion rc = rCamion(c);
        listOrderedInsert<RCamion>(ret, rc, cmpCamPriori);
    }

    return ret; 
}

int calcVolumen(Paquete p){
    return p.alto*p.ancho*p.prof;
}


Map<int, List<RCamion>> paquetesDistribuir(FILE* f, List<RCamion>& camCerrados){
    Map<int, List<RCamion>> camAsig = map<int, List<RCamion>>();
    
    Paquete p = read<Paquete>(f);
    while(!feof(f)){
        //agarro la lista de camiones abiertos que van a la zona del paquete
        List<RCamion>* camAbierCP = mapGet<int, List<RCamion>>(camAsig, p.cpDest);
        
        //si no existe ningun camion que vaya a ese codigo postal 
        if(camAbierCP == NULL){
            //lo creo sacando de los camiones cerrados, agregando el parquete porque al ser el primero el volumen va a alcanzar 
            List<RCamion> nuevo = list<RCamion>();
            RCamion rcam = listRemoveFirst<RCamion>(camCerrados); //abro nuevo camion
            rcam.volAclanzado=calcVolumen(p);
            listAdd<RCamion>(nuevo, rcam); //como la lista esta vacia no hace falta agregar ordenado
            mapPut<int, List<RCamion>>(camAsig, p.cpDest, nuevo);
        }else{
            //busco un camion que tenga lugar para este paquete
            bool asignado = false; 
            listReset<RCamion>(*camAbierCP);
            while(listHasNext<RCamion>(*camAbierCP) && !asignado){
                RCamion* rc = listNext<RCamion>(*camAbierCP);
                //si no pasa el max vol al añadirlo lo meto
                if( ( rc->volAclanzado + calcVolumen(p) ) <= rc->c.volCargaMax){
                    rc->volAclanzado+=calcVolumen(p);
                    asignado=true; //para que salga del while cuando lo meta en algun camion
                }
            }

            //ahora salio porque lo pudo asignar o porque se termino la lista de camiones abiertos sin poder meterlo 
            if(!asignado){//si fue porque se termino la lista 
                RCamion nuevoAbierto = listRemoveFirst<RCamion>(camCerrados); //saco de la lista 
                nuevoAbierto.volAclanzado+=calcVolumen(p); //meto el paquete en el camion nuevo que va a esta misma direccion
                listOrderedInsert<RCamion>(*camAbierCP, nuevoAbierto, cmpCamPriori); //lo meto en el map porque al ser puntero se va a editar 
            }
        }

        p = read<Paquete>(f);
    }

    return camAsig;
}

int porcentajeVol(int total, int alcanzado){
    return (alcanzado*100)/total;
}

void mostrarPunto1(Map<int, List<RCamion>> asignados){
    mapReset<int, List<RCamion>>(asignados);
    while(mapHasNext<int, List<RCamion>>(asignados)){
        int codPostal = mapNextKey<int, List<RCamion>>(asignados);
        cout << "Código postal: " << codPostal << "(" << cod2dest(codPostal) <<")"<<endl;
        List<RCamion>* camiones = mapGet<int, List<RCamion>>(asignados, codPostal);
        cout << "Camión (chofer)          Vol. max. (m3)         %"<<endl;
        listReset<RCamion>(*camiones);
        while(listHasNext<RCamion>(*camiones)){
            RCamion* rc = listNext<RCamion>(*camiones);
            cout << rc->c.idCam << " (" << rc->c.conductor << ")" << rc->c.volCargaMax << porcentajeVol(rc->c.volCargaMax, rc->volAclanzado) <<endl;
        }
    }
}

void mostrarPunto2(List<RCamion> lCam){
    cout << "Camiones sin asignacion:  Camión (chofer)" << endl;
    listReset<RCamion>(lCam);
    while(listHasNext<RCamion>(lCam)){
        RCamion* rc = listNext<RCamion>(lCam);
        cout << rc->c.idCam << " (" << rc->c.conductor << ")" <<endl; 
    }
}

int main()
{
    FILE* f = fopen("CAMIONESPAQUETES.dat", "r+b");

    List<RCamion> lCam = camionesSubir(f);
    Map<int, List<RCamion>> camAsig = paquetesDistribuir(f, lCam);
    mostrarPunto1(camAsig);
    mostrarPunto2(lCam);
    

    fclose(f);
    return 0;
}
