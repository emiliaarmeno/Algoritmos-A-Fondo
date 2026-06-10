
#include <iostream>
#include "biblioteca/funciones/millis.hpp"
#include "biblioteca/funciones/files.hpp"
#include "biblioteca/funciones/strings.hpp"
#include "biblioteca/funciones/tokens.hpp"
#include "biblioteca/tads/parte1/Fecha.hpp"
#include "biblioteca/tads/parte1/Timer.hpp"
#include "biblioteca/tads/parte1/Coll.hpp"
#include "biblioteca/tads/parte2/Array.hpp"
#include "biblioteca/tads/parte2/List.hpp"
#include "biblioteca/tads/parte2/Map.hpp"
#include "biblioteca/tads/parte2/Queue.hpp"
#include "biblioteca/tads/parte2/Stack.hpp"

#include "principal.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;

Coll<RCiudad> ciudadesSubir()
{
    Coll<RCiudad> ret = coll<RCiudad>();

    FILE* f = fopen("CIUDADES.dat","r+b");
    Ciudad c = read<Ciudad>(f);
    while( !feof(f) )
    {
        RCiudad x = rCiudad(c,0);        
        collAdd<RCiudad>(ret,x,rCiudadToString);

        c = read<Ciudad>(f);
    }

    fclose(f);
    return ret;
}


Coll<RVuelo> vuelosSubir()
{
    return {};
}

RVuelo vueloBuscar(int idVue,Coll<RVuelo> cVue)
{
    return {};
}

int cmpRCiudadId(RCiudad rc,int id)
{
    return rc.c.idCiu-id;
}

RCiudad ciudadBuscar(int id,Coll<RCiudad> cCiu)
{
    // busco en cCiu por id -> pos
    int pos = collFind<RCiudad,int>(cCiu,id,cmpRCiudadId,rCiudadFromString);
    RCiudad rc = collGetAt<RCiudad>(cCiu,pos,rCiudadFromString);
    return rc;
}
void punto1Procesar(RCiudad rcDes,Coll<RCiudad>& cCiu)
{
    // busco en cCiu -> rcDes.c.idCiu -> pos
    int id = rcDes.c.idCiu;
    int pos = collFind<RCiudad,int>(cCiu,id,cmpRCiudadId,rCiudadFromString);

    // incrementeo en rcDes.cont
    rcDes.cont++;

    // seteo en cCiu, en la posicion pos el valor de rcDes 
    collSetAt<RCiudad>(cCiu,rcDes,pos,rCiudadToString);
}

bool vueloAceptaReserva(Reserva r,RVuelo rv)
{
    return rv.v.cap-r.cant>=0;
}

int cmpRVueloId(RVuelo rv,int id)
{
    return rv.v.idVue-id;
}

void punto2Procesar(Reserva r,RVuelo rv,Coll<RVuelo>& cVue)
{
    // 1. determino si acepto o rechazo
    if( vueloAceptaReserva(r,rv) )
    {
        rv.cpa++;
    }
    else
    {
        rv.cpr++;        
    }

    // 2. busco rv en cVue ->
    int id = rv.v.idVue;
    int pos = collFind<RVuelo,int>(cVue,id,cmpRVueloId,rVueloFromString);
    collSetAt<RVuelo>(cVue,rv,pos,rVueloToString);
}

void punto1Mostrar(Coll<RCiudad> cCiu)
{

}
void punto2Mostrar(Coll<RVuelo> cVue)
{

}

int main()
{
    Coll<RCiudad> cCiu = ciudadesSubir();
    Coll<RVuelo> cVue = vuelosSubir();

    FILE* f = fopen("RESERVAS.dat","r+b");

    Reserva r = read<Reserva>(f);
    while( !feof(f) )
    {
        // busco el vuelo
        RVuelo rv = vueloBuscar(r.idVue,cVue);
        
        // busco las ciudades que conecta este vuelo
        RCiudad rcOri = ciudadBuscar(rv.v.idOri,cCiu);
        RCiudad rcDes = ciudadBuscar(rv.v.idDes,cCiu);

        // proceso punto 1
        punto1Procesar(rcDes,cCiu);

        // proceso punto 2
        punto2Procesar(r,rv,cVue);

        r = read<Reserva>(f);
    }

    punto1Mostrar(cCiu);
    punto2Mostrar(cVue);

    fclose(f);
    return 0;
}
