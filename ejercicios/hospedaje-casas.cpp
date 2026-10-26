
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

/* Se pide: Por cada búsqueda, emitir un listado (ordenado decrecientemente por el porcentaje de concordancia) 
de todas las casas disponibles cuya concordancia está por encima del valor tolerancia del registro de la búsqueda. 
Por cada casa, se debe invocar a la función mostrarCasa, que mostrará en la página Web las fotos, 
detalles y demás datos que resultarán de interés para el usuario. */
/*  archivos de consulta CARACTERISTICAS.dat y CASAS.dat voy a subir a memoria las casas
    las busquedas las voy leyendo una a una con las funciones que me da la consigna
*/
struct Caract
{
   int idCaract; //potencia de 2 que implica una comodidad diferente 
   char descr[100];
};

struct Casa
{
   int idCasa;
   char direcc[50];
   int idDueno;
   int caractMask; //máscara de bits de 4 bytes, donde cada bit indica la existencia o no de una determinada característica
   int idUsr; //si es cero es que esta disponible
};

struct Busqueda
{
   int idUsr;
   int caractMask;
   int dias;
   double tolerancia;
};

struct Resultado
{
    int idCasa;
    double conc;
};

//compara las características de una casa con 
//las carácterísticas que desea el potencial inquilino, y retorna un porcentaje de coincidencia
double concordancia(int idCasa,int mask);

// Retorna un registro con los parametros de busqueda indicados por el usuario
Busqueda leerBusqueda();  

// Indica si quedan mas busquedas por procesar
bool continuarOperando();

// Muestra por pantalla la casa especificada
void  mostrarCasa(int idCasa);

Array<Casa> subirCasas(){
    Array<Casa> ret = array<Casa>();
    FILE* f = fopen("CASAS.dat", "r+b");
    Casa c = read<Casa>(f);
    while(!feof(f)){
        if(c.idUsr==0){ //si la casa esta disponible 
            arrayAdd<Casa>(ret, c);
        }
        c = read<Casa>(f);
    }
    fclose(f);
    return ret; 
}
 
// decreciente por concordancia
int cmpResultado(Resultado a, Resultado b){
    if( a.conc > b.conc ) return -1;
    if( a.conc < b.conc ) return 1;
    return 0;
}

List<Resultado> filtrarCasas(Array<Casa> casas, Busqueda b){
    List<Resultado> ret = list<Resultado>();
    arrayReset<Casa>(casas);
    while( arrayHasNext<Casa>(casas) ){
        Casa c = *arrayNext<Casa>(casas);
        double conc = concordancia(c.idCasa, b.caractMask);
        if( conc > b.tolerancia ){ // concordancia con la busqueda es mayor que la tolerancia
            Resultado r;
            r.idCasa = c.idCasa;
            r.conc = conc;
            listOrderedInsert<Resultado>(ret, r, cmpResultado);
        }
    }
    return ret;
}

void mostrarCasas(List<Resultado> lst){
    listReset<Resultado>(lst);
    while( listHasNext<Resultado>(lst) ){
        Resultado r = *listNext<Resultado>(lst);
        mostrarCasa(r.idCasa);
    }
}

int main()
{
    //subo a memoria todas las casas disponibles
    Array<Casa> casasDispo = subirCasas();
    while(continuarOperando()){
        Busqueda busq = leerBusqueda();
        List<Resultado> res = filtrarCasas(casasDispo, busq);
        mostrarCasas(res);
    }

    return 0;
}
