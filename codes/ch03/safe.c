/***************** safe.c ****************/
#include <stdio.h>
int main()
{
  char ok[3]="OK";
  char s[5];
  printf("�п�J�@�Ӧr��s: ");
  fgets(s, sizeof(s), stdin);
  printf("ok=\"%s\"  s=\"%s\"\n", ok,s);
  return 0;
}
