#ifndef MKDISK_H
#define MKDISK_H
#include <QString>
#include "string.h"
#include "iostream"
#include "tipo_estructura.h"
#include <QStringList>

#include <sys/stat.h>
#include <dirent.h>

using namespace std;

class Mkdisk
{
public:
    Mkdisk(int size, QString unit, QString fit, QString path);
    void Ejecutar();

private:
    int tamano;
    QString unidad;
    QString ajuste;
    QString path;

    void CrearDirectorio(QString ruta);
    QString getDirectorioCarpetas(QString ruta);

};

#endif // MKDISK_H
