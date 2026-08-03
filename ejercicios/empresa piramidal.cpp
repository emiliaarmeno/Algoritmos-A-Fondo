
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

struct Socio
{
   int idSocio;
   char nombre[50];
   int  idSocioRef;  // socio referente
   Fecha fechaIngreso;
   double totalVentasAcumuladas;
   double liquidacionAnterior;
   double liquidacionesAcumuladas;
};

struct RSocio
{
    Socio s;
    double liquidacionActual; 
};

struct Venta
{
   int idSocio;
   int idProducto;
   char observ[100];
   Fecha fecha; 
   double importe;
};

/*
    SOCIOS.dat
    VENTAS.dat

    1. Imprimir un listado, ordenado por idSocio, detallando su nombre, fecha de ingreso, 
    liquidación anterior, liquidación actual (la que surge de procesar las ventas), 
    porcentaje de incremento o decremento de la liquidación actual respecto a la anterior, 
    y el total de acumulado desde su ingreso a la empresa.
    2. Actualice los campos liquidacionAnterior, liquidacionAcumulada y totalVentasAcumuladas del archivo SOCIOS.dat.
*/

string socioToString(Socio so);
Socio socioFromString(string st);

RSocio rsocioFromString(string s);
string rsocioToString(RSocio rs);

Coll<RSocio> rsociosSubir(){
    Coll<RSocio> cSoc = coll<RSocio>();
    FILE* f = fopen("SOCIOS.dat", "r+b");
    Socio s = read<Socio>(f);
    while(!feof(f)){
        RSocio rs = {s, 0}; //liquidacion actual empieza en cero en todas
        collAdd<RSocio>(cSoc, rs, rsocioToString);
        s = read<Socio>(f);
    }
    fclose(f);
    return cSoc;
}

//compara un rsocio con un id 
int cmpRSocId(RSocio rs, int id){
    return rs.s.idSocio-id;
}

RSocio socioBuscar(Coll<RSocio> cSoc, int id, int& posRS){
    posRS = collFind<RSocio, int>(cSoc, id, cmpRSocId, rsocioFromString);
    RSocio rs = collGetAt<RSocio>(cSoc, posRS, rsocioFromString);
    return rs;
}


void ventasProcesar(Venta v, Coll<RSocio>& cSoc){
    //buso el socio que hizo la venta 
    int posRS;
    RSocio rs = socioBuscar(cSoc, v.idSocio, posRS);

    //le sumo el importe al que hizo la venta
    rs.s.totalVentasAcumuladas+=v.importe;
    //el que hizo la venta se queda con el 30%
    double comision = v.importe*0.3; 
    rs.liquidacionActual+=comision;

    collSetAt<RSocio>(cSoc, rs, posRS, rsocioToString);

    while(rs.s.idSocioRef!=-1){ //mientras que siga habiendo un referente
        //busco el que lo refirio
        int posRecibCom;
        RSocio rsRecibeComi = socioBuscar(cSoc, rs.s.idSocioRef, posRecibCom);

        comision = comision * 0.3; //recibe el 30% de lo que se llevo el nivel anterior
        rsRecibeComi.liquidacionActual+=comision; 
        collSetAt<RSocio>(cSoc, rsRecibeComi, posRecibCom, rsocioToString);

        rs = rsRecibeComi; // subo un nivel en la piramide
    }

}

//compara dos rsocios segun sus idSocio
int cmpRS(RSocio a, RSocio b){
    return a.s.idSocio-b.s.idSocio;
}

void sociosMostrar(Coll<RSocio> cSoc){
    //ordeno la coleccion por idSocio
    collSort<RSocio>(cSoc, cmpRS, rsocioFromString, rsocioToString);

    //recorro y muestro
    collReset<RSocio>(cSoc);
    while(collHasNext<RSocio>(cSoc)){
        RSocio rs = collNext<RSocio>(cSoc, rsocioFromString);
        cout<<"Socio:"<<rs.s.idSocio<<" Nombre: "<<rs.s.nombre<<endl;
        cout<<"Fecha Ingreso: "<<fechaToString(rs.s.fechaIngreso)<<endl;
        cout<<"Liquidacion anterior: "<<rs.s.liquidacionAnterior<< " Liquidacion actual: "<<rs.liquidacionActual<<endl;
        
        double diferencia = rs.liquidacionActual-rs.s.liquidacionAnterior;
        double porcentaje = (diferencia / rs.s.liquidacionAnterior) * 100; //aca falta tener en cuents si nunca liquido antes (no se divide por cero)
        if(diferencia>=0){
            cout<<"Porcentaje de incremento de la liquidacion actual: "<<doubleToString(porcentaje)<<"%"<<endl;
        }else{
            cout<<"Porcentaje de decremento de la liquidacion actual: "<<doubleToString(-porcentaje)<<"%"<<endl; //hago que se cancele el signo
        }

        cout<<"Total acumulado desde el ingreso a la empresa: "<<rs.s.liquidacionesAcumuladas<<endl;
    }
}

//punto 2 actualizar los campos en el archivo de socios
void sociosActualizar(Coll<RSocio> cSoc){
    FILE* f = fopen("SOCIOS.dat", "w+b"); // "w+b" trunca el archivo para reescribirlo entero

    for(int i=0; i<collSize(cSoc); i++){
        RSocio rs = collGetAt<RSocio>(cSoc, i, rsocioFromString);
        Socio s = rs.s;

        s.liquidacionAnterior = rs.liquidacionActual;         // la de este periodo pasa a ser "la anterior" para el proximo
        s.liquidacionesAcumuladas += rs.liquidacionActual;     // se suma al historico
        // s.totalVentasAcumuladas ya viene actualizado dentro de rs.s (se modificó en ventasProcesar)

        write<Socio>(f, s);
    }

    fclose(f);
}

int main()
{
    Coll<RSocio> cSoc = rsociosSubir();

    FILE* f = fopen("VENTAS.dat", "r+b");
    
    Venta v = read<Venta>(f);
    while(!feof(f)){
        //liquido las comisiones y actualizo los campos de socios
        ventasProcesar(v, cSoc);
        v = read<Venta>(f);
    }
    sociosMostrar(cSoc);
    //para el punto 2
    sociosActualizar(cSoc);

    fclose(f);
    return 0;
}
