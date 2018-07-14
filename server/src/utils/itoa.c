#include "../../../include/myhead.h"

// 整型转字符串
char *my_itoa(int num, char *str)
{
	int i;
	int len;
	char ch;
	char *temp = str;

	while(num > 0)
	{
	    *temp = num % 10 + '0';
		num /= 10;
		temp++;
	}
	
	len = strlen(str);
	temp = str;

	for(i = 0; i < len / 2; i++)
	{
	    ch = *(temp + i);
		*(temp + i) = *(temp + len - 1 - i);
		*(temp + len - 1 - i) = ch;
	}

	temp[len] = '\0';

	return str;
}
