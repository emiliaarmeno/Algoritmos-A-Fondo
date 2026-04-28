
#ifndef MILLIS
#define MILLIS

#include <chrono>       // duration_cast, milliseconds, system_clock
#include <ctime>        // time_t, tm, localtime
#include <string>       // string
#include <sstream>      // ostringstream
#include <iomanip>      // put_time, setfill, setw

using std::string;
using std::ostringstream;
using std::tm;
using std::localtime;
using std::put_time;
using std::setfill;
using std::setw;

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;

// Retorna la cantidad de milisegundos transcurridos a partir de 1/enero/1970.
long long currTimeMillis()
{
    return duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
    ).count();
}

// Retorna una cadena que representa una fecha/hora expresada en milisegundos.
string millisToString(long long x)
{
    time_t seconds = x / 1000;
    int millis = x % 1000;

    tm *tm = localtime(&seconds);

    ostringstream oss;
    oss << put_time(tm, "%Y-%m-%d, %H:%M:%S")
        << "." << setfill('0') << setw(3) << millis;

    return oss.str();
}

long long attributesToMillis(int y, int mo, int d, int h, int mi, int s, int ms)
{
    std::tm tm = {};
    tm.tm_year = y - 1900;
    tm.tm_mon  = mo - 1;
    tm.tm_mday = d;
    tm.tm_hour = h;
    tm.tm_min  = mi;
    tm.tm_sec  = s;

    std::time_t t = std::mktime(&tm);

    if (t == -1)
    {
        return -1; // error
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::from_time_t(t).time_since_epoch()
    ).count() + ms;
}

void attributesFromMillis(long long ts,int& y, int& mo, int& d, int& h, int& mi, int& s, int& ms)
{
    // separar milisegundos
    ms = ts % 1000;
    if (ms < 0) ms += 1000; // por si ts es negativo

    std::time_t t = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::milliseconds(ts)
    ).count();

    std::tm* tm = std::localtime(&t);

    y  = tm->tm_year + 1900;
    mo = tm->tm_mon + 1;
    d  = tm->tm_mday;
    h  = tm->tm_hour;
    mi = tm->tm_min;
    s  = tm->tm_sec;
}

int _getYear(long long ms) {
    std::time_t seconds = ms / 1000;
    std::tm* tm = std::localtime(&seconds);
    return tm->tm_year + 1900; // tm_year cuenta desde 1900
}

int _getMonth(long long ms) {
    std::time_t seconds = ms / 1000;
    std::tm* tm = std::localtime(&seconds);
    return tm->tm_mon + 1;     // tm_mon es 0-based (enero=0)
}

int _getDay(long long ms) {
    std::time_t seconds = ms / 1000;
    std::tm* tm = std::localtime(&seconds);
    return tm->tm_mday;        // día del mes (1-31)
}

int _getHour(long long ms) {
    std::time_t seconds = ms / 1000;
    std::tm* tm = std::localtime(&seconds);
    return tm->tm_hour;   // hora (0-23)
}

int _getMinute(long long ms) {
    std::time_t seconds = ms / 1000;
    std::tm* tm = std::localtime(&seconds);
    return tm->tm_min;    // minutos (0-59)
}

int _getSecond(long long ms) {
    std::time_t seconds = ms / 1000;
    std::tm* tm = std::localtime(&seconds);
    return tm->tm_sec;   // segundos (0–59)
}

int _getMillis(long long ts)
{
    int ms = (int)(ts % 1000);
    if (ms < 0) ms += 1000;
    return ms;
}

void _setYear(long long& ts, int year) {
    std::time_t seconds = ts / 1000;
    std::tm* tm = std::localtime(&seconds);
    tm->tm_year = year - 1900;
    tm->tm_sec = 0; // segundos en cero
    seconds = std::mktime(tm);
    ts = static_cast<long long>(seconds) * 1000;
}

void _setMonth(long long& ts, int month) {
    std::time_t seconds = ts / 1000;
    std::tm* tm = std::localtime(&seconds);
    tm->tm_mon = month - 1;
    tm->tm_sec = 0;
    seconds = std::mktime(tm);
    ts = static_cast<long long>(seconds) * 1000;
}

void _setDay(long long& ts, int day) {
    std::time_t seconds = ts / 1000;
    std::tm* tm = std::localtime(&seconds);
    tm->tm_mday = day;
    tm->tm_sec = 0;
    seconds = std::mktime(tm);
    ts = static_cast<long long>(seconds) * 1000;
}

void _setHour(long long& ts, int hour) {
    std::time_t seconds = ts / 1000;
    std::tm* tm = std::localtime(&seconds);
    tm->tm_hour = hour;
    tm->tm_sec = 0;
    seconds = std::mktime(tm);
    ts = static_cast<long long>(seconds) * 1000;
}

void _setMinute(long long& ts, int minutes) {
    std::time_t seconds = ts / 1000;
    std::tm* tm = std::localtime(&seconds);
    tm->tm_min = minutes;
    tm->tm_sec = 0;
    seconds = std::mktime(tm);
    ts = static_cast<long long>(seconds) * 1000;
}

void _setSecond(long long& ts, int segundo) {
    std::time_t seconds = ts / 1000;
    std::tm* tm = std::localtime(&seconds);

    tm->tm_sec = segundo;   // fijar segundos
    seconds = std::mktime(tm);

    ts = static_cast<long long>(seconds) * 1000;
}

void _setMillis(long long& ts, int millis)
{
    int actual = _getMillis(ts);
    ts += (millis - actual);
}

#endif // MILLIS
