#ifndef _TLIST_T_
#define _TLIST_T_

#include <stdio.h>

#include <iostream>

template <typename T>
struct Node
{
    T info; //el dato 
    Node<T>* sig; //el puntero al siguiente 
};

template <typename T>
Node<T>* add(Node<T>*& p, T e)
{
    // creo un nuevo nodo
    Node<T>* nuevo = new Node<T>();
    nuevo->info = e;
    nuevo->sig = NULL;
    // la lista ya existe?
    if( p!=NULL )
    {
        // busco el ultimo
        Node<T>* aux = p;
        while(aux->sig!=NULL)
        {
            aux = aux->sig;
        }
        // enlazo el nuevo nodo
        aux->sig = nuevo;
    }
    else
    {
        // nuevo es el primero
        p = nuevo;
    }
    return nuevo;
}

template <typename T>
Node<T>* addFirst(Node<T>*& p, T e)
{
    Node<T>* nuevo = new Node<T>();
    nuevo->info = e;
    nuevo->sig = p; //hago que apunte al primer elemento
    p=nuevo; //ahora el primer elemento es el nuevo
    return nuevo;
}

template <typename T, typename K>
T remove(Node<T>*& p, K k, int cmpTK(T, K))
{   
    T t;
    Node<T>* aux = p;
    Node<T>* ant = NULL;

    while( aux!=NULL && cmpTK(aux->info,k)!=0 ) //mientras que no se termine la lista y no encuentre el valor sigo recorriendo 
    {
        ant = aux;
        aux = aux->sig;
    }

    if( aux!=NULL )            // solo si lo encontró
    {
        t = aux->info;         // guardo el valor

        if( ant==NULL )        // si era el primero
        {
            p = aux->sig;
        }
        else                   // estaba en el medio o al final
        {
            ant->sig = aux->sig;
        }

        delete aux;
    }

    return t;
}

template <typename T>
T removeFirst(Node<T>*& p)
{
    Node<T>* aux = p; //apunto al primer elemento 
    T t = aux->info; //me guardo el contenido
    p = aux->sig; //hago que la lista tenga como primer elemento al segundo 
    delete aux; 
    return t; //devuelvo el elemento que elimine
}

template <typename T, typename K>
Node<T>* find(Node<T>* p, K k, int cmpTK(T, K))
{
    Node<T>* aux = p;
    while( aux!=NULL && cmpTK(aux->info,k)!=0 ) //recorro
    {
        aux = aux->sig;
    }
    //salio ya sea porque encontro el elemento o porque se termino la lista
    return aux;
}

template <typename T>
Node<T>* orderedInsert(Node<T>*& p, T e, int cmpTT(T, T)) //La lista debe estar vacía u ordenada según cmpTT
{
    Node<T>* nuevo = new Node<T>();
    nuevo->info = e;

    Node<T>* ant = NULL;
    Node<T>* aux = p;
    while( aux!=NULL && cmpTT(aux->info, e)<0 ) //recorro hasta encontrar un valor mayor al que voy a insertar
    {
        ant = aux; //guardo el puntero al anterior
        aux = aux->sig; //sigo recorriendo
    }

    if(ant==NULL){ //salio porque la lista estaba vacía o el valor a insertar es menor que el primero
        nuevo->sig=p; 
        p=nuevo;
    }else{ 
        ant->sig = nuevo; 
        if(aux==NULL){ //salio porque estamos intentando insertar un valor mayor que el último
            nuevo->sig = NULL;
        }
        else
        {
            nuevo->sig=aux; //quedo a la mitad 
        }
    }
    return nuevo;
}

template <typename T>
Node<T>* searchAndInsert(Node<T>*& p, T e, bool& enc, int cmpTT(T, T))
{
    Node<T>* aux = find<T,T>(p, e, cmpTT);   // busco e
    enc = (aux != NULL);                      // lo encontre? 

    if( !enc )                                // no estaba, lo inserto en orden
    {
        aux = orderedInsert<T>(p, e, cmpTT);
    }

    return aux;                               // nodo encontrado o insertado
}

template <typename T>
void sort(Node<T>*& p, int cmpTT(T, T))
{
    //voy a crear una nueva lista e ir agregando de forma ordenada cada nodo
    Node<T>* ordenada = NULL;

    while( p!=NULL )
    {
        T e = removeFirst<T>(p);          // saco el primero de la original
        orderedInsert<T>(ordenada, e, cmpTT);  // lo inserto en orden en la nueva
    }

    p = ordenada;                          // la original ahora apunta a la ordenada
    //no hace falta devolverla porque al ser un puntero ya tengo la direccion a memoria
}

template <typename T>
bool isEmpty(Node<T>* p)
{
    return p==NULL;
}

template <typename T>
void free(Node<T>*& p)
{
    while( p!=NULL )
    {
        Node<T>* aux = p->sig;
        delete p;
        p = aux;
    }
}

//funciones para stack (LIFO)
template <typename T>
Node<T>* push(Node<T>*& p, T e)
{
    return addFirst(p, e);
}

template <typename T>
T pop(Node<T>*& p)
{
    return removeFirst(p); //devuelve el elemento que elimine 
}

//funciones para queue (FIFO) con un puntero a la lista (p) y uno al ultimo elem (q)
template <typename T>
Node<T>* enqueue(Node<T>*& p, Node<T>*& q, T e)
{
    Node<T>* nuevo = new Node<T>();
    nuevo->info = e;
    nuevo->sig = NULL;

    if(q!=NULL){ //si la lista no esta vacia 
        q->sig = nuevo; //hago que el ultimo de la lista apunte al nuevo
    }else{
         p = nuevo; //sera el unico elemento, p y q van a apuntar al nuevo
    }
    q = nuevo; //siempre apunta al ultimo 
    return nuevo;
}

template <typename T>
Node<T>* enqueue(Node<T>*& q, T e) //para lista circular
{
    //creo el nuevo que voy a encolar
    Node<T>* nuevo = new Node<T>();
    nuevo->info = e;

    if( q!=NULL ) //si la cola no esta vacia
    {
        nuevo->sig = q->sig; //el sig del nuevo tiene que apuntar al primer elem
        q->sig = nuevo; //pongo ahora al nuevo como un nuevo ultimo elemento
    }
    else
    {
        nuevo->sig=nuevo; //apunta a si mismo 
    }
    q = nuevo; //q siempre debe apuntar al ultimo elemento
    return NULL;
}

template <typename T>
T dequeue(Node<T>*& p, Node<T>*& q)
{
    T ret = p->info;
    Node<T>* aux = p; //me guardo la direccion del primer nodo 
    p = p->sig; //hago que el segundo sea el primero
    delete aux;

    if( p==NULL ) //acabo de desencolar el ultimo elemento de la lista
    {
        q = NULL;
    }

    return ret; //devuelvo lo que elimine
}

template <typename T>
T dequeue(Node<T>*& q)
{    
    Node<T>* aux = q->sig;; //me guardo el primer nodo de la lista
    q->sig = aux->sig; //apunto al segundo que sera el nuevo primer elemento 
    T ret = aux->info;
    if( aux==q) //si estoy desencolando el ultimo elemento de la lista
    {
        q = NULL;
    }
    delete aux;
    return ret;
}

#endif
