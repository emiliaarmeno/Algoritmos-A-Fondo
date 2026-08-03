
#include <iostream>
//#include "biblioteca/funciones/millis.hpp"
#include "../biblioteca/funciones/files.hpp"
#include "../biblioteca/funciones/strings.hpp"
#include "../biblioteca/funciones/tokens.hpp"
#include "../biblioteca/tads/parte1/Fecha.hpp"
//#include "biblioteca/tads/parte1/Timer.hpp"
#include "../biblioteca/tads/parte1/Coll.hpp"
#include "../biblioteca/tads/parte2/Array.hpp"
#include "../biblioteca/tads/parte2/List.hpp"
#include "../biblioteca/tads/parte2/Map.hpp"
#include "../biblioteca/tads/parte2/Queue.hpp"
#include "../biblioteca/tads/parte2/Stack.hpp"

#include "torneo de futbol tools.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;

/* 
    1.6.7. Rotar un conjunto de valores
    Además del conjunto de valores numéricos enteros, el usuario ingresará un valor k que
    llamaremos valor de rotación. Se pide mostrar todos los elementos del conjunto
    rotándolos k posiciones. 
    Por ejemplo, si el conjunto fuera: 1, 2, 3, 4, 5 y k=3, la
    salida deberá ser: 3, 4, 5, 1, 2.
    Es decir, en el ejemplo anterior el último valor es 5 y el primero es 1. Entonces,
    sacamos el último valor y lo colocamos en el primer lugar, desplazando todos los demás
    valores hacia la derecha. Así k veces.
    */

int main()
{
    Coll<int> c1 = coll<int>();
    cout<<"ingrese un numero (finaliza con cero): "<<endl;
    int n;
    cin>>n; 
    while(n!=0){
        collAdd<int>(c1, n, intToString);
        cin>>n;
    }
    cout<<"ingrese el valor de rotacion: "<<endl;
    int k;
    cin>>k;

    int s = collSize<int>(c1);

    Coll<int> c2 = c1;

    for(int i=0; i<s; i++){
        int elm1 = collGetAt<int>(c1, i, stringToInt);
        if(i+k<s){
            collSetAt<int>(c2, elm1, i+k, intToString);
        }else{
            collSetAt<int>(c2, elm1, (i+k)-s, intToString);
        }
    }

    //cout<<c2.s<<endl;
    collReset<int>(c2);
    int e;
    while(collHasNext<int>(c2)){
        e = collNext<int>(c2, stringToInt);
        cout<<e<<endl;
    }

    return 0;
}
