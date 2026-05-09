#ifndef _TSTRINGS_T_
#define _TSTRINGS_T_

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
using std::string;

int length(string s)
{
    int i = 0;
    while(s[i] != '\0')
    {
        i++;
    }
    return i;
}

int charCount(string s, char c)
{
    int cant = 0, i = 0;
    while(s[i] != '\0')
    {
        if(s[i] == c)
        {
            cant++;
            i++;
        }
        else
        {
            i++;
        }
    }
    return cant;
}

// devuelve subcadena entre caracter d inclusive y h no inclusive
string substring(string s, int d, int h)
{
    string ret = "";
    int i = d;
    while(i < h)
    {
        ret = ret + s[i];
        i++;
    }
    return ret;
}

string substring(string s, int d)  // devuelve substring desde el caracter d
                                   // hasta el final de la cadena
{
    string ret = "";
    int i = d;
    while(i < length(s))
    {
        ret = ret + s[i];
        i++;
    }
    return ret;
}

int indexOf(string s,
            char c)  // posicion de la primera vez que aparece c en una cadena
{
    int pos = 0;
    while(s[pos] != c && pos < length(s))
    {
        pos++;
    }
    if(pos >= length(s))
    {
        pos = -1;  // devuelve -1 si no esta en la cadena
    }
    return pos;
}

int indexOf(string s, char c,
            int offSet)  // retorna la primera posicion de c en s descartando
                         // los primeros offSet carateres
{
    string x = substring(s, offSet);
    int ret = indexOf(x, c);
    if(ret < 0)
    {
        ret = -1;
    }
    else
    {
        ret = ret + offSet;
    }
    return ret;
}

int indexOf(string s,
            string toSearch)  // busca la posicion de la primera vez que aparece
                              // una cadena adentro de otra
{
    int i = 0, primPos = 0, offSet = 0;
    bool encontrado = false, primeraVuelta = true;
    while(s[i] != '\0' && encontrado == false)
    {
        if(primeraVuelta)
        {
            primPos = indexOf(s, toSearch[0]);  // busco la primera vez que
                                                // aparece la letra de toSearch
            primeraVuelta = false;
        }
        else
        {
            primPos = indexOf(s, toSearch[0],
                              offSet);  // si es la segunda vez que busca
                                        // arranco desde donde deje
        }

        if(primPos >= 0)  // si pudo encontrar el index
        {
            if(substring(s, primPos, primPos + length(toSearch)) == toSearch)
            {
                encontrado = true;
            }
            else
            {
                offSet = primPos + 1;
            }
        }
        else
        {
            i = length(s);  // no encontro el indice --> no esta la palabra en
                            // la cadena fuerzo la salida del while
        }
    }
    // salio porque se termino la cadena o porque encontro la posicion de
    // toSearch
    return primPos;
}

int indexOf(string s, string toSearch, int offset)
{
    int i = 0, primPos = 0;
    bool encontrado = false;
    while(s[i] != '\0' && encontrado == false)
    {
        primPos = indexOf(s, toSearch[0], offset);

        if(primPos >= 0)  // si pudo encontrar el index
        {
            if(substring(s, primPos, primPos + length(toSearch)) == toSearch)
            {
                encontrado = true;
            }
            else
            {
                offset = primPos + 1;
            }
        }
        else
        {
            i = length(s);  // no encontro el indice --> no esta la palabra en
                            // la cadena fuerzo la salida del while
        }
    }
    // salio porque se termino la cadena o porque encontro la posicion de
    // toSearch
    return primPos;
}

int lastIndexOf(string s, char c)
{
    int i = length(s)-1, pos = 0; //arranca en length-1 porque sino es una posicion invalida
    bool encontrado = false;
    while(i >= 0 && !encontrado)
    {
        if(s[i] == c)
        {
            pos = i;
            encontrado = true;
        }
        else
        {
            pos = -1;
        }
        i--;
    }
    return pos;
}

int indexOfN(string s, char c, int n)
{
    int ret = 0, offSet = 0, cont = 0;
    if(n == 0)
    {  // devuelvo negativo si me pide cero cantidad de veces
        ret = -1;
    }

    while(cont < n)
    {                                 // mientras que no la encuentre n veces
        ret = indexOf(s, c, offSet);  // posicion del caracter
        offSet = indexOf(s, c, offSet) +
                 1;  // por si no era el final que arranque a buscar el
                     // siguiente desde un lugar desp del que esta

        if(ret < 0)
        {                     // no esta tantas veces el caracter en la cadena
            ret = length(s);  // la consigna dice que haga esto
        }
        cont++;
    }
    return ret;
}

int charToInt(char c)
{
    if(c >= '0' && c <= '9')
    {
        return c - '0';
    }

    if(c >= 'A' && c <= 'Z')
    {
        return c - 'A' + 10;
    }

    if(c >= 'a' && c <= 'z')
    {
        return c - 'a' + 10;
    }

    return -1;  // si no es un caracter valido para convertir
}

char intToChar(int i)
{
    char ret;
    if(i >= 0 && i <= 9)
    {
        ret = '0' + i;  // i = 3 → '0' + 3 = '3'
    }
    else if(i >= 10 && i <= 35)
    {
        ret = 'A' + (i - 10);  // i = 13 → 'A' + (13 - 10) = 'A' + 3 = 'D'
    }
    return ret;
}

int getDigit(int n, int i)
{
    int digito;
    int aux = 1;
    for(int j = 0; j < i; j++)
    {
        aux *= 10;  // multiplico el 10 i veces
    }
    aux = n / aux;      // asi se mueve el digito que quiero a la derecha
    digito = aux % 10;  // lo extraigo haciendo el resto
    return digito;
}

int digitCount(int n)
{
    int cont = 0;
    while(n != 0)
    {  // mientras que n no es cero sigo dividiendo por 10 y contando cuantas
       // veces
        cont++;
        n /= 10;
    }
    return cont;
}

string intToString(int i)
{
    if(i == 0)
        return "0";  // por las dudas para que no explote si el valor es cero
    string cadena = "";
    int totalDigitos = digitCount(i);
    for(int j = totalDigitos - 1; j >= 0; j--)
    {  // la condicion queda rara porque sino me pasaba el entero a cadena con
       // el orden inverso (porque arrancaba desde el principio)
        int digito = getDigit(i, j);
        cadena += intToChar(digito);
    }
    return cadena;
}

int stringToInt(string s, int b)  // no se si funciona
{
    if(s == "") return 0;
    int numero = 0;
    int multiplicador = 1;
    int totalCaracteres = length(s);

    for(int j = totalCaracteres - 1; j >= 0; j--)
    {
        char caracter = s[j];
        int valor = charToInt(caracter);
        numero += valor * multiplicador;
        multiplicador *= b;
    }

    return numero;
}

int stringToInt(string s)  //uso la otra pero fijo la base numerica decimal
{
    return stringToInt(s,10);
}

string charToString(char c)
{
    string s = "";
    s += c;
    return s;
}

char stringToChar(string s)
{
    // no se si funciona no lo puedo probar
    // capaz tengo que verificar que me llegue una cadena de una sola posicion
    return s[0];
}

string stringToString(string s)
{
    return s;
}

string doubleToString(double d)
{
    int parteEntera = int(d);
    double parteDecimal = d-parteEntera;
    string ptEntStr = intToString(parteEntera);
    int unDecimal = 0;
    string ptDecStr = "";
    int cantDecimales = 0; //para redondear
    while(parteDecimal!=0 && cantDecimales < 6){
        parteDecimal = parteDecimal * 10; //muevo pasando la coma un digito decimal 
        unDecimal = (int)parteDecimal;        // saco el dígito
        parteDecimal = parteDecimal - unDecimal;  // me quedo con los decimales restantes
        ptDecStr = ptDecStr+intToString(unDecimal); //lo agrego al string
        cantDecimales++;
    }
    string ret = ptEntStr + "." + ptDecStr;
    return ret;
}

double stringToDouble(string s)
{
    int i = 0;
    int n = s.size();

    // Manejar signo
    bool negativo = false;
    if(i < n && s[i] == '-')
    {
        negativo = true;
        i++;
    }

    // Leer parte entera
    double parteEntera = 0;
    while(i < n && s[i] >= '0' && s[i] <= '9')
    {
        parteEntera = parteEntera * 10 + (s[i] - '0');
        i++;
    }

    // Si hay parte decimal
    double parteDecimal = 0;
    double divisor = 1;
    if(i < n && s[i] == '.')
    {
        i++;
        while(i < n && s[i] >= '0' && s[i] <= '9')
        {
            parteDecimal = parteDecimal * 10 + (s[i] - '0');
            divisor *= 10;
            i++;
        }
    }

    double resultado = parteEntera + parteDecimal / divisor;
    if(negativo) resultado = -resultado;
    return resultado;
}

bool isEmpty(string s)
{
    return length(s) == 0;
}

bool startsWith(string s, string x)
{
    string parteAdelante = substring(s, 0, length(x));

    if(parteAdelante == x)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool endsWith(string s, string x)
{
    int desde = length(s) - length(x);  
    string parteAtras = substring(s, desde);

    if(parteAtras == x)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool contains(string s, char c)
{
    int posicion = indexOf(s, c);
    bool ret = false;
    if(posicion >= 0)
    {
        ret = true;
    }
    return ret;
}

string replace(string s, char oldChar, char newChar)
{
    for(int i = 0; s[i] != '\0'; i++)
    {
        if(s[i] == oldChar)
        {
            s[i] = newChar;
        }
    }

    return s;
}

string insertAt(string s, int pos, char c)
{
    string antes = substring(s, 0, pos);
    string despues = substring(s, pos, length(s));
    return antes + c + despues;
}

string removeAt(string s, int pos)
{
    string antes = substring(s, 0, pos);
    string despues = substring(s, pos + 1, length(s));
    return antes + despues;
}

string ltrim(string s) //le saca espacios de mas por izquierda 
{
    if(s[0] == ' ')
    {
        int i = 0;
        while(i < length(s) && s[i] == ' ')
        {
            i++;
        }
        string removed = substring(s, i, length(s));
        return removed;
    }
    else
    {
        return s;
    }
}

string rtrim(string s) //por derecha 
{
    int i = length(s) - 1;
    while(i >= 0 && s[i] == ' ')
    {  // aca verifico que i sea un indice valido
        i--;
    }
    string removed =
        substring(s, 0, i + 1);  // recorto a s desde que empieza hasta el
                                 // ultimo caracter que no es espacio
    return removed;
}

string trim(string s) //de ambos lados 
{
    int inicio = 0;
    int final = length(s) - 1;

    if(s[inicio] == ' ' || s[final] == ' ')
    {
        while(inicio < length(s) && s[inicio] == ' ')
        {
            inicio++;
        }
        while(final >= 0 && s[final] == ' ')
        {
            final--;
        }
        return substring(s, inicio, final + 1);
    }
    else
    {
        return s;
    }
}

string replicate(char c, int n) //hace una cadena del mismo caracter n veces 
{
    string s;
    for(int i = 0; i < n; i++)
    {
        s += c;
    }
    return s;
}

string spaces(int n) //lo mismo pero con espacios osea vacia 
{
    string s;
    for(int i = 0; i < n; i++)
    {
        s += " ";
    }
    return s;
}

string lpad(string s, int n, char c) //le suma espacios a una cadena para llegar al largo n 
{
    int largoDeS = length(s);
    if(largoDeS >= n)
    {
        return s;
    }
    string relleno;
    for(int i = 0; i < n - largoDeS; i++)
    {
        relleno += c;
    }
    return relleno + s;
}

string rpad(string s, int n, char c) //por derecha 
{
    int largoDeS = length(s);
    if(largoDeS >= n)
    {
        return s;
    }

    for(int i = 0; i < n - largoDeS; i++)
    {
        s += c;  // sumo los caracteres y se ponen a la derecha
    }

    return s;
}

string cpad(string s, int n, char c) //distribuye los espacios por ambos lados
{
    int largo = length(s);
    if(largo >= n)
    {
        return s;
    }

    int faltan = n - largo;
    int izquierda = faltan / 2;  // la mitad de los que faltan
    int derecha =
        faltan -
        izquierda;  // otra manera de asignarle la mitad de los que faltan

    string izquierdaRelleno, derechaRelleno;
    for(int i = 0; i < izquierda; i++)
    {
        izquierdaRelleno += c;
    }
    for(int i = 0; i < derecha; i++)
    {
        derechaRelleno += c;
    }

    return izquierdaRelleno + s + derechaRelleno;
}

bool isDigit(char c)
{
    return c >= '0' && c <= '9';  // esto funciona porque compara los digitos en
                                  // ascii entre 48 y 57
}

bool isLetter(char c)
{
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
    {  // creo que esto funciona porque compara las posiciones en ascii del 65
       // al 90 y del 97 al 122
        return true;
    }
    else
    {
        return false;
    }
}

bool isUpperCase(char c)
{
    return c >= 'A' && c <= 'Z';
}

bool isLowerCase(char c)
{
    return c >= 'a' && c <= 'z';
}

char toUpperCase(char c)  // me fije en la tabla ascii la conexion entre las
                          // mayusculas y minusculas
//'A'=65 y 'a'=97 --> 97-65=32 todos tienen 32 de diferencia
{
    if(c >= 'a' && c <= 'z')
    {
        c = c - 32;
    }
    return c;
}

char toLowerCase(char c)
{
    if(c >= 'A' && c <= 'Z')
    {
        c = c + 32;
    }
    return c;
}

string toUpperCase(string s)
{
    for(int i = 0; s[i] != '\0'; i++)
    {
        if(s[i] >= 'a' && s[i] <= 'z')
        {
            s[i] = s[i] - 32;
        }
    }
    return s;
}

string toLowerCase(string s)
{
    for(int i = 0; s[i] != '\0'; i++)
    {
        if(s[i] >= 'A' && s[i] <= 'Z')
        {
            s[i] = s[i] + 32;
        }
    }
    return s;
}

int cmpString(string a, string b)  // devuelve negativo si a precede a b, alfabeticamente
{
    if(a == b)
    {
        return 0;
    }
    else if(a < b)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

//negativo si a<b
//positivo si a>b
//cero si a=b
int cmpDouble(double a, double b)
{
    if(a == b)
    {
        return 0;
    }
    else if(a < b)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

char* stringToCString(string s)
{
    char* ret = new char[length(s) + 1];

    int i = 0;
    while(s[i] != '\0')
    {
        ret[i] = (char)s[i];
        i++;
    }

    ret[i] = '\0';
    return ret;
}

string cStringToString(char c[])
{
    return string(c);
}

#endif
