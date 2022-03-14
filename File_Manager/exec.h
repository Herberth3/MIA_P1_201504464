#ifndef EXEC_H
#define EXEC_H
#include "scanner.h"
#include "sintactico.h"
#include "parametro.h"
#include "iostream"
#include <fstream>
#include <vector>

using namespace std;
extern int yyparse();
extern int linea; // Linea del token
extern int yylineno;
extern QList<Parametro *> listParam;

class Exec
{
public:
    Exec(QString path);
    void Ejecutar();

private:
    QString path;
    void Interpretar(QString comando);
};

#endif // EXEC_H
