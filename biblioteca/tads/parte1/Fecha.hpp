#ifndef FECHA_H
#define FECHA_H

#include <string>

#include "../../funciones/millis.hpp"

using  std::string;
using  std::to_string;

// ============================
// Estructura
// ============================

struct Fecha
{

};

Fecha fecha(long long ts)
{
    Fecha f;
    return f;
}

Fecha fecha()
{
    Fecha f;
    return f;
}

Fecha fecha(int anio, int mes, int dia, int hora, int min)
{
    Fecha f;
    return f;
}

Fecha fecha(int anio, int mes, int dia)
{
    Fecha f;
    return f;
}

long long fechaInMillis(Fecha f)
{
    return 0;
}

Fecha fechaAddSegundos(Fecha ff, int n)
{
    Fecha f;
    return f;
}

Fecha fechaAddMinutos(Fecha ff, int n)
{
    Fecha f;
    return f;
}

Fecha fechaAddHoras(Fecha ff, int n)
{
    Fecha f;
    return f;
}

Fecha fechaAddDias(Fecha ff, int n)
{
    Fecha f;
    return f;
}

long long fechaDiffMillis(Fecha a, Fecha b)
{
    return 0;
}

int fechaDiffDias(Fecha a, Fecha b)
{
    return 0;
}

int fechaCmp(Fecha a, Fecha b)
{
    return 0;
}

string fechaToString(Fecha f)
{
    return "";
}

bool fechaEsAnioBisiesto(Fecha f)
{
    return false;
}

// =================
// Setters y getters
// =================

int fechaGetAnio(Fecha f)
{
    return 0;
}

int fechaGetMes(Fecha f)
{
    return 0;
}

int fechaGetDia(Fecha f)
{
    return 0;
}

int fechaGetHora(Fecha f)
{
    return 0;
}

int fechaGetMinuto(Fecha f)
{
    return 0;
}

int fechaGetSegundo(Fecha f)
{
    return 0;
}

void fechaSetAnio(Fecha& f, int anio)
{
}

void fechaSetMes(Fecha& f, int mes)
{
}

void fechaSetDia(Fecha& f, int dia)
{
}

void fechaSetHora(Fecha& f, int hora)
{
}

void fechaSetMinuto(Fecha& f, int minuto)
{
}

void fechaSetSegundo(Fecha& f, int segundo)
{
}

#endif  // FECHA_H