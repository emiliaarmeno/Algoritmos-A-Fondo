#ifndef _BITWRITER_
#define _BITWRITER_

#include <cstdio>
#include <string>
using std::string;

struct BitWriter
{
    FILE* f;      // archivo donde se graban los bytes
    string buff;  // bits pendientes, deben ser menos que 8
};

// inicializa el TAD
BitWriter bitWriter(FILE* f)
{
    BitWriter bw;
    bw.f = f;
    bw.buff = "";
    return bw;
}

// convierte un string de exactamente 8 caracteres en un byte
unsigned char bitWriterBuffToByte(const string& bits) //esto funciona porque el string tiene 0 y 1
{
    unsigned char byte = 0;
    for(int i = 0; i < 8; i++)
    {
        byte = byte << 1;          // hago lugar a la derecha
        if( bits[i] == '1' )
        {
            byte = byte | 1;       // hago 1 al bit menos significativo de la derecha
        }
    }
    return byte;
}

// graba el bit en el archivo encapsulado por el TAD
void bitWriterWrite(BitWriter& bw, int bit)
{
    //agrego el bit al buffer
    bw.buff += (bit != 0) ? '1' : '0';   // cualquier valor distinto de 0 es un 1 sino esta mal

    if( bw.buff.size() == 8 )            // buffer completo osea ya hay un byte
    {
        unsigned char byte = bitWriterBuffToByte(bw.buff);
        fwrite(&byte, sizeof(unsigned char), 1, bw.f);
        bw.buff = "";                    // vacío el buffer para el siguiente 
    }
}

// graba cada uno de los bit contenidos en la cadena bits
void bitWriterWrite(BitWriter& bw, string bits)
{
    for(unsigned int i = 0; i < bits.size(); i++) //por cada caracter llamo a la funcion que escribe un bit solo 
    {
        bitWriterWrite(bw, bits[i] == '1' ? 1 : 0);
    }
}

// indica que no habra mas bits y se deben grabar los que estan en el buffer
// devuelve la cantidad de bits de relleno (ceros) que se agregaron
int bitWriterFlush(BitWriter& bw)
{
    int relleno = 0;

    if( bw.buff.size() > 0 )             // hay bits pendientes
    {
        while( bw.buff.size() < 8 )      // completo el byte con ceros a la derecha
        {
            bw.buff += '0';
            relleno++;
        }
        unsigned char byte = bitWriterBuffToByte(bw.buff); //lo convierto en byte y lo escribo
        fwrite(&byte, sizeof(unsigned char), 1, bw.f);
        bw.buff = "";
    }

    return relleno;
}

#endif