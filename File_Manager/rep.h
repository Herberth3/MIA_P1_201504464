#ifndef REP_H
#define REP_H
#include "mount.h"
#include "structs.h"
#include <vector>
#include <fstream>
#include <sstream>

class Rep
{
public:
    Rep();
    void Ejecutar(string path, QString name, QString id, Mount mount, string rute);

private:
    void getDatosParticionMontada(QString id, Mount mount, string *path, int *part_starParticion, int *part_sizeParticion, string *part_nameParticion, int * error);
    void getDirectorio(Structs::arbolVirtual avd, string pathD, Structs::SuperBloque superBloque, string * codigo, string * codigoEnlaces, int pointer);
    void getTreeFile(Structs::arbolVirtual avd, string pathD, Structs::SuperBloque superBloque, string * codigo, string * codigoEnlaces, int pointer, vector<string> path);
    void recorrerDetalle(Structs::detalleDirectorio Archivos, int apuntador, vector<string> path, string pathDisco, Structs::SuperBloque superBloque, string * codigo, string * codigoEnlaces, string nombreCarpeta);
    void getTreeDirectorio(Structs::arbolVirtual avd, string pathD, Structs::SuperBloque superBloque, string * codigo, string * codigoEnlaces, int pointer, vector<string> path);
    void recorrerDetalleDirectorio(Structs::detalleDirectorio Archivos, int apuntador, string pathDisco, Structs::SuperBloque superBloque, string * codigo, string * codigoEnlaces, string nombreCarpeta);
    void getTreeComplete(Structs::arbolVirtual avd, string pathD, Structs::SuperBloque superBloque, string * codigo, string * codigoEnlaces, int pointer);
    void recorrerDetalleComplete(Structs::detalleDirectorio Archivos, int apuntador, string pathDisco, Structs::SuperBloque superBloque, string * codigo, string * codigoEnlaces, string nombreCarpeta);
    void recorrerInodo(Structs::InodoArchivo inodo, Structs::SuperBloque superBloque, string pathDisco, string * codigo, string * codigoEnlaces, string nombre);
};

#endif // REP_H
