#ifndef _TQUEUE_TAD_
#define _TQUEUE_TAD_

#include <iostream>

#include "../../funciones/lists.hpp"

using std::string;

template <typename T>
struct Queue
{
    Node<T>* first;   // primer nodo (de donde se desencola)
    Node<T>* last;    // último nodo (donde se encola)
    int size;
};

template <typename T>
Queue<T> queue()
{
    Queue<T> qu;
    qu.first=NULL;
    qu.last=NULL; 
    qu.size=0;
    return qu;
}

template <typename T>
T* queueEnqueue(Queue<T>& q, T e)
{
    Node<T>* nodo = enqueue<T>(q.first, q.last, e); //esta devuelve un nodo 
    q.size++;
    return &(nodo->info); //tengo que devolver la direccion en la que quedo el dato
}

template <typename T>
T queueDequeue(Queue<T>& q)
{
    q.size--;
    return dequeue<T>(q.first, q.last); //devuelve lo que desencolo 
}

template <typename T>
bool queueIsEmpty(Queue<T> q)
{
    return isEmpty(q.first);
}

template <typename T>
int queueSize(Queue<T> q)
{
    return q.size;
}

#endif
