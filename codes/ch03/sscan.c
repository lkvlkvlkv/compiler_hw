/******************** sscan.c ****************/
#include <stdio.h>
int main(void)
{
  char str[]="12 3.4";
  int i;
  double d;
  sscanf(str, "%d %lf", &i, &d);
  printf("���i=%d ����T�B�I��d=%lf\n", i,d);
  return 0;
}
