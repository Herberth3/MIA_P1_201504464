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
    void Eliminar_Particion(QString t_delete, QString path, QString name);
    void Crear_Particion(int size, QString unit, QString name, QString path, QString fit, QString type);
    void Crear_Logica(Partition particion, QString path);
    void show_Particiones(QString path);

};

#endif // FDISK_H
