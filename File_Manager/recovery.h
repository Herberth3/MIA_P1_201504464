#ifndef RECOVERY_H
#define RECOVERY_H
#include "mkfs.h"

class Recovery
{
public:
    Recovery();
    void Ejecutar(QString id, Mount mount);

private:
    void getDatosParticionMontada(QString id, Mount mount, string *path, int *part_starParticion, int *part_sizeParticion, string *part_nameParticion, int * error);
    string getFecha();
};

#endif // RECOVERY_H
