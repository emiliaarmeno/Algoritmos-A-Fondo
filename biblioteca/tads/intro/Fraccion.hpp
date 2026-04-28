#ifndef _TFRACCION_T_
#define _TFRACCION_T_

#include <iostream>

using std::string;
using std::to_string;

struct Fraccion
{
    int numerador;
    int denominador;
};

Fraccion fraccion(int n,int d)
{
    Fraccion ret;
    ret.numerador = n;
    ret.denominador = d;
    return ret;
}

string fraccionToString(Fraccion f)
{
    return to_string(f.numerador) + " / " + to_string(f.denominador);
}

Fraccion fraccionSumar(Fraccion a,Fraccion b)
{
    Fraccion ret;
    int numA = a.numerador;
    int denA = a.denominador;
    int numB = b.numerador;
    int denB = b.denominador;

    ret.numerador = numA*denB + numB*denA;
    ret.denominador = denA*denB;

    return ret;
}

Fraccion fraccionSimplificar(Fraccion f)
{
    Fraccion ret;
    int i=0;
    while(i<f.numerador && i<f.denominador){
        if(f.numerador%i==0 && f.denominador%i==0){ //multiplo comun
            f.numerador = f.numerador/i;
            f.denominador = f.denominador/i;
        }else{
            i++;
        }
    }
    ret.numerador = f.numerador;
    ret.denominador = f.denominador;
    return ret;
}

bool fraccionEsEntera(Fraccion f)
{
    return f.numerador%f.denominador==0;
}

int fraccionGetNumerador(Fraccion f)
{
    return f.numerador;
}

void fraccionSetNumerador(Fraccion& f,int n)
{
    f.numerador = n;
}

int fraccionGetDenominador(Fraccion f)
{
    return f.denominador;
}

void fraccionSetDenominador(Fraccion& f,int d)
{
    f.denominador = d;
}

#endif