#ifndef PARAMETRO_H
#define PARAMETRO_H
#include <QList>
#include <QString>

class Parametro
{
public:
    Parametro(QString name, QString value);
    QString getNombre();
    QString getValor();

private:
    QString nombre;
    QString valor;
};

#endif // PARAMETRO_H
