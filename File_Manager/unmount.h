#ifndef UNMOUNT_H
#define UNMOUNT_H
#include "mount.h"

class Unmount
{
public:
    Unmount();
    void Ejecutar(QString id, Mount &mount);

private:
    QString identificador;
};

#endif // UNMOUNT_H
