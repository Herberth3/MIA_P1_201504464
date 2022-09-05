#ifndef LOGIN_H
#define LOGIN_H
#include "mount.h"

class Login
{
public:
    Login();
    void Ejecutar(QString id, QString user, QString password, Mount mount, bool &is_login, Sesion &currentSession);

private:
    QString identificador;
    QString usuario;
    QString contrasena;
    Mount montaje;

    void getDatosParticionMontada(QString id, Mount mount, string *path, string *part_nameParticion, int *index, int * error);
    int log_in(string path_disco, string nombre_particion, QString user, QString password, int index, Sesion &currentSession);
    string getFecha();
    int buscarParticion_L(string path, string partition_name);
    int verificarDatos(QString user, QString password, string path, Sesion &currentSession);
    int buscarGrupo(QString group_name, Sesion &currentSession);
};

#endif // LOGIN_H
