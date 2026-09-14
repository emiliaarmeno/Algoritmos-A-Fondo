#ifndef _AGESTRUCTURAS_
#define _AGESTRUCTURAS_

#include <cassert>
#include <iostream>

#include "biblioteca/funciones/files.hpp"
#include "biblioteca/funciones/millis.hpp"
#include "biblioteca/funciones/strings.hpp"
#include "biblioteca/funciones/tokens.hpp"
#include "biblioteca/tads/parte1/Coll.hpp"
#include "biblioteca/tads/parte1/Fecha.hpp"
#include "biblioteca/tads/parte1/Timer.hpp"
#include "biblioteca/tads/parte1/BitWriter.hpp"
#include "biblioteca/tads/parte1/BitReader.hpp"
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

struct Integer
{
    unsigned short i; // entero de 2 bytes sin signo 
};

Integer integer(unsigned short i)
{
    return {i};
}

struct Byte
{
    unsigned char b;
};

Byte byte(unsigned char c)
{
    return {c};
}

struct Date
{
    unsigned short d;
};

Date date(unsigned short d)
{
    return {d};
}

struct RegType
{
    // {codigo de campo ; String}
    Map<unsigned char, string> mapRegType; 
};

RegType regType()
{
    RegType rT;
    rT.mapRegType = map<unsigned char, string>();
    return rT;
}

RegType regType(char** campos, Integer cntCampos)
{
    RegType rT;
    rT.mapRegType = map<unsigned char, string>();
    for (int i=0; i<cntCampos.i; i++)
    {
        mapPut<unsigned char, string>(rT.mapRegType, i+1, campos[i+2]);
    }
    return rT;
}

RegType regTypeAdd(RegType& rT, Byte id, string s)
{
    mapPut<unsigned char, string>(rT.mapRegType, id.b, s);
    return rT;
}

void regTypeReset(RegType& rT)
{
    mapReset<unsigned char, string>(rT.mapRegType);
}

struct RegData
{
    Byte cntCamposCompletos;
    Map<unsigned char, string> mapRegData; 
    // {codigo de campo; String}
};

RegData regData()
{
    RegData rData;
    rData.cntCamposCompletos=byte(0);
    rData.mapRegData = map<unsigned char, string>();
    return rData; 
}

RegData regDataAdd(RegData& rD, Byte id, string s)
{
    mapPut<unsigned char, string>(rD.mapRegData, id.b, s);
    return rD;
}

void regDataReset(RegData& rD)
{
    mapReset<unsigned char, string>(rD.mapRegData);
}

struct Contactos
{
    // un array de regData que contendra todos los contactos
    Array<RegData> arrContactos;
};

Contactos contactos()
{
    Contactos c;
    c.arrContactos = array<RegData>();
    return c;
}

void contactosAdd(Contactos& c, RegData datosContacto)
{
    arrayAdd<RegData>(c.arrContactos, datosContacto);
}

#endif