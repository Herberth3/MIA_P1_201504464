%{
#include <iostream>
//#include <QString>
#include "grammar.h"

//#include <string>
//#include <QList>

    int yylex();
    void yyerror(const char *s);
    //int linea = 0;
    //int columna = 1;
    //int anterior = 0;
%}

%option case-insensitive
/*yylineno ordena a flex que mantenga el numero de la linea actual*/
/*%option yylineno*/
/*noyywrap le dice a flex que lea solo un archivo de entrada*/
%option noyywrap 
%option nounput


numero_entero [0-9]+
letra [a-zA-ZÑñáéíóúÁÉÍÓÚ]+

identificador {letra}({letra}|{numero_entero}|"_")*
caracter "\'"("\\")?({letra}|{numero_entero})"\'"
cadena_string [\"][^\"\n]*[\"\n]
delimitador [\t\r\f" "]
directorio [(/){identificador}]+[.][dk]
/*%x COM*/


%%

"SIZE" { return r_size; }
"MKDISK" { return r_mkdisk; }
"U" { return r_u; }
"F" { return r_f; }
"PATH" { return r_path; }


{numero_entero} { return numero_entero; }
{cadena_string} { return cadena_string; }
{identificador} { return identificador; }
{caracter} { return caracter; }
{delimitador} {}
{directorio} { return directorio; }


"." { return punto; }
";" { return punto_y_coma; }
":" { return dos_puntos; }
"," { return coma; }
"[" { return corchete_izq; }
"]" { return corchete_der; }
"(" { return parentesis_izq; }
")" { return parentesis_der; }
"{" { return llave_izq; }
"}" { return llave_der; }
"+" { return s_mas; }
"-" { return s_menos; }
"*" { return s_por; }
"/" { return s_division; }
"^" { return potencia; }
"=" { return s_igual; }
"<" { return s_menor_que; }
">" { return s_mayor_que; }
"$" { return s_dolar; }

.   { std::cout<<"Error Lexico: "<<yytext<<std::endl; //printf("Error lexico en linea %d",linea); }

%%