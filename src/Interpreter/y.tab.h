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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    RW_CREATE = 258,
    RW_DROP = 259,
    RW_TABLE = 260,
    RW_INDEX = 261,
    RW_SELECT = 262,
    RW_FROM = 263,
    RW_WHERE = 264,
    RW_INSERT = 265,
    RW_DELETE = 266,
    RW_UPDATE = 267,
    RW_AND = 268,
    RW_INTO = 269,
    RW_VALUES = 270,
    RW_SET = 271,
    RW_USE = 272,
    RW_DATABASE = 273,
    RW_EXIT = 274,
    RW_TEST = 275,
    T_LT = 276,
    T_GT = 277,
    T_GE = 278,
    T_LE = 279,
    T_EQ = 280,
    T_NE = 281,
    T_EOF = 282,
    NO_TOKEN = 283,
    T_INT = 284,
    T_REAL = 285,
    T_STRING = 286,
    T_QSTRING = 287,
    T_RQSTRING = 288
  };
#endif
/* Tokens.  */
#define RW_CREATE 258
#define RW_DROP 259
#define RW_TABLE 260
#define RW_INDEX 261
#define RW_SELECT 262
#define RW_FROM 263
#define RW_WHERE 264
#define RW_INSERT 265
#define RW_DELETE 266
#define RW_UPDATE 267
#define RW_AND 268
#define RW_INTO 269
#define RW_VALUES 270
#define RW_SET 271
#define RW_USE 272
#define RW_DATABASE 273
#define RW_EXIT 274
#define RW_TEST 275
#define T_LT 276
#define T_GT 277
#define T_GE 278
#define T_LE 279
#define T_EQ 280
#define T_NE 281
#define T_EOF 282
#define NO_TOKEN 283
#define T_INT 284
#define T_REAL 285
#define T_STRING 286
#define T_QSTRING 287
#define T_RQSTRING 288

/* Value type.  */


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
