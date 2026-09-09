#include <cassert>
#include <iostream>

#include "Array.hpp"

using std::cout;
using std::endl;

// -- PROTOTIPOS --
void testArray();
void testArrayAdd();
void testArrayGet();
void testArraySet();
void testArrayInsert();
void testArraySize();
void testArrayRemove();
void testArrayRemoveAll();
void testArrayFind();
void testArrayOrderedInsert();
void testArrayDiscover();
void testArraySort();
void testArrayReset();
void testArrayHasNext();
void testArrayNext();

// -- MAIN --
int main()
{
    testArray();
    testArrayAdd();
    testArrayGet();
    testArraySet();
    testArrayInsert();
    testArraySize();
    testArrayRemove();
    testArrayRemoveAll();
    testArrayFind();
    testArrayOrderedInsert();
    testArrayDiscover();
    testArraySort();
    testArrayReset();
    testArrayHasNext();
    testArrayNext();

    cout << "Todos los tests de Array pasaron correctamente." << endl;
    return 0;
}

int cmpIntInt(int a, int b)
{
    return a < b ? -1 : a > b ? 1
                              : 0;
}

Array<int> buildArray()
{
    Array<int> a = array<int>();
    arrayAdd<int>(a, 10);
    arrayAdd<int>(a, 20);
    arrayAdd<int>(a, 30);
    return a;
}

void testArray()
{
    Array<int> a = array<int>();

    assert(arraySize<int>(a) == 0);
    assert(!arrayHasNext<int>(a));
}

void testArrayAdd()
{
    Array<int> a = array<int>();

    int p0 = arrayAdd<int>(a, 10);
    int p1 = arrayAdd<int>(a, 20);

    assert(p0 == 0);
    assert(p1 == 1);
    assert(arraySize<int>(a) == 2);
    assert(*arrayGet<int>(a, 0) == 10);
    assert(*arrayGet<int>(a, 1) == 20);
}

void testArrayGet()
{
    Array<int> a = buildArray();
    int* p = arrayGet<int>(a, 1);

    assert(p != NULL);
    assert(*p == 20);
}

void testArraySet()
{
    Array<int> a = buildArray();

    arraySet<int>(a, 1, 99);

    assert(*arrayGet<int>(a, 0) == 10);
    assert(*arrayGet<int>(a, 1) == 99);
    assert(*arrayGet<int>(a, 2) == 30);
}

void testArrayInsert()
{
    Array<int> a = buildArray();

    arrayInsert<int>(a, 15, 1);

    assert(arraySize<int>(a) == 4);
    assert(*arrayGet<int>(a, 0) == 10);
    assert(*arrayGet<int>(a, 1) == 15);
    assert(*arrayGet<int>(a, 2) == 20);
    assert(*arrayGet<int>(a, 3) == 30);
}

void testArraySize()
{
    Array<int> a = buildArray();
    assert(arraySize<int>(a) == 3);
}

void testArrayRemove()
{
    Array<int> a = buildArray();

    int removed = arrayRemove<int>(a, 1);

    assert(removed == 20);
    assert(arraySize<int>(a) == 2);
    assert(*arrayGet<int>(a, 0) == 10);
    assert(*arrayGet<int>(a, 1) == 30);
}

void testArrayRemoveAll()
{
    Array<int> a = buildArray();

    arrayRemoveAll<int>(a);

    assert(arraySize<int>(a) == 0);
    assert(!arrayHasNext<int>(a));
}

void testArrayFind()
{
    Array<int> a = buildArray();

    assert((arrayFind<int, int>(a, 10, cmpIntInt) == 0));
    assert((arrayFind<int, int>(a, 20, cmpIntInt) == 1));
    assert((arrayFind<int, int>(a, 99, cmpIntInt) == -1));
}

void testArrayOrderedInsert()
{
    Array<int> a = array<int>();
    arrayAdd<int>(a, 10);
    arrayAdd<int>(a, 20);
    arrayAdd<int>(a, 40);

    int p = arrayOrderedInsert<int>(a, 30, cmpIntInt);

    assert(p == 2);
    assert(arraySize<int>(a) == 4);
    assert(*arrayGet<int>(a, 0) == 10);
    assert(*arrayGet<int>(a, 1) == 20);
    assert(*arrayGet<int>(a, 2) == 30);
    assert(*arrayGet<int>(a, 3) == 40);
}

void testArrayDiscover()
{
    Array<int> a = buildArray();

    int* p1 = arrayDiscover<int>(a, 20, cmpIntInt);
    int* p2 = arrayDiscover<int>(a, 40, cmpIntInt);

    assert(p1 != NULL);
    assert(*p1 == 20);
    assert(p2 != NULL);
    assert(*p2 == 40);
    assert(arraySize<int>(a) == 4);
    assert(*arrayGet<int>(a, 3) == 40);
}

void testArraySort()
{
    Array<int> a = array<int>();
    arrayAdd<int>(a, 40);
    arrayAdd<int>(a, 10);
    arrayAdd<int>(a, 30);
    arrayAdd<int>(a, 20);

    arraySort<int>(a, cmpIntInt);

    assert(*arrayGet<int>(a, 0) == 10);
    assert(*arrayGet<int>(a, 1) == 20);
    assert(*arrayGet<int>(a, 2) == 30);
    assert(*arrayGet<int>(a, 3) == 40);
}

void testArrayReset()
{
    Array<int> a = buildArray();
    arrayNext<int>(a);
    arrayNext<int>(a);

    arrayReset<int>(a);

    assert(arrayHasNext<int>(a));
    assert(*arrayNext<int>(a) == 10);
}

void testArrayHasNext()
{
    Array<int> a = buildArray();

    assert(arrayHasNext<int>(a));
    arrayNext<int>(a);
    arrayNext<int>(a);
    arrayNext<int>(a);
    assert(!arrayHasNext<int>(a));
}

void testArrayNext()
{
    Array<int> a = buildArray();

    int* p = arrayNext<int>(a);

    assert(p != NULL);
    assert(*p == 10);
    assert(arrayHasNext<int>(a));
    assert(*arrayNext<int>(a) == 20);
}