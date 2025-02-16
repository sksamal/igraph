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

#ifndef YY_IGRAPH_PAJEK_YY_FOREIGN_PAJEK_PARSER_H_INCLUDED
# define YY_IGRAPH_PAJEK_YY_FOREIGN_PAJEK_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int igraph_pajek_yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NEWLINE = 258,
    NUM = 259,
    ALNUM = 260,
    QSTR = 261,
    PSTR = 262,
    NETWORKLINE = 263,
    VERTICESLINE = 264,
    ARCSLINE = 265,
    EDGESLINE = 266,
    ARCSLISTLINE = 267,
    EDGESLISTLINE = 268,
    MATRIXLINE = 269,
    VP_X_FACT = 270,
    VP_Y_FACT = 271,
    VP_IC = 272,
    VP_BC = 273,
    VP_LC = 274,
    VP_LR = 275,
    VP_LPHI = 276,
    VP_BW = 277,
    VP_FOS = 278,
    VP_PHI = 279,
    VP_R = 280,
    VP_Q = 281,
    VP_LA = 282,
    VP_FONT = 283,
    VP_URL = 284,
    VP_SIZE = 285,
    EP_C = 286,
    EP_S = 287,
    EP_A = 288,
    EP_W = 289,
    EP_H1 = 290,
    EP_H2 = 291,
    EP_A1 = 292,
    EP_A2 = 293,
    EP_K1 = 294,
    EP_K2 = 295,
    EP_AP = 296,
    EP_P = 297,
    EP_L = 298,
    EP_LP = 299,
    EP_LR = 300,
    EP_LPHI = 301,
    EP_LC = 302,
    EP_LA = 303,
    EP_SIZE = 304,
    EP_FOS = 305
  };
#endif
/* Tokens.  */
#define NEWLINE 258
#define NUM 259
#define ALNUM 260
#define QSTR 261
#define PSTR 262
#define NETWORKLINE 263
#define VERTICESLINE 264
#define ARCSLINE 265
#define EDGESLINE 266
#define ARCSLISTLINE 267
#define EDGESLISTLINE 268
#define MATRIXLINE 269
#define VP_X_FACT 270
#define VP_Y_FACT 271
#define VP_IC 272
#define VP_BC 273
#define VP_LC 274
#define VP_LR 275
#define VP_LPHI 276
#define VP_BW 277
#define VP_FOS 278
#define VP_PHI 279
#define VP_R 280
#define VP_Q 281
#define VP_LA 282
#define VP_FONT 283
#define VP_URL 284
#define VP_SIZE 285
#define EP_C 286
#define EP_S 287
#define EP_A 288
#define EP_W 289
#define EP_H1 290
#define EP_H2 291
#define EP_A1 292
#define EP_A2 293
#define EP_K1 294
#define EP_K2 295
#define EP_AP 296
#define EP_P 297
#define EP_L 298
#define EP_LP 299
#define EP_LR 300
#define EP_LPHI 301
#define EP_LC 302
#define EP_LA 303
#define EP_SIZE 304
#define EP_FOS 305

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 123 "foreign-pajek-parser.y" /* yacc.c:1909  */

  long int intnum;
  double   realnum;  
  struct {
    char *str;
    int len;
  } string;  

#line 163 "foreign-pajek-parser.h" /* yacc.c:1909  */
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



int igraph_pajek_yyparse (igraph_i_pajek_parsedata_t* context);

#endif /* !YY_IGRAPH_PAJEK_YY_FOREIGN_PAJEK_PARSER_H_INCLUDED  */
