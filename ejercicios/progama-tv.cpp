
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

/*
    Generar el archivo DESCARTADOS.dat, con aquellos programas que, por cualquier motivo, 
    fueron removidos de la propuesta de planificación. 
    La causa será: 1 si el programa se superponía con otro, o 2 si se emitía dentro del horario ATP 
    sin tener la calificación habilitante.
    Generar el archivo BACHES.dat con los intervalos de tiempo que, luego de haber descartado los programas problemáticos, 
    quedaron sin programación.

    programas es de consulta 
    planificacion es de novedades 
 */

struct Bache
{
    int horaDesde;
    int minDesde;
    int duracion;
};  
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

string programaToString(Programa p);
Programa programaFromString(string s);

struct Hora
{
    int hora; 
    int minuto; 
};
struct Descartado
{
    int idPlanif;
    int causa;
};
string descartadoToString(Descartado d);
Descartado descartadoFromString(string s);

//La función haySuperposicion retorna true si un programa que comienza a la hora hi1 (hora de inicio 1) 
//y tiene una duración de dur minutos se extenderá más allá de la hora hi2 (hora de inicio 2). 
//Sí hi2<hi1 será porque hi2 corresponde a un programa que comienza el siguiente día.
bool haySuperposicion(Hora hi1,int dur,Hora hi2);

//La función invadeHorarioATP retorna true si un programa, o parte del mismo, 
//transcurre dentro del horario Apto para Todo Público (ATP).
bool invadeHorarioATP(Hora h,int dur);

//La función calcularBache retorna la hora de inicio de un bache en la programación y le asigna a durb la duración de dicho bache, en minutos.
Hora calcularBache(Hora hi1,int durp,Hora hi2,int& durb);

//La función continuadoDiaAnterior retorna cuántos minutos del comienzo del día están
//comprometido con la emisión de un programa que continúa del día anterior. Su prototipo es:
int continuadoDiaAnterior();

Coll<Programa> programasSubir(){
    Coll<Programa> cProg = coll<Programa>();
    FILE* f = fopen("PROGRAMAS.dat", "r+b");
    Programa p = read<Programa>(f);
    while(!feof(f)){
        collAdd<Programa>(cProg, p, programaToString);
        p = read<Programa>(f);
    }
    return cProg; 
}

int cmpProgId(Programa p, int id){
    return p.idProg-id;
}

void verificarPrograma(Coll<Programa> cProg, Coll<Descartado>& cDes, Planificacion p1, Planificacion p2){
    Hora hi1 = {p1.horaInicio, p1.minutoInicio};
    Hora hi2 = {p2.horaInicio, p2.minutoInicio};
    //busco la duracion 
    int posP1 = collFind<Programa, int>(cProg, p1.idProg, cmpProgId, programaFromString);
    Programa progP1 = collGetAt<Programa>(cProg, posP1, programaFromString);
    if(haySuperposicion(hi1, progP1.duracion, hi2)){ //verifico si uno se superpone con el siguiente
        //si lo hace lo agrego a descartados (el que empieza mas tarde)
        Descartado d2 = {p2.idPlanif, 1}; //causa = 1 porque fue descartado por superposicion
        collAdd<Descartado>(cDes, d2, descartadoToString);
    }
    if(progP1.atp==1 && invadeHorarioATP(hi1, progP1.duracion)){
        Descartado d1 = {p1.idPlanif, 2}; //causa = 2 porque fue descartado por invadir horario atp
        collAdd<Descartado>(cDes, d1, descartadoToString);
    }
}

int main()
{
    Coll<Programa> cProg = programasSubir();
    Coll<Descartado> cDes = coll<Descartado>();

    FILE* f = fopen("PLANIFICACION.dat", "r+b");
    Planificacion progPlanif = read<Planificacion>(f);
    while(!feof(f)){
        Planificacion progSig = read<Planificacion>(f);
        verificarPrograma(cProg, cDes, progPlanif, progSig); //dado el programa de la planificacion si invade atp y superposicion se mete en descartados
        progSig=progPlanif;
        //escribirArchivo(cDes);

    }

    return 0;
}
