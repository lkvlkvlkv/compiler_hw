/********************** mfcalc.h ************************/
typedef double (*funtype) (double);    /*�w�q�禡���A*/
struct nodeTag
{
  int  sym;                         /*�ŰOVAR��ƽs��*/
  char name[36];                           /*�ŰO�W��*/
  union
  {
    double   var;                       /*�ŰOVAR����*/
    funtype funptr;                     /*�ŰOFUN����*/
  } value;
  struct nodeTag *next; /*�s���ܤU�@��VAR�ŰO�`�I����*/
};
struct nodeTag *nodestackTop;              /*���|����*/
struct nodeTag *putsym(char const *, int);     /*�[�J*/
struct nodeTag *getsym(char const *);          /*���o*/
char * nodeToString(struct nodeTag *p);    /*��ܸ`�I*/
char * nodestackToString();                /*��ܰ��|*/
