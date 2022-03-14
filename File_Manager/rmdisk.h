#ifndef RMDISK_H
#define RMDISK_H
#include <QString>
#include "iostream"
#include "fstream"

using namespace std;

class Rmdisk
{
public:
    Rmdisk(QString path);
    void Ejecutar();

private:
    QString path;
};

#endif // RMDISK_H
