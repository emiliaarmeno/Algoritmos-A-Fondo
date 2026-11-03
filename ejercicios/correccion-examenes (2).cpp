
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
    int calif;  // 5 a 10; 0 significa R
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

int cmpTTID(TTema tt, int idTem);
int cmpAluID(Alumno a, int idAlu);
int cmpPregIdAfir(Pregunta p, int idAfir);

// Funcion de inicializacion
TTema ttema(int id)
{
    TTema t;
    t.idTema = id;
    t.practicos = list<Pregunta>();
    t.teoricos = list<Pregunta>();
    return t;
}

RAlumno rAlumno();

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

        // si es de tipo teorico lo busco en la lista de preguntas teoricas del
        // tema
        if(r->tipoAfir == 'T')
        {
            // el ttema que me llega es el que hizo el alumno con las respuestas
            // ordenadas por tipo
            Pregunta afirCorrecta =
                *listFind<Pregunta, int>(t.teoricos, r->idAfir, cmpPregIdAfir);

            // me fijo si la respondio bien
            if(afirCorrecta.rta == r->rta)
            {
                teoCorrec++;
            }
            else
            {
                teoIncorr++;
            }
        }
        else
        {  // el tipo de afirm es practico
            // busco la afirmacion en la lista de afirm practicas
            Pregunta afirCorrecta =
                *listFind<Pregunta, int>(t.practicos, r->idAfir, cmpPregIdAfir);

            if(afirCorrecta.rta == r->rta)
            {
                praCorrec++;
            }
            else
            {
                praIncorr++;
            }
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
List<Tema> temasSubir();

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

        if( t.tipoAfir=='T' )
        {
            listAdd<Pregunta>(tt->teoricos, p);
        }
        else 
        {
            listAdd<Pregunta>(tt->practicos, p);
        }                 
    }
    return ret;
}

List<Alumno> alumnosSubir();
List<Curso> cursosSubir();

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
void rAlumnoMostrar(List<RAlumno> alumnosCalificados, List<Curso> cursos);

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
