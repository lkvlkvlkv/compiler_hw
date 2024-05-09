/*********************** idnodestack.c *******************/
#include <stdio.h>
#include <stdlib.h>
struct idnodeTag
{
  char name[36];
  int sym;
  int attr;
  int level;
  struct idnodeTag *next;
} *idnode, *idnodeStack;
int main()
{
  int i;
  idnode=malloc(sizeof(struct idnodeTag));  //�إ�speed���c
  strcpy(idnode->name, "speed");
  idnode->sym = 2;
  idnode->attr = 31;
  idnode->level = 1;
  idnode->next = NULL;
  idnodeStack = idnode;                      //�|�J���|����
  idnode = malloc(sizeof(struct idnodeTag)); //�إ�time���c
  strcpy(idnode->name, "time");
  idnode->sym = 2;
  idnode->attr = 31;
  idnode->level = 1;
  idnode->next = idnodeStack;
  idnodeStack = idnode;
  idnode = malloc(sizeof(struct idnodeTag)); //�إ�dist���c
  strcpy(idnode->name, "dist");
  idnode->sym = 2;
  idnode->attr = 31;
  idnode->level = 0;
  idnode->next = idnodeStack;
  idnodeStack = idnode;                      //�|�J���|����
  while (idnodeStack != NULL)          //�q���|���ݳv�@�|�X
  {
    printf(
      "name=\"%s\" sym=%d attr=%d level=%d next=0x%p\n",
      idnodeStack->name, idnodeStack->sym,
      idnodeStack->attr, idnodeStack->level,
      idnodeStack->next);
    idnodeStack = idnodeStack->next;
  }
  return 0;
}
