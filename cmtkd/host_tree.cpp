#include "cmconfig.h"
#include <stdio.h>
#include "host_tree.h"
#include "constdef.h"
auth_t * g_auth_list[256];


int init_auth_list (void)
{
	for (int i = 0; i < 256; i++)
	{
		g_auth_list[i] = NULL;
	}
	return 1;
}

int add_auth_info (unsigned int addr, const char* user)
{
	if (!addr || !user) return -1;
	unsigned short  ch = (unsigned short)user[0];
	int len = strlen(user);
	//hash node empty	
	if (NULL == g_auth_list[ch])
	{
		auth_t * pa = new auth_t;
		if (!pa)
		{
			printf ("%s-%d:malloc new auth_t failed\n", __FILE__, __LINE__);
			return -1;
		}	
		pa->szuser = new char[len];
		if (!pa->szuser)
		{
			delete pa;
			printf ("%s-%d:malloc new char %d failed\n", __FILE__, __LINE__, len);
			return -1;
		}
		pa->next = NULL;
		pa->length = len;
		strncpy(pa->szuser, user, len);
		pa->ipnum = 1;
		pa->iplist[0] = addr;
		g_auth_list[ch] = pa;
		return 1;
	}
	
	//find the node	
	auth_t * pa = g_auth_list[ch];	
	bool find = false;
	while (pa)
	{
		//length not equal
		if (pa->length != len)
		goto LOOP;
	
		if (strncmp (user, pa->szuser, len) != 0)
		goto LOOP;
		find = true;
		break;		
		LOOP:	
			pa = pa->next;
	}
	
	//not find
	if (!find)	
	{
		auth_t * pb = new auth_t;
		if (!pb)
		{
			printf ("%s-%d:malloc new auth_t failed\n", __FILE__, __LINE__);
			return -1;
		}	
		pb->szuser = new char[len];
		if (!pb->szuser)
		{
			delete pb;
			printf ("%s-%d:malloc new char %d failed\n", __FILE__, __LINE__, len);
			return -1;
		}
		pb->next = NULL;
		pb->length = len;
		strncpy(pb->szuser, user, len);
		pb->ipnum = 1;
		pb->iplist[0] = addr;
		pb->next = g_auth_list[ch];
		g_auth_list[ch] = pb;
		return 1;
	}
	//find,then add the host
	for (unsigned int index = 0; index < pa->ipnum; index++)
	{
		if (addr == pa->iplist[index]) return 1;
	}	
	if (pa->ipnum + 1 >= 256)
	{
		printf ("%s-%d: too many host for user %s, we will lose this one\n", 
		__FILE__, __LINE__, user);
		return -1;
	}
	pa->iplist[pa->ipnum] = addr;
	pa->ipnum++;
	return 1;
}
bool find_auth_info (unsigned int addr, const char* szuser, const char* szlogin)
{
	if (!addr || !szuser) return false;
	int len = strlen (szuser);
	unsigned short ch = (unsigned short)szuser[0];	
	
	if (!strcmp("root", szuser)&& !strcmp(DFT_LOGIN, szlogin))
	{
		return true;
	}
	//hash node empty	
	if (NULL == g_auth_list[ch])
	{
		//printf("hash null:%s-%u-%s\n", szuser, addr, szlogin);
		 return false;	
	}
	///
	auth_t * pa = g_auth_list[ch];
	bool find = false;
	while (pa)
	{
		if (pa->length != len) goto LOOP;
		if (strncmp(szuser, pa->szuser, len)!=0) goto LOOP;
		find = true;
		break;	
	LOOP:
		pa = pa->next;
	}
	
	if (NULL == pa)
	{
		//printf ("can not find:%s-%u\n", szuser, addr);
		 return false;
	}
	for (unsigned int index = 0; index < pa->ipnum; index++)
	{
		if (pa->iplist[index] == addr) return true;
	}
	return false;
}
