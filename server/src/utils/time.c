#include "../../../include/myhead.h"

//获取当前时间
void get_current_time(char *tm)
{
	struct tm *now;
	long t;
	t = time(NULL);
	now = localtime(&t);

//	sprintf(tm, "%d/%02d/%02d %02d:%02d", now->tm_year+1900, now->tm_mon + 1,now->tm_mday, now->tm_hour, now->tm_min);
	sprintf(tm, "%02d:%02d:%02d", now->tm_hour, now->tm_min, now->tm_sec);
}

//获取当前时间日期
void get_current_data_time(char *tm)
{
	struct tm *now;
	long t;
	t = time(NULL);
	now = localtime(&t);

	sprintf(tm, "%d/%02d/%02d %02d:%02d", now->tm_year+1900, now->tm_mon + 1,now->tm_mday, now->tm_hour, now->tm_min);
}
