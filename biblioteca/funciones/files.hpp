#ifndef _TFILES_T_
#define _TFILES_T_

#include <stdio.h>
#include <iostream>
#include "strings.hpp"

template <typename T>
void write(FILE* f, T t) //graba el valor t en la posicion actual del archivo f, siendo éste un archivo de registros tipo T, de longitud fija.
{
    //fseek(f,0,SEEK_CUR);
    fwrite(&t,sizeof(T),1,f);
}

template <typename T>
T read(FILE* f) //lee la posicion actual del archivo f 
{
    T t;
    fseek(f,0,SEEK_CUR);
    fread(&t, sizeof(T), 1, f);
    return t;
}

template <typename T>
void seek(FILE* f, int n) //mueve el indicador de posicion al inicio del registro n 
{
    fseek(f,n*sizeof(T),SEEK_SET);
}

template <typename T>
int fileSize(FILE* f) //devuelve la cantidad de registros tipo t que tiene el archivo f
{
    // tomo la posicion actual
    int curr=ftell(f);

    // muevo el puntero al final del archivo
    fseek(f,0,SEEK_END);  // SEEK_END hace referencia al final del archivo

    // tomo la posicion actual (ubicado al final)
    int ultimo=ftell(f);

    // vuelvo a donde estaba al principio
    fseek(f,curr,SEEK_SET);

    return ultimo/sizeof(T);
}

template <typename T>
int filePos(FILE* f) //devuelve el numero del registro al que apunta el indicador de posicion 
{
   return ftell(f)/sizeof(T);
}

bool readLine(FILE* f, string& ln) //Lee una línea de texto desde el archivo f y la asigna a ln. 
                                  //Retorna true si quedan más líneas por leer.
{
    //leo uno por uno con fread
    char c;
    int n = fread(&c, sizeof(char), 1, f);
    //mientras que no sea '\n' ni llegue al final del archivo agregarlo a ln 
    ln="";
    while(c!='\n' && n!=0){
        ln=ln+c;
        n = fread(&c, sizeof(char), 1, f);
    }
    //si salio porque se termino el archivo no hay mas lineas para leer
    bool ret;
    if(n==0){
        ret=false;
    }else{//salio porque se termino la linea 
        ret=true;
    }

    return ret;
}

bool readWord(FILE* f, string& w, string alsoAllowed)
{   
    char c;
    int n = fread(&c, sizeof(char), 1, f);
    //Saltear caracteres no válidos (espacios, puntos, comas, \n, etc.) hasta encontrar uno válido o llegar al fin de archivo
    w = "";
    bool valido = false; 
    while(!valido && n!=0){
        if(isDigit(c) || isLetter(c) || contains(alsoAllowed, c)){
            valido=true;
        }else{
            valido=false; 
            n = fread(&c, sizeof(char), 1, f);
        }
    }

    while(valido && n!=0) { 
        if(isDigit(c) || isLetter(c) || contains(alsoAllowed, c)){
            valido=true;
            w=w+c;
            n = fread(&c, sizeof(char), 1, f);
        }else{
            valido=false; 
            fseek(f, -1, SEEK_CUR); // retrocedo la posicion del archivo para que se lea este caracter
        }
     }

    return n!=0;
}

bool readWord(FILE* f,string& w)
{
    return readWord(f, w, "");
}

void writeString(FILE* f,string s)
{
    for(int i=0; s[i]!='\0'; i++){
        write(f, s[i]);
    }
}

void writeWord(FILE* f,string w)
{
    writeString(f,w);
    writeString(f, " ");
}

void writeLine(FILE* f, string ln)
{
    writeString(f, ln);
    writeString(f, "\n");
}

#endif
