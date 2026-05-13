#ifndef _TTOKENS_T_
#define _TTOKENS_T_

#include <stdio.h>

#include <iostream>

#include "strings.hpp"
using std::string;

int tokenCount(string s, char sep)
{
    // si la cadena esta vacia devuelve cero
    if(s == "")
    {
        return 0;
    }
    int cont = 0;
    int pos = indexOf(s, sep);
    while(pos != -1)
    {  // pudo encontrar la posicion del separador
        pos = indexOf(
            s, sep,
            pos + 1);  // vuelvo a buscar desde donde dejo con offset pos+1
        cont++;
    }
    // salio porque ya no encontro sep en la cadena
    return cont + 1;  // si hay n separadores van a haber n+1 tokens

    // tambien funciona con
    // return isEmpty(s)?0:charCount(s,sep)+1;
}

void addToken(string& s, char sep,
              string t)  // pasan por referencia la cadena tokenizada
{
    if(isEmpty(s))
    {  // si esta vacia no pone el separador al principio
        s = t;
    }
    else
    {
        s = s + sep + t;
    }
    // funciona con s = isEmpty(s)?t:s+sep+t;
}

string getTokenAt(string s, char sep, int i)
{
    int posInicial;
    if(i == 0)
    {
        posInicial = 0;
    }
    else
    {
        posInicial = indexOfN(s, sep, i) + 1;
    }

    int posFinal = indexOfN(s, sep, i + 1);

    return substring(s, posInicial, posFinal);
}

void removeTokenAt(string& s, char sep, int i)
{
    int posInicial;
    if(i == 0)
    {
        posInicial = 0;
    }
    else
    {
        posInicial = indexOfN(s, sep, i) +
                     1;  // busco donde esta el separador por la iesima vez
    }

    int posFinal =
        indexOfN(s, sep, i + 1);  // busco la posicion del separador numero i+1

    s = substring(s, 0, posInicial) +
        substring(s, posFinal, length(s));  // saco el token y el separador

    // verifico si quedo algun caracter separador en el medio al principio o al
    // final
    if(posInicial > 0 && s[posInicial - 1] == sep && s[posInicial] == sep)
    {
        s = substring(s, 0, posInicial) +
            substring(s, posInicial + 1, length(s));
    }

    if(s[0] == sep)
    {
        s = substring(s, 1, length(s));
    }

    if(s[length(s) - 1] == sep)
    {
        s = substring(s, 0, length(s) - 1);
    }
}

void setTokenAt(string& s, char sep, string t, int i)
{
    int posInicial;
    if(i == 0)
    {
        posInicial = 0;
    }
    else
    {
        posInicial = indexOfN(s, sep, i) + 1;
    }

    int posFinal = indexOfN(s, sep, i + 1);

    s = substring(s, 0, posInicial) + t + substring(s, posFinal, length(s));
}

int findToken(string s, char sep, string t)
{
    int i = 0;
    string token = getTokenAt(s, sep, i);

    while(token != "" && i <= length(s))
    {
        if(token == t)
        {
            return i;
        }

        i++;
        token = getTokenAt(s, sep, i);
    }

    return -1;
}

#endif
