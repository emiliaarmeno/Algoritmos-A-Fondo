#include <cassert>
#include <iostream>

#include "List.hpp"

using std::cout;
using std::endl;

// -- PROTOTIPOS --
void testList();
void testListAdd();
void testListAddFirst();
void testListRemove();
void testListRemoveFirst();
void testListFind();
void testListIsEmpty();
void testListSize();
void testListFree();
void testListDiscover();
void testListOrderedInsert();
void testListSort();
void testListReset();
void testListHasNext();
void testListNext();
void testListNext_withEndOfList();

// -- MAIN --
int main()
{
    testList();
    // testListAdd();
    // testListAddFirst();
    // testListRemove();
    // testListRemoveFirst();
    // testListFind();
    // testListIsEmpty();
    // testListSize();
    // testListFree();
    // testListDiscover();
    // testListOrderedInsert();
    // testListSort();
    // testListReset();
    // testListHasNext();
    // testListNext();
    // testListNext_withEndOfList();

    cout << "Todos los tests de List pasaron correctamente." << endl;
    return 0;
}



int cmpIntInt(int a, int b)
{
    return a < b ? -1 : a > b ? 1 : 0;
}

List<int> buildList()
{
    List<int> lst = list<int>();
    listAdd<int>(lst, 10);
    listAdd<int>(lst, 20);
    listAdd<int>(lst, 30);
    return lst;
}

void testList()
{
    List<int> lst = list<int>();

    assert(listSize<int>(lst) == 0);
    assert(listIsEmpty<int>(lst));
    assert(!listHasNext<int>(lst));
}

void testListSize()
{
    List<int> lst = buildList();
    assert(listSize<int>(lst) == 3);
}

void testListAdd()
{
    List<int> lst = list<int>();

    int* p1 = listAdd<int>(lst, 10);
    int* p2 = listAdd<int>(lst, 20);

    assert(p1 != NULL);
    assert(p2 != NULL);
    assert(*p1 == 10);
    assert(*p2 == 20);
    assert(listSize<int>(lst) == 2);

    listReset<int>(lst);
    assert(*listNext<int>(lst) == 10);
    assert(*listNext<int>(lst) == 20);
}

void testListAddFirst()
{
    List<int> lst = buildList();

    int* p = listAddFirst<int>(lst, 5);

    assert(p != NULL);
    assert(*p == 5);
    assert(listSize<int>(lst) == 4);
    listReset<int>(lst);
    assert(*listNext<int>(lst) == 5);
}

void testListRemove()
{
    List<int> lst = buildList();

    int removed = listRemove<int, int>(lst, 20, cmpIntInt);

    assert(removed == 20);
    assert(listSize<int>(lst) == 2);
    listReset<int>(lst);
    assert(*listNext<int>(lst) == 10);
    assert(*listNext<int>(lst) == 30);
}

void testListRemoveFirst()
{
    List<int> lst = buildList();

    int removed = listRemoveFirst<int>(lst);

    assert(removed == 10);
    assert(listSize<int>(lst) == 2);
    listReset<int>(lst);
    assert(*listNext<int>(lst) == 20);
    assert(*listNext<int>(lst) == 30);
}

void testListFind()
{
    List<int> lst = buildList();

    int* p1 = listFind<int, int>(lst, 10, cmpIntInt);
    int* p2 = listFind<int, int>(lst, 30, cmpIntInt);
    int* p3 = listFind<int, int>(lst, 99, cmpIntInt);

    assert(p1 != NULL && *p1 == 10);
    assert(p2 != NULL && *p2 == 30);
    assert(p3 == NULL);
}

void testListIsEmpty()
{
    List<int> lst = list<int>();
    assert(listIsEmpty<int>(lst));

    listAdd<int>(lst, 1);
    assert(!listIsEmpty<int>(lst));
}

void testListFree()
{
    List<int> lst = buildList();

    listFree<int>(lst);

    assert(listSize<int>(lst) == 0);
    assert(listIsEmpty<int>(lst));
    assert(!listHasNext<int>(lst));
}

void testListDiscover()
{
    List<int> lst = buildList();

    int* p1 = listDiscover<int>(lst, 20, cmpIntInt);
    int* p2 = listDiscover<int>(lst, 40, cmpIntInt);

    assert(p1 != NULL && *p1 == 20);
    assert(p2 != NULL && *p2 == 40);
    assert(listSize<int>(lst) == 4);
}

void testListOrderedInsert()
{
    List<int> lst = list<int>();

    listOrderedInsert<int>(lst, 20, cmpIntInt);
    listOrderedInsert<int>(lst, 10, cmpIntInt);
    listOrderedInsert<int>(lst, 30, cmpIntInt);
    int* p = listOrderedInsert<int>(lst, 25, cmpIntInt);

    assert(p != NULL);
    assert(*p == 25);
    assert(listSize<int>(lst) == 4);
    listReset<int>(lst);
    assert(*listNext<int>(lst) == 10);
    assert(*listNext<int>(lst) == 20);
    assert(*listNext<int>(lst) == 25);
    assert(*listNext<int>(lst) == 30);
}

void testListSort()
{
    List<int> lst = list<int>();
    listAdd<int>(lst, 40);
    listAdd<int>(lst, 10);
    listAdd<int>(lst, 30);
    listAdd<int>(lst, 20);

    listSort<int>(lst, cmpIntInt);

    listReset<int>(lst);
    assert(*listNext<int>(lst) == 10);
    assert(*listNext<int>(lst) == 20);
    assert(*listNext<int>(lst) == 30);
    assert(*listNext<int>(lst) == 40);
}

void testListReset()
{
    List<int> lst = buildList();
    listReset<int>(lst);
    listNext<int>(lst);

    listReset<int>(lst);

    assert(listHasNext<int>(lst));
    assert(*listNext<int>(lst) == 10);
}

void testListHasNext()
{
    List<int> lst = buildList();

    listReset<int>(lst);
    assert(listHasNext<int>(lst));

    listNext<int>(lst);
    listNext<int>(lst);
    listNext<int>(lst);
    assert(!listHasNext<int>(lst));
}

void testListNext()
{
    List<int> lst = buildList();
    listReset<int>(lst);

    int* p = listNext<int>(lst);

    assert(p != NULL);
    assert(*p == 10);
    assert(listHasNext<int>(lst));
    assert(*listNext<int>(lst) == 20);
}

void testListNext_withEndOfList()
{
    List<int> lst = buildList();
    listReset<int>(lst);
    bool endOfList = false;

    int* p1 = listNext<int>(lst, endOfList);
    int* p2 = listNext<int>(lst, endOfList);
    int* p3 = listNext<int>(lst, endOfList);
    int* p4 = listNext<int>(lst, endOfList);

    assert(p1 != NULL && *p1 == 10);
    assert(p2 != NULL && *p2 == 20);
    assert(p3 != NULL && *p3 == 30);
    assert(p4 == NULL);
    assert(endOfList);
}
