#ifndef _TARR_TAD_
#define _TARR_TAD_

#include <iostream>
#include <stdlib.h>
#include "../../funciones/arrays.hpp"

using std::string;

template<typename T>
struct Array
{
};

template<typename T>
Array<T> array()
{
   return {};
}

template<typename T>
int arrayAdd(Array<T>& a,T t)
{
    return 0;
}

template<typename T>
T* arrayGet(Array<T> a,int p)
{
   return NULL;
}

template<typename T>
void arraySet(Array<T>& a,int p,T t)
{
}

template<typename T>
void arrayInsert(Array<T>& a,T t,int p)
{
}

template<typename T>
int arraySize(Array<T> a)
{
   return 0;
}

template<typename T>
T arrayRemove(Array<T>& a,int p)
{
   return {};
}

template<typename T>
void arrayRemoveAll(Array<T>& a)
{
}

template<typename T,typename K>
int arrayFind(Array<T> a,K k,int cmpTK(T,K))
{
   return 0;
}

template<typename T>
int arrayOrderedInsert(Array<T>& a,T t,int cmpTT(T,T))
{
   return 0;
}

template<typename T>
T* arrayDiscover(Array<T>& a,T t,int cmpTT(T,T))
{
   return NULL;
}

template<typename T>
void arraySort(Array<T>& a,int cmpTT(T,T))
{
}

template<typename T>
void arrayReset(Array<T>& a)
{
}

template<typename T>
bool arrayHasNext(Array<T>& a)
{
    return true;
}

template<typename T>
T* arrayNext(Array<T>& a)
{
    return NULL;
}

#endif
