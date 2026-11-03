
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

// Archivo: REPRODUCCIONES.dat
struct Reproduccion
{
    int idUsuario;
    int idCuento;
    int fecha;
    int minutos;
};
// Archivo: CUENTOS.dat
struct Cuento
{
    int idCuento;
    int idRelator;
    int idAutor;
    char titulo[50];
    int duracion;
};
struct RCuento
{
    Cuento c;
    int completas;  // minutos >= duracion
    int mayA75;     // min >= 75%
    int mayA50;
    int mayA25;
    int menorA25;
};
// Archivo: RELATORES.dat
struct Relator
{
    int idRelator;
    char nombre[50];
};
struct RRelator
{
    Relator r;
    int cant;  // cantidad de reproducciones entre el 75% y 100%
};

RCuento rCuento(Cuento c)
{
    RCuento rc;
    rc.c = c;
    rc.completas = 0;
    rc.mayA75 = 0;
    rc.mayA50 = 0;
    rc.mayA25 = 0;
    rc.menorA25 = 0;
    return rc;
}

RRelator rRelator(Relator rel)
{
    RRelator rr;
    rr.r = rel;
    rr.cant = 0;
    return rr;
}

// compara dos rcuentos por id cuento
int cmpRC(RCuento a, RCuento b)
{
    return a.c.idCuento - b.c.idCuento;
}

List<RCuento> cuentosSubir()
{
    List<RCuento> lRC = list<RCuento>();
    FILE* f = fopen("CUENTOS.dat", "r+b");
    Cuento c = read<Cuento>(f);
    while(!feof(f))
    {
        RCuento rc = rCuento(c);  // creo el rcuento con el cuento y los cont en cero
        listOrderedInsert<RCuento>(lRC, rc, cmpRC);  // la guardo ordenada
        c = read<Cuento>(f);
    }
    fclose(f);
    return lRC;
}

int cmpRCID(RCuento rc, int idCuento)
{
    return rc.c.idCuento - idCuento;
}

double porcentajeEscuchado(int dur, int minEsc)
{
    return (minEsc / dur) * 100;
}

void aumentarContador(RCuento* rc, int dur, int min)
{  // tengo que pasarlo por referencia al rc??
    if(min >= dur)
        rc->completas++;
    else if(min * 100 >= dur * 75)
        rc->mayA75++;
    else if(min * 100 >= dur * 50)
        rc->mayA50++;
    else if(min * 100 >= dur * 25)
        rc->mayA25++;
    else
        rc->menorA25++;
}

void reproduccionesProcesar(List<RCuento> lRC)
{  // tendria que ir por referencia la lista????
    FILE* f = fopen("REPRODUCCIONES.dat", "r+b");
    Reproduccion r = read<Reproduccion>(f);
    while(!feof(f))
    {
        RCuento* rc = listFind<RCuento, int>(lRC, r.idCuento, cmpRCID);
        aumentarContador(rc, rc->c.duracion, r.minutos);
        r = read<Reproduccion>(f);
    }
    fclose(f);
}

void punto1Mostrar(List<RCuento> lRC)
{
    listReset<RCuento>(lRC);
    while(listHasNext<RCuento>(lRC))
    {
        RCuento rc = *listNext<RCuento>(lRC);  // agarro la copia porque no necesito cambiar la info que tiene
        cout << "El cuento " << rc.c.titulo << " tuvo " << endl;
        cout << rc.completas << " reproducciones completas" << endl;
        cout << rc.mayA75 << " reproducciones mayores al 75%" << endl;
        cout << rc.mayA50 << " reproducciones mayores al 50%" << endl;
        cout << rc.mayA25 << " reproducciones mayores al 25%" << endl;
        cout << rc.menorA25 << " reproducciones menores al 25%" << endl;
    }
}

List<RRelator> relatoresSubir()
{
    List<RRelator> lRR = list<RRelator>();
    FILE* f = fopen("RELATORES.dat", "r+b");
    Relator r = read<Relator>(f);
    while(!feof(f))
    {
        RRelator rr = rRelator(r);
        listAdd<RRelator>(lRR, rr);
        r = read<Relator>(f);
    }
    fclose(f);
    return lRR;
}

int cmpRRID(RRelator rr, int idRelator)
{
    return rr.r.idRelator - idRelator;
}

// recorro los rcuentos y le sumo al relator ccorresp la cantidad de completas
// mas la de may a 75
void relatoresProcesar(List<RRelator> lRR, List<RCuento> lRC)
{
    listReset<RCuento>(lRC);
    // estoy recorriendo bien la lista?? o me estoy olvidaddo del ultimo??
    while(listHasNext<RCuento>(lRC))
    {
        RCuento rc = *listNext<RCuento>(lRC);  // agarro la copia porque no quiero cambiarle nada
        RRelator* rr = listFind<RRelator, int>(lRR, rc.c.idRelator, cmpRRID);  // agarro el puntero porque le quiero cambiar la cantid y que eso se refreje en la lista
        rr->cant += rc.completas + rc.mayA75;
    }
}

int cmpRRCant(RRelator a, RRelator b)
{
    return b.cant - a.cant;  // de mayor a menor
}

void punto2Mostrar(List<RRelator>& lRR)
{
    cout << "Los 10 relatores cuyas lecturas tuvieron la mayor cantidad de reproducciones entre el 75% y 100% fueron: "<< endl;
    listSort<RRelator>(lRR, cmpRRCant);
    listReset<RRelator>(lRR);
    int i = 0;
    while(i < 10 && listHasNext<RRelator>(lRR))
    {
        RRelator rr = *listNext<RRelator>(lRR);
        cout << rr.r.nombre << " con " << rr.cant << " reproducciones." << endl;
        i++;
    }
}

int main()
{
    // 1. subir CUENTOS.dat a List<RCuento> con contadores en 0, ordenada por idCuento
    List<RCuento> lRC = cuentosSubir();
    // 2. recorrer REPRODUCCIONES.dat: por cada una, listFind del cuento y sumar al contador del tramo
    reproduccionesProcesar(lRC);
    // 3. mostrar la lista (punto 1)
    punto1Mostrar(lRC);
    // 4. subir RELATORES.dat a List<RRelator> con cant = 0
    List<RRelator> lRR = relatoresSubir();
    // 5. recorrer los RCuento y sumar mayA75 al relator del cuento
    relatoresProcesar(lRR, lRC);
    // 6. ordenar RRelator por cant decreciente y mostrar los primeros 10 (punto 2)
    punto2Mostrar(lRR);

    listFree<RCuento>(lRC);
    listFree<RRelator>(lRR);
    return 0;
}
