
#include <iostream>
// #include "biblioteca/funciones/millis.hpp"
#include "../biblioteca/funciones/files.hpp"
#include "../biblioteca/funciones/strings.hpp"
#include "../biblioteca/funciones/tokens.hpp"
#include "../biblioteca/tads/parte1/Fecha.hpp"
// #include "biblioteca/tads/parte1/Timer.hpp"
#include "../biblioteca/tads/parte1/Coll.hpp"
#include "../biblioteca/tads/parte2/Array.hpp"
#include "../biblioteca/tads/parte2/List.hpp"
#include "../biblioteca/tads/parte2/Map.hpp"
#include "../biblioteca/tads/parte2/Queue.hpp"
#include "../biblioteca/tads/parte2/Stack.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;

struct Categoria
{
   int idCat;
   char descrip[50];
   int m3Desde;  // mts cubicos anuales
   int m3Hasta;  // mts cubicos anuales
   double valorM3;
};
struct RCategoria
{   
    Categoria categ;
    List<int> idCli; //lista con clientes añadidios a la categoria 
};
struct Cliente
{
   int idCli; // ordenado
   char nombre[100];
   char direccion[200];
   int idCatAnt; 
   int lecturaAnterior;
   char consumos[36];
};
struct Medicion
{
   int idCli;
   int lecturaActual;
   int fecha; // aaaammdd
};
struct Consumo
{
   int m3Consumidos;
   int fecha; // aaaammdd
};

Consumo consumo(){
    Consumo ret; 
    ret.m3Consumidos=0;
    ret.fecha=0;
    return ret;
}

RCategoria rcategoria(Categoria c){
    RCategoria ret;
    ret.categ = c;
    ret.idCli = list<int>();
    return ret;
}

int buscarCliente(int idCli,FILE* fCli){
    return 0;
}

// Retorna un array (TAD) con 12 estructuras Consumo que describen el historial de 
// consumo de los ultimos 12 meses, siendo el más antiguo el que ocupa la
// posicion 0 del array, el siguiente menos antiguo ocupa la posicion 1 y asi sucesivamente
Array<Consumo> decodeConsumo(char consumos[]){
    return {};
}

// Funcion inversa a la anterior, retorna el char[36] como char*
char* encodeConsumo(Array<Consumo> arr){
    return NULL;
}

// Retorna el consumo anual segun el actual y el acumulado
int calcularConsumoAnual(int consumoActual,Array<Consumo>& consumos, int fechaActual){
    //eliminar primer dato del array
    arrayRemove<Consumo>(consumos, 0);
    //creo el consumo para agregarlo al array
    Consumo actual = consumo();
    actual.m3Consumidos = consumoActual;
    actual.fecha = fechaActual;
    //agregar cons actual
    arrayAdd<Consumo>(consumos, actual);
    //sumar todos los meses 
    int totConsumo = 0;
    arrayReset<Consumo>(consumos);
    while(arrayHasNext<Consumo>(consumos)){
        Consumo cons = *arrayNext<Consumo>(consumos);
        totConsumo += cons.m3Consumidos; 
    }
    return totConsumo;
}

Map<int, RCategoria> rcategoriaSubir(){
    Map<int, RCategoria> ret = map<int, RCategoria>();
    FILE* f = fopen("CATEGORIAS.dat", "r+b");

    Categoria cat = read<Categoria>(f);
    while(!feof(f)){
        RCategoria rCat = rcategoria(cat);
        mapPut<int, RCategoria>(ret, cat.idCat, rCat);
        cat = read<Categoria>(f);
    }
    fclose(f);
    return ret; 
}

Cliente obtenerCliente(int pos, FILE* fCli){
    seek<Cliente>(fCli, pos);
    return read<Cliente>(fCli);
}

RCategoria asignarCategoria(int mts3Anual, Map<int, RCategoria> mRCat, int idActual){
    //busco la categoria a la que pertenece ahora y comparo el consumo hasta?
    RCategoria rCat = *mapGet<int, RCategoria>(mRCat, idActual); //con * para tener el valor no la pos
    //si se pasa veo si entra en la siguiente categoria hasta encontrar en la que cae 
    while(mts3Anual<rCat.categ.m3Desde || mts3Anual>rCat.categ.m3Hasta){ //osea no cae en la categoria en la que esta actualmente 
        rCat = *mapNextValue<int, RCategoria>(mRCat); //asumo que esta ordenado categorias por desde (de menor a mayor)
    }
    return rCat; 
}

void actualizarArchivoCliente(Cliente cli, FILE* fCli, int pos){
    seek<Cliente>(fCli, pos);
    write<Cliente>(fCli, cli);
    seek<Cliente>(fCli, pos); 
}

void procesarMedicion(Cliente cli, Medicion med, Map<int, RCategoria>& mRCat, FILE* fCli, int pos){
    //vamos a decodificar consumo 
    Array<Consumo> arrCons = decodeConsumo(cli.consumos);
    //calcular consumo anual 
    int mts3Anual = calcularConsumoAnual(med.lecturaActual, arrCons, med.fecha);
    RCategoria catNueva = asignarCategoria(mts3Anual, mRCat, cli.idCatAnt);
    //para punto 1
    //si finalmente el idCategoria es diferente al idCatAnt es que cambio de categoria y lo meto en la list 
    if(catNueva.categ.idCat!=cli.idCatAnt){
        listAdd<int>(catNueva.idCli, cli.idCli);
        //actualizo la categoria
        cli.idCatAnt=catNueva.categ.idCat;
    }
    //para punto 2
    //actualizo lectura y consumo 
    cli.lecturaAnterior=med.lecturaActual; 
    strcpy(cli.consumos, encodeConsumo(arrCons));
    actualizarArchivoCliente(cli, fCli, pos);
}

void mostrarCambiosCategoria(Map<int, RCategoria> mRCat){
    mapReset<int, RCategoria>(mRCat);
    while(mapHasNext<int, RCategoria>(mRCat)){
        RCategoria rCat = *mapNextValue(mRCat);
        cout<<"Los clientes agregados a la categoria "<<rCat.categ.idCat<<"son: "<<endl;
        for(int i=0; i<listSize<int>(rCat.idCli); i++){
            cout<<listRemoveFirst<int>(rCat.idCli)<<endl; //esa funcion devuelve el id que elimina 
            //asi queda la list vacia para el proximo mes chequear si alguien cambio de categoria
        }
    }
}

int main()
{
    Map<int,RCategoria> mRCat = rcategoriaSubir(); //las categorias con la lista de nuevos clientes 

    FILE* fCli = fopen("CLIENTES.dat", "r+b");
    FILE* fMed = fopen("MEDICIONES.dat", "r+b");

    Medicion med = read<Medicion>(fMed);
    while(!feof(fMed)){
        int pos = buscarCliente(med.idCli, fCli);
        if(pos>=0){ //si existe ese cliente
            Cliente cli = obtenerCliente(pos, fCli);
            procesarMedicion(cli, med, mRCat, fCli, pos);
        }
        med = read<Medicion>(fMed);
    }
    
    mostrarCambiosCategoria(mRCat);
    fclose(fCli);
    fclose(fMed);
    return 0;
}
