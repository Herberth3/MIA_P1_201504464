#include "lexico.h"

#include <QCoreApplication>
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include "qdebug.h"
#include "qstring.h"
#include <ctime>
#include <stdio.h>
#include <stdlib.h>

Lexico::Lexico()
{

}

void Lexico::Concatenar(char *p, QString direccion){
    char palabra[400];
    int espacio = 0;
    int recien = 0;
    int contador = 0;
    int salto = 0;
    int comillas = 0;
    int comentario = 0;
    int simbolo = 0;

    bool guardar = false;
    bool numero = false;
    bool letra = false;

    FILE *archivo;
    archivo = fopen(direccion , "w");
}
