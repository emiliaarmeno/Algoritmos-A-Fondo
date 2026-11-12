
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

struct Producto
{
 int idProd;
 char desc[20];
 int stockActual;
 int stockMinimo;
 int stockReposi; //cantidad de productos que se necesitan para llegar al stock minimo 
};
struct Orden
{
 int idOrden;
 int idCli;
 int fecha;
 int cantItems;
};
struct Detalle
{
 int idProd;
 int cant;
};

/* El archivo comienza con 1 byte sin bit de signo que indica una cantidad de productos
(n), y a continuación n registros Producto. Luego tiene un registro Orden, suponiendo que hubiera órdenes,
y tantos registros Detalle como indica el campo
cantItems de la orden. Y así sucesivamente hasta finalizar el archivo.
Nota: En una misma orden no aparecerá dos veces el mismo producto. */
void subirDatos(List<Producto>& productos, Map<Orden, List<Detalle>>& ordenes){
    FILE* f = fopen("DATOS.dat", "r+b");
    unsigned char n = read<unsigned char>(f);
    for(int i=0; i<n; i++){
        Producto p = read<Producto>(f);
        listAdd<Producto>(productos, p);
    }
    while(!feof(f)){
        Orden o = read<Orden>(f);
        if(o.cantItems>0){
            List<Detalle> detalles = list<Detalle>();
            for(int j=0; j<o.cantItems; j++){
                Detalle d = read<Detalle>(f);
                listAdd<Detalle>(detalles, d); 
            }
            mapPut<Orden, List<Detalle>>(ordenes, o, detalles); 
        }
    }
    fclose(f);
}

int cmpPId(Producto p, int id){ return p.idProd - id; }

bool stockVerificar(List<Producto> productos, List<Detalle> pedido){
    bool suficiente = true; 
    listReset<Detalle>(pedido);
    while( listHasNext<Detalle>(pedido) && suficiente ){
        Detalle* d = listNext<Detalle>(pedido);
        Producto* p = listFind<Producto, int>(productos, d->idProd, cmpPId);
        if( p->stockActual<d->cant ){
            suficiente = false; 
        }
    }
    return suficiente; 
}

void stockActualizar(List<Producto>& productos, List<Detalle> pedido){
    listReset<Detalle>(pedido);
    while( listHasNext<Detalle>(pedido) ){
        Detalle* d = listNext<Detalle>(pedido);
        Producto* p = listFind<Producto, int>(productos, d->idProd, cmpPId);
        p->stockActual -= d->cant;
    }
}

List<Orden> ordenesProcesar(List<Producto>& productos, Map<Orden, List<Detalle>>& ordenes){
    List<Orden> rechazadas = list<Orden>();
    mapReset<Orden, List<Detalle>>(ordenes);
    while(mapHasNext<Orden, List<Detalle>>(ordenes)){
        Orden o = mapNextKey<Orden, List<Detalle>>(ordenes);
        List<Detalle>* pedido = mapGet<Orden, List<Detalle>>(ordenes, o);
        bool suficienteStock = stockVerificar(productos, *pedido);
        if(suficienteStock){
            stockActualizar(productos, *pedido);
        }else{
            listAdd<Orden>(rechazadas, o);
        }
    }
    return rechazadas;
}

string ordenMostrar(Orden o);

void punto1Mostrar(List<Orden> rechazadas){
    cout << "Pedidos rechazados por faltantes de stock: " <<endl;
    listReset<Orden>(rechazadas);
    while(listHasNext<Orden>(rechazadas)){
        Orden* o = listNext<Orden>(rechazadas);
        cout << ordenMostrar(*o) <<endl;
    }
}

void reposiProcesar(List<Producto>& productos){
    listReset<Producto>(productos);
    while(listHasNext<Producto>(productos)){
        Producto* p = listNext<Producto>(productos);
        if(p->stockActual < p->stockMinimo){
            int solicitar = (p->stockMinimo - p->stockActual);
            p->stockReposi = solicitar; 
        }
    }
}

void punto2Mostrar(List<Producto> productos){
    cout << "Unidades a solicitar para su reposicion: " <<endl;
    listReset<Producto>(productos);
    while(listHasNext<Producto>(productos)){
        Producto* p = listNext<Producto>(productos);
        if(p->stockReposi>0){
            cout << "De " << p->desc << " se necesitan " << p->stockReposi << " unidades" <<endl;
        }
    }
}

/* Se pide:
1. Emitir un listado indicando las órdenes que serán rechazadas por faltantes
de stock en alguno de los productos involucrados. Se debe tener en cuenta
que las órdenes rechazadas no generan movimiento de stock.
2. Para cada producto cuyo stock haya quedado por debajo del stockMin,
indicar cuántas unidades de este se deben solicitar al proveedor. */
int main()
{
    List<Producto> productos = list<Producto>();
    Map<Orden, List<Detalle>> ordenes = map<Orden, List<Detalle>>(); //esto esta mal
    subirDatos(productos, ordenes);
    List<Orden> rechazadas = ordenesProcesar(productos, ordenes);
    punto1Mostrar(rechazadas);
    reposiProcesar(productos);
    punto2Mostrar(productos);
    return 0;
}


/* List<Orden> ordenesProcesar(FILE* f, List<Producto>& productos)
{
    List<Orden> rechazadas = list<Orden>();

    Orden o = read<Orden>(f);
    while( !feof(f) )
    {
        // leo los detalles de esta orden
        List<Detalle> pedido = list<Detalle>();
        for(int i=0; i<o.cantItems; i++)
        {
            Detalle d = read<Detalle>(f);
            listAdd<Detalle>(pedido, d);
        }

        // decido con el stock actual
        if( stockVerificar(productos, pedido) )
            stockActualizar(productos, pedido);
        else
            listAdd<Orden>(rechazadas, o);

        listFree<Detalle>(pedido);
        o = read<Orden>(f);                 // leo la siguiente al final
    }
    return rechazadas;
}

int main()
{
    List<Producto> productos = list<Producto>();

    FILE* f = fopen("DATOS.dat", "rb");
    unsigned char n = read<unsigned char>(f);
    for(int i=0; i<n; i++)
    {
        Producto p = read<Producto>(f);
        listAdd<Producto>(productos, p);
    }

    List<Orden> rechazadas = ordenesProcesar(f, productos);
    fclose(f);

    punto1Mostrar(rechazadas);
    reposiProcesar(productos);
    punto2Mostrar(productos);
    return 0;
} */