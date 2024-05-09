/********************** idobjqueue.c *******************/
#include <stdio.h>
#include <stdlib.h>
struct idobjTag
{
  char name[36];
  int sym;
  int attr;
  int level;
  struct idobjTag *next;
} node, *head=NULL, *tail=NULL, *p;
int main()
{
  p = malloc(sizeof(node));   //�إ�"speed"�`�I
  strcpy(p->name, "speed");
  p->sym = 2;
  p->attr = 31;
  p->level = 1;
  p->next = NULL;
  head = p;                   //�@���Y�Χ��`�I
  tail = p;
  p = malloc(sizeof(node));   //�إ�"time"�`�I
  strcpy(p->name, "time");
  p->sym = 2;
  p->attr = 31;
  p->level = 1;
  p->next = NULL;
  tail->next = p;             //�[�J����
  tail = p;
  p = malloc(sizeof(node));   //�إ�"dist"�`�I
  strcpy(p->name, "dist");
  p->sym = 2;
  p->attr = 31;
  p->level = 0;
  p->next = NULL;
  tail->next = p;             //�[�J����
  tail = p;
  printf("head=0x%p  tail=0x%p\n", head, tail);
  p = head;
  while (p!=NULL)             //�C�L�Ҧ��`�I
  {
    printf(
      "name=\"%s\" sym=%d attr=%d level=%d next=0x%p\n",
      p->name, p->sym, p->attr, p->level, p->next);
    p = p->next;
  }
  return 0;
}
