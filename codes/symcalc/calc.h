/************************ calc.h ************************/
struct nodeTag
{
  int  sym;                            /*�ŰOVAR��ƽs��*/
  char name[36];                              /*�ŰO�W��*/
  union
  {
    double   var;                          /*�ŰOVAR����*/
  } value;
  struct nodeTag *next;    /*�s���ܤU�@��VAR�ŰO�`�I����*/
};
struct nodeTag *nodestackTop;                 /*���|����*/
struct nodeTag *putsym(char const *, int);        /*�|�J*/
struct nodeTag *getsym(char const *);       /*�q���|���o*/
char * nodeToString(struct nodeTag *p);      /*p�`�I���e*/
char * nodestackToString();               /*�`�I���|���e*/
