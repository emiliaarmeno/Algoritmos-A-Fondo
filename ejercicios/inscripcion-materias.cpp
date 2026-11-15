
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

struct Materia
{
    int idMat;
    char desc[20];
    int dto;
    int cupoMin;
    int cupoMax;
};

struct Inscripcion
{
    int idEst;
    int idMat;
    char turno;
    int fecha;
};

struct RMateria
{
    Materia m;
    Map<char, int> turnoCupoActual;  // alumnos del grupo que se esta formando
    Map<char, int> turnoCursos;      // cursos formados
};

RMateria rMateria(Materia m)
{
    RMateria ret;
    ret.m = m;
    ret.turnoCupoActual = map<char, int>();
    ret.turnoCursos = map<char, int>();
    return ret;
}

int cmpRMateriaId(RMateria rm, int id)
{
    return rm.m.idMat - id;
}

// sube a memoria las materias
List<RMateria> rMateriasSubir(FILE* f)
{
    List<RMateria> ret = list<RMateria>();

    unsigned char cantMaterias = read<unsigned char>(f);
    for(int i = 0; i < cantMaterias; i++)
    {
        Materia m = read<Materia>(f);
        RMateria rm = rMateria(m);
        mapPut<char, int>(rm.turnoCupoActual, 'm', 0);
        mapPut<char, int>(rm.turnoCupoActual, 't', 0);
        mapPut<char, int>(rm.turnoCupoActual, 'n', 0);
        mapPut<char, int>(rm.turnoCursos, 'm', 0);
        mapPut<char, int>(rm.turnoCursos, 't', 0);
        mapPut<char, int>(rm.turnoCursos, 'n', 0);
        listAdd<RMateria>(ret, rm);
    }
    return ret;
}

void inscripcionesProcesar(List<RMateria>& lstRMaterias, FILE* f)
{
    Inscripcion ins = read<Inscripcion>(f);
    while(!feof(f))
    {
        RMateria* rm = listFind<RMateria, int>(lstRMaterias, ins.idMat, cmpRMateriaId);
        assert(rm != NULL && "Inscripcion a una materia inexistente");

        int* cupo = mapDiscover<char, int>(rm->turnoCupoActual, ins.turno, 0);
        int* cursos = mapDiscover<char, int>(rm->turnoCursos, ins.turno, 0);

        (*cupo)++;

        // al llegar a cupoMin se abre un curso
        if(*cupo == rm->m.cupoMin)
        {
            (*cursos)++;
        }
        // al llegar a cupoMax se cierra y se empieza un grupo nuevo
        // (son dos if independientes para que funcione si cupoMin == cupoMax)
        if(*cupo == rm->m.cupoMax)
        {
            *cupo = 0;
        }

        ins = read<Inscripcion>(f);
    }
}

void mostrarListado(List<RMateria>& lstRMaterias)
{
    listReset<RMateria>(lstRMaterias);
    while(listHasNext<RMateria>(lstRMaterias))
    {
        RMateria* rm = listNext<RMateria>(lstRMaterias);
        cout << "Materia: " << rm->m.idMat << " - " << rm->m.desc << endl;

        mapReset<char, int>(rm->turnoCursos);
        while(mapHasNext<char, int>(rm->turnoCursos))
        {
            char turno = mapNextKey<char, int>(rm->turnoCursos);
            int cursos = *mapGet<char, int>(rm->turnoCursos, turno);
            int resto = *mapGet<char, int>(rm->turnoCupoActual, turno);

            // si el resto alcanza cupoMin, esos alumnos ya estan en un curso abierto
            int pendientes = 0;
            if(resto > 0 && resto < rm->m.cupoMin)
            {
                pendientes = resto;
            }

            cout << "  Turno " << turno << ": " << cursos << " cursos formados, ";
            cout << pendientes << " estudiantes pendientes de reasignar" << endl;
        }
    }
}

int main()
{
    FILE* f = fopen("DATOS.dat", "r+b");

    List<RMateria> lstRMaterias = rMateriasSubir(f);
    inscripcionesProcesar(lstRMaterias, f);
    mostrarListado(lstRMaterias);

    fclose(f);
    return 0;
}

/* struct Materia
{
   int idMat;
   char desc[20];
   int dto;
   int cupoMin;
   int cupoMax; 
};
struct RMateria
{
    Materia m;
    Map<char, int> turnoCupoActual;
    // cuando llega al maximo cerramos el curso
    // y lo reinicamos, de esa forma si queda en un numero distinto de 0
    // esa sera la cantidad de estudiantes a reasignar
};
RMateria rMateria(Materia m){
    return {m,map<char, int>()};
}
struct Inscripcion
{
   int idEst;
   int idMat;
   char turno;
   int fecha;
};
struct Curso
{
    char turno; 
    int idCurso;
    int cantInscriptos;
    bool cursoAbierto; // indica si el curso esta abierto
};
Curso curso(char turno, int cantInscriptos)
{
    Curso c;
    c.cantInscriptos=cantInscriptos;
    c.cursoAbierto=true;
    c.idCurso=0;
    c.turno=turno;
    return c;
}
int cmpCursosTurnoAbierto(Curso a, Curso b)
{
    if(a.turno == b.turno)
    {
        if(a.cursoAbierto==true)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        if(a.turno=='m'&&b.turno!='m')
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
}

// sube a memoria las materias
List<RMateria> rMateriasSubir(FILE* f){
    List<RMateria> ret = list<RMateria>();
    
    unsigned char cantMaterias = read<unsigned char>(f);
    for( int i=0; i<cantMaterias; i++){
        Materia m = read<Materia>(f);
        RMateria rm = rMateria(m);
        mapPut<char, int>(rm.turnoCupoActual, 'm', 0);
        mapPut<char, int>(rm.turnoCupoActual, 't', 0);
        mapPut<char, int>(rm.turnoCupoActual, 'n', 0);
        listAdd<RMateria>(ret, rm);
    }
    
    return ret;
}

int cmpRMateriaId(RMateria rm, int id){return rm.m.idMat-id;}

int cmpCursoTurno(Curso a, char turno){
    return a.turno==turno ? 0: -1;
}

void inscripcionesProcesar(Map<int, List<Curso>> mFormados, List<RMateria> lstRMaterias, FILE* f){
    Inscripcion ins = read<Inscripcion>(f);
    int contIdCurso = 1; 
    while(!feof(f)){
        // obtengo la RMateria de la inscripcion
        RMateria* rm = listFind<RMateria>(lstRMaterias, ins.idMat, cmpRMateriaId);
        int* cuposActuales = mapGet<char,int>(rm->turnoCupoActual, ins.turno);
        cuposActuales++;
        
        List<Curso> cursosVacios = list<Curso>();
        List<Curso>* lstCursos = mapDiscover<int, List<Curso>>(mFormados, ins.idMat, cursosVacios);
        // ordena la lista por turno y primero cursos abiertos
        listSort<Curso>(*lstCursos, cmpCursosTurnoAbierto);
        if(*cuposActuales > rm->m.cupoMin && *cuposActuales < rm->m.cupoMax)
        {
            // entonces hay un curso de esta materia abierto, debo buscarlo y añadirlo a cantInscriptos de ese curso de ese turno
            // Obtengo el curso ABIERTO donde inscribir el alumno
            Curso* c = listFind<Curso>(*lstCursos, ins.turno, cmpCursoTurno);
            c->cantInscriptos++;
            if(c->cantInscriptos==rm->m.cupoMax)
            {
                c->cursoAbierto=false;
                c->idCurso=contIdCurso;
                contIdCurso++;
                // se asignaron los estudiantes, reinicio el cupo actual para el proximo idCurso
                cuposActuales=0;
            }
        }
        else
        {
            if(*cuposActuales==rm->m.cupoMin)
            {
                // Creamos el curso
                Curso c = curso(ins.turno, *cuposActuales);
                listOrderedInsert<Curso>(*lstCursos, c, cmpCursosTurnoAbierto);
            }
        }
        
        ins = read<Inscripcion>(f);
    }
}

void mostrarCursosFormados(Map<int, List<Curso>> mFormados)
{
    mapReset<int, List<Curso>>(mFormados);
    while(mapHasNext<int, List<Curso>>(mFormados))
    {
        int idMat = mapNextKey<int, List<Curso>>(mFormados);
        List<Curso>* lstCursos = mapGet<int, List<Curso>>(mFormados, idMat);
        listReset<Curso>(*lstCursos);
        cout << "La materia: " << idMat << " tuvo " << listSize<Curso>(*lstCursos) << " cursos repartidos en 3 turnos" << endl;
        while(listHasNext<Curso>(*lstCursos))
        {
            int cont = 0;
            char turnoAnt = 'm';
            Curso* c = listNext<Curso>(*lstCursos);
            if(c->turno==turnoAnt)
            {
                cont++;
            }
            else
            {
                cout << "La materia tuvo " << cont << " cursos formados en el turno " << (int)turnoAnt << endl;
                turnoAnt = c->turno;
                cont = 0;
            }
        }
    }
}

void mostrarEstudiantesPendientes(List<RMateria> lstRMaterias)
{
    listReset<RMateria>(lstRMaterias);
    while(listHasNext<RMateria>(lstRMaterias))
    {
        RMateria* rm = listNext<RMateria>(lstRMaterias);
        mapReset<char, int>(rm->turnoCupoActual);
        while(mapHasNext<char, int>(rm->turnoCupoActual))
        {
            char turno = mapNextKey<char, int>(rm->turnoCupoActual);
            int* cupos = mapGet<char, int>(rm->turnoCupoActual, turno);
            if(*cupos < rm->m.cupoMin)
            {
                cout << "El turno " << (int)turno << " de la materia: " << rm->m.desc; 
                cout << " no pudo completar la inscripcion de " << *cupos << " alumnos y deberan ser reasignados" << endl;
            }
        }
    }
}

// Los cursos se abren con cupoMin estudiantes y se cierran al llegar a cupoMax. 
// Es decir, los primero cupoMax  estudiantes que se inscriban a una misma materia, en un mismo turno, formarán el curso 1, luego comenzará a formarse el curso 2 y así sucesivamente.
// Emitir un listado indicando, para cada materia, para cada turno, cuántos cursos llegaron a formarse y cuántos estudiantes quedan pendientes de ser reasignados.
int main()
{
    Map<int, List<Curso>> mFormados = map<int, List<Curso>>();
    
    FILE* f = fopen("DATOS.dat","r+b");
    List<RMateria> lstRMaterias = rMateriasSubir(f);
    
    inscripcionesProcesar(mFormados, lstRMaterias, f);
    
    mostrarCursosFormados(mFormados);
    mostrarEstudiantesPendientes(lstRMaterias);
    fclose(f);
    return 0;
} */