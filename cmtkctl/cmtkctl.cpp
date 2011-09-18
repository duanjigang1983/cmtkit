#include "cmtkctl.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
ip_struct_t ip_list[500];
int  ip_num = 0;
int main (int argc, char* argv[])
{
	int op = 0;
	char szfile[128] = {0};
	char szuser[64] = {0};
	char szhost[64] = {0};
	char * flag = 0;
	srand (time(0));
	if ((argc != 4) && (argc != 3))
	{
		printf ("usage: %s <add/del user@host>/<list> file\n", argv[0]);
		return 1;
	}
	if (strcmp(argv[1], "add") && strcmp(argv[1], "del")&&(strcmp(argv[1], "list")))
	{
		printf ("usage: %s <add/del user@host>/<list> file\n", argv[0]);
		return 1;
	}

	if ((strcmp(argv[1], "add") == 0)||(strcmp(argv[1], "del")==0))
	{
		if (argc != 4)
		{
			printf ("usage: %s <add/del user@host>/<list> file\n", argv[0]);
			return 1;
		}
		flag = strchr (argv[2], '@');
		if (!flag)
		{
			printf ("invalid parameter '%s' when add or del an authorization <user@host>\n", argv[2]);
			return 1;
		}
		//getting user
		strncpy (szuser, argv[2], flag - argv[2]);
		//getting host
		strcpy (szhost, flag + 1);
	}
	if (strcmp(argv[1], "list") == 0)
	{
		ip_struct_t node;
		sprintf (szfile, "%s", argv[2]);
		FILE * fp = fopen (szfile, "r");
		if (!fp)
		{
		
			printf ("open file [%s] failed when try to list all  address\n", szfile);
			return 1;
		}	

		while(fread (&node, sizeof(ip_struct_t), 1, fp) > 0)
		{
			char szip[20] = {0};
			STR_IP(szip, node.ipaddr);
			printf ("%s@%s(%s)\n",  node.user, node.hostname, szip);
			memset (&node, 0, sizeof(ip_struct_t));
		}
		fclose (fp);
		return 1;
	}
	
	struct hostent * host  = 0;
	host = gethostbyname(szhost);
	
	if (!host)
	{
		printf ("gethostbyname[%s] failed\n", szhost);
		return 1;
	}
        			

	if (strcmp(argv[1], "add") == 0)
	{
		op = 1;
	}else
	{
		op = -1;
	}
	
	ip_struct_t g_node;
	g_node.ipaddr = inet_addr(inet_ntoa(*((struct in_addr *)host->h_addr)));
	strcpy(g_node.user, szuser);
	
	sprintf (szfile, "%s", argv[3]);
	

	//add an addr
	if (op == 1)
	{
		FILE * fp = fopen (szfile, "r");
		ip_struct_t node;
		if (!fp)
		{
			//printf ("try to open file %s for reading failed\n", szfile);
			//return 1;
		}else
		{
			while(fread (&node, sizeof(ip_struct_t), 1, fp) > 0)
			{
				if ((strcmp(szuser, node.user) == 0) && (g_node.ipaddr == node.ipaddr))
				{
					printf ("%s@%s<%u> already exist\n", szuser, szhost, node.ipaddr);
					fclose(fp);
					return 1;
				}
			}
			fclose (fp);
		}
		
		g_node.rand_num1 = rand()%12345;
		strcpy(g_node.hostname, szhost);
		
		for (unsigned int i = 0; i < sizeof(g_node.nothing); i++)
		{
			g_node.nothing[i] = rand()%256;
		}

		fp = fopen (szfile, "a+");
		if (!fp)
		{
			printf ("try to write file [%s] failed\n", szfile);
			return 1;
		}
		fwrite (&g_node, sizeof(ip_struct_t), 1, fp);
		fclose (fp);
		
		
	}else //delete an addr
	{
		ip_struct_t node;
		char szfile1[256] = {0};
		sprintf (szfile1, "%s.back", szfile);
		FILE * fp = fopen (szfile, "r");
		FILE * fp1 = fopen (szfile1, "w");
		if (!fp || !fp1)
		{
		
			printf ("open file [%s] failed when try to delete an ip address\n", szfile);
			return 1;
		}	
		while(fread (&node, sizeof(ip_struct_t), 1, fp) > 0)
		{
			if ((strcmp(node.user, g_node.user) == 0)&& (node.ipaddr == node.ipaddr))
			{
				continue;
			}
			fwrite (&node, sizeof(ip_struct_t), 1, fp1);
		}
		fclose (fp);
		fclose (fp1);
		if (unlink(szfile))
		{
			printf ("unlink file %s failed\n", szfile);
			return 1;
		}
		if (rename(szfile1, szfile))
		{
			printf ("rename %s %s failed\n", szfile1, szfile);
		}
	}	
	return 1;
} 
