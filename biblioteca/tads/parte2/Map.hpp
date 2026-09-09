
#ifndef _TMAP_TAD_
#define _TMAP_TAD_

#include <iostream>

#include "Array.hpp"

using std::string;

template <typename K, typename T>
struct Map
{
    Array<K> keys; //tipo de dato primitivo
    Array<T> values;
    int curr;
};

template <typename K, typename V>
Map<K, V> map()
{
   Map<K,V> m;
   m.keys = array<K>();
   m.values=array<V>();
   m.curr=0;
   return m;
}

template <typename K>
int cmpKK(K a, K b)
{
    return a<b?-1:a==b?0:1;
}

template <typename K, typename V>
V* mapGet(Map<K, V> m, K k)
{
    int p = arrayFind<K,K>(m.keys,k,cmpKK);
    return p>=0?arrayGet<V>(m.values,p):NULL;
}

template <typename K, typename V>
V* mapPut(Map<K, V>& m, K k, V v)
{
   int p = arrayFind<K,K>(m.keys,k,cmpKK);
   if( p<0 ) //no lo encontro 
   {
      arrayAdd<K>(m.keys,k);
      arrayAdd<V>(m.values,v);
   }
   else
   {
      arraySet<V>(m.values,p,v); //ya hay alguien asoc con esa key, cambio el value 
   }

   return mapGet<K,V>(m,k);
}

template <typename K, typename V>
bool mapContains(Map<K, V> m, K k)
{
   return mapGet<K,V>(m,k)!=NULL;
}

template <typename K, typename V>
V mapRemove(Map<K, V>& m, K k)
{
    V v;
    int p = arrayFind<V,K>(m.keys,k,cmpKK);
    arrayRemove<K>(m.keys,p);
    v = arrayRemove<V>(m.values,p);
    return v;
}

template <typename K, typename V>
void mapRemoveAll(Map<K, V>& m)
{
    arrayRemoveAll<K>(m.keys);
    arrayRemoveAll<V>(m.values);
}

template <typename K, typename V>
int mapSize(Map<K, V> m)
{
    return arraySize<K>(m.keys);
}

template <typename K, typename V>
bool mapHasNext(Map<K, V> m)
{
   int x = arraySize<K>(m.keys);
   return m.curr<x;
}

template <typename K, typename V>
K mapNextKey(Map<K, V>& m)
{
    return *arrayGet<K>(m.keys,m.curr++);
}

template <typename K, typename V>
V* mapNextValue(Map<K, V>& m)
{
    return arrayGet<V>(m.values,m.curr++);
}

template <typename K, typename V>
void mapReset(Map<K, V>& m)
{
    m.curr=0;   
}

template <typename K, typename V>
V* mapDiscover(Map<K, V>& m, K k, V v)
{  
    V* x = mapGet<K,V>(m,k);
    if( x==NULL )
    {
        x = mapPut<K,V>(m,k,v);
    }

    return x;
}

template <typename K, typename V>
void mapSortByKeys(Map<K, V>& m, int cmpKK(K, K))
{
    int n = mapSize<K,V>(m);
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n-1; j++)
        {
            K k0 = *arrayGet<K>(m.keys,j);
            K k1 = *arrayGet<K>(m.keys,j+1);
            if( cmpKK(k0,k1)>0 )
            {
            arraySet<K>(m.keys,j,k1);
            arraySet<K>(m.keys,j+1,k0);

            V v0 = *arrayGet<V>(m.values,j);
            V v1 = *arrayGet<V>(m.values,j+1);

            arraySet<V>(m.values,j,v1);
            arraySet<V>(m.values,j+1,v0);
            }
        }
    }
}

template <typename K, typename V>
void mapSortByValues(Map<K, V>& m, int cmpVV(V, V))
{
    int n = mapSize<K,V>(m);
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n-1; j++)
        {
            V v0 = *arrayGet<V>(m.values,j);
            V v1 = *arrayGet<V>(m.values,j+1);
            if( cmpVV(v0,v1)>0 )
            {
            arraySet<V>(m.values,j,v1);
            arraySet<V>(m.values,j+1,v0);

            K k0 = *arrayGet<K>(m.keys,j);
            K k1 = *arrayGet<K>(m.keys,j+1);

            arraySet<K>(m.keys,j,k1);
            arraySet<K>(m.keys,j+1,k0);
            }
        }
    }
}

#endif
