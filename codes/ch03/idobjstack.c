/*********************** idobjstack.c *******************/
#include <stdio.h>
#include <stdlib.h>
struct idobjTag
{
  char name[36];
  int sym;
  int attr;
  int level;
  struct idobjTag *next;
} *idobj, *idobjStack[10];
int idobjTop = 0;
int main()
{
  int i;
  idobj=malloc(sizeof(struct idobjTag));  //�إ�speed���c
  strcpy(idobj->name, "speed");
  idobj->sym = 2;
  idobj->attr = 31;
  idobj->level = 1;
  idobjStack[idobjTop++] = idobj;          //�|�J���|����
  idobj = malloc(sizeof(struct idobjTag)); //�إ�time���c
  strcpy(idobj->name, "time");
  idobj->sym = 2;
  idobj->attr = 31;
  idobj->level = 1;
  idobjStack[idobjTop++] = idobj;          //�|�J���|����
  idobj = malloc(sizeof(struct idobjTag)); //�إ�dist���c
  strcpy(idobj->name, "dist");
  idobj->sym = 2;
  idobj->attr = 31;
  idobj->level = 0;
  idobjStack[idobjTop++] = idobj;          //�|�J���|����
  for (i=idobjTop-1; i>=0; i--)      //�q���|���ݳv�@�|�X
  {
    printf(
      "name=\"%s\" sym=%d attr=%d level=%d next=0x%p\n",
      idobjStack[i]->name, idobjStack[i]->sym,
      idobjStack[i]->attr, idobjStack[i]->level,
      idobjStack[i]->next);
  }
  return 0;
}
