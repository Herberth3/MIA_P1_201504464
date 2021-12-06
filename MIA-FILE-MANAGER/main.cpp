#include <QCoreApplication>
#include <FlexLexer.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "driver.h"

using namespace std;

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    //ifstream in("entrada.txt");
    yyFlexLexer* lexer = new yyFlexLexer(& std::cin);
    //while(lexer->yylex() != 0)
    //    ;
    float resultado = 0.0;
    calculadora_driver driver;
    if(driver.parse("entrada.txt")){
        printf("La entrada es incorrecta\n");
    }else{
        printf("La entrada es correcta\n");
        //printf("Resultado = %f\n",driver.resultado);
    }

    //return a.exec();
    return 0;
}
