/******************** scandays.c *******************/
#include <stdio.h>
int main()
{
  float days;
  int months;
  double daysPerMonth;
  printf("�п�J�@�~�Ѽ�: ");
  scanf("%f", &days);
  printf("�п�J���: ");
  scanf("%d", &months);
  daysPerMonth = days / months;
  printf("�@�~�Ѽ�=%6.2f ���=%d �C�륭���Ѽ�=%lf\n",
         days, months, daysPerMonth);
  return 0;
}
