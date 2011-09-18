#ifndef _CM_CONFIG_H_
#define _CM_CONFIG_H_
#include <sys/types.h>
#include <stdarg.h>
#include <netdb.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define STR_IP(str, n) sprintf((str), "%u.%u.%u.%u", (n)  & 0xff, ((n)>> 8) & 0xff, ((n)>>16) & 0xff, ((n)>>24) & 0xff)
typedef struct
{
	char nothing[10];
	unsigned int ipaddr;
	int rand_num1;
	char 	user[64];
	char	hostname[64];
}ip_struct_t;
#endif
