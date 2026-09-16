
#ifndef _BITWRITER_
#define _BITWRITER_

#include <iostream>
using std::string;

struct BitWriter
{
    string buff;
};

// inicializa el TAD
BitWriter bitWriter(FILE* f)
{
    return {};
}

// graba el bit en el archivo encapsulado por el TAD
void bitWriterWrite(BitWriter bw,int bit)
{
}

// graba cada uno de los bit contenidos en la cadena bits,
// la cual solo contiene 1s y 0s. Ej.: "1100111001111"
void bitWriterWrite(BitWriter bw,string bits)
{
}

// indica que no habra mas bits y se deben grabar los que estan en el buffer
int bitWriterFlush(BitWriter bw)
{
    return 0;
}

#endif
