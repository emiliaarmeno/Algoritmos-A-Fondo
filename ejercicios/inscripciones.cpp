
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

#include "torneo de futbol tools.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;

struct Inscripcion
{
   int idAlu;
   int idCur;
   int fecha; // aaaammdd de inscripcion
};

struct Curso
{
   int idCur; 
   char turno; // M, T o N
   int cap;
   char materia[20]; 
};

struct RCurso
{
   Curso c;
   int cantInscriptos; //este campo es para comparar con cap (esta en curso, c.cap)
};

struct RAlumno
{
    int idAlu;
    Coll<string> materiasRechazadas; // coleccion de materias, cada una un string
};

//como es mi estructura que uso para resolver el problema no me obliga a seguir trabajando con char array
struct RMateria
{
    string materia; //char materia[20]; esto me complicaba aasi que dejo el otro
    Coll<int> idCurDisp;
};

//esto lo tendria que hacer con algoritmos tools 
string rCursoToString(RCurso rc){
    return "";
}

RCurso rCursoFromString(string s);

RAlumno rAlumnoFromString(string s);

string rAlumnoToString(RAlumno ra);

string rMateriaToString(RMateria rm){
    return rm.materia + "-" + collToString(rm.idCurDisp); // concateno string con string directo
}

RMateria rMateriaFromString(string s){
    int pos = indexOf(s, '-');
    RMateria rm;
    rm.materia = substring(s, 0, pos);       // asignacion directa, sin loop
    rm.idCurDisp = collFromString<int>(substring(s, pos+1));
    return rm;
}

Coll<RCurso> cursosSubir(){
    //creo coll vacio 
    Coll<RCurso> cCur = coll<RCurso>();

    FILE* f = fopen("CURSOS.dat", "r+b");
    Curso c = read<Curso>(f); //el archivo tiene elementos del tipo Curso no RCurso
    while(!feof(f)){
        RCurso rc = {c, 0}; //si tuviera el algoritmos tools haria: RCurso rc = rCurso(c,0);
        collAdd<RCurso>(cCur, rc, rCursoToString);
        c = read<Curso>(f);
    }
    fclose(f);
    return cCur;
};

int cmpRCursoId(RCurso rc, int id){
    return rc.c.idCur-id;
}

int cmpRAluId(RAlumno ra, int id){
    return ra.idAlu-id;
}

void punto1Procesar(Inscripcion ins, Coll<RCurso>& cCur, Coll<RAlumno>& cAlu){//paso cursos por referencia para que la proxima inscripcion lo tenga
    //busco el curso al que se quiere inscribir el alumno
    int posCur = collFind<RCurso, int>(cCur, ins.idCur, cmpRCursoId, rCursoFromString);
    RCurso rc = collGetAt(cCur, posCur, rCursoFromString);

    if(rc.cantInscriptos<rc.c.cap){ //si alcanza el lugar lo acepto
        rc.cantInscriptos++;
        collSetAt(cCur, rc, posCur, rCursoToString); //seteo los cambios en la coleccion por referencia
        
    }else{
        //esta materia fue rechazada, no agrego mas alumnos inscriptos sino que hago descubrimiento
        int pos = collFind<RAlumno, int>(cAlu, ins.idAlu, cmpRAluId, rAlumnoFromString);
        if(pos>=0){
            //esta en la coleccion entonces le agrego la materia rechazada y lo seteo
            //agarro el alumno de la inscripcion rechazada
            RAlumno ralu = collGetAt<RAlumno>(cAlu, pos, rAlumnoFromString);
            //le agrego a su campo coleccion de materias rechazadas la nueva 
            collAdd<string>(ralu.materiasRechazadas, rc.c.materia, stringToString);
            collSetAt<RAlumno>(cAlu, ralu, pos, rAlumnoToString);

        }else{
            //creo un nuevo alumno y le agrego la materia rechazada
            Coll<string> mRech = coll<string>('-'); //uso un separador distinto para que no se confunda con el de cAlu que es el default '|'
            RAlumno ra = {ins.idAlu, mRech}; 
            collAdd<string>(ra.materiasRechazadas, rc.c.materia, stringToString);
            collAdd<RAlumno>(cAlu, ra, rAlumnoToString);
        }
    }
}

void punto1Mostrar(Coll<RAlumno> cAlu){
    collReset(cAlu);
    while(collHasNext(cAlu)){
        RAlumno ra = collNext(cAlu, rAlumnoFromString);
        cout<<"Alumno"<<ra.idAlu<<endl;
        cout<<"Materias rechazadas:"<<endl;
        //recorro y muestro las materias rechazadas (estan en la coleccion)
        collReset(ra.materiasRechazadas);
        while(collHasNext(ra.materiasRechazadas)){
            string matRech = collNext(ra.materiasRechazadas, stringToString);
            cout<<matRech<<endl;
        }
    }
}

int cmpRMatString(RMateria rm, string s){
    return cmpString(rm.materia, s);
    //esta opcion qeuda mas prolija 
    //return cmpString(cStringToString(rm.materia), s);
}

void punto2Procesar(Inscripcion ins, Coll<RCurso> cCur, Coll<RMateria>& cMat){
    //recorro la coleccion de cursos y me fijo cuales todavia tienen menor cantInscriptos que capacidad 
    //a esos los meto en la coleccion de materias

    collReset<RCurso>(cCur);
    while(collHasNext<RCurso>(cCur)){
        RCurso rc = collNext<RCurso>(cCur, rCursoFromString);

        if(rc.cantInscriptos<rc.c.cap){
            //tengo que agregar un elemento a la coleccion de materias
            //busco si ya esta esa materia
            int posMat = collFind<RMateria, string>(cMat, rc.c.materia, cmpRMatString, rMateriaFromString);

            if(posMat>=0){//esta en la coleccion, la encontro
                RMateria rm = collGetAt<RMateria>(cMat, posMat, rMateriaFromString);
                //solo tengo que agregar este nuevo curso a su coleccion
                collAdd<int>(rm.idCurDisp, rc.c.idCur, intToString);
                //seteo los cambios en la coleccion de materias
                collSetAt<RMateria>(cMat, rm, posMat, rMateriaToString);
            }else{ //la materia que todavia tiene cupo no esta en la coleccion
                //agrego un nuevo elemento a la coleccion de materias, agregandole el curso a la coleccion de cursos del elemento
                Coll<int> cursosDisp = coll<int>('-');
                collAdd<int>(cursosDisp, rc.c.idCur, intToString);
                RMateria rmNuev = {rc.c.materia, cursosDisp};
                collAdd<RMateria>(cMat, rmNuev, rMateriaToString);
            }
        }
    }


}

void punto2Mostrar(Coll<RMateria> cMat){
    cout<<"Cursos con cupos disponibles: "<<endl; 
    collReset<RMateria>(cMat);
    while(collHasNext<RMateria>(cMat)){
        RMateria rm = collNext(cMat, rMateriaFromString);
        cout<<rm.materia<<endl;
        //muestro todos los cursos recorriendo la coleccion de int 
        collReset<int>(rm.idCurDisp);
        while(collHasNext<int>(rm.idCurDisp)){
            int id = collNext<int>(rm.idCurDisp, stringToInt);
            cout<<id<<endl;
        }
    }
}

/* 
    INSCRIPCIONES.dat archivo de novedades, ordenado ascendentemente según la fecha de la inscripción.
    CURSOS.dat archivo de consulta (siempre la misma informacion)

    1. Por cada alumno, emitir el listado de materias en que su inscripción resultó rechazada por falta de capacidad en el curso.
    por cada alumno significa ir viendo cada inscripcion 

    voy recorriendo el archivo de INSCRIPCIONES.dat y por cada inscripcion me fijo si segun el idCur la capacidad no fue superada
    creo que esto no --> tengo que wrappear inscripcion con un bool para inscripcion aceptada o rechazada
    y tambien a curso para un campo de cantidad de inscriptos asi lo comparo con capacidad 
    
    de ahi cada vez que tenga una inscripcion busco el curso comparo cantInscriptos con capacidad, 
    si es menor le sumo uno a cantInscriptos y pongo el campo de inscripcion en aceptada
    si es igual (porque mayor no dejo) no sumo a cantInscriptos y pongo en el campo de inscripicon rechazda 

    al final lo resolvi con una nueva coleccion (de RAlumno) y en cada elemento una coleccion con las materias rechazadas 

    2. Por cada materia, generar un listado de cursos donde, luego de procesar las inscripciones, quedaron cupos disponibles.
    tengo que hacer otra estructura que sea materia (nombre, coll int de idCur), 
    entonces tendria una coleccion de materias que tienen el nombre y una coleccion de cursos todavia con cupo
    se que todavia tiene cupo porque cuando recorro el archivo de inscripciones me fijo 
*/

int main()
{
    //subo cursos a memoria (como coleccion)
    Coll<RCurso> cCur = cursosSubir();
    Coll<RAlumno> cAlu = coll<RAlumno>();
    Coll<RMateria> cMat = coll<RMateria>();

    //barro archivo movimientos
    FILE* f = fopen("INSCRIPCIONES.dat", "r+b");
    Inscripcion ins = read<Inscripcion>(f);
    while(!feof(f)){
        punto1Procesar(ins, cCur, cAlu);
        ins = read<Inscripcion>(f);
    }
    punto2Procesar(ins, cCur, cMat);
    punto1Mostrar(cAlu);
    punto2Mostrar(cMat);

    fclose(f);
    return 0;
}
