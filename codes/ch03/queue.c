/********************* queue.c *******************/
#include <stdio.h>
#include <stdlib.h>
struct wordTag
{
  char word[36];
  struct wordTag *next;
};
struct wordTag *head=NULL, *tail;
int isEmpty()
{
  if (head==NULL)
    return 1;
  else
    return 0;
}
void queuein(char *word)
{
  struct wordTag *p;
  p = malloc(sizeof(struct wordTag));
  strcpy(p->word, word);
  if (isEmpty())
  {
    head = p;
    tail = p;
    p->next = NULL;
  }
  else
  {
    p->next = tail->next;
    tail->next = p;
  }
}
struct wordTag * queueout()
{
  struct wordTag *p;
  if (isEmpty())
    return NULL;
  p = head;
  head = head->next;
  return p;
}
int main()
{
  char word[36];
  struct wordTag *wp;
  printf("�гv����J�r���J��C�ACtrl-Z����\n");
  while (fgets(word,sizeof(word),stdin)!=NULL)
  {
    queuein(word);
  }
  printf("�v���q��C��X���X\n");
  while ((wp=queueout())!=NULL)
  {
    printf("%s", wp->word);
  }
  return 0;
}
