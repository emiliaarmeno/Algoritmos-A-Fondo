
#include <iostream>
// #include "biblioteca/funciones/millis.hpp"
#include "../biblioteca/funciones/files.hpp"
#include "../biblioteca/funciones/strings.hpp"
#include "biblioteca/funciones/tokens.hpp"
#include "biblioteca/tads/parte1/Fecha.hpp"
// #include "biblioteca/tads/parte1/Timer.hpp"
#include "biblioteca/tads/parte1/Coll.hpp"
#include "biblioteca/tads/parte2/Array.hpp"
#include "biblioteca/tads/parte2/List.hpp"
#include "biblioteca/tads/parte2/Map.hpp"
#include "biblioteca/tads/parte2/Queue.hpp"
#include "biblioteca/tads/parte2/Stack.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;

//GASTOS.dat (novedades) y CONSORCIOS.dat (consulta)
struct Gasto
{
   int idCons; // ver situaciones A y B
   int fecha;  // aaaammdd
   double importe;
   char categoria[20]; // ej. Limpieza, Sueldos, Servicios...
};
struct Consorcio
{
   int idCons;
   char direccion[50];
   int cantPisos;
   int dtosPorPiso; //esto dicta cuantos tipos va a haber 
};
struct RConsorcio{
    Consorcio c; 
    int impTotal; 
    Map<int, double> metrosPorcentuales; //el tipo de depto con los mt% que ocupa -- lo vamos a ordenar por mts
    Map<string, int> gastos; //acumulo por categoria los gastos
};

RConsorcio rConsorcio(Consorcio c){
    RConsorcio ret;
    ret.c = c; 
    ret.impTotal=0;
    ret.metrosPorcentuales = map<int, double>();
    ret.gastos = map<string, int>();
    return ret; 
}

double mtsPorcencuales(int idCons, int tipoDepto);

int cmpMts(double a, double b){
    return a-b;
}

void calcularMetrosPorcentuales(RConsorcio& rc){
    //agarramos el idCons que esta en el c de rc 
    //vemos cuantos deptos por piso tiene y hago for 
    //llamo a la funcion que me da la consigna y agrego al map 
    int idCons = rc.c.idCons;
    int dtos = rc.c.dtosPorPiso;
    for(int i=1; i<=dtos; i++){
        double mts = mtsPorcencuales(idCons, i);
        mapPut<int, double>(rc.metrosPorcentuales, i, mts);
    }
    //ordeno por metros decrecientememte 
    mapSortByValues<int, double>(rc.metrosPorcentuales, cmpMts);
}

List<RConsorcio> subirRConsorcio(){
    List<RConsorcio> ret = list<RConsorcio>();
    FILE* f = fopen("CONSORCIO.dat", "r+b");
    Consorcio c = read<Consorcio>(f);
    while(!feof(f)){
        RConsorcio rc = rConsorcio(c);
        //llamar a funcion que inicialice el map de los metros porcentuales y lo ordenamos de mayor a menor 
        calcularMetrosPorcentuales(rc);
        c = read<Consorcio>(f);
    }
    fclose(f);
    return ret; 
}

int cmpidCons(RConsorcio rc, int idCons){
    return idCons-rc.c.idCons;
}

void procesarGasto(Gasto g, List<RConsorcio>& lRC){
    RConsorcio* rc = listFind<RConsorcio, int>(lRC, g.idCons, cmpidCons);
    //me fijo si ya existe ese key 
    if(mapContains<string, int>(rc->gastos, g.categoria)){
        int* importeDelGasto = mapGet<string, int>(rc->gastos, g.categoria);
        *importeDelGasto += g.importe; //acumulo el total gastado para esta categoria 
    }else{
        mapPut<string, int>(rc->gastos, g.categoria, g.importe);
    }

    //sumo a importe total el importe del gasto que estamos procesando 
    rc->impTotal+=g.importe;
}

//agarra el primer consorcio que es el que acabo de procesar
void mostrarDatos(List<RConsorcio> lRC, int idCons){
    //como gasto esta ordenado por idCons 
    //y mi lista de RC tambien
    RConsorcio rc = listRemoveFirst<RConsorcio>(lRC);
    cout<<"Calle consorcio: "<<rc.c.direccion<<" Total gastos: "<<rc.impTotal<<endl;
    cout<<"Depto.	     M2. (%)	    Total a pagar ($)"<<endl;
    
    mapReset<int, double>(rc.metrosPorcentuales);
    int tipo = mapNextKey<int, double>(rc.metrosPorcentuales);
    //esto para cuando mostremos
    //del total a pagar dividido la cantidad de pisos
    //cada tipo de depto va a pagar eso por los metros porcentuales (que como es un numero menor a uno es como dividir)
    double mts = *mapGet<int, double>(rc.metrosPorcentuales, tipo);
    while(mapHasNext<int, double>(rc.metrosPorcentuales)){
        cout<<tipo<<mts*100<<(rc.impTotal/rc.c.cantPisos)*mts;
        tipo = mapNextKey<int, double>(rc.metrosPorcentuales);
        mts = *mapGet<int, double>(rc.metrosPorcentuales, tipo);    
    }

    //ahora muestro los gastos por categ 
    cout<<"Categoría	Gastos ($)"<<endl;
    mapReset<string, int>(rc.gastos);
    string categoria = mapNextKey<string, int>(rc.gastos);
    int importe = *mapGet<string, int>(rc.gastos, categoria);
    while(mapHasNext<string, int>(rc.gastos)){
        cout<<categoria<<importe<<endl;
        categoria = mapNextKey<string, int>(rc.gastos);
        importe = *mapGet<string, int>(rc.gastos, categoria);
    }
    cout<<"Total: "<<rc.impTotal<<endl;
}

int cmpRCons(RConsorcio a, RConsorcio b){
    return a.c.idCons - b.c.idCons;
}

int main()
{
    //subir consorcios a memoria en list rconsorcio 
    List<RConsorcio> lRC = subirRConsorcio();
    listSort<RConsorcio>(lRC, cmpRCons);

    //recorremos gastos (que estan ordenados por consorcio) 
    FILE* f = fopen("GASTOS.dat", "r+b");
    Gasto g = read<Gasto>(f);

    int idAnt = g.idCons;
    
    while(!feof(f)){
        //mientras que no cambie el idConsorcio
        while(g.idCons==idAnt && !feof(f)){
            //procesamos el gasto (sumo a impTot y añado al map de gastos)
            procesarGasto(g, lRC);
            idAnt = g.idCons;
            g = read<Gasto>(f);
        }
        //cuando cambia el idCons muestro los datos del consorcio 
        mostrarDatos(lRC, g.idCons);
        //el ultimo id que lei que hizo que salga del while interno hago que coincida con el idAnt asi entra en la prox vuelta
        idAnt=g.idCons; 
    }
    fclose(f);
    return 0;
}
