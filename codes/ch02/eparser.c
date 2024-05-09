/********************** eparser.c **********************/
#include <stdio.h>
char nextChar=' ';
char line[512];
int cp=0, errorCount=0;
void error()
{
   errorCount++;
}
void advance()
{
   nextChar = line[cp++];
}
void F()
{
   if (nextChar=='x' || nextChar=='y' || nextChar=='z')
      advance();
   else
      error();
}
void T()
{
   F();
   while (nextChar=='*' || nextChar=='/')
   {
      advance();
      F();
   }
}
void E()
{
   T();
   while (nextChar=='+' || nextChar=='-')
   {
      advance();
      T();
   }
   if (nextChar!='\n') error();
}
int main(int argc, char *argv[])
{
   printf("�п�J�@�Ӧr��(Ctrl-Z����): ");
   while (fgets(line,sizeof(line),stdin)!=NULL)
   {
      errorCount = 0;
      cp = 0;
      advance();
      E();
      if (errorCount>0)
         printf("  *ERROR* \n");
      else
         printf("  *OK* \n");
      printf("�п�J�@�Ӧr��(Ctrl-Z����): ");
   }
   getch();
   return 0;
}
