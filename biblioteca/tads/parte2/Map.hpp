
#ifndef _TMAP_TAD_
#define _TMAP_TAD_

#include <iostream>

#include "Array.hpp"

using std::string;

template <typename K, typename T>
struct Map
{
};

template <typename K, typename V>
Map<K, V> map()
{
    return {};
}

template <typename K>
int cmpKK(K a, K b)
{
    return 0;
}

template <typename K, typename V>
V* mapGet(Map<K, V> m, K k)
{
    return NULL;
}

template <typename K, typename V>
V* mapPut(Map<K, V>& m, K k, V v)
{
    return NULL;
}

template <typename K, typename V>
bool mapContains(Map<K, V> m, K k)
{
    return true;
}

template <typename K, typename V>
V mapRemove(Map<K, V>& m, K k)
{
    V v;
    return v;
}

template <typename K, typename V>
void mapRemoveAll(Map<K, V>& m)
{
}

template <typename K, typename V>
int mapSize(Map<K, V> m)
{
    return 0;
}

template <typename K, typename V>
bool mapHasNext(Map<K, V> m)
{
    return true;
}

template <typename K, typename V>
K mapNextKey(Map<K, V>& m)
{
    K k;
    return k;
}

template <typename K, typename V>
V* mapNextValue(Map<K, V>& m)
{
    return NULL;
}

template <typename K, typename V>
void mapReset(Map<K, V>& m)
{
}

template <typename K, typename V>
V* mapDiscover(Map<K, V>& m, K k, V v)
{
    return NULL;
}

template <typename K, typename V>
void mapSortByKeys(Map<K, V>& m, int cmpKK(K, K))
{
}

template <typename K, typename V>
void mapSortByValues(Map<K, V>& m, int cmpVV(V, V))
{
}

#endif
