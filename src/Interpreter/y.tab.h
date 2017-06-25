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
    RW_UNIQUE = 272,
    RW_PRIMARY = 273,
    RW_KEY = 274,
    RW_ON = 275,
    RW_USE = 276,
    RW_DATABASE = 277,
    RW_EXECFILE = 278,
    RW_INTEGER = 279,
    RW_FLOAT = 280,
    RW_CHAR = 281,
    RW_EXIT = 282,
    RW_TEST = 283,
    T_LT = 284,
    T_GT = 285,
    T_GE = 286,
    T_LE = 287,
    T_EQ = 288,
    T_NE = 289,
    T_EOF = 290,
    NO_TOKEN = 291,
    T_INT = 292,
    T_REAL = 293,
    T_STRING = 294,
    T_QSTRING = 295,
    T_RQSTRING = 296
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
#define RW_UNIQUE 272
#define RW_PRIMARY 273
#define RW_KEY 274
#define RW_ON 275
#define RW_USE 276
#define RW_DATABASE 277
#define RW_EXECFILE 278
#define RW_INTEGER 279
#define RW_FLOAT 280
#define RW_CHAR 281
#define RW_EXIT 282
#define RW_TEST 283
#define T_LT 284
#define T_GT 285
#define T_GE 286
#define T_LE 287
#define T_EQ 288
#define T_NE 289
#define T_EOF 290
#define NO_TOKEN 291
#define T_INT 292
#define T_REAL 293
#define T_STRING 294
#define T_QSTRING 295
#define T_RQSTRING 296

/* Value type.  */


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
