
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

// entra en memoria
struct Paquete
{
    int idPaq;
    char dirDest[20];
    int cpDest;  // cod. post
    int alto;
    int ancho;
    int prof;  // para calcular el volumen del paquete
};
// entra en memoria
struct Camion
{
    int idCam;
    char conductor[20];
    int volCargaMax;
    int prioridad;  // 0=>max el cero tiene mayor prioridad
};
// cada camion se dirige a una unica zona

struct Asignacion
{
    Camion cam;   // el camión
    int volAcum;  // sumo el vol de los paquetes que lleva
};


// Funcion hardcodeada. Solo valida para este ejercicio
// y con los datos que se proveen
string cod2dest(int cp)
{
    if(cp == 1401) return "Boedo";
    if(cp == 1402) return "Once";
    if(cp == 1403) return "Caballito";
    if(cp == 1404) return "Belgrano";
};

int cmpCamiones(Camion a, Camion b)
{
    if(a.prioridad < b.prioridad) return -1;
    if(a.prioridad > b.prioridad) return 1;
    if(a.prioridad == b.prioridad) return 0;
}

int cmpPaquetes(Paquete a, Paquete b)
{
    return a.cpDest - b.cpDest;
}

// leo cuantos camiones (primer byte sin signo)
// por la cantidad de camiones hago un for y los meto al list corresp
// cuando sale leo, hasta que se termine el archivo, los paquetes y los voy
// metiendo en la list tengo que ordenar los camiones por prioridad y los
// paquetes por cod postal
void leerArchivo(List<Camion>& lCam, List<Paquete>& lPaq)
{
    FILE* f = fopen("DATOS.dat", "r+b");
    int cantCamiones = read<unsigned char>(f);
    for(int i = 0; i < cantCamiones; i++)
    {
        Camion c = read<Camion>(f);
        listOrderedInsert<Camion>(lCam, c, cmpCamiones);
    }
    Paquete p = read<Paquete>(f);
    while(!feof(f))
    {
        listOrderedInsert<Paquete>(lPaq, p, cmpPaquetes);
        p = read<Paquete>(f);
    }
    fclose(f);
}

int paqueteVolumen(Paquete p)
{
    return p.alto * p.ancho * p.prof;
}


//si no tiene lugar suficiente para el volumen del paquete devuelve null
Asignacion* buscarConLugar(List<Asignacion>& asigs, int vol)
{
    listReset<Asignacion>(asigs);
    while( listHasNext<Asignacion>(asigs) )
    {
        Asignacion* a = listNext<Asignacion>(asigs);
        if( a->volAcum + vol <= a->cam.volCargaMax )
        {
            return a;
        }
    }
    return NULL;
}


void asignarPaquete(List<Asignacion>& asigs, List<Camion>& libres, Paquete p)
{
    int vol = paqueteVolumen(p);
    Asignacion* a = buscarConLugar(asigs, vol); //el camion tiene lugar para el paquete?

    //si no hay lugar en el camion y quedan camiones
    if( a==NULL && !listIsEmpty<Camion>(libres) ) // empiezo a llenar un camión nuevo
    {
        Asignacion nueva;
        nueva.cam = listRemoveFirst<Camion>(libres); //lo saco de libres 
        nueva.volAcum = 0;
        a = listAdd<Asignacion>(asigs, nueva); //lo meto en asigs 
    }

    //si hay lugar meto el paquete
    if( a!=NULL )
    {
        a->volAcum += vol;
    }
}

void mostrarZona(int cp, List<Asignacion>& asigs)
{
    cout << "Código Postal: [" << cp << ", " << cod2dest(cp) << "]" << endl;
    cout << "Chofer  Carga %" << endl;
    listReset<Asignacion>(asigs);
    while( listHasNext<Asignacion>(asigs) )
    {
        Asignacion a = *listNext<Asignacion>(asigs);
        int porc = a.volAcum * 100 / a.cam.volCargaMax;
        cout << a.cam.conductor << "  " << porc
             << " (Volumen: " << a.volAcum << " / Max: " << a.cam.volCargaMax << ")" << endl;
    }
}


int main()
{
    List<Camion> libres = list<Camion>();
    List<Paquete> lPaq = list<Paquete>();
    leerArchivo(libres, lPaq);

    List<Asignacion> asigs = list<Asignacion>();
    int cpActual = -1;

    listReset<Paquete>(lPaq);
    while( listHasNext<Paquete>(lPaq) ) //recorro los paquetes ordenados por zona 
    {
        Paquete p = *listNext<Paquete>(lPaq);//me quedo con la copia, no necesito editarla
        if( p.cpDest != cpActual )           // si empieza una zona nueva
        {
            if( cpActual != -1 ) //si no es la primera vuelta
            {
                mostrarZona(cpActual, asigs);
                listFree<Asignacion>(asigs); // limpio asigs para reutilizarla para la próxima zona
            }
            cpActual = p.cpDest;
        }
        asignarPaquete(asigs, libres, p); //sumo el volumen del paquete al camion (si no entra empiezo a llenar uno nuevo)
    }
    //para que se muestre el ultimo (si no hay otro no pude mostrar el ultimo)
    if( cpActual != -1 )
    {
        mostrarZona(cpActual, asigs);
        listFree<Asignacion>(asigs);
    }

    cout << "--------------------------------------------------" << endl;
    cout << "CAMIONES SIN ASIGNACIÓN POR FALTA DE CARGA:" << endl;
    listReset<Camion>(libres);
    while( listHasNext<Camion>(libres) )
    {
        Camion c = *listNext<Camion>(libres);
        cout << "- " << c.conductor << " (ID: " << c.idCam
             << ", Prioridad: " << c.prioridad << ")" << endl;
    }
    return 0;
}
