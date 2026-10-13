#ifndef _TSTACK_TAD_
#define _TSTACK_TAD_

#include <iostream>

#include "../../funciones/lists.hpp"

using std::string;

template <typename T>
struct Stack
{
    Node<T>* top;   // primer nodo de la lista
    int size;       // cantidad de elementos
};

template <typename T>
Stack<T> stack()
{
    Stack<T> stk; 
    stk.top = NULL;
    stk.size = 0;
    return stk;
}

template <typename T>
T* stackPush(Stack<T>& st, T e)
{
    Node<T>* nodo = addFirst<T>(st.top, e);
    st.size++;
    return &(nodo->info); //devuelvo la dire del dato
}

template <typename T>
T stackPop(Stack<T>& st)
{
    T t;
    t = removeFirst<T>(st.top);
    st.size--;
    return t;
}

template <typename T>
bool stackIsEmpty(Stack<T> st)
{
    return isEmpty<T>(st.top);
}

template <typename T>
int stackSize(Stack<T> st)
{
    return st.size;
}

#endif
