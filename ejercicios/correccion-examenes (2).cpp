
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
struct TTema
{
    int idTema;
    List<Tema> practicos;
    List<Tema> teoricos;
};
struct Respuesta
{
    int idAlu;
    char tipoAfir;  // 'T' o 'P'
    int idAfir;     // 1 a 10
    char rta;       // 'V' o 'F'
    int idTema;     // 1 a 5
};
struct Calificacion
{
    int idCur;
    int idAlu;
    char c;  // calificacion
};

Calificacion calificacion(){
    Calificacion c; 
    c.idAlu = 0;
    c.idCur = 0;
    c.c = ' ';
    return c; 
}

// Funcion de inicializacion
TTema ttema(int id)
{
    TTema t;
    t.idTema = id;
    t.practicos = list<Tema>();
    t.teoricos = list<Tema>();
    return t;
}

int cmpTT(Tema a, Tema b)
{
    return a.idTema - b.idTema;
}
int cmpTTId(TTema a, int id)
{
    return a.idTema - id;
}

// devuelve los temas con las afirmaciones ordenadas por teoricos o practicos
List<TTema> subirTTemas()
{
    List<TTema> ret =
        list<TTema>();  // vamos a tener 5 cosas y en cada una las 20 rtas
    List<Tema> afirmaciones = list<Tema>();

    // subo todas las afirmaciones ordenandolas por idTema
    FILE* f = fopen("TEMAS.dat", "r+b");
    Tema t = read<Tema>(f);

    while(!feof(f))
    {
        listOrderedInsert<Tema>(afirmaciones, t, cmpTT);
        t = read<Tema>(f);
    }

    fclose(f);

    // recorro las afirmaciones y las añado a TTema
    listReset<Tema>(afirmaciones);
    while(listHasNext<Tema>(afirmaciones))
    {
        Tema t = *listNext<Tema>(afirmaciones);
        TTema* tt = listFind<TTema, int>(ret, t.idTema, cmpTTId);

        if(&t == NULL)
        {  // no esta en mi list de TTema
            // tengo que agregarlo
            TTema ttnuevo = ttema(t.idTema);
            if(t.tipoAfir == 'T')
            {
                listAdd<Tema>(ttnuevo.teoricos, t);
            }
            else
            {
                listAdd<Tema>(ttnuevo.practicos, t);
            }
            listAdd<TTema>(ret, ttnuevo);
        }
        else
        {
            // ya lo tengo en el list ret
            // tengo que agregar el tema nomas al ret
            if(t.tipoAfir == 'T')
            {
                listAdd<Tema>(tt->teoricos, t);
            }
            else
            {
                listAdd<Tema>(tt->practicos, t);
            }
        }
    }

    return ret;
}

// Evalua un examen y retorna la calificacion
int ttemaEvaluar(TTema t, List<Respuesta> lst);

int main()
{

    // 1. subir temas (afirmaciones correctas) a memoria
    // 2. subir alumnos y cursos a memoria
    // 3. leer RESPUESTAS.dat y generar una Calificacion por alumno
    // 4. ordenar las calificaciones por curso
    // 5. recorrer por curso (corte de control) e imprimir





    List<Calificacion> calif = list<Calificacion>();
    // subir ttemas, decvuelve lista con ttemas con las afir separadas por toe
    List<TTema> lTTem = subirTTemas();
    // recorremos respuestas que esta ordenado por idAlu, tipoAfir, idAfir
    FILE* f = fopen("RESPUESTA.dat", "r+b");
    Respuesta r = read<Respuesta>(f);
    int idAnt = r.idAlu;
    while(!feof(f))
    {
        // agarro todas las respuestas del mismo alumno y las meto en un list
        List<Respuesta> respAlu = list<Respuesta>();
        while(r.idAlu == idAnt)
        {
            listAdd<Respuesta>(respAlu, r);
            r = read<Respuesta>(f);
            idAnt = r.idAlu;
        }
        
        // llamo a ttemaEvaluar y me devuelve una calificacion
        TTema temaDeAlu = *listFind<TTema, int>(lTTem, r.idTema, cmpTTId);
        int nota = ttemaEvaluar(temaDeAlu, respAlu);
        Calificacion c = calificacion();
        

        //para que en la prox vuelta entre
        idAnt = r.idAlu;
    }
    fclose(f);
    return 0;
}
