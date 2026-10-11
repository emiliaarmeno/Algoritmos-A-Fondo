#ifndef _AGMOSTRAR_
#define _AGMOSTRAR_

#include <cassert>
#include <iostream>

#include "biblioteca/funciones/files.hpp"
#include "biblioteca/funciones/millis.hpp"
#include "biblioteca/funciones/strings.hpp"
#include "biblioteca/funciones/tokens.hpp"
#include "biblioteca/tads/parte1/Coll.hpp"
#include "biblioteca/tads/parte1/Fecha.hpp"
#include "biblioteca/tads/parte1/BitWriter.hpp"
#include "biblioteca/tads/parte1/BitReader.hpp"
#include "biblioteca/tads/parte2/Array.hpp"
#include "biblioteca/tads/parte2/List.hpp"
#include "biblioteca/tads/parte2/Map.hpp"
#include "biblioteca/tads/parte2/Queue.hpp"
#include "biblioteca/tads/parte2/Stack.hpp"

#include "agest.hpp"
#include "agread.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;

void mostrarNumeroDeSerie(FILE* f)
{
    cout << "Nro. de serie: " << leerInteger(f).i << endl;
}

void mostrarFileName(FILE* f)
{
    cout << "Nombre del archivo: " << leerString(f) << endl;
}

Date codificarFecha()
{
    return {};
}

void decodificarFecha(FILE* f, int& anio, int& mes, int& dia)
{
    Date d = leerDate(f);
    cout << "FECHA INCOMPLETA: " << d.d << endl;
}

void mostrarDate(FILE* f)
{
    int anio = 0;
    int mes = 0;
    int dia = 0;
    // debo leer Date y decodificarlo
    decodificarFecha(f, anio, mes, dia);
    // se supone que al acceder se modifica la fecha?
    // me posiciono en el byte de date y escribo tipo:
    // seek(f,-2)
    // write<Date>(f, codificarFechaActual());
    // cout << "Fecha de ultimo acceso: " << dia << "/" << mes << "/" << anio << endl;
}

Integer mostrarCantidadDeCamposConfigurados(FILE* f)
{
    Integer cntCampos = leerInteger(f);
    cout << "Cantidad de campos configurados: " << cntCampos.i << endl;
    return cntCampos;
}

RegType mostrarCamposConfigurados(FILE* f, Integer cntCampos)
{
    RegType rT = leerRegType(f, cntCampos);
    regTypeReset(rT);
    Byte k;
    string v;
    while(mapHasNext<unsigned char, string>(rT.mapRegType))
    {
        k.b = mapNextKey<unsigned char, string>(rT.mapRegType);
        v = *mapGet<unsigned char, string>(rT.mapRegType, k.b);
        cout << "Campo [codigo: " << (int)k.b << ", descripcion: " << v << "]" << endl;
    }
    return rT;
}

Integer mostrarCantidadDeContactos(FILE* f)
{
    Integer cntContactos = leerInteger(f);
    cout << "Cantidad de Registros (contactos): " << cntContactos.i << endl;
    return cntContactos;
}

void mostrarRegData(RegData rD, RegType rT)
{
    regTypeReset(rT);
    regDataReset(rD);
    Byte k;
    string v;
    string desc;
    for (int i=0; i<rD.cntCamposCompletos.b; i++)
    {
        cout << endl;
        k.b = mapNextKey<unsigned char, string>(rD.mapRegData);
        v = *mapGet<unsigned char, string>(rD.mapRegData, k.b);
        desc = *mapGet<unsigned char, string>(rT.mapRegType, k.b);
        cout << desc << ": " << v << endl;
        cout << endl;
    }
}

void mostrarContactos(FILE* f, Integer cntContactos, RegType rTCampos)
{
    cout << "-----------------------------------" << endl;
    for(int i=0; i<cntContactos.i; i++)
    {
        RegData rDContacto = leerRegData(f);
        mostrarRegData(rDContacto, rTCampos);
        cout << "-----------------------------------" << endl;
    }
    cout << "--------[FIN DEL ARCHIVO]--------" << endl;
}

#endif