#include <cassert>
#include <iostream>

#include "arrays.hpp"

using std::cout;
using std::endl;

int cmpIntInt(int a, int b)
{
   return a < b ? -1 : a > b ? 1 : 0;
}

void testAdd()
{
   int arr[10];
   int len = 0;

   assert(add<int>(arr, len, 10) == 0);
   assert(add<int>(arr, len, 20) == 1);
   assert(add<int>(arr, len, 30) == 2);
   assert(len == 3);
   assert(arr[0] == 10);
   assert(arr[1] == 20);
   assert(arr[2] == 30);
}

void testInsert()
{
   int arr[10] = {10, 30, 40};
   int len = 3;

   insert<int>(arr, len, 20, 1);
   assert(len == 4);
   assert(arr[0] == 10);
   assert(arr[1] == 20);
   assert(arr[2] == 30);
   assert(arr[3] == 40);

   insert<int>(arr, len, 5, 0);
   assert(len == 5);
   assert(arr[0] == 5);
   assert(arr[1] == 10);

   insert<int>(arr, len, 50, len);
   assert(len == 6);
   assert(arr[5] == 50);
}

void testRemove()
{
   int arr1[10] = {10, 20, 30, 40};
   int len1 = 4;
   assert(remove<int>(arr1, len1, 0) == 10);
   assert(len1 == 3);
   assert(arr1[0] == 20);
   assert(arr1[1] == 30);
   assert(arr1[2] == 40);

   int arr2[10] = {10, 20, 30, 40};
   int len2 = 4;
   assert(remove<int>(arr2, len2, 1) == 20);
   assert(len2 == 3);
   assert(arr2[0] == 10);
   assert(arr2[1] == 30);
   assert(arr2[2] == 40);

   int arr3[10] = {10, 20, 30, 40};
   int len3 = 4;
   assert(remove<int>(arr3, len3, 3) == 40);
   assert(len3 == 3);
   assert(arr3[0] == 10);
   assert(arr3[1] == 20);
   assert(arr3[2] == 30);
}

void testFind()
{
   int arr[5] = {10, 20, 30, 40, 50};
   int len = 5;

   assert((find<int, int>(arr, len, 10, cmpIntInt) == 0));
   assert((find<int, int>(arr, len, 30, cmpIntInt) == 2));
   assert((find<int, int>(arr, len, 50, cmpIntInt) == 4));
   assert((find<int, int>(arr, len, 99, cmpIntInt) < 0));
}

void testOrderedInsert()
{
   int arr[10] = {10, 20, 40, 50};
   int len = 4;

   assert(orderedInsert<int>(arr, len, 30, cmpIntInt) == 2);
   assert(len == 5);
   assert(arr[0] == 10);
   assert(arr[1] == 20);
   assert(arr[2] == 30);
   assert(arr[3] == 40);
   assert(arr[4] == 50);

   assert(orderedInsert<int>(arr, len, 5, cmpIntInt) == 0);
   assert(arr[0] == 5);

   assert(orderedInsert<int>(arr, len, 60, cmpIntInt) == 6);
   assert(arr[6] == 60);
}

void testSort()
{
   int arr1[6] = {40, 10, 30, 20, 60, 50};
   sort<int>(arr1, 6, cmpIntInt);
   assert(arr1[0] == 10);
   assert(arr1[1] == 20);
   assert(arr1[2] == 30);
   assert(arr1[3] == 40);
   assert(arr1[4] == 50);
   assert(arr1[5] == 60);

   int arr2[1] = {7};
   sort<int>(arr2, 1, cmpIntInt);
   assert(arr2[0] == 7);

   int arr3[3] = {2, 2, 1};
   sort<int>(arr3, 3, cmpIntInt);
   assert(arr3[0] == 1);
   assert(arr3[1] == 2);
   assert(arr3[2] == 2);
}

int main()
{
   testAdd();
   testInsert();
   testRemove();
   testFind();
   testOrderedInsert();
   testSort();

   cout << "Todos los tests de arrays pasaron correctamente." << endl;
   return 0;
}