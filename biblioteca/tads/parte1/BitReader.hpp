#ifndef _BITREADER_
#define _BITREADER_

#include <cstdio>

struct BitReader
{
    FILE* f;             // archivo desde donde se leen los bytes
    unsigned char byte;  // último byte leído del archivo
    int restantes;       // cuántos bits de 'byte' faltan entregar (0 a 8)
};

// inicializa el TAD
BitReader bitReader(FILE* f)
{
    BitReader br;
    br.f = f;
    br.byte = 0;
    br.restantes = 0;    // todavía no leímos ningún byte
    return br;
}

// lee el siguiente bit desde el archivo encapsulado por el TAD
// devuelve 0 o 1, o -1 si ya no quedan bits (fin de archivo)
int bitReaderRead(BitReader& br)
{
    if( br.restantes == 0 )                  // me quedé sin bits: leo otro byte
    {
        int c = fgetc(br.f);
        if( c == EOF )
        {
            return -1;                       // fin de archivo (feof pasa a true)
        }
        br.byte = (unsigned char)c;
        br.restantes = 8;
    }

    br.restantes--;
    // desplazo el bit buscado a la posición 0 y lo aíslo con la máscara 1
    int bit = (br.byte >> br.restantes) & 1;
    return bit;
}

#endif