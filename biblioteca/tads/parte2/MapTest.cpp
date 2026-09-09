#include <cassert>
#include <iostream>

#include "Map.hpp"

using std::cout;
using std::endl;

// -- PROTOTIPOS --
void testMap();
void testMapGet();
void testMapPut();
void testMapContains();
void testMapRemove();
void testMapRemoveAll();
void testMapSize();
void testMapHasNext();
void testMapNextKey();
void testMapNextValue();
void testMapReset();
void testMapDiscover();
void testMapSortByKeys();
void testMapSortByValues();

// -- MAIN --
int main()
{
    testMap();
    testMapGet();
    testMapPut();
    testMapContains();
    testMapRemove();
    testMapRemoveAll();
    testMapSize();
    testMapHasNext();
    testMapNextKey();
    testMapNextValue();
    testMapReset();
    testMapDiscover();
    testMapSortByKeys();
    testMapSortByValues();

    cout << "Todos los tests de Map pasaron correctamente." << endl;
    return 0;
}

int cmpIntInt(int a, int b)
{
    return a < b ? -1 : a > b ? 1
                              : 0;
}

Map<int, int> buildMap()
{
    Map<int, int> m = map<int, int>();
    mapPut<int, int>(m, 10, 100);
    mapPut<int, int>(m, 20, 200);
    mapPut<int, int>(m, 30, 300);
    return m;
}

void testMap()
{
    Map<int, int> m = map<int, int>();

    assert((mapSize<int, int>(m) == 0));
    assert((!mapHasNext<int, int>(m)));
}

void testMapGet()
{
    Map<int, int> m = buildMap();

    int* p1 = mapGet<int, int>(m, 10);
    int* p2 = mapGet<int, int>(m, 30);
    int* p3 = mapGet<int, int>(m, 99);

    assert(p1 != NULL && *p1 == 100);
    assert(p2 != NULL && *p2 == 300);
    assert(p3 == NULL);
}

void testMapPut()
{
    Map<int, int> m = map<int, int>();

    int* p1 = mapPut<int, int>(m, 10, 100);
    int* p2 = mapPut<int, int>(m, 10, 999);

    assert(p1 != NULL);
    assert(p2 != NULL);
    assert(*p2 == 999);
    assert((mapSize<int, int>(m) == 1));
    assert((*mapGet<int, int>(m, 10) == 999));
}

void testMapContains()
{
    Map<int, int> m = buildMap();

    assert((mapContains<int, int>(m, 10)));
    assert((mapContains<int, int>(m, 20)));
    assert((!mapContains<int, int>(m, 99)));
}

void testMapRemove()
{
    Map<int, int> m = buildMap();

    int value = mapRemove<int, int>(m, 20);

    assert(value == 200);
    assert((mapSize<int, int>(m) == 2));
    assert((!mapContains<int, int>(m, 20)));
    assert((mapContains<int, int>(m, 10)));
    assert((mapContains<int, int>(m, 30)));
}

void testMapRemoveAll()
{
    Map<int, int> m = buildMap();

    mapRemoveAll<int, int>(m);

    assert((mapSize<int, int>(m) == 0));
    assert((!mapHasNext<int, int>(m)));
}

void testMapSize()
{
    Map<int, int> m = buildMap();
    assert((mapSize<int, int>(m) == 3));
}

void testMapHasNext()
{
    Map<int, int> m = buildMap();

    assert((mapHasNext<int, int>(m)));
    mapNextKey<int, int>(m);
    mapNextKey<int, int>(m);
    mapNextKey<int, int>(m);
    assert((!mapHasNext<int, int>(m)));
}

void testMapNextKey()
{
    Map<int, int> m = buildMap();

    int key1 = mapNextKey<int, int>(m);
    int key2 = mapNextKey<int, int>(m);

    assert(key1 == 10);
    assert(key2 == 20);
}

void testMapNextValue()
{
    Map<int, int> m = buildMap();

    int* value1 = mapNextValue<int, int>(m);
    int* value2 = mapNextValue<int, int>(m);

    assert(value1 != NULL);
    assert(value2 != NULL);
    assert(*value1 == 100);
    assert(*value2 == 200);
}

void testMapReset()
{
    Map<int, int> m = buildMap();
    mapNextKey<int, int>(m);
    mapNextKey<int, int>(m);

    mapReset<int, int>(m);

    assert((mapHasNext<int, int>(m)));
    assert((mapNextKey<int, int>(m) == 10));
}

void testMapDiscover()
{
    Map<int, int> m = buildMap();

    int* p1 = mapDiscover<int, int>(m, 20, 999);
    int* p2 = mapDiscover<int, int>(m, 40, 400);

    assert(p1 != NULL && *p1 == 200);
    assert(p2 != NULL && *p2 == 400);
    assert((mapSize<int, int>(m) == 4));
}

void testMapSortByKeys()
{
    Map<int, int> m = map<int, int>();
    mapPut<int, int>(m, 30, 300);
    mapPut<int, int>(m, 10, 100);
    mapPut<int, int>(m, 20, 200);

    mapSortByKeys<int, int>(m, cmpIntInt);
    mapReset<int, int>(m);

    assert((mapNextKey<int, int>(m) == 10));
    assert((mapNextKey<int, int>(m) == 20));
    assert((mapNextKey<int, int>(m) == 30));
    assert((*mapGet<int, int>(m, 10) == 100));
    assert((*mapGet<int, int>(m, 20) == 200));
    assert((*mapGet<int, int>(m, 30) == 300));
}

void testMapSortByValues()
{
    Map<int, int> m = map<int, int>();
    mapPut<int, int>(m, 10, 300);
    mapPut<int, int>(m, 20, 100);
    mapPut<int, int>(m, 30, 200);

    mapSortByValues<int, int>(m, cmpIntInt);
    mapReset<int, int>(m);

    assert((*mapNextValue<int, int>(m) == 100));
    assert((*mapNextValue<int, int>(m) == 200));
    assert((*mapNextValue<int, int>(m) == 300));
    assert((*mapGet<int, int>(m, 10) == 300));
    assert((*mapGet<int, int>(m, 20) == 100));
    assert((*mapGet<int, int>(m, 30) == 200));
}