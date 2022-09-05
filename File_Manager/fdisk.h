#ifndef FDISK_H
#define FDISK_H
#include <QString>
#include "iostream"
#include "tipo_estructura.h"

using namespace std;

class Fdisk
{
public:
    Fdisk(int size, QString unit, QString path, QString type, QString fit, QString t_delete, QString name, int add);
    void Ejecutar();

private:
    int tamano;
    int agregar_espacio;    // Puede ser positivo o negativo
    QString unidad;
    QString path;
    QString tipo_particion;
    QString ajuste;
    QString tipo_eliminacion;
    QString nombre; // Nombre de la particion

    void Modificar_Espacio(int add, QString unit, QString name, QString path);
    int buscarParticion_L(QString path, QString name);
    void Eliminar_Particion(QString t_delete, QString path, QString name);
    void Crear_Particion_Primaria();
    void Crear_Particion_Extendida();
    void Crear_Particion_Logica();
    void show_Particiones(QString path);
    bool existeParticion(QString path, QString name);
};

#endif // FDISK_H
