/********************* lexer.c *******************/
#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
struct symbolTag *token;
int main(int argc, char *argv[])
{
  FILE *f=fopen(argv[1], "r");
  scanner(f);
  while ((token=nextToken())!=NULL)
  {
    if (token->sym==symerror || argc==3)
    {
      if (token->sym==symerror)
        printf("***���~***");

    }
          printf("\t�ŰO�s��=%d\t�C��=%d\t�渹=%d"
        "\t�ŰO�W��=\"%s\"\n", token->sym,
        token->left, token->right, token->value);
  }
  fclose(f);
  printf("\n  �y�J���R�����C");
  return 0;
}
