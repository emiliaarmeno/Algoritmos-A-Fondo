#ifndef _MAINHPP
#define _MAINHPP
#include <iostream>
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "../biblioteca/funciones/strings.hpp"
#include "../biblioteca/funciones/tokens.hpp"
#include "../biblioteca/tads/parte1/Coll.hpp"
using namespace std;

struct RAutor
{
	string nombre;
	Coll<string> frases;
};

struct RAnio
{
	int anio;
	Coll<RAutor> autores;
};

string rAutorToString(RAutor x)
{
	char sep = 1;
	string sNombre=x.nombre;
	string sFrases=collToString<string>(x.frases);
	return sNombre+sep+sFrases;
}

RAutor rAutorFromString(string s)
{
	char sep = 1;
	RAutor x;
	string t0 = getTokenAt(s,sep,0);
	x.nombre=t0;
	string t1 = getTokenAt(s,sep,1);
	x.frases=collFromString<string>(t1);
	return x;
}

string rAutorToDebug(RAutor x)
{
	stringstream sout;
	sout<< "[";
	sout << x.nombre;
	sout << ",";
	collReset<string>(x.frases);
	int n=collSize<string>(x.frases);
	sout << "{";
	for(int i=0; i<n; i++)
	{
		string q = collGetAt<string>(x.frases,i,stringToString);
		sout << stringToString(q) << ((i<n-1)?",":"");
	}
	sout << "}";
	sout<< "]";
	return sout.str();
}

string rAutorToDebug(string mssg,RAutor x)
{
	stringstream sout;
	sout<< mssg<<":[";
	sout << x.nombre;
	sout << ",";
	collReset<string>(x.frases);
	int n=collSize<string>(x.frases);
	sout << "{";
	for(int i=0; i<n; i++)
	{
		string q = collGetAt<string>(x.frases,i,stringToString);
		sout << stringToString(q) << ((i<n-1)?",":"");
	}
	sout << "}";
	sout<< "]";
	return sout.str();
}

RAutor rAutor(string nombre,Coll<string> frases)
{
	RAutor a;
	a.nombre = nombre;
	a.frases = frases;
	return a;
}

bool rAutorEquals(RAutor a,RAutor b)
{
	if(a.nombre!=b.nombre) return false;
	if(collToString<string>(a.frases)!=collToString<string>(b.frases)) return false;
	return true;
}

string rAnioToString(RAnio x)
{
	char sep = 2;
	string sAnio=to_string(x.anio);
	string sAutores=collToString<RAutor>(x.autores);
	return sAnio+sep+sAutores;
}

RAnio rAnioFromString(string s)
{
	char sep = 2;
	RAnio x;
	string t0 = getTokenAt(s,sep,0);
	x.anio=stoi(t0);
	string t1 = getTokenAt(s,sep,1);
	x.autores=collFromString<RAutor>(t1);
	return x;
}

string rAnioToDebug(RAnio x)
{
	stringstream sout;
	sout<< "[";
	sout << x.anio;
	sout << ",";
	collReset<RAutor>(x.autores);
	int n=collSize<RAutor>(x.autores);
	sout << "{";
	for(int i=0; i<n; i++)
	{
		RAutor q = collGetAt<RAutor>(x.autores,i,rAutorFromString);
		sout << rAutorToDebug(q) << ((i<n-1)?",":"");
	}
	sout << "}";
	sout<< "]";
	return sout.str();
}

string rAnioToDebug(string mssg,RAnio x)
{
	stringstream sout;
	sout<< mssg<<":[";
	sout << x.anio;
	sout << ",";
	collReset<RAutor>(x.autores);
	int n=collSize<RAutor>(x.autores);
	sout << "{";
	for(int i=0; i<n; i++)
	{
		RAutor q = collGetAt<RAutor>(x.autores,i,rAutorFromString);
		sout << rAutorToDebug(q) << ((i<n-1)?",":"");
	}
	sout << "}";
	sout<< "]";
	return sout.str();
}

RAnio rAnio(int anio, Coll<RAutor> autores)
{
	RAnio b;
	b.anio = anio;
	b.autores = autores;
	return b;
}

bool rAnioEquals(RAnio a,RAnio b)
{
	if(a.anio!=b.anio) return false;
	if(collToString<RAutor>(a.autores)!=collToString<RAutor>(b.autores)) return false;
	return true;
}

#endif
