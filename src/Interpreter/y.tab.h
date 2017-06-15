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
    RW_EXIT = 272,
    RW_TEST = 273,
    T_LT = 274,
    T_GT = 275,
    T_GE = 276,
    T_LE = 277,
    T_EQ = 278,
    T_NE = 279,
    T_EOF = 280,
    NO_TOKEN = 281,
    T_INT = 282,
    T_REAL = 283,
    T_STRING = 284,
    T_QSTRING = 285,
    T_ASTRING = 286,
    ddl = 287,
    query = 288,
    delete_op = 289,
    update = 290,
    create_table = 291,
    create_index = 292,
    drop_table = 293,
    drop_index = 294
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
#define RW_EXIT 272
#define RW_TEST 273
#define T_LT 274
#define T_GT 275
#define T_GE 276
#define T_LE 277
#define T_EQ 278
#define T_NE 279
#define T_EOF 280
#define NO_TOKEN 281
#define T_INT 282
#define T_REAL 283
#define T_STRING 284
#define T_QSTRING 285
#define T_ASTRING 286
#define ddl 287
#define query 288
#define delete_op 289
#define update 290
#define create_table 291
#define create_index 292
#define drop_table 293
#define drop_index 294

/* Value type.  */


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
