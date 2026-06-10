#ifndef _TFILES_T_
#define _TFILES_T_

#include <stdio.h>
#include <strings.hpp>
#include <iostream>

template <typename T>
void write(FILE* f, T t) //graba el valor t en el archivo f, siendo éste un archivo de registros tipo T, de longitud fija.
{
    fwrite(&t,sizeof(T),1,f);
}

template <typename T>
T read(FILE* f)
{
    T t;
    return t;
}

template <typename T>
void seek(FILE* f, int n)
{
}

template <typename T>
int fileSize(FILE* f)
{
    return 0;
}

template <typename T>
int filePos(FILE* f)
{
    return 0;
}

bool readLine(FILE* f,string& ln)
{
    return false;
}

bool readWord(FILE* f,string& w,string alsoAllowed)
{
    return false;
}

bool readWord(FILE* f,string& w)
{
    return false;
}

void writeString(FILE* f,string s)
{
}

void writeWord(FILE* f,string w)
{
}

void writeLine(FILE* f,string ln)
{
}

#endif
