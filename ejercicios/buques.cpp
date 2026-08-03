
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

#include "buques tools.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;


/* Coll<Buque> buquesSubir(){
    Coll<Buque> ret = coll<Buque>();

    FILE* f = fopen("BUQUES.dat", "r+b");
    Buque b = read<Buque>(f);
    while( !feof(f) )
    {       
        collAdd<Buque>(ret, b, buqueToString);
        b = read<Buque>(f);
    }

    fclose(f);
    return ret;
} */

Coll<Container> containerSubir(){
    Coll<Container> ret = coll<Container>();

    FILE* f = fopen("CONTAINERS.dat", "r+b");
    Container c = read<Container>(f);
    while( !feof(f) )
    {       
        collAdd<Container>(ret, c, containerToString);
        c = read<Container>(f);
    }

    collReset<Container>(ret);
    fclose(f);
    return ret;
}

/* 
    Los archivos BUQUES.dat y CONTAINERS.dat
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

    Emitir un listado, con el diseño que se describe más abajo, 
    indicando en qué orden se deben cargar o descargar (según sea el caso) los containers de cada buque para que este se mantenga estable. 
    Se garantiza que siempre existirá un orden adecuado para cargar o descargar todos los contenedores.
    Listado de cargas y descargas del día: 99-99-9999
    Id. Buque: 9999, [Carga o Descarga], Grúa: 999
    Nombre: xxxxxxxxxxxxx, Dársena: 999
    Fecha: 99-99-9999 de [Salida o Llegada], Cant. containers: 9999
    Orden	Id. Container	Longitud	Peso
    1	99999	9999	999
    2	99999	9999	999
    3	99999	9999	999
    :	:	:	:
    Peso total transportado: 9999

*/

bool estable(int peso,int longitud,int cteFlotacion){
    return true; 
}


bool cargaDescarga(int fechaBuque,Fecha fhoy)
{
    //devuelve true si es carga y false si es descarga 
    //si la fecha del buque es anterior a la del dia de hoy es descarga 
    //si la fecha es posterior es carga porque todavia no salio 
    int anioB = fechaBuque/10000;
    int mesB = (fechaBuque%10000)/100;
    int diaB = fechaBuque%100;
    Fecha fB = fecha(anioB, mesB, diaB);

    //return fechaCmpDiaMesAnio(fB,fhoy);
}

void buqueMostrar(Buque b){
    //muestra datos del buque que se esta procesando 
    Fecha f = fecha();
    cout<<"Listado de cargas y descargas del día: "<<fechaToString(f)<<endl;
    cout<<"Id. Buque:" <<b.idBuque<<endl;
    bool cod = cargaDescarga(b.fecha, f);
    if(cod){
        cout<<"Carga"<<endl;
    }else{
        cout<<"Descarga"<<endl;
    }
    cout<<"Grúa: "<<b.grua<<endl;
    cout<<"Nombre: "<<b.nombre<<endl;
    cout<<"Dársena: "<<b.darsena<<endl;
    if(cod){
        cout<<"Fecha: "<<b.fecha<<" de salida."<<endl;;
    }else{
        cout<<"Fecha: "<<b.fecha<<" de llegada."<<endl;
    }
    cout<<"Cant. containers: "<<b.cantContainers<<endl;
}

int cmpContainersIdBuque(Container a, Container b)
{
    return a.idBuque-b.idBuque;
}

int cmpIdBuqueContainer(Container a, int idBuque)
{
    return a.idBuque-idBuque;
}

bool estable(int peso,int longitud,int cteFlotacion);

int cmpidContainer(Container c, int idContainer)
{
    return c.idContainer-idContainer;
}

Coll<Container> containersOrdenar(Coll<Container>& cContainers, Buque b)
{
    // ordeno la coleccion por id 
    collSort<Container>(cContainers, cmpContainersIdBuque, containerFromString, containerToString);
    // creo una coleccion auxiliar que tendra unicamente los containers del buque actual
    Coll<Container> cContAux = coll<Container>();
    int i=0;
    int j=0;
    int pos = collFind<Container, int>(cContainers, b.idBuque, cmpIdBuqueContainer, containerFromString);
    while (i<b.cantContainers)
    {
        Container c = collGetAt<Container>(cContainers, pos + j , containerFromString);

        bool esEstable = estable(c.peso, c.longitud, b.cteFlotacion);
        if (esEstable)
        {
            collAdd<Container>(cContAux, c, containerToString);
            i++;
            collRemoveAt(cContainers, pos + j);
        }
        else
        {
            j++;
        }
        if (j==b.cantContainers)
        {
            j=0;
        }
    }

    return cContAux;
}

void containerMostrar(Buque b, Coll<Container> cCnt){
    collReset<Container>(cCnt);
    bool eoc = false; 
    int i = 0; 
    int acumPeso=0;
    cout<< "Orden    Id. Container    Longitud    Peso" <<endl;
    while(eoc){
        Container c = collNext<Container>(cCnt, eoc, containerFromString);
        cout<<i<<"   "<<c.idContainer<<"   "<<c.longitud<<"   "<<c.peso<<endl;
        i++;
        acumPeso+=c.peso;
    }
    cout<<"Peso total transportado: "<<acumPeso<<endl;
}

int main()
{
    // colecciones a memoria
    //Coll<Buque> cBuq = buquesSubir();
    Coll<Container> cCnt = containerSubir();

    // barro archivo de movimientos
    FILE* f = fopen("BUQUES.dat","r+b");
    Buque b = read<Buque>(f);
    while( !feof(f) )
    {
        buqueMostrar(b);
        Coll<Container> cContB = containersOrdenar(cCnt, b);
        containerMostrar(b, cContB);

        b = read<Buque>(f);
    }

    fclose(f);
    return 0;
}
