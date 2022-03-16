#ifndef CARPETA_H
#define CARPETA_H
#include "mount.h"
#include "mkfs.h"
#include "structs.h"
#include <vector>
#include <sstream>

class carpeta
{
public:
    carpeta();
    void mkDir(QString path, int p, QString id, Mount mount, bool is_bitacora);

private:
    void getDatosParticionMontada(QString id, Mount mount, string *path, int *part_starParticion, int *part_sizeParticion, string *part_nameParticion, int * error);
    void crearCarpeta(Structs::arbolVirtual avd, vector<string> array_directorios, string pathDisco, Structs::SuperBloque superBloque, int pointer);
    string getFecha();
};

#endif // CARPETA_H
