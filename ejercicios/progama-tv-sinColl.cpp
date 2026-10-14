
#include <iostream>
//#include "biblioteca/funciones/millis.hpp"
#include "../biblioteca/funciones/files.hpp"
#include "../biblioteca/funciones/strings.hpp"
#include "biblioteca/funciones/tokens.hpp"
#include "biblioteca/tads/parte1/Fecha.hpp"
//#include "biblioteca/tads/parte1/Timer.hpp"
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

struct Planificacion
{
    int idPlanif; 
    int idProg;
    int horaInicio;     
    int minutoInicio;   
};
struct Programa
{
    int idProg;
    char titulo[100];
    int duracion; // minutos
    int atp; // 0=>no, 1=>si 
};
struct Descartado
{
    int idPlanif;
    int causa; //1 superposicion 2 invade ATP
};
struct Bache
{
    int horaDesde;
    int minDesde;
    int duracion;
};  

List<Programa> subirProgramas(){
    List<Programa> ret = list<Programa>();
    FILE* f = fopen("PROGRAMAS.dat", "r+b");
    Programa pr = read<Programa>(f);

    while(!feof(f)){
        listAdd<Programa>(ret, pr);
        pr = read<Programa>(f);
    }

    return ret; 
}

int horaToMinutos(int h, int m){
    return 0;
}

int cmpProgId(Programa p, int id){
    return p.idProg-id;
}

void inicializarArray(int arrMin[]){
    for(int i=0; i<1440; i++){
        arrMin[i]=0;
    }
}

//se fija si donde queremos insertar es todo igual a cero osea que no hay un programa ahi 
bool checkSuperposicion(int arrMin[], int inicio, int duracion, bool& superposicion){
    int i=0;
    while(arrMin[inicio+i]==0 && i<inicio+duracion){
        i++;
    }
    //sale si no es igual a cero (hay programa --> lo descartamos) o si se termino el espacio que verificamos
    //devuelve true cuando no habia programa y false cuando hay que descartar
    superposicion = !(i==inicio+duracion);
    return superposicion; 
}

bool checkATP(int arrMin[], int esATP, int inicio, int duracion, bool& invade){
    int inicioATP = 420; //7am
    int finATP = 1320; //22pm 
    
    if(esATP==0){//es atp
        invade=false; 
    }else{ //no es atp

        //si el inicio mayor a inicio ATP  el final (inicio+duracion) menor a finATP 
        //esta dentro del horario atp entonces
        if(inicio>=inicioATP && inicio<=finATP){
            invade = true; 
        }else{
            //si el programa sigue pasando cuando el horario ATP empezo 
            if(inicio+duracion>=inicioATP){
                invade = true; 
            }
        }
    }
    return invade; 
}

void agregarIdProg(int arrMin[], Planificacion pla, List<Programa> lProg, List<Descartado> lDesc){
    int inicio = horaToMinutos(pla.horaInicio, pla.minutoInicio);
    Programa p = *listFind<Programa, int>(lProg, pla.idProg, cmpProgId); //pongo el asterisco para que me devuelva el valor en vez de la direccion de memoria 
    bool superposicion;
    bool invade; 
    checkSuperposicion(arrMin, inicio, p.duracion, superposicion);
    checkATP(arrMin, p.atp, inicio, p.duracion, invade);

    if(!superposicion && !invade){ //solo agrego si no superpone ni invade 
        for(int i=0; i<p.duracion; i++){
            arrMin[inicio+i]=p.idProg; //metemos en el espacio la programacion
        }
    }else{
        //descartamos este programa 
        int causa; 
        if(superposicion){
            causa = 1;
        }else if(invade){
            causa = 2; 
        }
        Descartado d = {pla.idPlanif, causa}; //la causa es 1 porque fue descartado por superposicion 
        listAdd<Descartado>(lDesc, d);
    }
}

void horaTransform(int minTot, int& horaDesde, int& minDesde);

void buscarBaches(int arrMin[], List<Bache>& lBach){
    Bache b = {0,0,0}; 
    bool estoyEnBache;
    for(int i=0; i<1440; i++){
        if(arrMin[i]==0 && !estoyEnBache){ //entra cuando no hay programa y es el inicio del bache
            estoyEnBache=true; 
            horaTransform(i, b.horaDesde, b.minDesde);
        }
        if(estoyEnBache && arrMin[i]!=0){ //entra cuando finaliza el bache y comienza un programa
            estoyEnBache=false;
            int inicioBache = horaToMinutos(b.horaDesde, b.minDesde);
            b.duracion = i-inicioBache;

            listAdd<Bache>(lBach, b);
        }
    }
}

void generarArchivoDescartes(List<Descartado> lDesc){
    FILE* f = fopen("DESCARTADOS.dat", "w+b");
    listReset<Descartado>(lDesc);
    while(listHasNext<Descartado>(lDesc)){
        write<Descartado>(f, listRemoveFirst<Descartado>(lDesc));//remove first devuelve el nodo que elimino
    }
    fclose(f);
}
void generarArchivoBaches(List<Bache> lBach){
    FILE* f = fopen("BACHES.dat", "w+b");
    listReset<Bache>(lBach);
    while(listHasNext<Bache>(lBach)){
        write<Bache>(f, listRemoveFirst<Bache>(lBach));//remove first devuelve el nodo que elimino
    }
    fclose(f);
}

int main()
{
    FILE* f = fopen("PLANIFICACION.dat", "r+b");
    List<Programa> lProg = subirProgramas();
    List<Descartado> lDesc = list<Descartado>();
    List<Bache> lBach = list<Bache>();
    int arrMin[1440]; //el array que por cada minuto tiene el idProg asociado
    inicializarArray(arrMin);

    Planificacion pla = read<Planificacion>(f);
    while(!feof(f)){
        agregarIdProg(arrMin, pla, lProg, lDesc); //agrega o descarta por superposicion 
        pla = read<Planificacion>(f);
    }
    buscarBaches(arrMin, lBach);
    fclose(f);

    generarArchivoDescartes(lDesc);
    generarArchivoBaches(lBach);
    
    return 0;
}
