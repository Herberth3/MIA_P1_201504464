/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_SINTACTICO_H_INCLUDED
# define YY_YY_SINTACTICO_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    r_mkdisk = 258,
    r_rmdisk = 259,
    r_fdisk = 260,
    r_mount = 261,
    r_unmount = 262,
    r_mkfs = 263,
    r_login = 264,
    r_mkfile = 265,
    r_mkdir = 266,
    r_rename = 267,
    r_copy = 268,
    r_pause = 269,
    r_exec = 270,
    r_recovery = 271,
    r_loss = 272,
    r_size = 273,
    r_unit = 274,
    r_fit = 275,
    r_path = 276,
    r_type = 277,
    r_delete = 278,
    r_name = 279,
    r_add = 280,
    r_id = 281,
    r_fs = 282,
    r_password = 283,
    r_usuario = 284,
    r_p = 285,
    r_r = 286,
    r_cont = 287,
    r_destino = 288,
    punto = 289,
    punto_y_coma = 290,
    dos_puntos = 291,
    coma = 292,
    corchete_izq = 293,
    corchete_der = 294,
    parentesis_izq = 295,
    parentesis_der = 296,
    llave_izq = 297,
    llave_der = 298,
    s_mas = 299,
    s_menos = 300,
    s_por = 301,
    s_division = 302,
    potencia = 303,
    s_igual = 304,
    s_menor_que = 305,
    s_mayor_que = 306,
    s_dolar = 307,
    s_virulilla = 308,
    numero_entero = 309,
    numero_real = 310,
    cadena_string = 311,
    identificador = 312,
    id_mount = 313,
    id_fs = 314,
    directorio = 315,
    directorio_exec = 316,
    password_l = 317,
    directorio_gen = 318
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 33 "sintactico.y" /* yacc.c:1909  */

//se especifican los tipo de valores para los no terminales y lo terminales
//char TEXT [256];
//QString TEXT;
char TEXT[256];
class Parametro *param;
class Comando * comand;

#line 127 "sintactico.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_SINTACTICO_H_INCLUDED  */
