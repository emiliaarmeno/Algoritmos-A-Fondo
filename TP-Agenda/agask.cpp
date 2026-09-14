
#include <cassert>
#include <iostream>

#include "agest.hpp"
#include "agread.hpp"
#include "agshow.hpp"
#include "biblioteca/funciones/files.hpp"
#include "biblioteca/funciones/millis.hpp"
#include "biblioteca/funciones/strings.hpp"
#include "biblioteca/funciones/tokens.hpp"
#include "biblioteca/tads/parte1/BitReader.hpp"
#include "biblioteca/tads/parte1/BitWriter.hpp"
#include "biblioteca/tads/parte1/Coll.hpp"
#include "biblioteca/tads/parte1/Fecha.hpp"
#include "biblioteca/tads/parte1/Timer.hpp"
#include "biblioteca/tads/parte2/Array.hpp"
#include "biblioteca/tads/parte2/List.hpp"
#include "biblioteca/tads/parte2/Map.hpp"
#include "biblioteca/tads/parte2/Queue.hpp"
#include "biblioteca/tads/parte2/Stack.hpp"
#include "principal.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;

//.\agask.exe AGENDA-TEST.dat nombre Pablo
int main(int argc, char** argv)
{
    FILE* f = fopen(argv[1], "r+b");

    leerInteger(f);
    leerString(f);
    leerDate(f);
    Integer cntCamposConfig = leerInteger(f);
    RegType rT = leerRegType(f, cntCamposConfig);
    string aux = argv[2];
    int pos = arrayFind<string, string>(rT.mapRegType.value, aux, cmpSTRING);
    Byte k;
    if(pos < 0)
    {
        cout << "No hay un campo llamado " << aux << endl;
    }
    else
    {
        k.b = *arrayGet<unsigned char>(rT.mapRegType.key, pos);

        Integer cntContactos = leerInteger(f);

        string v;
        for(int i = 0; i < cntContactos.i; i++)
        {
            RegData rD = leerRegData(f);
            v = *mapGet<unsigned char, string>(rD.mapRegData, k.b);
            string aux2 = argv[3];
            // muestra todos los que contengan argv[3] (lo ingresado)
            if(indexOf2(v, aux2) >=0)
            {
                mostrarRegData(rD, rT);
            }
        }
    }
    fclose(f);
    return 0;
}