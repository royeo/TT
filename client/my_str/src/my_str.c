/*==================================
*  Copyright (C) 2015 All rights reserved.
*   Filename：      my_str.c
*   Author：        Mr.Lu
*   Version：       1.0
*   Date：          2015/07/16
*   Description：	字符串操作
*   Function List： strlen() strcmp()
*					strcpy()
* 
==========================================*/

#include "../../include/myhead.h"

int strlen(const char *s)
{
    int count = 0;

    while(*s != '\0')
    {
        count++;
		s++;
    }

    return count;
}


char *strcpy(char *dest, const char *src)
{
    char *temp;
   
    temp = dest;
    
    while(*src != '\0')
    {
        *temp = *src;
		temp++;
		src++;
    }

    *temp = '\0';

    return dest;
}

int strcmp(const char *s1, const char *s2)
{
    if(strlen(s1) > strlen(s2))
    {
        return 1;
    }
    
    if(strlen(s1) < strlen(s2))
    {
        return -1;
    }  

    while(*s1 != '\0')
    {
        if(*s1 > *s2)
		{
	    	return 1;
		}

		if(*s1 < *s2)
		{
	    	return -1;
		}

		s1++;
		s2++;
    }

    return 0;
}
