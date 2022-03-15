#ifndef MKFS_H
#define MKFS_H
#include "string.h"
#include "mount.h"
#include "unmount.h"
#include "tipo_estructura.h"
#include "structs.h"
#include "archivo.h"
#include "carpeta.h"

class Mkfs
{
public:
    Mkfs();
    void Ejecutar(QString id, QString type, QString fs, Mount mount);

private:
    QString identificador;
    QString tipo_formateo;
    QString tipo_sistema;
    Mount montaje;

    void getDatosID(QString id, Mount mount, string *path, int *part_starParticion, int *part_sizeParticion, string *part_nameParticion, int * error);
    string getFecha();
};

#endif // MKFS_H
