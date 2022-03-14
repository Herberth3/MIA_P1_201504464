#ifndef COMANDO_H
#define COMANDO_H
#include <QList>
#include "parametro.h"
#include "mkdisk.h"
#include "rmdisk.h"
#include "fdisk.h"
#include "mount.h"
#include "unmount.h"
#include "mkfs.h"
#include "archivo.h"
#include "carpeta.h"

using namespace std;

class Comando
{
public:
    Comando();
    void Ejecutar(QString command, QList<Parametro *> parameters);

private:
    int comandoID;
    QList<Parametro *> parametros;

    int size_valor;
    int add_valor;
    int p_valor;
    QString unit_valor;
    QString fit_valor;
    QString path_valor;
    QString type_valor;
    QString delete_valor;
    QString name_valor;
    QString id_valor;
    QString fs_valor;
    QString cont_valor;
    QString dest_valor;

    int size_flag;
    int unit_flag;
    int fit_flag;
    int path_flag;
    int type_flag;
    int delete_flag;
    int name_flag;
    int add_flag;
    int id_flag;
    int fs_flag;
    int p_flag;
    int cont_flag;
    int dest_flag;

    // Instancia unica de MOUNT para manejar la estructura encargada de tener
    // las particiones en memoria. Unica para que no se reinscriba la instancia y se pierda la informacion.
    Mount montaje;

    int getComandoID(QString comando);
    int getParametroID(QString parametro);

};

#endif // COMANDO_H
