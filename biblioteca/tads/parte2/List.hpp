#ifndef _TLIST_TAD_
#define _TLIST_TAD_

#include <iostream>

#include "../../funciones/lists.hpp"

using std::string;

template <typename T>
struct List
{
};

template <typename T>
List<T> list()
{
    return {};
}

template <typename T>
T* listAdd(List<T>& lst, T e)
{
    return NULL;
}

template <typename T>
T* listAddFirst(List<T>& lst, T e)
{
    return NULL;
}

template <typename T, typename K>
T listRemove(List<T>& lst, K k, int cmpTK(T, K))
{
    return {};
}

template <typename T>
T listRemoveFirst(List<T>& lst)
{
    return {};
}

template <typename T, typename K>
T* listFind(List<T> lst, K k, int cmpTK(T, K))
{
    return NULL;
}

template <typename T>
bool listIsEmpty(List<T> lst)
{
    return true;
}

template <typename T>
int listSize(List<T> lst)
{
    return 0;
}

template <typename T>
void listFree(List<T>& lst)
{
}

template <typename T>
T* listDiscover(List<T>& lst, T t, int cmpTT)
{
    return NULL;
}

template <typename T>
T* listOrderedInsert(List<T>& lst, T t, int cmpTT(T, T))
{
    return NULL;
}

template <typename T>
void listSort(List<T>& lst, int cmpTT(T, T))
{
}

template <typename T>
void listReset(List<T>& lst)
{
}

template <typename T>
bool listHasNext(List<T> lst)
{
    return true;
}

template <typename T>
T* listNext(List<T>& lst)
{
    return NULL;
}

template <typename T>
T* listNext(List<T>& lst, bool& endOfList)
{
    return NULL;
}

#endif
