/******************** token.c ******************/
#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[])
{
  char buf[256], *token;
  printf("�гv����J�r��A������Ctrl-Z�䵲��\n");
  while (fgets(buf,sizeof(buf),stdin)!=NULL)
  {
    token=strtok(buf, " \t\n");
    while (token != NULL)
    {
      puts(token);
      token=strtok(NULL, " \t\n");
    }
  }
  return 0;
}
