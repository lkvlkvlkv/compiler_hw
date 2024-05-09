/********************** tree.c ***************************/
#include <stdio.h>
#include <stdlib.h>
struct wordTag
{
  char word[36];
  struct wordTag *linkL, *linkR;                /*����, �k��*/
};
struct wordTag node, *root, *p, *q;       /*�𵲺c�ܼ�, ����*/
void createRoot(char word[])
{
  root=malloc(sizeof(struct wordTag));   /*�إ߸`�Ip*/
  root->linkL=NULL;                               /*����ť�*/
  root->linkR=NULL;                               /*�k��ť�*/
  strcpy(root->word, word);
}
struct wordTag *insertNode(char word[])
{
  p=root;
  q=root;
  while (p!=NULL)
  {
    q = p;
    if (strcmp(word, p->word) < 0)
      p = p->linkL;                                   /*����*/
    else if (strcmp(word, p->word) > 0)
      p = p->linkR;                                   /*���k*/
    else
      return p;
  }
  p = malloc(sizeof(struct wordTag));       /*�إ߸`�Ip*/
  p->linkL=NULL;                                  /*����ť�*/
  p->linkR=NULL;                                  /*�k��ť�*/
  strcpy(p->word, word);                              /*�����*/
  if (strcmp(word, q->word) < 0)
    q->linkL = p;                                     /*����*/
  else
    q->linkR = p;                                     /*�k��*/
}
void inOrder(struct wordTag *p)
{
  if (p != NULL)
  {
    inOrder(p->linkL);
    printf("%p   %p  %p  %s\n", p,p->linkL,p->linkR,p->word);
    inOrder(p->linkR);
  }
}
int main()
{
  char key[21];                                     /*�����*/
  root=NULL;                              /*�ڸ`�I����, �ť�*/
  while (1)                           /*���O�Ŧr��ɭ��а���*/
  {
    printf("keyin key : ");
    gets(key);                           /*��J�����key�r��*/
    if (strlen(key)==0) break;       /*�Ŧr����Xwhile()�j��*/
    if (root==NULL)                                   /*�ž�*/
      createRoot(key);                          /*�إ߮ڸ`�I*/
    else                                            /*�D�ž�*/
      insertNode(key);                /*��Mkey,�䤣��ɫإ�*/
  }
  printf("\nIn order sequence, root=0x%p\n", root); /*�ګ���*/
  printf("  p   linkL  linkR key\n");         /*����,�k��,��*/
  inOrder(root);                                  /*���ǦL�X*/
  return 0;
}
