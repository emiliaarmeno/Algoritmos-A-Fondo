
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

#include "ART tools.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;


Coll<REmpresa> empresasSubir(){
    Coll<REmpresa> ret = coll<REmpresa>();

    FILE* f = fopen("EMPRESAS.dat", "r+b");
    Empresa e = read<Empresa>(f);
    while( !feof(f) )
    {
        Coll<RTrabajador> rT = coll<RTrabajador>();
        REmpresa x = rEmpresa(e, 0, rT);        
        collAdd<REmpresa>(ret,x,rEmpresaToString);

        e = read<Empresa>(f);
    }

    fclose(f);
    return ret;
}

double porcentaje(int cantTrabajadores,int cantAccidentes){
    return 0; 
}

int cmpREmpresaInt(REmpresa re, int id){
    return re.e.idEmpresa-id;
}

void punto1Procesar(Accidente a,Coll<REmpresa>& cEmp) //calculo la cantidad de accidentes por empresa 
{
    //busco la empresa por idEmpresa en la coleccion de empresas 
    int id = a.idEmpresa;
    int pos = collFind<REmpresa, int>(cEmp, id, cmpREmpresaInt, rEmpresaFromString);
    REmpresa re = collGetAt(cEmp, pos, rEmpresaFromString);
    //le sumo uno al contador de accidentes de esa empresa (wrappeado)
    re.cantAccidentes++;
    //seteo los cambios 
    collSetAt(cEmp, re, pos,rEmpresaToString);
}

int cmpRTrabajadoraLegajo(RTrabajador rt, int legajo){
    return rt.legajo-legajo; 
}

int cmpREmpresaLegajo(REmpresa re, int legajo){

    int pos = collFind<RTrabajador, int>(re.rT, legajo, cmpRTrabajadoraLegajo, rTrabajadorFromString);
    RTrabajador rt = collGetAt(re.rT, pos, rTrabajadorFromString);
    return cmpRTrabajadoraLegajo(rt, legajo);
}

int descubrirTrabajador(Coll<REmpresa> cEmp, int legajo, int cantDiasLicencia){
    //lo busco en la coleccion 
     int pos = collFind<REmpresa, int>(cEmp, legajo, cmpREmpresaLegajo, rEmpresaFromString);
     REmpresa re = collGetAt<REmpresa>(cEmp, pos, rEmpresaFromString);
     //si no estaba en la coleccion lo agrego
     if(pos<0){
        RTrabajador t = rTrabajador(legajo, cantDiasLicencia);
        pos = collAdd<RTrabajador>(re.rT, t, rTrabajadorToString);
     }else{
        //si ya estaba le sumo la cantidad de dias de licencia 
        int pos = collFind<RTrabajador, int>(re.rT, legajo, cmpRTrabajadoraLegajo, rTrabajadorFromString);
        RTrabajador rt = collGetAt(re.rT, pos, rTrabajadorFromString);
        rt.cantDiasLicencia+=cantDiasLicencia;
     }
     return pos; 
}

void punto2Procesar(Accidente a, Coll<REmpresa>& cEmp)
{
    //recorro empresa descubriendo los trabajadores 
    int pos = descubrirTrabajador(cEmp, a.legajo, a.cantDiasLicencia);
}

void punto1Mostrar(Coll<REmpresa> cEmp)
{
    // recorrer y mostrar
    collReset<REmpresa>(cEmp);
    while(collHasNext<REmpresa>(cEmp)){
        REmpresa elm = collNext<REmpresa>(cEmp, rEmpresaFromString);
        //busco el porcentaje 
        int cantTrabajadores = elm.e.cantTrabAsegurados;
        int cantAccidentes = elm.cantAccidentes;
        double p = porcentaje(cantTrabajadores, cantAccidentes);
        //muestro cada empresa
        cout << "Empresa: "<< elm.e.razonSocial << " Reajuste: " << p << endl;
    }
}
void punto2Mostrar(Coll<REmpresa> cEmp)
{
    // recorrer y mostrar
    collReset<REmpresa>(cEmp);
    while(collHasNext<REmpresa>(cEmp)){
        REmpresa elm = collNext<REmpresa>(cEmp, rEmpresaFromString);
        cout << "Razon social: " << elm.e.razonSocial<<endl;
        //por cada empresa recorro la coleccion de trabajadores mostrando legajo y dias de licencia 
        collReset<RTrabajador>(elm.rT);
        while(collHasNext<RTrabajador>(elm.rT)){
            RTrabajador rt = collNext<RTrabajador>(elm.rT, rTrabajadorFromString);
            cout << "Legajo: " << rt.legajo << "Total dias ausente: " << rt.cantDiasLicencia <<endl;
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
    FILE* f = fopen("ACCIDENTES.dat","r+b");
    Accidente a = read<Accidente>(f);
    while( !feof(f) )
    {
        //por cada accidente agrego info
        punto1Procesar(a,cEmp);
        punto2Procesar(a,cEmp);

        a = read<Accidente>(f);
    }

    punto1Mostrar(cEmp);
    punto2Mostrar(cEmp);

    fclose(f);
    return 0;
}
