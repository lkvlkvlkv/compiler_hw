/*********************** yeardays.c ********************/
#include <stdio.h>
int main()
{
  float days = 365.25;
  int months = 12;
  double daysPerMonth = days / months;
  char title[13] = "�C�륭���Ѽ�";
  printf("%s\n�@�~�Ѽ�=%6.2f ���=%d �C�륭���Ѽ�=%lf\n",
    title, days, months, daysPerMonth);
  return 0;
}
