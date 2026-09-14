#include "principal.hpp"

#include <cassert>
#include <iostream>

#include "biblioteca/funciones/files.hpp"
#include "biblioteca/funciones/millis.hpp"
#include "biblioteca/funciones/strings.hpp"
#include "biblioteca/funciones/tokens.hpp"
#include "biblioteca/tads/parte1/Coll.hpp"
#include "biblioteca/tads/parte1/Fecha.hpp"
//#include "biblioteca/tads/parte1/BitWriter.hpp"
//#include "biblioteca/tads/parte1/BitReader.hpp"
#include "biblioteca/tads/parte2/Array.hpp"
#include "biblioteca/tads/parte2/List.hpp"
#include "biblioteca/tads/parte2/Map.hpp"
#include "biblioteca/tads/parte2/Queue.hpp"
#include "biblioteca/tads/parte2/Stack.hpp"

#include "agest.hpp"
#include "agshow.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;

void mostrarArchivo(FILE* f)
{
    mostrarNumeroDeSerie(f);
    
    mostrarFileName(f);
    
    mostrarDate(f);
    
    Integer cntCampos = mostrarCantidadDeCamposConfigurados(f);
    
    RegType rTCampos = mostrarCamposConfigurados(f, cntCampos);
    
    Integer cntContactos = mostrarCantidadDeContactos(f);
    
    mostrarContactos(f, cntContactos, rTCampos);
}
// .\agall.exe AGENDA-TEST.dat
int main(int argc, char** argv)
{      
    FILE* f = fopen(argv[1],"r+b");
    
    mostrarArchivo(f);
    
    fclose(f);
    return 0;
}