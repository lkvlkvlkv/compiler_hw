/*********************** parser.c **********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
** ?菢q?簎Y檔
*/
  #include "scanner.h"
  #include "resword.h"
  #include "err.h"
  #include "followsym.h"
/*
** ?菢q常數
*/
  #define IDLEN      36
  #define BUFSIZE   512
/*
** ?菢q函式?鴢?
*/
  void Identifier();
  void Number();
  void IdentifierList();
  void Factor();
  void Term();
  void Expression();
  void Condition();
  void WriteStatement();
  void ReadStatement();
  void WhileStatement();
  void IfStatement();
  void CompoundStatement();
  void CallStatement();
  void AssignmentStatement();
  void Statement();
  void ProcDeclaration();
  void VarDeclaration();
  void ConstDeclaration();
  void Block();
  void ProgramHead();
  void Program();
/*
** 整體變數
*/
  struct symbolTag *token;
  int errorCount = 0;
  char str[BUFSIZE];
/*
** Error()
*/
  void Error(int n)
  {
    int j;
    printf("****");
    for (j=0; j<token->right; j++)
      printf(" ");
    sprintf(str, "^%d  %s\n",n, errmsgs[n]);
    printf(str);
    errorCount++;
  }
/*
** skip()
*/
  void skip(char follows[], int n)
  {
    if (follows[token->sym]==0)
    {
      Error(n);
      while (follows[token->sym]==0)
        token = nextToken();
    }
  }
/*
** 語法規則#1 <Program>
*/
  void Program()
  {
    ProgramHead();
    Block();
    if (token->sym != symPERIOD) Error(0);
  }
/*
** 語法規則#2 <ProgramHead>
*/
  void ProgramHead()
  {
    if (strcmp(token->value,"PROGRAM")==0)
    {
      token = nextToken();
      if (token->sym == symIDENTIFIER)
      {
        token = nextToken();
        if (token->sym == symSEMI)
          token = nextToken();
        else
          Error(3);
      }
      else
        Error(2);
    }
    else
      Error(1);
  }
/*
** 語法規則#3 <Block>
*/
  void Block()
  {
    if (strcmp(token->value, "CONST")==0)
      ConstDeclaration();
    if (strcmp(token->value, "VAR")==0)
      VarDeclaration();
    if (strcmp(token->value, "PROCEDURE")==0)
      ProcDeclaration();
    CompoundStatement();
  }
/*
** 語法規則#4 <ConstDeclaration>
*/
  void ConstDeclaration()
  {
    if (strcmp(token->value, "CONST")==0)
    {
      token = nextToken();
      Identifier();
      if (token->sym == symEQ)
      {
        token = nextToken();
        if (token->sym == symSTRING)
          token = nextToken();
        else
        {
          Error(27);
          skip(statement, 23);
        }
        while (token->sym == symCOMMA)
        {
          token = nextToken();
          Identifier();
          if (token->sym == symEQ)
          {
            token = nextToken();
            if (token->sym == symSTRING)
              token = nextToken();
            else
            {
              Error(27);
              skip(statement, 23);
            }
          }
          else
          {
            Error(5);
            skip(statement, 23);
          }
        }
        if (token->sym == symSEMI)
          token = nextToken();
        else
        {
          Error(6);
          skip(statement, 23);
          if (token->sym == symSEMI)
            token = nextToken();
        }
      }
      else
      {
        Error(5);
        skip(statement, 23);
        if (token->sym == symSEMI)
          token = nextToken();
      }
    }
    else
    {
      Error(4);
    }
  }
/*
** 語法規則#5 <VarDeclaration>
*/
  void VarDeclaration()
  {
    if (strcmp(token->value, "VAR")==0)
    {
      token = nextToken();
      IdentifierList();
      if (token->sym == symSEMI)
        token = nextToken();
      else
      {
        Error(6);
        skip(statement, 23);
        if (token->sym == symSEMI)
          token = nextToken();
      }
    }
    else
    {
      Error(7);
    }
  }
/*
** 語法規則#6 <ProcDeclaration>
*/
  void ProcDeclaration()
  {
    while (strcmp(token->value, "PROCEDURE")==0)
    {
      token = nextToken();
      Identifier();
      if (token->sym == symSEMI)
      {
        token = nextToken();
        Block();
        if (token->sym == symSEMI)
          token = nextToken();
        else
        {
          Error(6);
          skip(statement, 23);
          if (token->sym == symSEMI)
            token = nextToken();
        }
      }
      else
      {
        Error(6);
      }
    }
  }
/*
** 語法規則#7 <Statement>
*/
  void Statement()
  {
    if (isResword(token->value) != -1)
    {
      if (strcmp(token->value,"IF")==0)
        IfStatement();
      else if (strcmp(token->value,"BEGIN")==0)
        CompoundStatement();
      else if (strcmp(token->value,"WHILE")==0)
        WhileStatement();
      else if (strcmp(token->value,"READ")==0)
        ReadStatement();
      else if (strcmp(token->value,"WRITE")==0)
        WriteStatement();
      else if (strcmp(token->value,"CALL")==0)
        CallStatement();
    }
    else if (token->sym == symIDENTIFIER)
      AssignmentStatement();
    else
      skip(statement, 23);
  }
/*
** 語法規則#8 <AssignmentStatement>
*/
  void AssignmentStatement()
  {
    Identifier();
    if (token->sym == symBECOMES)
    {
      token = nextToken();
      Expression();
    }
    else
    {
      Error(8);
      skip(statement, 23);
    }
  }
/*
** 語法規則#9 <CallStatement>
*/
  void CallStatement()
  {
    if (strcmp(token->value, "CALL")==0)
    {
      token = nextToken();
      Identifier();
    }
    else
    {
      Error(9);
      skip(statement, 23);
    }
  }
/*
** 語法規則#10 <CompoundStatement>
*/
  void CompoundStatement()
  {
    if (strcmp(token->value,"BEGIN")==0)
    {
      token = nextToken();
      Statement();
      while (token->sym == symSEMI)
      {
        token = nextToken();
        Statement();
      }
      if (strcmp(token->value,"END")==0)
        token = nextToken();
      else
      {
        Error(11);
        skip(statement, 23);
      }
    }
    else
    {
      Error(10);
      skip(statement, 23);
    }
  }
/*
** 語法規則#11 <IfStatement>
*/
  void IfStatement()
  {
    if (strcmp(token->value,"IF")==0)
    {
      token = nextToken();
      Condition();
      if (strcmp(token->value, "THEN")==0)
      {
        token = nextToken();
        Statement();
      }
      else
      {
        Error(13);
        skip(statement, 23);
      }
      if (strcmp(token->value, "ELSE")==0)
      {
        token = nextToken();
        Statement();
      }
    }
    else
    {
      Error(12);
      skip(statement, 23);
    }
  }
/*
** 語法規則#12 <WhileStatement>
*/
  void WhileStatement()
  {
    if (strcmp(token->value,"WHILE")==0)
    {
      token = nextToken();
      Condition();
      if (strcmp(token->value,"DO")==0)
      {
        token = nextToken();
        Statement();
      }
      else
      {
        Error(15);
        skip(statement, 23);
      }
    }
    else
    {
      Error(14);
      skip(statement, 23);
    }
  }
/*
** 語法規則#13 <ReadStatement>
*/
  void ReadStatement()
  {
    if (strcmp(token->value,"READ")==0)
    {
      token = nextToken();
      if (token->sym == symLPAREN)
      {
        token = nextToken();
        IdentifierList();
        if (token->sym == symRPAREN)
          token = nextToken();
        else
        {
          Error(18);
          skip(statement, 23);
        }
      }
      else
      {
        Error(17);
        skip(statement, 23);
      }
    }
    else
    {
      Error(16);
      skip(statement, 23);
    }
  }
/*
** 語法規則#14 <WriteStatement>
*/
  void WriteStatement()
  {
    if (strcmp(token->value, "WRITE")==0)
    {
      token = nextToken();
      if (token->sym == symLPAREN)
      {
        token = nextToken();
        IdentifierList();
        if (token->sym == symRPAREN)
          token = nextToken();
        else
        {
          Error(18);
          skip(statement, 23);
        }
      }
      else
      {
        Error(17);
        skip(statement, 23);
      }
    }
    else
    {
      Error(19);
      skip(statement, 23);
    }
  }
/*
** 語法規則#15 <IdentifierList>
*/
  void IdentifierList()
  {
    Identifier();
    while (token->sym == symCOMMA)
    {
      token = nextToken();
      Identifier();
    }
  }
/*
** 語法規則#16 <Condition>
*/
  void Condition()
  {
    Expression();
    if (token->sym == symLESS ||
        token->sym == symLEQ ||
        token->sym == symEQ ||
        token->sym == symNEQ ||
        token->sym == symGREATER ||
        token->sym == symGEQ)
    {
      token = nextToken();
      Expression();
    }
    else
    {
      skip(condition, 20);
    }
  }
/*
** 語法規則#17 <Expression>
*/
  void Expression()
  {
    if (token->sym == symPLUS ||
        token->sym == symMINUS)
    {
      token = nextToken();
    }
    Term();
    while (token->sym == symPLUS ||
           token->sym == symMINUS)
    {
      token = nextToken();
      Term();
    }
  }
/*
** 語法規則#18 <Term>
*/
  void Term()
  {
    Factor();
    while (token->sym == symMUL ||
           token->sym == symDIV ||
           strcmp(token->value, "MOD")==0||
           strcmp(token->value, "DIV")==0)
    {
      token = nextToken();
      Factor();
    }
  }
/*
** 語法規則#19 <Factor>
*/
  void Factor()
  {
    if (token->sym == symIDENTIFIER)
      Identifier();
    else if (token->sym == symNUMBER)
      Number();
    else if (token->sym == symLPAREN)
    {
      token = nextToken();
      Expression();
      if (token->sym == symRPAREN)
        token = nextToken();
      else
      {
        Error(18);
        skip(factor, 23);
      }
    }
  }
/*
** 識?O?r符記處理
*/
  void Identifier()
  {
    if (token->sym == symIDENTIFIER)
      token = nextToken();
    else
      Error(21);
  }
/*
** 數?r符記處理
*/
  void Number()
  {
    if (token->sym == symNUMBER)
      token = nextToken();
    else
      Error(22);
  }
/*
************************** ?D程式 **********************
*/
  int main(int argc, char *argv[])
  {
    FILE *f=fopen(argv[1], "r");
    scanner(f);
    followsyminit();
    token = nextToken();
    Program();
    fclose(f);
    printf("\n  Plone compile completed. "
      "\n  Error count : %d\n", errorCount);
    return 0;
  }
