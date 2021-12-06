#ifndef LEXICO_H
#define LEXICO_H
#include <stdio.h>
#include <QString>

using namespace std;


class Lexico
{
public:
    Lexico();
    void Concatenar(char *p, QString direccion);
    const char *palabrasReservadas(char palabra[400]);
};

#endif // LEXICO_H
