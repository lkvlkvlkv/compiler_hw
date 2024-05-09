/***************** symcalc.h *****************/
struct nodeTag
{
  int  sym;                 /*�ŰOVAR��ƽs��*/
  char name[36];                /*�ŰOVAR�W��*/
  union
  {
    double   var;             /*�ŰOVAR�y�N��*/
  } value;
  struct nodeTag *next;          /*�U�@�Ӹ`�I*/
};
char *nodeToString(struct nodeTag *p);
char *nodestackToString();
struct nodeTag *putsym(char const *, int);
struct nodeTag *getsym(char const *);
