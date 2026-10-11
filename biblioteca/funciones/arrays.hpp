#ifndef _TARRAYS_T_
#define _TARRAYS_T_

#include <iostream>
#include <cassert>

template <typename T>
int add(T arr[],int& len,T e)
{
    assert(len>=0 && "len debe ser >=0");
    arr[len++]=e;
    return len; //devuelvo el indice de lo que acabo de agregar 
}

template <typename T>
void insert(T arr[],int& len,T e,int p)
{
    assert(len>=0 && "len debe ser >=0");
    assert(p>=0 && p<=len && "len debe ser >=0");

    for(int i=len-1; i>=p; i--){
        arr[i+1]=arr[i];
    }
    arr[p]=e;
    len++;
}

template <typename T>
T remove(T arr[],int& len,int p)
{
   T ret= arr[p];
   for(int i=p+1; i<len; i++){
    arr[i-1]=arr[i];
   }
   len--;
   return ret;
}

template <typename T, typename K>
int find(T arr[],int len,K k,int cmpTK(T,K))
{
   int i=0;
   while( i<len && cmpTK(arr[i],k)!=0 )
   {
      i++;
   }

   return i<len?i:-1;

}

template <typename T>
int orderedInsert(T arr[],int& len,T e,int cmpTT(T,T))
{
    int i=0;
    while( i<len && cmpTT(arr[i],e)<=0 )
    {
        i++;
    }

    insert<T>(arr,len,e,i);

    return i;

}

template <typename T>
void sort(T arr[],int len,int cmpTT(T,T))
{ 
    bool ordenado=false;
    while(!ordenado)
    {
        ordenado=true;
        for(int i=0; i<len-1; i++)
        {
            if( cmpTT(arr[i],arr[i+1])>0 )
            {
            T aux = arr[i];
            arr[i] = arr[i+1];
            arr[i+1] = aux;
            ordenado = false;
            }
        }
    }
}

#endif
