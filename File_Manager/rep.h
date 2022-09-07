#ifndef REP_H
#define REP_H
#include "mount.h"
#include "structs.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <time.h>
#include <string>

#include <sys/stat.h>
#include <dirent.h>

class Rep
{
public:
    Rep();
    // El parametro 'mount' no se pasa por referencia, pues no se va a modificar solo a leer
    void Ejecutar(QString path, QString name, QString id, Mount mount, QString rute);

private:
    void getDatosParticionMontada(QString id, Mount mount, string *path, int * error);
    QString getExtension(QString ruta);
    QString getDirectorioCarpetas(QString ruta);

    void graficarMBR(string path, QString ruta, QString extension);
    void graficarDisco(string path, QString ruta, QString extension);

    void getTreeFile(Structs::arbolVirtual avd, string pathD, Structs::SuperBloque superBloque, string * codigo, string * codigoEnlaces, int pointer, vector<string> path);
    void recorrerDetalle(Structs::detalleDirectorio Archivos, int apuntador, vector<string> path, string pathDisco, Structs::SuperBloque superBloque, string * codigo, string * codigoEnlaces, string nombreCarpeta);
    void getTreeDirectorio(Structs::arbolVirtual avd, string pathD, Structs::SuperBloque superBloque, string * codigo, string * codigoEnlaces, int pointer, vector<string> path);
    void recorrerDetalleDirectorio(Structs::detalleDirectorio Archivos, int apuntador, string pathDisco, Structs::SuperBloque superBloque, string * codigo, string * codigoEnlaces, string nombreCarpeta);
    void getTreeComplete(Structs::arbolVirtual avd, string pathD, Structs::SuperBloque superBloque, string * codigo, string * codigoEnlaces, int pointer);
    void recorrerDetalleComplete(Structs::detalleDirectorio Archivos, int apuntador, string pathDisco, Structs::SuperBloque superBloque, string * codigo, string * codigoEnlaces, string nombreCarpeta);
    void recorrerInodo(Structs::InodoArchivo inodo, Structs::SuperBloque superBloque, string pathDisco, string * codigo, string * codigoEnlaces, string nombre);
};

#endif // REP_H
