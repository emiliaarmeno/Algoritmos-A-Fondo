#ifndef _AGESCRIBIR_
#define _AGESCRIBIR_

#include <cassert>
#include <iostream>

#include "agest.hpp"
#include "biblioteca/funciones/files.hpp"
#include "biblioteca/funciones/millis.hpp"
#include "biblioteca/funciones/strings.hpp"
#include "biblioteca/funciones/tokens.hpp"
#include "biblioteca/tads/parte1/BitReader.hpp"
#include "biblioteca/tads/parte1/BitWriter.hpp"
#include "biblioteca/tads/parte1/Coll.hpp"
#include "biblioteca/tads/parte1/Fecha.hpp"
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

void escribirByte(FILE* f, Byte b)
{
    write<Byte>(f, b);
}

void escribirInteger(FILE* f, Integer i)
{
    write<Integer>(f, i);
}

void escribirString(FILE* f, string s)
{
    Integer longS = integer(length(s));
    if(longS.i > 254)
    {
        Byte flag = byte(255);
        escribirByte(f, flag);
        escribirInteger(f, longS);
        for(int i = 0; i < longS.i; i++)
        {
            escribirByte(f, byte(s[i]));
        }
    }
    else
    {
        escribirByte(f, byte(longS.i));
        for(int i = 0; i < longS.i; i++)
        {
            escribirByte(f, byte(s[i]));
        }
    }
}

void escribirDate(FILE* f)
{
    // temporal
    write<Integer>(f,integer(789));
    // write<Date>(f);
}

void escribirRegType(FILE* f, RegType rT, Integer cntCampos)
{
    regTypeReset(rT);
    Byte k;
    string v;
    for(int i=0; i<cntCampos.i; i++)
    {
        k.b = mapNextKey<unsigned char, string>(rT.mapRegType);
        v = *mapGet<unsigned char, string>(rT.mapRegType, k.b);
        
        escribirByte(f, k);
        escribirString(f, v);
    }
    
}

void escribirRegData(FILE* f, RegData rD, Integer cntContactos)
{
}

#endif