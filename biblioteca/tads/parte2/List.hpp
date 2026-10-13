#ifndef _TLIST_TAD_
#define _TLIST_TAD_

#include <iostream>

#include "../../funciones/lists.hpp"

using std::string;

template <typename T>
struct List
{
    Node<T>* first;   // primer nodo de la lista
    Node<T>* curr;    // nodo actual, para recorrer con listNext
    int size;         // cantidad de elementos
};

template <typename T>
List<T> list()
{
    List<T> lst;
    lst.first = NULL;
    lst.curr = NULL;
    lst.size = 0;
    return lst;
}

template <typename T>
T* listAdd(List<T>& lst, T e)
{
    Node<T>* nodo = add<T>(lst.first, e);
    lst.size++;
    return &(nodo->info);
}

template <typename T>
T* listAddFirst(List<T>& lst, T e)
{
    Node<T>* nodo = addFirst<T>(lst.first, e);
    lst.size++;
    return &(nodo->info);
}

template <typename T, typename K>
T listRemove(List<T>& lst, K k, int cmpTK(T, K))
{
    if( find<T,K>(lst.first, k, cmpTK) != NULL )   // solo si existe
    {
        lst.size--;
    }
    return remove<T,K>(lst.first, k, cmpTK);
}

template <typename T>
T listRemoveFirst(List<T>& lst)
{
    lst.size--;
    return removeFirst<T>(lst.first);
}

template <typename T, typename K>
T* listFind(List<T> lst, K k, int cmpTK(T, K))
{
    Node<T>* nodo = find<T,K>(lst.first, k, cmpTK);
    return nodo==NULL ? NULL : &(nodo->info);   // devuelve la dirección del dato, no del nodo
}

template <typename T>
bool listIsEmpty(List<T> lst)
{
    return isEmpty<T>(lst.first);
}

template <typename T>
int listSize(List<T> lst)
{
    return lst.size;
}

template <typename T>
void listFree(List<T>& lst)
{
    free<T>(lst.first);
    lst.curr = NULL;
    lst.size = 0;
}

template <typename T>
T* listDiscover(List<T>& lst, T t, int cmpTT(T, T))
{
    bool enc;
    Node<T>* nodo = searchAndInsert<T>(lst.first, t, enc, cmpTT);
    if( !enc )
    {
        lst.size++;
    }
    return &(nodo->info);
}

template <typename T>
T* listOrderedInsert(List<T>& lst, T t, int cmpTT(T, T))
{
    Node<T>* nodo = orderedInsert<T>(lst.first, t, cmpTT);
    lst.size++;
    return &(nodo->info);
}

template <typename T>
void listSort(List<T>& lst, int cmpTT(T, T))
{
    sort<T>(lst.first, cmpTT);
}

template <typename T>
void listReset(List<T>& lst)
{
    lst.curr = lst.first;
}

template <typename T>
bool listHasNext(List<T> lst)
{
    return lst.curr != NULL;
}

template <typename T>
T* listNext(List<T>& lst)
{
    T* ret = NULL;
    if( lst.curr != NULL )
    {
        ret = &(lst.curr->info);
        lst.curr = lst.curr->sig;
    }
    return ret;
}

template <typename T>
T* listNext(List<T>& lst, bool& endOfList)
{
    T* ret = NULL;
    if( lst.curr != NULL )
    {
        ret = &(lst.curr->info);
        lst.curr = lst.curr->sig;
    }
    endOfList = (lst.curr == NULL);
    return ret;
}

#endif
