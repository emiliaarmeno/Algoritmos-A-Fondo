#include <iostream>
#include "../biblioteca/funciones/files.hpp"
#include "../biblioteca/funciones/strings.hpp"
#include "../biblioteca/funciones/tokens.hpp"
#include "../biblioteca/tads/parte1/Fecha.hpp"
#include "../biblioteca/tads/parte1/Coll.hpp"
#include "../biblioteca/tads/parte2/Array.hpp"
#include "../biblioteca/tads/parte2/List.hpp"
#include "../biblioteca/tads/parte2/Map.hpp"
#include "../biblioteca/tads/parte2/Queue.hpp"
#include "../biblioteca/tads/parte2/Stack.hpp"

#include <cassert>

#include "frases tools.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;

/*
    Leer el archivo FRASES.txt y generar un listado donde se clasifiquen las frases segÃºn el anio y
    el autor. El listado debe tener el siguiente formato:

    2003
       Maradona, D.
            La vidaaaa eeeeeeeeessss maravillosa con una pelota en el pie.
            La Claudia ya me tiene podrido.
            :
       Galvan, M.
            No somos nada o lo somos todo.
            El mundo es un slot y la vida es una plaqueta.
            :
    2007
       Maradona, D.
            :
       Galindez, R.
            :
     :
*/
int cmpRAutorString(RAutor ra, string nom){
    return cmpString(ra.nombre, nom);
}

int cmpRAnioInt(RAnio ra, int a){
    return ra.anio-a;
}

//asumo que no hay frases repetidas asi que las agrego todas
void frasesAgregar(Coll<string>& cFrases, string ln){
    int posFraLn = 0;
    string frace = getTokenAt(ln, '|', posFraLn);

    collAdd<string>(cFrases, frace, stringToString);
}


void autoresDescubrir(Coll<RAutor>& cAutores, string ln){
    //agarro el autor de ln para buscarlo
    int posAutLn = 1;
    string nombreAutor = getTokenAt(ln, '|', posAutLn);

    int pos = collFind<RAutor, string>(cAutores, nombreAutor, cmpRAutorString, rAutorFromString);
    if(pos<0){ //no esta, lo agrego
        RAutor rAutNue = rAutor(nombreAutor, coll<string>('&')); //le paso una coleccion vacia osea la funcion de inicializacion que devuelve eso
        pos = collAdd(cAutores, rAutNue, rAutorToString);
    }

    RAutor y = collGetAt<RAutor>(cAutores, pos, rAutorFromString);
    //le agrego al autor de la linea la frase
    frasesAgregar(y.frases, ln);
    collSetAt<RAutor>(cAutores, y, pos, rAutorToString);
}

void aniosDescubrir(Coll<RAnio>& cAnios, string ln){
    //agarro el año de la linea para buscar en la coleccion
    int posAnioLn=2;
    string anio = getTokenAt(ln, '|', posAnioLn);
    int anioInt = stringToInt(anio);

    int pos = collFind<RAnio, int>(cAnios, anioInt, cmpRAnioInt, rAnioFromString);
    if(pos<0){ //no esta en la coleccion hay que agregarlo
        RAnio raNuevo = rAnio(anioInt, coll<RAutor>('$'));
        pos = collAdd(cAnios, raNuevo, rAnioToString);
    }

    RAnio x = collGetAt<RAnio>(cAnios, pos, rAnioFromString);
    //la agrego al año de la linea el autor  
    autoresDescubrir(x.autores, ln);
    //seteo los cambios
    collSetAt<RAnio>(cAnios, x, pos, rAnioToString);
}

string stringFromString(string s){
    return s;
}

void mostrarFrases(Coll<string> cFrases){
    collReset<string>(cFrases);
    while(collHasNext<string>(cFrases)){
        string elm = collNext<string>(cFrases, stringFromString);
        cout<<"        "<<elm<<endl;
    }
}

void mostrarAutor(Coll<RAutor> cAut){
    collReset<RAutor>(cAut);
    while(collHasNext<RAutor>(cAut)){
        RAutor elm = collNext<RAutor>(cAut, rAutorFromString);
        cout<<"    "<<elm.nombre<<endl;
        mostrarFrases(elm.frases);
    }
}

void mostrar(Coll<RAnio> cAnios){
    collReset<RAnio>(cAnios);
    while(collHasNext<RAnio>(cAnios)){
        RAnio elm = collNext<RAnio>(cAnios, rAnioFromString);
        cout<< elm.anio << endl;
        mostrarAutor(elm.autores);
    }
}


int main()
{
    Coll<RAnio> cAnios = coll<RAnio>();

    FILE* f = fopen("C:/vscode/Workspace/AlgoritmosAFondo_EMPTY_v2.7.32/ejercicios/FRASES.txt","r+b");
    string ln;
    bool quedan = readLine(f,ln);
    while( quedan )
    {
        //subimos los datos del archivo sin repetir: descubrimiento
        aniosDescubrir(cAnios, ln);
        quedan = readLine(f,ln);
    }

    mostrar(cAnios);

    fclose(f);


    return 0;
}