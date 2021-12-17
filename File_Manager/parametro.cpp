#include "parametro.h"

Parametro::Parametro(QString name, QString value)
{
    this->nombre = name;
    this->valor = value;
}

QString Parametro::getNombre()
{
    return this->nombre;
}

QString Parametro::getValor()
{
    return this->valor;
}
