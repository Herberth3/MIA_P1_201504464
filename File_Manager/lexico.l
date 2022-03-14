%option case-insensitive
/*yylineno ordena a flex que mantenga el numero de la linea actual*/
%option yylineno

%{
/*-----------------------------------
|         Definiciones en C          |
------------------------------------*/
#include <iostream>
#include "sintactico.h"
int yylex();
void yyerror(const char *s);
int linea = 0;

%}

/*-----------------------------------
|        Definiciones de Flex        |
------------------------------------*/
/*noyywrap le dice a flex que lea solo un archivo de entrada*/
%option noyywrap
%option nounput

/*-----------------------------------
|       Expresiones regulares        |
------------------------------------*/
NUMERO_ENTERO           [0-9]+
NUMERO_REAL             [-]?[0-9]+
LETRA               [a-zA-ZÑñáéíóúÁÉÍÓÚ]+

IDENTIFICADOR               {LETRA}({LETRA}|{NUMERO_ENTERO}|"_")*
ID_FS                               {NUMERO_ENTERO}({LETRA})+
CADENA_STRING               [\"][^\"\n]*[\"\n]
DELIMITADOR             [\t\r\f" "]
DIRECTORIO              ([\/]{IDENTIFICADOR})+[\.][d][i][s][k]

%x COM


%%

"MKDISK"        { strcpy(yylval.TEXT, yytext); return r_mkdisk; }
"RMDISK"        { strcpy(yylval.TEXT, yytext); return r_rmdisk; }
"FDISK"             { strcpy(yylval.TEXT, yytext); return r_fdisk; }
"MOUNT"             { strcpy(yylval.TEXT, yytext); return r_mount; }
"UNMOUNT"             { strcpy(yylval.TEXT, yytext); return r_unmount; }
"MKFS"             { strcpy(yylval.TEXT, yytext); return r_mkfs; }
"MKFILE"             { strcpy(yylval.TEXT, yytext); return r_mkfile; }
"MKDIR"             { strcpy(yylval.TEXT, yytext); return r_mkdir; }
"CP"             { strcpy(yylval.TEXT, yytext); return r_cp; }
"PAUSE"             { strcpy(yylval.TEXT, yytext); return r_pause; }
"SIZE"              { strcpy(yylval.TEXT, yytext); return r_size; }
"UNIT"                 { strcpy(yylval.TEXT, yytext); return r_unit; }
"FIT"                 { strcpy(yylval.TEXT, yytext); return r_fit; }
"PATH"          { strcpy(yylval.TEXT, yytext); return r_path; }
"TYPE"          { strcpy(yylval.TEXT, yytext); return r_type; }
"DELETE"          { strcpy(yylval.TEXT, yytext); return r_delete; }
"NAME"          { strcpy(yylval.TEXT, yytext); return r_name; }
"ADD"          { strcpy(yylval.TEXT, yytext); return r_add; }
"ID"          { strcpy(yylval.TEXT, yytext); return r_id; }
"FS"          { strcpy(yylval.TEXT, yytext); return r_fs; }
"P"          { strcpy(yylval.TEXT, yytext); return r_p; }
"CONT"          { strcpy(yylval.TEXT, yytext); return r_cont; }
"DEST"          { strcpy(yylval.TEXT, yytext); return r_dest; }


{NUMERO_ENTERO}     { strcpy(yylval.TEXT, yytext); return numero_entero; }
{NUMERO_REAL}     { strcpy(yylval.TEXT, yytext); return numero_real; }
{CADENA_STRING}         { strcpy(yylval.TEXT, yytext); return cadena_string; }
{IDENTIFICADOR}         { strcpy(yylval.TEXT, yytext); return identificador; }
{ID_FS}         { strcpy(yylval.TEXT, yytext); return id_fs; }
{DELIMITADOR}           {}
{DIRECTORIO}                { strcpy(yylval.TEXT, yytext); return directorio; }


"." { strcpy(yylval.TEXT, yytext); return punto; }
";" { strcpy(yylval.TEXT, yytext); return punto_y_coma; }
":" { strcpy(yylval.TEXT, yytext); return dos_puntos; }
"," { strcpy(yylval.TEXT, yytext); return coma; }
"[" { strcpy(yylval.TEXT, yytext); return corchete_izq; }
"]" { strcpy(yylval.TEXT, yytext); return corchete_der; }
"(" { strcpy(yylval.TEXT, yytext); return parentesis_izq; }
")" { strcpy(yylval.TEXT, yytext); return parentesis_der; }
"{" { strcpy(yylval.TEXT, yytext); return llave_izq; }
"}" { strcpy(yylval.TEXT, yytext); return llave_der; }
"+" { strcpy(yylval.TEXT, yytext); return s_mas; }
"-" { strcpy(yylval.TEXT, yytext); return s_menos; }
"*" { strcpy(yylval.TEXT, yytext); return s_por; }
"/" { strcpy(yylval.TEXT, yytext); return s_division; }
"^" { strcpy(yylval.TEXT, yytext); return potencia; }
"=" { strcpy(yylval.TEXT, yytext); return s_igual; }
"<" { strcpy(yylval.TEXT, yytext); return s_menor_que; }
">" { strcpy(yylval.TEXT, yytext); return s_mayor_que; }
"$" { strcpy(yylval.TEXT, yytext); return s_dolar; }
"~" { strcpy(yylval.TEXT, yytext); return s_virulilla; }

"\n" { linea++; }

.   { std::cout<<"Error Lexico: "<<linea<<std::endl; }