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
    void getDatosParticionMontada(QString id, Mount mount, string *path, string *name_disk, int *part_starParticion, int * error);
    QString getExtension(QString ruta);
    QString getDirectorioCarpetas(QString ruta);

    void graficarMBR(string path, QString ruta, QString extension);
    void graficarDisco(string path, QString ruta, QString extension);
    void graficarSB(string path, QString ruta, QString extension, string name_disk, int part_start_SB);
    void graficarTREE(string path, QString ruta, QString extension, int part_start_Partition);
    void graficarInodo(string path, QString ruta, QString extension, int bm_inodo_start, int inodo_start, int bm_block_start);
    void graficarJournal(string path, QString ruta, QString extension, int part_start_SB);

};

#endif // REP_H
