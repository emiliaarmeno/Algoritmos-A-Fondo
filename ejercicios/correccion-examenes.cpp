
#include <iostream>
//#include "biblioteca/funciones/millis.hpp"
#include "../biblioteca/funciones/files.hpp"
#include "../biblioteca/funciones/strings.hpp"
#include "biblioteca/funciones/tokens.hpp"
#include "biblioteca/tads/parte1/Fecha.hpp"
//#include "biblioteca/tads/parte1/Timer.hpp"
#include "biblioteca/tads/parte1/Coll.hpp"
#include "biblioteca/tads/parte2/Array.hpp"
#include "../biblioteca/tads/parte2/List.hpp"
#include "biblioteca/tads/parte2/Map.hpp"
#include "biblioteca/tads/parte2/Queue.hpp"
#include "biblioteca/tads/parte2/Stack.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;

struct Alumno
{
   int idAlu;
   int legajo;
   char nombre[50];
   int idCur;
};

struct Curso
{
   int idCur; 
   char descr[10]; // K1027
   char turno; // 'M','T'... 
};

struct Tema //tiene la respuesta correcta de una sola afirmacion
{
   int idTema; 
   int idAfir;
   char tipoAfir;// 'T','P'
   char afir[250]; 
   char rta; // 'V', 'F'
};

//busco en el archivo todos los que tienen el mismo idTema y lo guardo aca
//junta las 20 afirmaciones de un mismo tema
struct TTema 
{
    int idTema;
    Coll<Tema> afirmaciones; // 20 elementos: 10 con tipoAfir='T', 10 con tipoAfir='P'
};

struct Respuesta
{
   int idAlu; 
   char tipoAfir; // 'T' o 'P' 
   int idAfir; // 1 a 10
   char rta; // 'V' o 'F'  
   int idTema; // 1 a 5
};

struct Nota
{
    int idAlu;
    int idCur;
    int nota; 
};

/* 
    Cada examen tiene 20 afirmaciones VoF
    Existen 5 temas diferentes. Cada tema representa un conjunto de 20 afirmaciones; 10 prácticas y 10 teóricas.
    
    La calificacion se calcula: 0,5*b-0,5*m (cantidad de respuestas b bien y m mal)
    El examen estara aprobado cuando la nota es mayor o igual a 6, hay una restriccion 
    se tiene que haber contestado 3 puntos y cada parte sumar un mínimo de 2,5 puntos por categoría sino la calificación será R (reprobado).

    // 1. cargo temas: armo un TTema por cada uno de los 5 temas (los busco filtrando TEMAS.dat)
    // 2. cargo alumnos y cursos en colecciones, para consulta

*/
string alumnoToString(Alumno a);
Alumno alumnoFromString(string s);

string notaToString(Nota n);

Coll<Alumno> alumnosSubir(){
    FILE* f = fopen("ALUMNOS.dat", "r+b");
    Coll<Alumno> ret = coll<Alumno>();
    Alumno a = read<Alumno>(f);
    while(!feof(f)){
        collAdd<Alumno>(ret, a, alumnoToString);
        a = read<Alumno>(f);
    }
    fclose(f);
    return ret; 
}

string temaToString(Tema t);
Tema temaFromString(string s);

//funciones que pide el punto 1 

// Funcion de inicializacion
TTema ttema(){
    TTema tt; 
    tt.idTema = -1; //sin cargar 
    tt.afirmaciones = coll<Tema>();
    return tt; 
}

int cmpTemaId(TTema t, int id){
    return t.idTema-id;
}

Coll<TTema> ttemasSubir(){
    FILE* f = fopen("TEMAS.dat", "r+b");
    Coll<TTema> ret = coll<TTema>();

    while(!feof(f)){
        Tema t = read<Tema>(f);
        
        //tengo que hacer descubrimiento
        int posTem = collFind<TTema, int>(ret, t.idTema, cmpTemaId, tTemaFromString);
        
        if(posTem>=0){ //lo encontro
            TTema tt = collGetAt<TTema>(ret, posTem, tTemaFromString);
            collAdd<Tema>(tt.afirmaciones, t, temaToString); //esto es en el interno 
            collSetAt<TTema>(ret, tt, posTem, tTemaToString);
        }else{//no lo encontro
            //lo creo con los datos necesarios
            TTema tt = ttema();
            tt.idTema=t.idTema;
            collAdd<Tema>(tt.afirmaciones, t, temaToString);
            //lo agrego a la coleccion que retorno
            collAdd<TTema>(ret, tt, tTemaToString);
        }

    }

    fclose(f);
    return ret;
}

//funcion de comparacion para buscar por clave compuesta (tipoAfir + idAfir)
int cmpRTema(Tema t, Respuesta r) 
{
    if(t.tipoAfir != r.tipoAfir){
        return t.tipoAfir - r.tipoAfir; //distinto tipo, no coincide
    }
    return t.idAfir - r.idAfir; //mismo tipo, comparo por idAfir
}


TTema tTemaFromString(string s);
string tTemaToString(TTema t);

TTema tTemaBuscar(Coll<TTema> cTT, int idTema){
    int posTema = collFind<TTema, int>(cTT, idTema, cmpTemaId, tTemaFromString);
    TTema t = collGetAt<TTema>(cTT, posTema, tTemaFromString);
    return t;
}

// Evalua un examen y retorna la calificacion
//si el examen tiene un R sera un numero negativo
int ttemaEvaluar(TTema t, List<Respuesta> lst){
    int correctasTeo = 0, incorrectasTeo = 0;
    int correctasPrac = 0, incorrectasPrac = 0;

    listReset(lst);
    while(listHasNext(lst)){
        Respuesta* pr = listNext(lst); // puntero a la Respuesta actual
        Respuesta r = *pr;             // "desreferencio" para tener el valor

        // busco la afirmacion correcta correspondiente en el TTema
        int pos = collFind<Tema, Respuesta>(t.afirmaciones, r, cmpRTema, temaFromString);
        Tema correcta = collGetAt(t.afirmaciones, pos, temaFromString);

        bool acerto = (correcta.rta == r.rta);

        if(r.tipoAfir == 'T'){
            if(acerto) correctasTeo++; else incorrectasTeo++;
        } else { // 'P'
            if(acerto) correctasPrac++; else incorrectasPrac++;
        }
    }

    double ptTeo = 0.5*correctasTeo - 0.5*incorrectasTeo;
    double ptPrac = 0.5*correctasPrac - 0.5*incorrectasPrac;

    // regla 1: si alguna parte no llega a 2.5, es R (uso -1 como codigo para R)
    if(ptTeo < 2.5 || ptPrac < 2.5){
        return -1; // representa "R"
    }
    
    // regla 2: si alguna parte no llega a 3, reprobado con nota 5
    // (esto se chequea AUNQUE el total sumado de 6 o mas, por eso va antes de mirar el total)
    if(ptTeo < 3 || ptPrac < 3){
        return 5;
    }

    // regla 3: nota final segun el total
    double total = ptTeo + ptPrac;
    int nota = (int)total;

    return nota;
}

int cmpAluId(Alumno a, int idAlumno){
    return a.idAlu-idAlumno;
}

void guardarNota(int nota, int idAlumno, Coll<Alumno> cAlu, Coll<Nota>& cNta){
    int pos = collFind<Alumno, int>(cAlu, idAlumno, cmpAluId, alumnoFromString);
    Alumno alum = collGetAt<Alumno>(cAlu, pos, alumnoFromString);
    int idCurso = alum.idCur;
    Nota n = {idAlumno, idCurso, nota};
    collAdd<Nota>(cNta, n, notaToString);
}

int main()
{
    //voy a recorrer el archivo de Respuestas que esta ordenado por idAlu 
    //corte de control
    Coll<TTema> cTT = ttemasSubir();
    Coll<Nota> cNta = coll<Nota>();
    Coll<Alumno> cAlu = alumnosSubir();

    FILE* f = fopen("RESPUESTAS.dat", "r+b");
    Respuesta res = read<Respuesta>(f);
    int idActual = res.idAlu;
    int idTemaActual = res.idTema;

    //hago una lista para mandar a la funcion de evaluar 
    List<Respuesta> respAlumno = list<Respuesta>();

    while(!feof(f)){ //mientras que no se termine el archivo 
        //si cambio de alumno ya termine de cargar sus respuestas entonces lo evaluo (archivo ordenado por idAlu)
        if(res.idAlu!=idActual){ 
            TTema temActual = tTemaBuscar(cTT, idTemaActual);
            int nota = ttemaEvaluar(temActual, respAlumno);
            // guardo o muestro la nota de idAluActual 
            guardarNota(nota, idActual, cAlu, cNta); 
            // arranco de nuevo para el nuevo alumno
            idActual = res.idAlu;
            respAlumno = list<Respuesta>(); //lista vacia
        }

        listAdd(respAlumno, res); // agrego esta respuesta a la lista del alumno actual 
        res = read<Respuesta>(f);
        
    }

    // evaluo al ultimo alumno, que no disparo el cambio dentro del while
    TTema temActual = tTemaBuscar(cTT, idTemaActual);
    int nota = ttemaEvaluar(temActual, respAlumno);
    guardarNota(nota, idActual, cAlu, cNta);
    
    fclose(f);
    return 0;
}
