#ifndef _TQUEUE_TAD_
#define _TQUEUE_TAD_

#include <iostream>

#include "../../funciones/lists.hpp"

using std::string;

template <typename T>
struct Queue
{
};

template <typename T>
Queue<T> queue()
{
    return {};
}

template <typename T>
T* queueEnqueue(Queue<T>& q, T e)
{
    return NULL;
}

template <typename T>
T queueDequeue(Queue<T>& q)
{
    T t;
    return;
}

template <typename T>
bool queueIsEmpty(Queue<T> q)
{
    return true;
}

template <typename T>
int queueSize(Queue<T> q)
{
    return 0;
}

#endif
