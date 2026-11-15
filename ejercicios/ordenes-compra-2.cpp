
#include <iostream>
#include "../biblioteca/funciones/millis.hpp"
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
   int stockReposi;
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

List<Producto> productosSubir(FILE* f){
    List<Producto> ret = list<Producto>();
    unsigned char cantProdu = read<unsigned char>(f);
    for(int i=0; i<cantProdu; i++){
        Producto p = read<Producto>(f);
        listAdd<Producto>(ret, p);
    }
    return ret; 
}

int cmpProdId(Producto p, int id){
    return p.idProd - id; 
}

bool stockSuficiente(List<Producto> productos, List<Detalle> detOrden){
    bool ret = true; 
    listReset<Detalle>(detOrden);
    while(listHasNext<Detalle>(detOrden)){
        Detalle* d = listNext<Detalle>(detOrden);
        Producto* p = listFind<Producto, int>(productos, d->idProd, cmpProdId);
        
        if(d->cant > p->stockActual){
            ret = false; 
        }
    }
    return ret; 
}

void stockActualizar(List<Producto>& productos, List<Detalle> detOrden){
    listReset<Detalle>(detOrden);
    while(listHasNext<Detalle>(detOrden)){
        Detalle* d = listNext<Detalle>(detOrden);
        Producto* p = listFind<Producto, int>(productos, d->idProd, cmpProdId);

        p->stockActual-=d->cant;
    }
}

List<Orden> ordenesProcesar(FILE* f, List<Producto>& productos){
    List<Orden> rechazadas = list<Orden>();
    List<Detalle> detOrden = list<Detalle>();

    while(!feof(f)){
        Orden o = read<Orden>(f);
        
        for(int i=0; i<o.cantItems; i++){
            Detalle d = read<Detalle>(f);
            listAdd<Detalle>(detOrden, d);
        }

        if(stockSuficiente(productos, detOrden)){
            stockActualizar(productos, detOrden);
        }else{
            listAdd<Orden>(rechazadas, o);
            listFree<Detalle>(detOrden);
        }
    }

    return rechazadas;
}

void punto1Mostrar(List<Orden> recha){
    cout<<"Las ordenes que fueron rechazadas por falta de stock fueron: "<<endl; 
    listReset<Orden>(recha);
    while(listHasNext<Orden>(recha)){
        Orden* o = listNext<Orden>(recha);
        cout<<"Id orden: "<<o->idOrden<<" del Cliente: "<<o->idCli<<" en el dia "<<o->fecha<<endl;
    }
}

void punto2Mostrar(List<Producto> lProd){
    cout<<"Unidades por producto a solicitar al proveedor: "<<endl; 
    listReset<Producto>(lProd);
    while(listHasNext<Producto>(lProd)){
        Producto* p = listNext<Producto>(lProd);
        if(p->stockActual < p->stockMinimo){
            cout << "Del producto "<< p->desc << " faltan " << p->stockReposi << " unidades" << endl;
        }
    }
}

int main()
{
    FILE* f = fopen("DATOS.dat", "r+b");

    List<Producto> lProd = productosSubir(f);
    List<Orden> rechazadas = ordenesProcesar(f, lProd);
    punto1Mostrar(rechazadas);
    punto2Mostrar(lProd);

    fclose(f);
    return 0;
}
