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
    int anio;
    int mes;
    int dia;
    int hora;
    int minuto;
    int segundo;
};

// Crea una fecha a partir de los milisegundos (timestamp) indicados
Fecha fecha(long long ts)
{
    Fecha f;
    f.segundo = _getSecond(ts);
    f.minuto = _getMinute(ts);
    f.hora = _getHour(ts);
    f.dia = _getDay(ts);
    f.mes = _getMonth(ts);
    f.anio = _getYear(ts);
    return f;
}

// Crea una fecha a partir de la fecha/hora actual
Fecha fecha()
{   
    long long ms = currTimeMillis();
    Fecha f = fecha(ms);
    return f;
}

// Crea una fecha completa
Fecha fecha(int anio, int mes, int dia, int hora, int min)
{
    return {anio, mes, dia, hora, min, 0};  // dejo los segundos en cero
}

// Crea una fecha dejando hora y minuto en 0.
Fecha fecha(int anio, int mes, int dia)
{
    return {anio, mes, dia, 0, 0, 0};
}

long long fechaInMillis(Fecha f)
{
    long long ms = attributesToMillis(f.anio, f.mes, f.dia, f.hora, f.minuto, f.segundo, 0);
    return ms;
}

Fecha fechaAddSegundos(Fecha ff, int n)
{
    long long milisegundosPorSegundo = 1000;  // 1 segundo = 1 seg × 1000 ms = 1000 ms
    long long msf = attributesToMillis(ff.anio, ff.mes, ff.dia, ff.hora, ff.minuto, ff.segundo, 0);
    // paso la cantidad de horas n a milisegundos
    long long msfSumado = msf + (n * milisegundosPorSegundo);  // sumo todo en milisegundos
    Fecha ret = fecha(msfSumado);
    return ret;
}

Fecha fechaAddMinutos(Fecha ff, int n)
{
    long long milisegundosPorMinuto = 60000;  // 1 minuto = 60 seg × 1000 ms = 60.000 ms
    long long msf = attributesToMillis(ff.anio, ff.mes, ff.dia, ff.hora, ff.minuto, ff.segundo, 0);
    // paso la cantidad de horas n a milisegundos
    long long msfSumado = msf + (n * milisegundosPorMinuto);  // sumo todo en milisegundos
    Fecha ret = fecha(msfSumado);
    return ret;
}

Fecha fechaAddHoras(Fecha ff, int n)
{
    long long milisegundosPorHora = 3600000;  // 1 hora = 60 min × 60 seg × 1000 ms = 3.600.000 ms
    long long msf = attributesToMillis(ff.anio, ff.mes, ff.dia, ff.hora, ff.minuto, ff.segundo, 0);
    // paso la cantidad de horas n a milisegundos
    long long msfSumado = msf + (n * milisegundosPorHora);  // sumo todo en milisegundos
    Fecha ret = fecha(msfSumado);
    return ret;
}

Fecha fechaAddDias(Fecha ff, int n)
{
    long long milisegundosPorDia = 86400000;  // 1 día = 24 hs × 60 min × 60 seg × 1000 ms = 86.400.000 ms
    long long msf = attributesToMillis(ff.anio, ff.mes, ff.dia, ff.hora, ff.minuto, ff.segundo, 0);
    // paso la cantidad de dias n a milisegundos
    long long msfSumado = msf + (n * milisegundosPorDia);  // sumo todo en milisegundos
    Fecha ret = fecha(msfSumado);
    return ret;
}

long long fechaDiffMillis(Fecha a, Fecha b)
{
    long long msa = attributesToMillis(a.anio, a.mes, a.dia, a.hora, a.minuto, a.segundo, 0);
    long long msb = attributesToMillis(b.anio, b.mes, b.dia, b.hora, b.minuto, b.segundo, 0);
    return msa - msb;
}

int fechaDiffDias(Fecha a, Fecha b)
{
    long long diferencia = fechaDiffMillis(a, b);
    // tengo que pasar de milisegundos a dias
    long long milisegundosPorDia = 86400000;
    int ret = diferencia / milisegundosPorDia;
    return ret;
}

// Retorna:
// n > 0 si a > b
// n < 0 si a < b
// n == 0 si a == b
int fechaCmp(Fecha a, Fecha b)
{
    int n;
    if(fechaDiffMillis(a, b) > 0)
    {
        n = 1;
    }
    else if(fechaDiffMillis(a, b) < 0)
    {
        n = -1;
    }
    else
    {
        n = 0;
    }
    return n;
}

string fechaToString(Fecha f)
{
    // funcion to_string toma un entero y devuelve una cadena para que la podamos
   // concatenar
    string s = to_string(f.dia) + "/" + to_string(f.mes) + "/" +
               to_string(f.anio) + "-" + to_string(f.hora) + ":" +
               to_string(f.minuto) + ":" +
               to_string(f.segundo);  // concatenar enteros con cadenas no se
                                      // puede en c++ (en java si)
    return s;  // retornamos dia/mes/anio
}

bool fechaEsAnioBisiesto(Fecha f)
{
    // es divisible por 4 o por 400 y no por 100
    return f.anio % 4 == 0 || (f.anio % 400 == 0 && f.anio % 100 != 0);
}

// =================
// Setters y getters
// =================

int fechaGetAnio(Fecha f)
{
    return f.anio;
}

int fechaGetMes(Fecha f)
{
    return f.mes;
}

int fechaGetDia(Fecha f)
{
    return f.dia;
}

int fechaGetHora(Fecha f)
{
     return f.hora;
}

int fechaGetMinuto(Fecha f)
{
    return f.minuto;;
}

int fechaGetSegundo(Fecha f)
{
    return f.segundo;
}

void fechaSetAnio(Fecha& f, int anio)
{
    f.anio = anio;
}

void fechaSetMes(Fecha& f, int mes)
{
    f.mes = mes;
}

void fechaSetDia(Fecha& f, int dia)
{
    f.dia = dia;
}

void fechaSetHora(Fecha& f, int hora)
{
    f.hora = hora;
}

void fechaSetMinuto(Fecha& f, int minuto)
{
    f.minuto = minuto;
}

void fechaSetSegundo(Fecha& f, int segundo)
{
    f.segundo = segundo; 
}

#endif  // FECHA_H