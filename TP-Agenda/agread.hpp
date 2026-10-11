#ifndef _AGLEER_
#define _AGLEER_

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

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;

Integer leerInteger(FILE* f)
{
    return read<Integer>(f);
}

Byte leerByte(FILE* f)
{
    return read<Byte>(f);
}

string leerString(FILE* f)
{
    Byte flag = leerByte(f);
    string aux = "";
    if( flag.b==255 )
    {
        Integer longS = leerInteger(f);
        for (int i=0; i<longS.i; i++)
        {
            aux += leerByte(f).b;
        }
    }
    else
    {
        for(int i=0; i<flag.b; i++)
        {
            aux += leerByte(f).b;
        }
    }
    return aux;
}

Date leerDate(FILE* f)
{
    return read<Date>(f);
}

RegType leerRegType(FILE* f, Integer cntCampos)
{
    RegType rT = regType();
    for(int i=0; i<cntCampos.i; i++)
    {
        Byte k = leerByte(f);
        string v = leerString(f);
        regTypeAdd(rT, k, v);       
    }
    return rT;
}

RegData leerRegData(FILE* f)
{
    Byte cntCampos = leerByte(f);
    RegData rD = regData();
    rD.cntCamposCompletos = cntCampos;
    for(int i=0; i<cntCampos.b; i++)
    {
        Byte k = leerByte(f);
        string v = leerString(f);
        regDataAdd(rD, k, v);
    }
    return rD;
}

#endif