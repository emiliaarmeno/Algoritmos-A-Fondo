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
#include "agwrite.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::to_string;

void agregarCampos(FILE* f, Integer cntCampos)
{
    
}
// .\agmk.exe AGENDA.dat Nombre Celular Email Instagram
int main(int argc, char** argv)
{   
    FILE* f = fopen(argv[1],"w+b");
    
    Integer numSerie;
    escribirInteger(f, numSerie);
    
    escribirString(f, argv[1]);
    
    // falta codificar la fecha
    escribirDate(f);
    
    Integer cntCampos = integer(argc-2);
    escribirInteger(f, cntCampos);
    
    RegType rT = regType(argv, cntCampos); 
    escribirRegType(f, rT, cntCampos);
    
    escribirInteger(f, integer(0));
    
    fclose(f);
    return 0;
}