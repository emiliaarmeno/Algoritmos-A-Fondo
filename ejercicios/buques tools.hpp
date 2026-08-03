#ifndef _MAINHPP
#define _MAINHPP
#include <iostream>
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "biblioteca/funciones/strings.hpp"
#include "biblioteca/funciones/tokens.hpp"
#include "biblioteca/tads/parte1/Coll.hpp"
using namespace std;

struct Buque
{
	int idBuque;
	char nombre[50];
	int darsena;
	int grua;
	int cantContainers;
	int fecha;
	int cteFlotacion;
	char oriODest[100];
};

struct Container
{
	int idContainer;
	int peso;
	int longitud;
	int idBuque;
};

string buqueToString(Buque x)
{
	char sep = 1;
	string sIdBuque=to_string(x.idBuque);
	string sNombre=x.nombre;
	string sDarsena=to_string(x.darsena);
	string sGrua=to_string(x.grua);
	string sCantContainers=to_string(x.cantContainers);
	string sFecha=to_string(x.fecha);
	string sCteFlotacion=to_string(x.cteFlotacion);
	string sOriODest=x.oriODest;
	return sIdBuque+sep+sNombre+sep+sDarsena+sep+sGrua+sep+sCantContainers+sep+sFecha+sep+sCteFlotacion+sep+sOriODest;
}

Buque buqueFromString(string s)
{
	char sep = 1;
	Buque x;
	string t0 = getTokenAt(s,sep,0);
	x.idBuque=stoi(t0);
	string t1 = getTokenAt(s,sep,1);
	strcpy(x.nombre,t1.c_str());
	string t2 = getTokenAt(s,sep,2);
	x.darsena=stoi(t2);
	string t3 = getTokenAt(s,sep,3);
	x.grua=stoi(t3);
	string t4 = getTokenAt(s,sep,4);
	x.cantContainers=stoi(t4);
	string t5 = getTokenAt(s,sep,5);
	x.fecha=stoi(t5);
	string t6 = getTokenAt(s,sep,6);
	x.cteFlotacion=stoi(t6);
	string t7 = getTokenAt(s,sep,7);
	strcpy(x.oriODest,t7.c_str());
	return x;
}

string buqueToDebug(Buque x)
{
	stringstream sout;
	sout<< "[";
	sout << x.idBuque;
	sout << ",";
	sout << x.nombre;
	sout << ",";
	sout << x.darsena;
	sout << ",";
	sout << x.grua;
	sout << ",";
	sout << x.cantContainers;
	sout << ",";
	sout << x.fecha;
	sout << ",";
	sout << x.cteFlotacion;
	sout << ",";
	sout << x.oriODest;
	sout<< "]";
	return sout.str();
}

string buqueToDebug(string mssg,Buque x)
{
	stringstream sout;
	sout<< mssg<<":[";
	sout << x.idBuque;
	sout << ",";
	sout << x.nombre;
	sout << ",";
	sout << x.darsena;
	sout << ",";
	sout << x.grua;
	sout << ",";
	sout << x.cantContainers;
	sout << ",";
	sout << x.fecha;
	sout << ",";
	sout << x.cteFlotacion;
	sout << ",";
	sout << x.oriODest;
	sout<< "]";
	return sout.str();
}

Buque buque(int idBuque,string nombre,int darsena,int grua,int cantContainers,int fecha,int cteFlotacion,string oriODest)
{
	Buque a;
	a.idBuque = idBuque;
	strcpy(a.nombre,nombre.c_str());
	a.darsena = darsena;
	a.grua = grua;
	a.cantContainers = cantContainers;
	a.fecha = fecha;
	a.cteFlotacion = cteFlotacion;
	strcpy(a.oriODest,oriODest.c_str());
	return a;
}

bool buqueEquals(Buque a,Buque b)
{
	if(a.idBuque!=b.idBuque) return false;
	if(a.darsena!=b.darsena) return false;
	if(a.grua!=b.grua) return false;
	if(a.cantContainers!=b.cantContainers) return false;
	if(a.fecha!=b.fecha) return false;
	if(a.cteFlotacion!=b.cteFlotacion) return false;
	return true;
}

string containerToString(Container x)
{
	char sep = 2;
	string sIdContainer=to_string(x.idContainer);
	string sPeso=to_string(x.peso);
	string sLongitud=to_string(x.longitud);
	string sIdBuque=to_string(x.idBuque);
	return sIdContainer+sep+sPeso+sep+sLongitud+sep+sIdBuque;
}

Container containerFromString(string s)
{
	char sep = 2;
	Container x;
	string t0 = getTokenAt(s,sep,0);
	x.idContainer=stoi(t0);
	string t1 = getTokenAt(s,sep,1);
	x.peso=stoi(t1);
	string t2 = getTokenAt(s,sep,2);
	x.longitud=stoi(t2);
	string t3 = getTokenAt(s,sep,3);
	x.idBuque=stoi(t3);
	return x;
}

string containerToDebug(Container x)
{
	stringstream sout;
	sout<< "[";
	sout << x.idContainer;
	sout << ",";
	sout << x.peso;
	sout << ",";
	sout << x.longitud;
	sout << ",";
	sout << x.idBuque;
	sout<< "]";
	return sout.str();
}

string containerToDebug(string mssg,Container x)
{
	stringstream sout;
	sout<< mssg<<":[";
	sout << x.idContainer;
	sout << ",";
	sout << x.peso;
	sout << ",";
	sout << x.longitud;
	sout << ",";
	sout << x.idBuque;
	sout<< "]";
	return sout.str();
}

Container container(int idContainer,int peso,int longitud,int idBuque)
{
	Container a;
	a.idContainer = idContainer;
	a.peso = peso;
	a.longitud = longitud;
	a.idBuque = idBuque;
	return a;
}

bool containerEquals(Container a,Container b)
{
	if(a.idContainer!=b.idContainer) return false;
	if(a.peso!=b.peso) return false;
	if(a.longitud!=b.longitud) return false;
	if(a.idBuque!=b.idBuque) return false;
	return true;
}

#endif
