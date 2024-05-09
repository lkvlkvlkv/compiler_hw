/************************ cal.h **********************/
enum enumTag  { enumNUM, enumVAR, enumOPR };
struct numTag { int num; };                    /*�`��*/
struct varTag { int var; };                    /*�ܼ�*/
struct oprTag                            /*�B��l�`�I*/
{
  int oper;                              /*�B��l�s��*/
  struct nodeTag *op[2];          /*�B�⤸�`�I(�̦h2)*/
};
struct nodeTag                                 /*�`�I*/
{
  enum enumTag type;               /*�`�I���A��ƽs��*/
  union
  {
    struct numTag num;                         /*�`��*/
    struct varTag var;                         /*�ܼ�*/
    struct oprTag opr;                       /*�B��l*/
  };
};
