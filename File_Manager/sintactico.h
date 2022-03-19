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
    r_rep = 273,
    r_size = 274,
    r_unit = 275,
    r_fit = 276,
    r_path = 277,
    r_type = 278,
    r_delete = 279,
    r_name = 280,
    r_add = 281,
    r_id = 282,
    r_fs = 283,
    r_password = 284,
    r_usuario = 285,
    r_p = 286,
    r_r = 287,
    r_cont = 288,
    r_destino = 289,
    r_ruta = 290,
    punto = 291,
    punto_y_coma = 292,
    dos_puntos = 293,
    coma = 294,
    corchete_izq = 295,
    corchete_der = 296,
    parentesis_izq = 297,
    parentesis_der = 298,
    llave_izq = 299,
    llave_der = 300,
    s_mas = 301,
    s_menos = 302,
    s_por = 303,
    s_division = 304,
    potencia = 305,
    s_igual = 306,
    s_menor_que = 307,
    s_mayor_que = 308,
    s_dolar = 309,
    s_virulilla = 310,
    numero_entero = 311,
    numero_real = 312,
    cadena_string = 313,
    identificador = 314,
    id_mount = 315,
    id_fs = 316,
    directorio = 317,
    directorio_exec = 318,
    password_l = 319,
    directorio_gen = 320
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

#line 129 "sintactico.h" /* yacc.c:1909  */
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
