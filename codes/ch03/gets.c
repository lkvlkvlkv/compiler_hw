/****************** gets.c **************/
#include <stdio.h>
int main(void)
{
  char buffer[100];
  printf("�п�J�@�r��: ");
  fgets(buffer, sizeof(buffer), stdin);
  printf("�z���J���r��= ");
  puts(buffer);
  return 0;
}
