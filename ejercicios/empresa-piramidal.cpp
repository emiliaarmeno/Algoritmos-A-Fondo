
#include "principal.hpp"

#include <cassert>
#include <iostream>

#include "biblioteca/funciones/files.hpp"
#include "biblioteca/funciones/millis.hpp"
#include "biblioteca/funciones/strings.hpp"
#include "biblioteca/funciones/tokens.hpp"
#include "biblioteca/tads/parte1/Coll.hpp"
#include "biblioteca/tads/parte1/Fecha.hpp"
#include "biblioteca/tads/parte1/Timer.hpp"
#include "biblioteca/tads/parte2/List.hpp"
#include "biblioteca/tads/parte2/Queue.hpp"
#include "biblioteca/tads/parte2/Stack.hpp"
#include "biblioteca/tads/parte2/Array.hpp"
#include "biblioteca/tads/parte2/Map.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;

#define COMISION .3

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
    double liqActual;
};

RSocio rSocio(Socio s,double licAct)
{
    return {s,licAct};
}

struct Venta
{
   int idSocio;
   int idProducto;
   char observ[100];
   Fecha fecha; 
   double importe;
};

Map<int,RSocio> sociosSubir()
{
    Map<int,RSocio> ret = map<int,RSocio>();
    FILE* f = fopen("SOCIOS.dat","r+b");

    Socio s = read<Socio>(f);
    while( !feof(f) )
    {
        RSocio rs = rSocio(s,0);
        mapPut<int,RSocio>(ret,s.idSocio,rs);
        s = read<Socio>(f);
    }

    fclose(f);
    return ret;
}

void ventaLiquidar(int idSocioALiquidar,double impVenta,double comision, Map<int,RSocio>& m)
{
    // obtengo el socio que vendio
    RSocio* rs = mapGet<int,RSocio>(m,idSocioALiquidar);

    // acumulo el importe en totVtas
    rs->s.totalVentasAcumuladas+=impVenta;

    // le comisiono al vendedor
    double leQueda = comision-comision*COMISION;
    rs->s.liquidacionesAcumuladas+=leQueda;
    rs->liqActual+=leQueda;

    int idRef = rs->s.idSocioRef;
    if( idRef>0 )
    {
        double reparte = comision*COMISION;
        ventaLiquidar(idRef,0,reparte,m);
    }
}

void ventaProcesar(Venta v,Map<int,RSocio>& m)
{
    double com = v.importe*COMISION;
    ventaLiquidar(v.idSocio,v.importe,com,m);
}

int cmpIdSoc(int id1,int id2)
{
    return id1-id2;
}

void punto1Mostrar(Map<int,RSocio> mSoc)
{
    // ordeno por id
    mapSortByKeys<int,RSocio>(mSoc,cmpIdSoc);

    // itero y muestro
    mapReset<int,RSocio>(mSoc);
    while( mapHasNext<int,RSocio>(mSoc) )
    { 
        RSocio* rs = mapNextValue<int,RSocio>(mSoc);
        cout << rs->s.nombre;
        cout << rs->s.liquidacionAnterior; 
        cout << rs->liqActual;

        double porc = rs->liqActual*100/rs->s.liquidacionAnterior;
        cout << porc << endl;
        cout << rs->s.liquidacionesAcumuladas;
    }
}    

void punto2Procesar(Map<int,RSocio> mSoc)
{
    FILE* f = fopen("SOCIOS.dat","r+b");

    Socio s = read<Socio>(f);
    while( !feof(f) )
    {
        // busco el socio
        RSocio* rs = mapGet<int,RSocio>(mSoc,s.idSocio); 

        // actualizo el registro
        s.liquidacionAnterior = rs->liqActual;
        s.liquidacionesAcumuladas = rs->s.liquidacionesAcumuladas;
        s.totalVentasAcumuladas = rs->s.totalVentasAcumuladas;

        // grabo
        seek<Socio>(f,filePos<Socio>(f)-1);
        write<Socio>(f,s);

        s = read<Socio>(f);
    }

    fclose(f);
}


int main()
{
    // subimos los socios a memoria
    Map<int,RSocio> mSoc = sociosSubir();

    //recorro el archivo de ventas y las proceso
    FILE* f = fopen("VENTAS.dat","r+b");

    Venta v = read<Venta>(f);
    while( !feof(f) )
    {
        ventaProcesar(v,mSoc);
        v = read<Venta>(f);
    }

    punto1Mostrar(mSoc);   
    
    punto2Procesar(mSoc);

    fclose(f);
    return 0;
}
