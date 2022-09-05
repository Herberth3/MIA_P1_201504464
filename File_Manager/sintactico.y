%{
#include "scanner.h"//se importa el header del analisis lexico
#include <QString>
#include <QList>
#include <string>
#include "qdebug.h"
#include <iostream>

#include "parametro.h"
#include "comando.h"
//#include "obmkdisk.h"
using namespace std;
extern int yylineno; //linea actual donde se encuentra el parser (analisis lexico) lo maneja BISON
//extern int columna; //columna actual donde se encuentra el parser (analisis lexico) lo maneja BISON
extern char *yytext; //lexema actual donde esta el parser (analisis lexico) lo maneja BISON

QList<Parametro *> listParam;
Comando comando;

int yyerror(const char* mens)
{
    std::cout << mens <<" "<<yytext<< std::endl;
    return 0;
}
%}

//error-verbose si se especifica la opcion los errores sintacticos son especificados por BISON
%defines "sintactico.h"
%output "sintactico.cpp"
%error-verbose
%locations

%union{
//se especifican los tipo de valores para los no terminales y lo terminales
//char TEXT [256];
//QString TEXT;
char TEXT[256];
class Parametro *param;
class Comando * comand;
}

//TERMINALES DE TIPO TEXT, SON STRINGS

%token<TEXT> r_mkdisk;
%token<TEXT> r_rmdisk;
%token<TEXT> r_fdisk;
%token<TEXT> r_mount;
%token<TEXT> r_unmount;
%token<TEXT> r_mkfs;
%token<TEXT> r_login;
%token<TEXT> r_mkfile;
%token<TEXT> r_mkdir;
%token<TEXT> r_rename;
%token<TEXT> r_copy;
%token<TEXT> r_pause;
%token<TEXT> r_exec;
%token<TEXT> r_recovery;
%token<TEXT> r_loss;
%token<TEXT> r_rep;

%token<TEXT> r_size;
%token<TEXT> r_unit;
%token<TEXT> r_fit;
%token<TEXT> r_path;
%token<TEXT> r_type;
%token<TEXT> r_delete;
%token<TEXT> r_name;
%token<TEXT> r_add;
%token<TEXT> r_id;
%token<TEXT> r_fs;
%token<TEXT> r_password;
%token<TEXT> r_usuario;
%token<TEXT> r_p;
%token<TEXT> r_r;
%token<TEXT> r_cont;
%token<TEXT> r_destino;
%token<TEXT> r_ruta;
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
%token<TEXT> s_virulilla;

%token<TEXT> numero_entero;
%token<TEXT> numero_real;
%token<TEXT> cadena_string;
%token<TEXT> identificador;
%token<TEXT> id_mount;
%token<TEXT> id_fs;
%token<TEXT> directorio;
%token<TEXT> directorio_exec;
%token<TEXT> password_l;
%token<TEXT> directorio_gen;


%type <param> PARAMETERS; // lista de instrucciones
%type <comand> COMMAND;
%type <param> LIST_PARAMETERS;

%left s_mas s_menos
%left s_por s_division
%left potencia

%start INICIO

/* Definicion de la gramatica */
%%

INICIO
    : COMMAND { }
;

COMMAND
    : r_mkdisk LIST_PARAMETERS  { comando.Ejecutar($1, listParam); $$ = new Comando(); }
    | r_rmdisk s_menos r_path s_menos s_mayor_que directorio      { Parametro *nuevo = new Parametro($3, $6); listParam.append(nuevo); comando.Ejecutar($1, listParam); $$ = new Comando(); }
    | r_rmdisk s_menos r_path s_menos s_mayor_que cadena_string      { Parametro *nuevo = new Parametro($3, $6); listParam.append(nuevo); comando.Ejecutar($1, listParam); $$ = new Comando(); }
    | r_fdisk LIST_PARAMETERS   { comando.Ejecutar($1, listParam); $$ = new Comando(); }
    | r_mount LIST_PARAMETERS   { comando.Ejecutar($1, listParam); $$ = new Comando(); }
    | r_unmount s_menos r_id s_menos s_mayor_que id_mount      { Parametro *nuevo = new Parametro($3, $6); listParam.append(nuevo); comando.Ejecutar($1, listParam); $$ = new Comando(); }
    | r_mkfs LIST_PARAMETERS   { comando.Ejecutar($1, listParam); $$ = new Comando(); }
    | r_login LIST_PARAMETERS   { comando.Ejecutar($1, listParam); $$ = new Comando(); }
    | r_mkfile LIST_PARAMETERS   { comando.Ejecutar($1, listParam); $$ = new Comando(); }
    | r_mkdir LIST_PARAMETERS   { comando.Ejecutar($1, listParam); $$ = new Comando(); }
    | r_copy LIST_PARAMETERS   { comando.Ejecutar($1, listParam); $$ = new Comando(); }
    | r_pause      { comando.Ejecutar($1, listParam); $$ = new Comando(); }
    | r_exec s_menos r_path s_menos s_mayor_que directorio_exec      { Parametro *nuevo = new Parametro($3, $6); listParam.append(nuevo); comando.Ejecutar($1, listParam); $$ = new Comando(); }
    | r_exec s_menos r_path s_menos s_mayor_que cadena_string      { Parametro *nuevo = new Parametro($3, $6); listParam.append(nuevo); comando.Ejecutar($1, listParam); $$ = new Comando(); }
    | r_rename LIST_PARAMETERS   { comando.Ejecutar($1, listParam); $$ = new Comando(); }
    | r_recovery s_menos r_id s_menos s_mayor_que id_mount      { Parametro *nuevo = new Parametro($3, $6); listParam.append(nuevo); comando.Ejecutar($1, listParam); $$ = new Comando(); }
    | r_loss s_menos r_id s_menos s_mayor_que id_mount      { Parametro *nuevo = new Parametro($3, $6); listParam.append(nuevo); comando.Ejecutar($1, listParam); $$ = new Comando(); }
    | r_rep LIST_PARAMETERS   { comando.Ejecutar($1, listParam); $$ = new Comando(); }
;

LIST_PARAMETERS
    : LIST_PARAMETERS PARAMETERS { }
    | PARAMETERS { }
;

PARAMETERS
    : s_menos r_size s_menos s_mayor_que numero_entero  { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_path s_menos s_mayor_que cadena_string      { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_path s_menos s_mayor_que directorio      { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_path s_menos s_mayor_que directorio_gen      { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_unit s_menos s_mayor_que identificador { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_fit s_menos s_mayor_que identificador { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_type s_menos s_mayor_que identificador { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_delete s_menos s_mayor_que identificador { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_name s_menos s_mayor_que identificador { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_name s_menos s_mayor_que cadena_string { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_add s_menos s_mayor_que numero_real { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_id s_menos s_mayor_que id_mount { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_fs s_menos s_mayor_que id_fs { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_password s_menos s_mayor_que password_l { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_password s_menos s_mayor_que cadena_string { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_usuario s_menos s_mayor_que identificador { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_usuario s_menos s_mayor_que cadena_string { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_p { $$ = new Parametro($2, ""); listParam.append($$); }
    | s_menos r_r { $$ = new Parametro($2, ""); listParam.append($$); }
    | s_menos r_cont s_menos s_mayor_que cadena_string { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_cont s_menos s_mayor_que directorio_gen { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_destino s_menos s_mayor_que cadena_string { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_destino s_menos s_mayor_que directorio_gen { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_ruta s_menos s_mayor_que cadena_string { $$ = new Parametro($2, $5); listParam.append($$); }
    | s_menos r_ruta s_menos s_mayor_que directorio_gen { $$ = new Parametro($2, $5); listParam.append($$); }
;
