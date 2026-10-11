#ifndef _TARR_TAD_
#define _TARR_TAD_

#include <iostream>
#include <stdlib.h>
#include "../../funciones/arrays.hpp"

using std::string;

template<typename T>
struct Array
{
   T* a;
   int len;
   int cap;
   int curr;
};

template<typename T>
Array<T> array()
{
   Array<T> a;
   a.len=0;
   a.cap = 100;
   a.a = new T[a.cap];
   a.curr = 0;
   return a;
}


//redimensiono el array, creo uno con mas capacidad y elimino el anterior 
//hago que el anterior apunte al nuevo, esto para agregar mas capacidad 
template<typename T>
void _arrayResize(Array<T>& a,int size)
{
   T* b = new T[size]; 
   for(int i=0;i<a.len;i++)
   {
      b[i]=a.a[i];
   }

   delete[] a.a;

   a.a = b;
   a.cap = size;
}

template<typename T>
int arrayAdd(Array<T>& a,T t)
{
    if( a.len>=a.cap ) //si pretendo agregar un elemento que no entra porque no me da la capacidad 
    {
       _arrayResize(a,a.cap*2); //redimensiono al doble de capacidad 
    }
    
    return add<T>(a.a,a.len,t);
}

template<typename T>
T* arrayGet(Array<T> a,int p)
{
    return &(a.a[p]); //devuelvo la direccion
}

template<typename T>
void arraySet(Array<T>& a,int p,T t)
{
    a.a[p]=t;
}

template<typename T>
void arrayInsert(Array<T>& a,T t,int p)
{
    if( a.len>=a.cap )
    {
        _arrayResize(a,a.cap*2);
    }

   insert<T>(a.a,a.len,t,p);
}

template<typename T>
int arraySize(Array<T> a)
{
   return a.len;
}

template<typename T>
T arrayRemove(Array<T>& a,int p)
{
   return remove<T>(a.a,a.len,p);
}

template<typename T>
void arrayRemoveAll(Array<T>& a)
{
    a.len=0;
}

template<typename T,typename K>
int arrayFind(Array<T> a,K k,int cmpTK(T,K))
{
   return find<T,K>(a.a,a.len,k,cmpTK);
}

template<typename T>
int arrayOrderedInsert(Array<T>& a,T t,int cmpTT(T,T))
{
   return orderedInsert<T>(a.a,a.len,t,cmpTT);
}

template<typename T>
T* arrayDiscover(Array<T>& a,T t,int cmpTT(T,T))
{
   int p = arrayFind<T,T>(a,t,cmpTT);
   if( p<0 )
   {
      p = arrayAdd<T>(a,t);
   }

   return arrayGet<T>(a,p);
}

template<typename T>
void arraySort(Array<T>& a,int cmpTT(T,T))
{
    sort<T>(a.a,a.len,cmpTT);
}

template<typename T>
void arrayReset(Array<T>& a)
{
     a.curr = 0;
}

template<typename T>
bool arrayHasNext(Array<T>& a)
{
   return a.curr<a.len;
}

template<typename T>
T* arrayNext(Array<T>& a)
{
    T* x = arrayGet<T>(a,a.curr);
    a.curr++;
    return x;
}

#endif
