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
struct nodeTag *putsym(char const *, int);    /*�[�J�ŰO*/
struct nodeTag *getsym(char const *);         /*���o�ŰO*/
