#include <QCoreApplication>
#include <iostream>
#include "scanner.h"
#include "sintactico.h"
#include "parametro.h"
#include <fstream>
#include <stdio.h>
#include <QTextStream>
#include <QString>

using namespace std;
extern int yyparse();
extern int linea; // Linea del token
extern int yylineno;
extern QList<Parametro *> listParam;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextStream qtin(stdin);
    QString line;

    while (line != "salir") {//esto me sirve para seguir leyendo siempre los comandos sin salirme
        cout << "------------------------------Ingrese un comando------------------------------"<<endl;

        line = qtin.readLine();
        if(line != "salir"){

            if(line.isEmpty()==false){

                YY_BUFFER_STATE buffer = yy_scan_string(line.toUtf8().constData());

                /*Limpiamos los contadores ya que son variables globales*/
                linea = 0;
                yylineno = 0;

                if (listParam.count() > 0) {
                    listParam.clear();
                }

                if(yyparse()==0) // Si nos da un número negativo, signifca error.
                {
                    printf("\nComando ejecutado correctamente\n");

                }else {

                    printf("\nhay errores\n");
                }
                yy_delete_buffer(buffer);

            }
        }
    }
    exit(1);

    return a.exec();
}
