#ifndef _TSTRINGS_T_
#define _TSTRINGS_T_

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
using std::string;

int length(string s)
{
    return 0;
}

int charCount(string s, char c)
{
    return 0;
}

string substring(string s, int d, int h)
{
    return "";
}

string substring(string s, int d)  // ok
{
    return "";
}

int indexOf(string s, char c)  // ok
{
    return 0;
}

int indexOf(string s, char c, int offSet)  // ok
{
    return 0;
}

int indexOf(string s, string toSearch)  // ok
{
    return 0;
}

int indexOf(string s, string toSearch, int offset)
{
    return 0;
}

int lastIndexOf(string s, char c)
{
    return 0;
}

int indexOfN(string s, char c, int n)
{
    return 0;
}

int charToInt(char c)
{
    return 0;
}

char intToChar(int i)
{
    return '0';
}

int getDigit(int n, int i)
{
    return 0;
}

int digitCount(int n)
{
    return 0;
}

string intToString(int i)
{
    return "";
}

int stringToInt(string s, int b)  // ok
{
    return 0;
}

int stringToInt(string s)  // ok
{
    return 0;
}

string charToString(char c)
{
    return "";
}

char stringToChar(string s)
{
    return '0';
}

string stringToString(string s)
{
    return "";
}

string doubleToString(double d)
{
    return "";
}

double stringToDouble(string s)
{
    return 1.0;
}

bool isEmpty(string s)
{
    return true;
}

bool startsWith(string s, string x)
{
    return true;
}

bool endsWith(string s, string x)
{
    return true;
}

bool contains(string s, char c)
{
    return true;
}

string replace(string s, char oldChar, char newChar)
{
    return "";
}

string insertAt(string s, int pos, char c)
{
    return "";
}

string removeAt(string s, int pos)
{
    return "";
}

string ltrim(string s)
{
    return "";
}

string rtrim(string s)
{
    return "";
}

string trim(string s)
{
    return "";
}

string replicate(char c, int n)
{
    return "";
}

string spaces(int n)
{
    return "";
}

string lpad(string s, int n, char c)
{
    return "";
}

string rpad(string s, int n, char c)
{
    return "";
}

string cpad(string s, int n, char c)
{
    return "";
}

bool isDigit(char c)
{
    return true;
}

bool isLetter(char c)
{
    return true;
}

bool isUpperCase(char c)
{
    return true;
}

bool isLowerCase(char c)
{
    return true;
}

char toUpperCase(char c)
{
    return '0';
}

char toLowerCase(char c)
{
    return '0';
}

string toUpperCase(string s)
{
    return "";
}

string toLowerCase(string s)
{
    return "";
}

int cmpString(string a, string b)
{
    return 0;
}

int cmpDouble(double a, double b)
{
    return 0;
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
