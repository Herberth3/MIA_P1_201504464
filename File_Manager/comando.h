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
#include "login.h"
#include "archivo.h"
#include "carpeta.h"
#include "exec.h"
#include "recovery.h"
#include "loss.h"
#include "rep.h"

using namespace std;

class Comando
{
public:
    Comando();
    void Ejecutar(QString command, QList<Parametro *> parameters);

private:
    int comandoID;
    QList<Parametro *> parametros;

    /** ALMACENAN EL VALOR QUE TRAE EL PARAMETRO RESPECTIVO**/
    int size_valor;
    int add_valor;
    QString unit_valor;
    QString fit_valor;
    QString path_valor;
    QString type_valor;
    QString delete_valor;
    QString name_valor;
    QString id_valor;
    QString fs_valor;
    QString usuario_valor;
    QString password_valor;
    QString cont_valor;
    QString destino_valor;
    QString ruta_valor;

    /** BANDERAS QUE CONFIRMAN SI EL PARAMETRO ESTA REPETIDO**/
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
    int usuario_flag;
    int password_flag;
    int p_flag;
    int r_flag;
    int cont_flag;
    int destino_flag;
    int ruta_flag;

    /** VARIABLES GLOBALES
    * Instancia unica de MOUNT para manejar la estructura encargada de tener
    * las particiones en memoria. Unica para que no se reinscriba la instancia y se pierda la informacion.
    **/
    Mount montaje;
    bool flag_login = false;
    Sesion currentSession;

    /**
      * Retornan el numero identificador en el ENUM para cada comando y parametro
      **/
    int getComandoID(QString comando);
    int getParametroID(QString parametro);
    /**
      * Retorna FALSO si la particion no esta montada
    **/
    bool existeMontaje(string path, string name_partition);

};

#endif // COMANDO_H
