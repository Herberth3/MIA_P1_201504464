%{
#include "grammar.h"//se importa el header del analisis sintactico
#include <QString>
#include <string>
#include "qdebug.h"
#include <iostream>
//#include "obmkdisk.h"
using namespace std;
//extern int yylineno; //linea actual donde se encuentra el parser (analisis lexico) lo maneja BISON
//extern int columna; //columna actual donde se encuentra el parser (analisis lexico) lo maneja BISON
extern char *yytext; //lexema actual donde esta el parser (analisis lexico) lo maneja BISON

int yyerror(const char* mens)
{
std::cout << mens <<" "<<yytext<< std::endl;
return 0;
}
%}

//error-verbose si se especifica la opcion los errores sintacticos son especificados por BISON
%defines "grammar.h"
%output "grammar.cpp"
%error-verbose
%locations

%union{
//se especifican los tipo de valores para los no terminales y lo terminales
//char TEXT [256];
//QString TEXT;
char TEXT[256];
//class obmkdisk *mdisk;
}

//TERMINALES DE TIPO TEXT, SON STRINGS

%token<TEXT> r_size;
%token<TEXT> r_mkdisk;
%token<TEXT> r_u;
%token<TEXT> r_f;
%token<TEXT> r_path;
%token<TEXT> punto;
%token<TEXT> punto_y_coma;
%token<TEXT> dos_puntos;
%token<TEXT> coma;
%token<TEXT> corchete_izq;
%token<TEXT> corchete_der;
%token<TEXT> parentesis_izq;
%token<TEXT> parentesis_der;
%token<TEXT> llave_izq;
%token<TEXT> llave_der;
%token<TEXT> s_mas;
%token<TEXT> s_menos;
%token<TEXT> s_por;
%token<TEXT> s_division;
%token<TEXT> potencia;
%token<TEXT> s_igual;
%token<TEXT> s_menor_que;
%token<TEXT> s_mayor_que;
%token<TEXT> s_dolar;

%token<TEXT> numero_entero;
%token<TEXT> cadena_string;
%token<TEXT> identificador;
%token<TEXT> caracter;
%token<TEXT> directorio;


//%type<mdisk> COMANDOMKDISK; // lista de instrucciones

%left s_mas s_menos
%left s_por s_division
%left potencia

%start INICIO

/* Definicion de la gramatica */
/* Mkdisk -Size=3000 –u=K -path=/home/user/Disco1.dk */
%%

INICIO 
    : COMMAND { }
;

COMMAND
    : r_mkdisk LIST_PARAMETERS  { }
;

LIST_PARAMETERS
    : LIST_PARAMETERS PARAMETERS { $1.push($2); $$ = $1; }
    | PARAMETERS { $$ = [$1]; }
;

PARAMETERS
    : s_menos r_size s_igual numero_entero  { }
    | s_menos r_path s_igual TYPE_PATH      { }
    | s_menos r_u s_igual identificador {}
    | s_menos r_f s_igual identificador {}
    : s_menos r_size s_igual numero_entero {int tam=atoi($4); obmkdisk *disco=new obmkdisk(); disco->size=tam;  $$=disco;}
;

TYPE_PATH
    : cadena_string { $$ = $1; }
    | directorio    { $$ = $1; }
;