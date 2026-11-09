
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


struct Camion
{
   int idCam;
   char conductor[20];
   int volCargaMax;
   int prioridad; // 0=>max
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
struct Asignacion
{
    Camion c; 
    int volAlcanzado; //sumo el volumen de todos los paquetes asignados a este camion 
};

//retorna el nombre de la zona geográfica representada por un código postal
string cod2dest(int cp);

int cmpCamXPriori(Camion a, Camion b){ return a.prioridad - b.prioridad; }
int cmpPaqXCod(Paquete a, Paquete b){ return a.cpDest - b.cpDest; }

//sube camiones (por prioridad) y paquetes (por codigo)
void subirArchivos(List<Camion>& camiones, List<Paquete>& paquetes){
    FILE* f = fopen("DATOS.dat", "r+b");
    unsigned char cantCamiones = read<unsigned char>(f);
    for(int i=0; i<cantCamiones; i++){
        Camion c = read<Camion>(f);
        listOrderedInsert<Camion>(camiones, c, cmpCamXPriori);
    }
    while(!feof(f)){
        Paquete p = read<Paquete>(f);
        listOrderedInsert<Paquete>(paquetes, p, cmpPaqXCod);
    }
    fclose(f);
}

void zonaMostrar(int cp, List<Asignacion> asigs){
    cout << "Código postal: " << cp << " (" << cod2dest(cp) << ")" << endl;
    listReset<Asignacion>(asigs);
    while( listHasNext<Asignacion>(asigs) )
    {
        Asignacion a = *listNext<Asignacion>(asigs);
        cout << a.c.idCam << " (" << a.c.conductor << ")\t"
             << a.c.volCargaMax << "\t" << a.volAlcanzado*100/a.c.volCargaMax << "%" << endl;
    }
}

int volumen(Paquete* p){ return p->alto * p->ancho * p->prof; }

Asignacion* buscarCamionConLugar(List<Asignacion> asigs, int vol){
    listReset<Asignacion>(asigs);
    while( listHasNext<Asignacion>(asigs) )
    {
        Asignacion* a = listNext<Asignacion>(asigs);
        if( a->volAlcanzado + vol <= a->c.volCargaMax ) return a;
    }
    return NULL;
}

void distribuir(List<Camion> libres, List<Paquete> paquetes){

    List<Asignacion> asigs = list<Asignacion>();
    int codActual = -1; 

    listReset<Paquete>(paquetes);
    while(listHasNext<Paquete>(paquetes)){
        Paquete* p = listNext<Paquete>(paquetes);

        //cambio de zona?? (porque paquetes esta ordenado por zona)
        if(codActual != p->cpDest){
            //si no es la primera vuelta
            if(codActual != -1){
                //cierro el grupo anterior
                zonaMostrar(codActual, asigs); 
                listFree<Asignacion>(asigs);
            }

            codActual=p->cpDest;
        }

        //acumulo p en el grupo actual
        int volPaq = volumen(p);
        Asignacion* a = buscarCamionConLugar(asigs, volPaq); 
        if( a==NULL && !listIsEmpty<Camion>(libres) ) // abro el próximo camión libre
        {
            Asignacion nueva;
            nueva.c = listRemoveFirst<Camion>(libres);
            nueva.volAlcanzado = 0;
            a = listAdd<Asignacion>(asigs, nueva);
        }
        if( a!=NULL ) a->volAlcanzado += volPaq;
    }

    // cierro el último
    if( codActual != -1 )                              
    {
        zonaMostrar(codActual, asigs);
        listFree<Asignacion>(asigs);
    }

}

int main()
{
    //se pide: Distribuir los paquetes, según su código postal de destino, entre los diferentes
    //camiones, respetando su prioridad y maximizando la carga a transportar.

    List<Camion> camiones = list<Camion>();
    List<Paquete> paquetes = list<Paquete>();

    subirArchivos(camiones, paquetes);

    //voy mostrando por zona 
    distribuir(camiones, paquetes);

    return 0;
}
