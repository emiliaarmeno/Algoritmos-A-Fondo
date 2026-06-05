#ifndef _TCOLL_T_
#define _TCOLL_T_

#include <iostream>

#include "../../funciones/strings.hpp"

using std::string;

template <typename T>
struct Coll
{
    // cadena tokenizada
    string s;
    char sep;
};

template <typename T>
Coll<T> coll(char sep) //inicializar una colección, especificando el caracter separador
{
    Coll<T> c;
    c.s = "";
    c.sep = sep;
    return c;
}

template <typename T>
Coll<T> coll() //sobrecarga harcodeando el caracter separador
{
    Coll<T> c;
    c.s = "";
    c.sep = '|';
    return c;
}

template <typename T>
int collSize(Coll<T> c)
{
    return tokenCount(c.s, c.sep);
}

template <typename T>
void collRemoveAll(Coll<T>& c)
{
    c.s = "";
}

template <typename T>
void collRemoveAt(Coll<T>& c, int p)
{
    removeTokenAt(c.s, c.sep, p);
}

template <typename T>
int collAdd(Coll<T>& c, T t, string tToString(T)) //agregar un elemento al final 
{
    addToken(c.s,c.sep,tToString(t));
    return tokenCount(c.s,c.sep)-1; //devuelve la posición en la cual quedó colocado el elemento que acabamos de agregar
}

template <typename T>
void collSetAt(Coll<T>& c, T t, int p, string tToString(T))
{
    string str = tToString(t);
    setTokenAt(c.s, c.sep, str, p);
}

template <typename T>
T collGetAt(Coll<T> c, int p, T tFromString(string))
{
    T t;
    string str = getTokenAt(c.s, c.sep, p);
    t = tFromString(str);
    return t;
}

template <typename T, typename K>
int collFind(Coll<T> c, K k, int cmpTK(T, K), T tFromString(string)) //devuelve la posicion de la primera ocurrencia de k o un valor negativo si no esta
{
    for(int i=0; i<collSize(c); i++){
        T tEni = collGetAt(c, i, tFromString); //cuando paso una funcion como parametro no le mando los parametrosde ella tambien
        if(cmpTK(tEni, k)==0){
            return i; 
        }
    }
    return -1;
}

template <typename T>
void collSort(Coll<T>& c, int cmpTT(T, T), T tFromString(string), string tToString(T))
{
}

template <typename T>
bool collHasNext(Coll<T> c)
{
    return true;
}

template <typename T>
T collNext(Coll<T>& c, T tFromString(string))
{
    T t;
    return t;
}

template <typename T>
T collNext(Coll<T>& c, bool& endOfColl, T tFromString(string))
{
    T t;
    return t;
}

template <typename T>
void collReset(Coll<T>& c)
{
}

#endif
