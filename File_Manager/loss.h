#ifndef LOSS_H
#define LOSS_H
#include "mount.h"
#include "structs.h"

class Loss
{
public:
    Loss();
    void Ejecutar(QString id, Mount mount);

private:
    void getDatosParticionMontada(QString id, Mount mount, string *path, int *part_starParticion, int *part_sizeParticion, string *part_nameParticion, int * error);
};

#endif // LOSS_H
