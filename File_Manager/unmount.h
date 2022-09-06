#ifndef UNMOUNT_H
#define UNMOUNT_H
#include "mount.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>

class Unmount
{
public:
    Unmount();
    void Ejecutar(QString id, Mount &mount);

private:
    QString identificador;
};

#endif // UNMOUNT_H
