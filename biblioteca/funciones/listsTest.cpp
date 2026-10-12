#include <cassert>
#include <iostream>

#include "lists.hpp"

using std::cout;
using std::endl;

// -- PROTOTIPOS --
void testAdd();
void testAddFirst();
void testRemove();
void testRemoveFirst();
void testFind();
void testOrderedInsert();
void testSearchAndInsert();
void testSort();
void testIsEmpty();
void testFree();
void testPush();
void testPop();
void testEnqueue_withTwoPointers();
void testEnqueue_withCircularQueue();
void testDequeue_withTwoPointers();
void testDequeue_withCircularQueue();

// -- MAIN --
int main()
{
    testAdd();
    // testAddFirst();
    // testRemove();
    // testRemoveFirst();
    // testFind();
    // testOrderedInsert();
    // testSearchAndInsert();
    // testSort();
    // testIsEmpty();
    // testFree();
    // testPush();
    // testPop();
    // testEnqueue_withTwoPointers();
    // testEnqueue_withCircularQueue();
    // testDequeue_withTwoPointers();
    // testDequeue_withCircularQueue();

    cout << "Todos los tests de lists pasaron correctamente." << endl;
    return 0;
}

int cmpIntInt(int a, int b)
{
    return a < b ? -1 : a > b ? 1
                              : 0;
}

void assertListEquals(Node<int>* p, const int expected[], int n)
{
    for(int i = 0; i < n; i++)
    {
        assert(p != NULL);
        assert(p->info == expected[i]);
        p = p->sig;
    }
    assert(p == NULL);
}

void testAdd()
{
    Node<int>* p = NULL;

    Node<int>* n1 = add<int>(p, 10);
    Node<int>* n2 = add<int>(p, 20);

    int expected[] = {10, 20};
    assert(n1 != NULL);
    assert(n2 != NULL);
    assert(p == n1);
    assert(n1->sig == n2);
    assertListEquals(p, expected, 2);

    free<int>(p);
}

void testAddFirst()
{
    Node<int>* p = NULL;

    add<int>(p, 20);
    Node<int>* n = addFirst<int>(p, 10);

    int expected[] = {10, 20};
    assert(n != NULL);
    assert(p == n);
    assertListEquals(p, expected, 2);

    free<int>(p);
}

void testRemove()
{
    Node<int>* p = NULL;

    add<int>(p, 10);
    add<int>(p, 20);
    add<int>(p, 30);

    int removed = remove<int, int>(p, 20, cmpIntInt);
    int expected[] = {10, 30};

    assert(removed == 20);
    assertListEquals(p, expected, 2);

    free<int>(p);
}

void testRemoveFirst()
{
    Node<int>* p = NULL;

    add<int>(p, 10);
    add<int>(p, 20);
    add<int>(p, 30);

    int removed = removeFirst<int>(p);
    int expected[] = {20, 30};

    assert(removed == 10);
    assertListEquals(p, expected, 2);

    free<int>(p);
}

void testFind()
{
    Node<int>* p = NULL;

    add<int>(p, 10);
    add<int>(p, 20);
    add<int>(p, 30);

    Node<int>* found = find<int, int>(p, 20, cmpIntInt);
    Node<int>* missing = find<int, int>(p, 99, cmpIntInt);

    assert(found != NULL);
    assert(found->info == 20);
    assert(missing == NULL);

    free<int>(p);
}

void testOrderedInsert()
{
    Node<int>* p = NULL;

    orderedInsert<int>(p, 20, cmpIntInt);
    orderedInsert<int>(p, 10, cmpIntInt);
    Node<int>* inserted = orderedInsert<int>(p, 30, cmpIntInt);

    int expected[] = {10, 20, 30};
    assert(inserted != NULL);
    assert(inserted->info == 30);
    assertListEquals(p, expected, 3);

    free<int>(p);
}

void testSearchAndInsert()
{
    Node<int>* p = NULL;

    orderedInsert<int>(p, 10, cmpIntInt);
    orderedInsert<int>(p, 30, cmpIntInt);

    bool enc = false;
    Node<int>* inserted = searchAndInsert<int>(p, 20, enc, cmpIntInt);
    int expected[] = {10, 20, 30};

    assert(!enc);
    assert(inserted != NULL);
    assert(inserted->info == 20);
    assertListEquals(p, expected, 3);

    free<int>(p);
}

void testSort()
{
    Node<int>* p = NULL;

    add<int>(p, 40);
    add<int>(p, 10);
    add<int>(p, 30);
    add<int>(p, 20);

    sort<int>(p, cmpIntInt);

    int expected[] = {10, 20, 30, 40};
    assertListEquals(p, expected, 4);

    free<int>(p);
}

void testIsEmpty()
{
    Node<int>* p = NULL;
    assert(isEmpty<int>(p));

    add<int>(p, 1);
    assert(!isEmpty<int>(p));

    free<int>(p);
}

void testFree()
{
    Node<int>* p = NULL;

    add<int>(p, 1);
    add<int>(p, 2);

    free<int>(p);

    assert(p == NULL);
    assert(isEmpty<int>(p));
}

void testPush()
{
    Node<int>* p = NULL;

    Node<int>* pushed = push<int>(p, 10);
    push<int>(p, 20);

    int expected[] = {20, 10};
    assert(pushed != NULL);
    assert(pushed->info == 10);
    assertListEquals(p, expected, 2);

    free<int>(p);
}

void testPop()
{
    Node<int>* p = NULL;

    push<int>(p, 10);
    push<int>(p, 20);

    int value = pop<int>(p);
    int expected[] = {10};

    assert(value == 20);
    assertListEquals(p, expected, 1);

    free<int>(p);
}

void testEnqueue_withTwoPointers()
{
    Node<int>* p = NULL;
    Node<int>* q = NULL;

    Node<int>* inserted = enqueue<int>(p, q, 10);
    enqueue<int>(p, q, 20);
    enqueue<int>(p, q, 30);

    int expected[] = {10, 20, 30};
    assert(inserted != NULL);
    assert(p != NULL);
    assert(q != NULL);
    assert(inserted == p);
    assert(q->info == 30);
    assertListEquals(p, expected, 3);

    free<int>(p);
    q = NULL;
}

void testEnqueue_withCircularQueue()
{
    Node<int>* q = NULL;

    Node<int>* inserted = enqueue<int>(q, 10);
    enqueue<int>(q, 20);
    enqueue<int>(q, 30);

    assert(inserted != NULL);
    assert(q != NULL);
    assert(q->info == 30);
    assert(q->sig->info == 10);
    assert(q->sig->sig->info == 20);
    assert(q->sig->sig->sig == q);

    while(q != NULL)
    {
        dequeue<int>(q);
    }
}

void testDequeue_withTwoPointers()
{
    Node<int>* p = NULL;
    Node<int>* q = NULL;

    enqueue<int>(p, q, 10);
    enqueue<int>(p, q, 20);

    int value = dequeue<int>(p, q);

    assert(value == 10);
    assert(p != NULL);
    assert(q != NULL);
    assert(p->info == 20);
    assert(q->info == 20);

    free<int>(p);
    q = NULL;
}

void testDequeue_withCircularQueue()
{
    Node<int>* q = NULL;

    enqueue<int>(q, 10);
    enqueue<int>(q, 20);

    int value = dequeue<int>(q);

    assert(value == 10);
    assert(q != NULL);
    assert(q->info == 20);
    assert(q->sig == q);

    dequeue<int>(q);
    assert(q == NULL);
}
