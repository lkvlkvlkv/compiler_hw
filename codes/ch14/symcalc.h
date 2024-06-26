/***************** symcalc.h *****************/
struct nodeTag
{
  int  sym;                 /*符記VAR整數編號*/
  char name[36];                /*符記VAR名稱*/
  union
  {
    double   var;             /*符記VAR語意值*/
  } value;
  struct nodeTag *next;          /*下一個節點*/
};
char *nodeToString(struct nodeTag *p);
char *nodestackToString();
struct nodeTag *putsym(char const *, int);
struct nodeTag *getsym(char const *);
