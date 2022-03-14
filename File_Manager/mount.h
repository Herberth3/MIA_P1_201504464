#ifndef MOUNT_H
#define MOUNT_H
#include <QString>
#include "iostream"
#include "tipo_estructura.h"

using namespace std;

class Mount
{
public:
    Mount();
    void Ejecutar(QString path, QString name);
    void Show_Montajes();

    typedef struct
    {
        char letra;
        int estado = 0;
        char nombre[16] = "";
    } PARTICIONMONTADA;

    typedef struct
    {
        char path[100] = "";
        int numero = 0;
        int estado = 0;
        PARTICIONMONTADA particiones[99];
    } DISCOMONTADO;

    DISCOMONTADO discos[26];

private:
    QString path;
    QString nombre;
};

#endif // MOUNT_H
