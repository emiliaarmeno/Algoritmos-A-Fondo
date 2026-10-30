
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

struct Fecha
{
    int dia;
    int mes;
    int anio;
};
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
struct Container
{ 
   int idContainer;
   int peso;
   int longitud; 
   int idBuque;
};

bool estable(int peso,int longitud,int cteFlotacion);

Fecha fechaActual();
string fechaToString(Fecha hoy);
int cmpFF(Fecha a, Fecha b); //devuelve positivo si a es posterior a b y negativo si es anterior

//compara containers por idBuque
int cmpCC(Container a, Container b){
    return a.idBuque-b.idBuque;
}

List<Container> containersSubir(){
    List<Container> ret = list<Container>();
    FILE* f = fopen("CONTAINERS.dat", "r+b");
    Container c = read<Container>(f);
    while(!feof(f)){
        listOrderedInsert<Container>(ret, c, cmpCC);
        c = read<Container>(f);
    }
    fclose(f);
    return ret; 
}

void calcularLlegadaOSalida(Fecha fechaBuque, string& CargaoDescarga, string& SalidaoLlegada){
    //si hoy es posterior a la fecha --> esta llegando descargando 
    //si hoy es anterior a la fecha --> esta por salir cargando
    //si la fecha es hoy lo tomo como carga 
    int llegaOSale = cmpFF(fechaActual(), fechaBuque);
    
    if(llegaOSale>0){ 
        CargaoDescarga = "Descarga";
        SalidaoLlegada = "Llegada";
    }else{
        CargaoDescarga = "Carga";
        SalidaoLlegada = "Salida";
    }
}

//muestra la informacion del buque con el diseño que pide la consigna
void buqueMostrarInfo(Buque b){
    string CargaoDescarga = "";
    string SalidaoLlegada = "";
    calcularLlegadaOSalida(b.fecha, CargaoDescarga, SalidaoLlegada);

    cout<<"Id. Buque: " << b.idBuque << " " << CargaoDescarga << " Grua: " << b.grua<<endl;
    cout<<"Nombre: " << b.nombre << " Darsena: " << b.darsena << endl;
    cout<<"Fecha: "<< fechaToString(b.fecha) << " de " << SalidaoLlegada << " Cant. containers: " << b.cantContainers << endl; 
}

//guardo el orden en el que se debe cargar o descargar los containers del buque 
Queue<Container> conteinerOrdenar(Buque b, List<Container> lCont){
    //separo los containers de este buque en pendientes
    Queue<Container> pendientes = queue<Container>();
    listReset<Container>(lCont);
    while( listHasNext<Container>(lCont) )
    {
        Container c = *listNext<Container>(lCont);
        if( c.idBuque == b.idBuque )
        {
            queueEnqueue<Container>(pendientes, c);
        }
    }

    //los voy ordenando y reintento los que no 
    Queue<Container> orden = queue<Container>(); //aca los voy a meter ordenados 
    while( !queueIsEmpty<Container>(pendientes) )
    {
        Container c = queueDequeue<Container>(pendientes);
        if( estable(c.peso, c.longitud, b.cteFlotacion) )
        {
            queueEnqueue<Container>(orden, c); //si es estable en este orden lo meto en orden
        }
        else
        {
            //no es estable lo vuelvo a meter en pendientes
            queueEnqueue<Container>(pendientes, c);//lo vuelvo a enconlar para probar más tarde
        }
    }
    return orden;
}

//muestro con el diseño de la consigna los containers del buque en el orden de carga/descarga
//asimismo voy sumando el peso de todos los containers para finalmente mostrar el peso total que transporta
void conteinerMostrarOrden(Queue<Container> orden){
    int pesoTot = 0;
    int i = 1; 
    cout<< "Orden	Id. Container	Longitud	Peso"<<endl; 
    while( !queueIsEmpty<Container>(orden) ){
        Container c = queueDequeue<Container>(orden);
        cout<<i<<" "<<c.idContainer<<" "<<c.longitud<<" "<<c.peso<<endl; 
        pesoTot+=c.peso;
        i++;
    }
    cout<<"Peso total transportado: "<<pesoTot<<endl; 
}

int main()
{
    //subo los containers a memoria y los ordeno por idBuque
    List<Container> lCont = containersSubir();

    //muestro la primera linea 
    Fecha hoy = fechaActual();
    string sHoy = fechaToString(hoy);
    cout<<"Listado de cargas y descargas del día: "<<sHoy<<endl;

    //recorro los buques
    FILE* f = fopen("BUQUES.dat", "r+b");
    Buque b = read<Buque>(f);
    while(!feof(f)){
        //muestro info del buque
        buqueMostrarInfo(b);
        Queue<Container> ordenCont = conteinerOrdenar(b, lCont);
        conteinerMostrarOrden(ordenCont);
        b = read<Buque>(f);
    }

    fclose(f);
    return 0;
}
