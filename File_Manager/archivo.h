#ifndef ARCHIVO_H
#define ARCHIVO_H
#include "mount.h"
#include "mkfs.h"
#include "carpeta.h"
#include <cmath>
#include <vector>

class archivo
{
public:
    archivo();
    void mkFile(QString id, QString path, int r, int size, QString cont, Mount mount, bool is_bitacora);

private:
    void getDatosParticionMontada(QString id, Mount mount, string *path, int *part_starParticion, int *part_sizeParticion, string *part_nameParticion, int * error);
    string getFecha();
    void recorrerRuta(Structs::arbolVirtual avd, vector<string> path, string pathDisco, Structs::SuperBloque superBloque, int size, string cont);
    void crearArchivo(Structs::detalleDirectorio Archivos, int apuntador, vector<string> path, string pathDisco, Structs::SuperBloque superBloque, int size, string cont);
    void crearInodo(Structs::InodoArchivo inodo, string nombre_dir, string pathDisco, Structs::SuperBloque superBloque, string cont, int cantidadBloques);
};

#endif // ARCHIVO_H
