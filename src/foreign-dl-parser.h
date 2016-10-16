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

#ifndef YY_IGRAPH_DL_YY_FOREIGN_DL_PARSER_H_INCLUDED
# define YY_IGRAPH_DL_YY_FOREIGN_DL_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int igraph_dl_yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUM = 258,
    NEWLINE = 259,
    DL = 260,
    NEQ = 261,
    DATA = 262,
    LABELS = 263,
    LABELSEMBEDDED = 264,
    FORMATFULLMATRIX = 265,
    FORMATEDGELIST1 = 266,
    FORMATNODELIST1 = 267,
    DIGIT = 268,
    LABEL = 269,
    EOFF = 270
  };
#endif
/* Tokens.  */
#define NUM 258
#define NEWLINE 259
#define DL 260
#define NEQ 261
#define DATA 262
#define LABELS 263
#define LABELSEMBEDDED 264
#define FORMATFULLMATRIX 265
#define FORMATEDGELIST1 266
#define FORMATNODELIST1 267
#define DIGIT 268
#define LABEL 269
#define EOFF 270

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 91 "foreign-dl-parser.y" /* yacc.c:1909  */

  long int integer;
  igraph_real_t real;

#line 89 "foreign-dl-parser.h" /* yacc.c:1909  */
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



int igraph_dl_yyparse (igraph_i_dl_parsedata_t* context);

#endif /* !YY_IGRAPH_DL_YY_FOREIGN_DL_PARSER_H_INCLUDED  */
