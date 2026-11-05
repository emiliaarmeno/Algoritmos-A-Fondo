
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

struct Alumno
{
    int idAlu;
    int legajo;
    char nombre[50];
    int idCur;
};
struct RAlumno
{
    Alumno a;
    int calif;  // 5 a 10, 0 significa R
};
struct Curso
{
    int idCur;
    char descr[10];  // K1027
    char turno;      // 'M','T'...
};
struct Tema  // una respuesta correcta
{
    int idTema;
    int idAfir;
    char tipoAfir;  // 'T','P'
    char afir[250];
    char rta;  // 'V', 'F'
};
struct Pregunta
{
    int idAfir;
    char rta;
};
struct TTema
{
    int idTema;
    List<Pregunta> practicos;
    List<Pregunta> teoricos;
};
struct Respuesta
{
    int idAlu;
    char tipoAfir;  // 'T' o 'P'
    int idAfir;     // 1 a 10
    char rta;       // 'V' o 'F'
    int idTema;     // 1 a 5
};

int cmpTTID(TTema tt, int idTem){ return tt.idTema-idTem; }
int cmpAluID(Alumno a, int idAlu){ return a.idAlu-idAlu; }
int cmpPregIdAfir(Pregunta p, int idAfir){ return p.idAfir-idAfir; }
int cmpTT(Tema a, Tema b){ return a.idTema-b.idTema; }
int cmpRACur(RAlumno a, RAlumno b){ return a.a.idCur-b.a.idCur; }
int cmpCurID(Curso c, int id){ return c.idCur - id; }

// Funcion de inicializacion
TTema ttema(int id)
{
    TTema t;
    t.idTema = id;
    t.practicos = list<Pregunta>();
    t.teoricos = list<Pregunta>();
    return t;
}

RAlumno rAlumno(){
    RAlumno ra;
    ra.calif=0;
    return ra;
}

// Evalua un examen y retorna la calificacion
int ttemaEvaluar(TTema t, List<Respuesta> lst)
{
    int calificacion;
    // contadores de respuestas correctas e incorrectas
    int praCorrec = 0;
    int teoCorrec = 0;
    int praIncorr = 0;
    int teoIncorr = 0;

    // recorro las respuestas y las voy comparando con las correctas
    listReset<Respuesta>(lst);
    while(listHasNext<Respuesta>(lst))
    {
        Respuesta* r = listNext<Respuesta>(lst);

        // si es de tipo teorico lo busco en la lista de preguntas teoricas del tema
        if(r->tipoAfir == 'T')
        {
            // el ttema que me llega es el que hizo el alumno con las respuestas ordenadas por tipo
            Pregunta afirCorrecta = *listFind<Pregunta, int>(t.teoricos, r->idAfir, cmpPregIdAfir);

            // me fijo si la respondio bien y sumo para cont de teoricos 
            (afirCorrecta.rta == r->rta)? teoCorrec++ : teoIncorr++;
        }
        else
        {   // el tipo de afirm es practico
            // busco la afirmacion en la lista de afirm practicas
            Pregunta afirCorrecta = *listFind<Pregunta, int>(t.practicos, r->idAfir, cmpPregIdAfir);

            //segun como respondio sumo a los contadores
            (afirCorrecta.rta == r->rta)? praCorrec++ : praIncorr++;
        }
    }

    // calculo nota con los contadores
    double totTeo = (0.5 * teoCorrec) - (0.5 * teoIncorr);
    double totPra = (0.5 * praCorrec) - (0.5 * praIncorr);

    if(totPra < 2.5 || totTeo < 2.5)
    {
        // no tendra nota numerica, solo R
        calificacion = 0;  // esto lo tomo como R
    }
    else if((totPra >= 2.5 || totTeo >= 2.5) && (totPra < 3 || totTeo < 3))
    {
        // tiene nota numerica pero no aprueba
        calificacion = 5;
    }
    else if(totPra >= 3 || totTeo >= 3)
    {
        // aprobo
        calificacion = totTeo + totPra;
    }

    return calificacion;
}

//los meto en la lista odenando por idTema
List<Tema> temasSubir(){
    List<Tema> ret = list<Tema>();
    FILE* f = fopen("TEMAS.dat", "r+b");

    Tema t = read<Tema>(f);
    while(!feof(f)){
        listOrderedInsert<Tema>(ret, t, cmpTT);
        t = read<Tema>(f);
    }

    fclose(f);
    return ret; 
}

List<TTema> tTemaOrdenar(List<Tema>& desordenado){

    List<TTema> ret = list<TTema>();

    listReset<Tema>(desordenado);
    while( listHasNext<Tema>(desordenado) )
    {
        Tema t = *listNext<Tema>(desordenado);

        //ya esta el tema en ordenado? 
        TTema* tt = listFind<TTema,int>(ret, t.idTema, cmpTTID);

        //no estaba 
        if( tt==NULL )                                   
        {
            //lo creo y me quedo con el puntero
            tt = listAdd<TTema>(ret, ttema(t.idTema));   // listAdd devuelve T*
        }

        Pregunta p;
        p.idAfir = t.idAfir;
        p.rta = t.rta;

        //lo agrego al adecuado
        ( t.tipoAfir=='T' )? listAdd<Pregunta>(tt->teoricos, p) : listAdd<Pregunta>(tt->practicos, p);
                        
    }

    return ret;
}

List<Alumno> alumnosSubir(){
    List<Alumno> ret = list<Alumno>();
    FILE* f = fopen("ALUMNOS.dat", "r+b");

    Alumno a = read<Alumno>(f);
    while(!feof(f)){
        listAdd<Alumno>(ret, a);
        a = read<Alumno>(f);
    }

    fclose(f);
    return ret; 
}
List<Curso> cursosSubir(){
    List<Curso> ret = list<Curso>();
    FILE* f = fopen("CURSOS.dat", "r+b");

    Curso a = read<Curso>(f);
    while(!feof(f)){
        listAdd<Curso>(ret, a);
        a = read<Curso>(f);
    }

    fclose(f);
    return ret; 
}

//voy agarrando todas las respuestas del alumno y las comparo con las correctas asi con todos los alumnos 
//devuelvo la lista con los alumnos ya con nota 
List<RAlumno> respuestasProcesar(List<TTema> ordenado, List<Alumno> alumnos)
{
    List<RAlumno> calificados = list<RAlumno>();
    // recorro respuestas y los que no aparecen es porque no fueron a rendir
    FILE* f = fopen("RESPUESTAS.dat", "r+b");
    Respuesta r = read<Respuesta>(f);
    while(!feof(f))
    {
        int idTemaAnt = r.idTema;
        int idAluAnt = r.idAlu;
        List<Respuesta> respuestaAlumno = list<Respuesta>();
        while(idAluAnt == r.idAlu && idTemaAnt == r.idTema && !feof(f))
        {
            // voy guardando todas las respuestas de un mismo alumno en una list
            // (para mandar al ttemaEvaluar)
            listAdd<Respuesta>(respuestaAlumno, r);

            idTemaAnt = r.idTema;
            idAluAnt = r.idAlu;
            r = read<Respuesta>(f);
        }

        // agarro el tema y el alumno, creo el ralumno
        TTema* t = listFind<TTema, int>(ordenado, idTemaAnt, cmpTTID);
        Alumno* a = listFind<Alumno, int>(alumnos, idAluAnt, cmpAluID);

        RAlumno ra = rAlumno();
        ra.a = *a;
        // llamo a ttemaEvaluar la calificacion que devuelve la meto en la list que retorno
        int calificacion = ttemaEvaluar(*t, respuestaAlumno);
        ra.calif = calificacion;
        listAdd<RAlumno>(calificados, ra);

        listFree<Respuesta>(respuestaAlumno);
    }
    fclose(f);
    return calificados;
}


void cursoMostrar(int idCur, Queue<RAlumno>& q, int aprobaron, List<Curso>& cursos)
{
    Curso* c = listFind<Curso,int>(cursos, idCur, cmpCurID);
    int rindieron = queueSize<RAlumno>(q);
    int reprobaron = rindieron - aprobaron;

    cout << "Curso: " << c->descr << endl;
    cout << "Rindieron: " << rindieron << endl;
    cout << "Aprobaron: " << aprobaron << " (" << aprobaron*100/rindieron << "%)" << endl;
    cout << "Reprobaron: " << reprobaron << " (" << reprobaron*100/rindieron << "%)" << endl;
    cout << "Alumno\tCalificación" << endl;

    int suma = 0;
    while( !queueIsEmpty<RAlumno>(q) )
    {
        RAlumno ra = queueDequeue<RAlumno>(q);
        cout << ra.a.nombre << "\t";
        if( ra.calif==0 ) cout << "R"; else cout << ra.calif;
        cout << endl;
        suma += ra.calif;           // decisión: la R cuenta como 0 en el promedio
    }
    cout << "Promedio: " << suma / rindieron << endl;
}

void rAlumnoMostrar(List<RAlumno>& calificados, List<Curso> cursos){
    //ordeno los alumnos por curso
    listSort<RAlumno>(calificados, cmpRACur);

    Queue<RAlumno> delCurso = queue<RAlumno>();
    int idCurActual = -1;
    int aprobaron = 0;

    listReset<RAlumno>(calificados);
    while(listHasNext<RAlumno>(calificados)){
        RAlumno ra = *listNext<RAlumno>(calificados);

        if( ra.a.idCur != idCurActual )              // empieza un curso nuevo
        {
            if( idCurActual != -1 )
            {
                cursoMostrar(idCurActual, delCurso, aprobaron, cursos);
                aprobaron = 0;
            }
            idCurActual = ra.a.idCur;
        }

        queueEnqueue<RAlumno>(delCurso, ra);
        if( ra.calif >= 6 ) aprobaron++;   
    }

    // cierro el último curso
    if( idCurActual != -1 )                          
    {
        cursoMostrar(idCurActual, delCurso, aprobaron, cursos);
    }
}

int main()
{
    // subir temas (afirmaciones correctas) a memoria
    List<Tema> desordenado = temasSubir();
    // ordeno los temas en una list de ttema, osea guardo por cada tema las
    // afirmaciones teoricas y practicas separadas
    List<TTema> ordenado = tTemaOrdenar(desordenado);
    // subir alumnos y cursos a memoria (en su estructura original)
    List<Alumno> alumnos = alumnosSubir();
    List<Curso> cursos = cursosSubir();
    // leer RESPUESTAS.dat procesar y generar una Calificacion por alumno
    // las calificaciones las meto en la list de RAlumno y los ordenar las
    // calificaciones por curso
    List<RAlumno> alumnosCalificados = respuestasProcesar(ordenado, alumnos);
    // recorrer por curso (corte de control) e imprimir
    rAlumnoMostrar(alumnosCalificados, cursos);

    return 0;
}
