
#include <iostream>
// #include "biblioteca/funciones/millis.hpp"
#include "../biblioteca/funciones/files.hpp"
#include "../biblioteca/funciones/strings.hpp"
#include "biblioteca/funciones/tokens.hpp"
#include "biblioteca/tads/parte1/Fecha.hpp"
// #include "biblioteca/tads/parte1/Timer.hpp"
#include "ART tools.hpp"
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

Coll<REmpresa> empresasSubir()
{
    Coll<REmpresa> ret = coll<REmpresa>();

    FILE* f = fopen("EMPRESAS.dat", "r+b");
    Empresa e = read<Empresa>(f);
    while(!feof(f))
    {
        Coll<RTrabajador> rT = coll<RTrabajador>();
        REmpresa x = rEmpresa(e, 0, rT);
        collAdd<REmpresa>(ret, x, rEmpresaToString);

        e = read<Empresa>(f);
    }

    fclose(f);
    return ret;
}

double porcentaje(int cantTrabajadores, int cantAccidentes)
{
    return 0;
}

int cmpREmpresaInt(REmpresa re, int id)
{
    return re.e.idEmpresa - id;
}

void punto1Procesar(Accidente a, Coll<REmpresa>& cEmp)  // calculo la cantidad de accidentes por empresa
{
    // busco la empresa por idEmpresa en la coleccion de empresas
    int id = a.idEmpresa;
    int pos = collFind<REmpresa, int>(cEmp, id, cmpREmpresaInt, rEmpresaFromString);
    REmpresa re = collGetAt(cEmp, pos, rEmpresaFromString);
    // le sumo uno al contador de accidentes de esa empresa (wrappeado)
    re.cantAccidentes++;
    // seteo los cambios
    collSetAt(cEmp, re, pos, rEmpresaToString);
}

int cmpRTrabajadoraLegajo(RTrabajador rt, int legajo)
{
    return rt.legajo - legajo;
}

int cmpREmpresaId(REmpresa re, int id)
{
    return re.e.idEmpresa-id;   
}

REmpresa empresaBuscar(int idEmpresa, Coll<REmpresa> cEmp, int& pos){
    pos = collFind<REmpresa, int>(cEmp, idEmpresa, cmpREmpresaId, rEmpresaFromString);
    return collGetAt<REmpresa>(cEmp, pos, rEmpresaFromString);
}

RTrabajador trabajadorDescubrir(Coll<RTrabajador>& cTrab, int legajo, int& posTrab)
{
    //busco si esta trabajador
    posTrab = collFind<RTrabajador, int>(cTrab, legajo, cmpRTrabajadoraLegajo, rTrabajadorFromString);

    // si no estaba en la coleccion lo agrego
    if(posTrab < 0)
    {
        RTrabajador t = rTrabajador(legajo, 0);
        posTrab = collAdd<RTrabajador>(cTrab, t, rTrabajadorToString);
    }

    //ahora si esta el trabajador en posTrab
    return collGetAt<RTrabajador>(cTrab, posTrab, rTrabajadorFromString);
}

void empresaActualizar(REmpresa re, Coll<REmpresa>& cEmp, int posEmp){
    collSetAt<REmpresa>(cEmp, re, posEmp, rEmpresaToString);
}

void trabajadorActualizar(RTrabajador rt, Coll<RTrabajador>& cTrab, int posTrab){
    collSetAt<RTrabajador>(cTrab, rt, posTrab, rTrabajadorToString);
}


void punto2Procesar(Accidente a, Coll<REmpresa>& cEmp)
{
    //busco empresa 
    int posEmp;
    REmpresa re = empresaBuscar(a.idEmpresa, cEmp, posEmp);

    // descubro trabajador
    int posTrab;
    Coll<RTrabajador> cTrab = re.rT; //variable auxiliar 
    RTrabajador rt = trabajadorDescubrir(cTrab, a.legajo, posTrab);

    //acumulo el total de dias de licencia
    rt.cantDiasLicencia+=a.cantDiasLicencia;
    trabajadorActualizar(rt, cTrab, posTrab); //seteo

    //seteo los cambios 
    re.rT = cTrab;
    empresaActualizar(re, cEmp, posEmp);
}

void punto1Mostrar(Coll<REmpresa> cEmp)
{
    // recorrer y mostrar
    collReset<REmpresa>(cEmp);
    while(collHasNext<REmpresa>(cEmp))
    {
        REmpresa elm = collNext<REmpresa>(cEmp, rEmpresaFromString);
        // busco el porcentaje
        int cantTrabajadores = elm.e.cantTrabAsegurados;
        int cantAccidentes = elm.cantAccidentes;
        double p = porcentaje(cantTrabajadores, cantAccidentes);
        // muestro cada empresa
        cout << "Empresa: " << elm.e.razonSocial << " Reajuste: " << p << endl;
    }
}

void punto2Mostrar(Coll<REmpresa> cEmp)
{
    // recorrer y mostrar
    collReset<REmpresa>(cEmp);
    while(collHasNext<REmpresa>(cEmp))
    {
        REmpresa elm = collNext<REmpresa>(cEmp, rEmpresaFromString);
        cout << "Razon social: " << elm.e.razonSocial << endl;
        // por cada empresa recorro la coleccion de trabajadores mostrando
        // legajo y dias de licencia
        collReset<RTrabajador>(elm.rT);
        while(collHasNext<RTrabajador>(elm.rT))
        {
            RTrabajador rt =
                collNext<RTrabajador>(elm.rT, rTrabajadorFromString);
            cout << "Legajo: " << rt.legajo
                 << "Total dias ausente: " << rt.cantDiasLicencia << endl;
        }
    }
}

/*
    EMPRESAS.dat es el archivo de consultas
    ACCIDENTES.dat es el de novedades
*/

int main()
{
    // colecciones a memoria
    Coll<REmpresa> cEmp = empresasSubir();

    // barro archivo de movimientos
    FILE* f = fopen("ACCIDENTES.dat", "r+b");
    Accidente a = read<Accidente>(f);
    while(!feof(f))
    {
        // por cada accidente agrego info
        punto1Procesar(a, cEmp);
        punto2Procesar(a, cEmp);

        a = read<Accidente>(f);
    }

    punto1Mostrar(cEmp);
    punto2Mostrar(cEmp);

    fclose(f);
    return 0;
}
