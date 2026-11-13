
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
struct Movil
{
   int idMovil; 
   char conductor[100];
   int zona;
   char celular[50];
};
struct Caso
{
    int tiempoInicio;
    int tiempoFinal;
};
struct RMovil
{
    Movil m;
    int tiempoTotal;
    Map<int, Caso> casos; // map que por cada numero de caso tiene el caso que atendio
};
RMovil rMovil(Movil m)
{
    RMovil ret;
    ret.m = m;
    ret.tiempoTotal = 0;
    ret.casos = map<int, Caso>();
    return ret;
}
struct Abonado
{
   int idAbo; 
   char nombre[100]; 
   char celular[50]; 
   bool cuotasAlDia;
};
struct Colas
{
    Queue<Abonado> aboEspera = queue<Abonado>();
    Queue<Movil> libres = queue<Movil>();
};
Colas colas()
{
    return {queue<Abonado>(), queue<Movil>()};
}

// envia un SMS al celular especificado como parametro
void notificarAsignacion(string celularDestino,int nroCaso,string nombre);

// retorna la hora actual expresada en milisegundos
int getTime(); 

List<Abonado> abonadosSubir()
{
    List<Abonado> ret = list<Abonado>();
    FILE* f = fopen("ABONADOS.dat","r+b");
    Abonado a = read<Abonado>(f);
    while(!feof(f))
    {
        listAdd<Abonado>(ret, a);
        a = read<Abonado>(f);
    }
    fclose(f);
    return ret;
}

List<RMovil> rMovilesSubir()
{
    List<RMovil> ret = list<RMovil>();
    FILE* f = fopen("MOVILES.dat","r+b");
    Movil m = read<Movil>(f);
    while(!feof(f))
    {
        RMovil rm = rMovil(m);
        listAdd<RMovil>(ret, rm);
        m = read<Movil>(f);
    }
    fclose(f);
    return ret;
}

int cmpAbonadoId(Abonado a, int id){return a.idAbo-id;}

int cmpRMovilId(RMovil rm, int id){return rm.m.idMovil-id;}

void informarTiempoEspera();

void asignacionPosible(Colas& c, int nroCaso, List<RMovil>& lstRMov)
{
    // verificar si es posible asignar un movil.
    /* Para hacer una asignacion, tenemos que verificar si en la cola de espera de esa zona hay algun movil disponible
    */
    // si las colas no estan vacias, entonces se puede asignar un movil
    if (!queueIsEmpty<Movil>(c.libres) && !queueIsEmpty<Abonado>(c.aboEspera))
    {
        Movil m = queueDequeue<Movil>(c.libres);
        Abonado a = queueDequeue<Abonado>(c.aboEspera);
        // notifico por SMS a los involucrados
        notificarAsignacion(m.celular, nroCaso, a.nombre);
        notificarAsignacion(a.celular, nroCaso, m.conductor);
        
        // guardo el tiempo de inicio de la asignacion en el RMovil por su nroCaso
        RMovil* rm = listFind<RMovil>(lstRMov, m.idMovil, cmpRMovilId);
        Caso* c = mapGet<int, Caso>(rm->casos, nroCaso);
        c->tiempoInicio = getTime();
    }
}

int cmpRMovilZona(RMovil a, RMovil b)
{
    return a.m.zona-b.m.zona;
}

// recorre el map de casos y devuele la suma de las diferencias de los tiempos por caso
int tiemposMovil(Map<int, Caso> casos)
{
    int acumTiempoCaso = 0;
    mapReset<int, Caso>(casos);
    while(mapHasNext<int, Caso>(casos))
    {
        Caso* c = mapNextValue<int, Caso>(casos);
        acumTiempoCaso += c->tiempoFinal-c->tiempoInicio;
    }
    return acumTiempoCaso;
}

void mostrarMovilesInferiores(List<RMovil>& lstRMov)
{
    listReset<RMovil>(lstRMov);
    listSort<RMovil>(lstRMov, cmpRMovilZona);
    
    Map<int, int> tiempoPorZona = map<int, int>();
    
    int contCasos = 0;
    int acumTiempoRespuestaTotalZona = 0;
    int zonaAnt = -1;
    int prom;
    while(listHasNext<RMovil>(lstRMov))
    {
        RMovil* rm = listNext<RMovil>(lstRMov);
        // sigue en la zona
        if(rm->m.zona==zonaAnt || zonaAnt==-1)
        {
            contCasos++;
            rm->tiempoTotal = tiemposMovil(rm->casos);
            acumTiempoRespuestaTotalZona += rm->tiempoTotal;
        }
        else // si cambia la zona
        {
            prom = acumTiempoRespuestaTotalZona/contCasos;
            mapPut<int, int>(tiempoPorZona, zonaAnt, prom);
            zonaAnt = rm->m.zona;
        }
    }
    prom = acumTiempoRespuestaTotalZona/contCasos;
    mapPut<int, int>(tiempoPorZona, zonaAnt, prom);
    
    mapReset<int, int>(tiempoPorZona);
    listReset<RMovil>(lstRMov);
    int zona = mapNextKey<int, int>(tiempoPorZona);
    int* promedioZona = mapGet<int, int>(tiempoPorZona, zona);
    while(listHasNext<RMovil>(lstRMov))
    {
        RMovil* rm = listNext<RMovil>(lstRMov);
        if(rm->m.zona==zona)
        {
            if (rm->tiempoTotal<*promedioZona)
            {
                cout << "El movil id: " << rm->m.idMovil << "cuyo conductor es "; 
                cout << rm->m.conductor << " de la zona: " << rm->m.zona;
                cout << " tuvo un tiempo de atencion menor al promedio: " << *promedioZona << endl;
            }
        }
        else
        {
            zona = mapNextKey<int, int>(tiempoPorZona);
            promedioZona = mapGet<int, int>(tiempoPorZona, zona);
        }
    }
}

int main()
{
    // subir moviles y abonados
    List<Abonado> lstAbo = abonadosSubir();
    List<RMovil> lstRMov = rMovilesSubir();
    
    Map<int, Colas> mEsperaXZona = map<int, Colas>();
    
    int numCaso;
    cout << "Ingrese numero de caso: " << endl;
    cin >> numCaso;
    
    // variables
    int idAbo;
    int zona;
    int idMovil;
    int operacion;
    cout << "Ingrese la operacion que desea realizar (1: abonado; 2: movil; 3: finalizar): " << endl;
    cin >> operacion;
    // mientras no se ingrese un 3 continuamos operando
    while(operacion != 3)
    {
        // si llama un abonado 
        if (operacion == 1)
        {
            // pedrile el id, la zona (verificar entre 0 y 9)  
            cout << "Ingrese su id de abonado" << endl;
            cin >> idAbo;
            cout << "Ingrese la zona del accidente: " << endl;
            cin >> zona;
            while(zona<0 || zona>9)
            {
                cout << "Error, ingrese la zona correcta del accidente: " << endl;
                cin >> zona;
            }
            
            // verificar cuotas al dia
            Abonado* a = listFind<Abonado>(lstAbo, idAbo, cmpAbonadoId);
            if(a->cuotasAlDia)
            {
                // agregarlo a la lista de espera para su atencion
                Colas vacio = colas();
                Colas* espera = mapDiscover<int, Colas>(mEsperaXZona, zona, vacio);
                
                queueEnqueue<Abonado>(espera->aboEspera, *a);
                informarTiempoEspera();
                
                asignacionPosible(*espera, numCaso+1, lstRMov);
            }
        }
        
        
        // si llama un movil
        if (operacion == 2)
        {
            cout << "Ingrese id del movil: " << endl;
            cin >> idMovil;
            // agarrar numero de caso que acaba de resolver
            cout << "Ingrese numero de caso completado: " << endl;
            cin >> numCaso;
            
            RMovil* rm = listFind<RMovil, int>(lstRMov, idMovil, cmpRMovilId);
            if(rm!=NULL)
            {
                Colas vacio = colas();
                Colas* espera = mapDiscover<int, Colas>(mEsperaXZona, rm->m.zona, vacio);
                // meter el movil a la lista de libres 
                queueEnqueue<Movil>(espera->libres, rm->m);
                informarTiempoEspera();
                
                // Como se libero el movil, entonces agrego el tiempo de finalizacion de su caso
                Caso* c = mapGet<int, Caso>(rm->casos, numCaso);
                c->tiempoFinal = getTime();
                
                asignacionPosible(*espera, numCaso+1, lstRMov);
            }
        }
    }
    // procesar todos los tiempos 
    mostrarMovilesInferiores(lstRMov);
    return 0;
}