#include "cmutils.h"
#include "cmconfig.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>

extern cm_server_config_t g_server_config;

int	parse_options (int argc, char* argv[])
{
	memset (&g_server_config, 0, sizeof(g_server_config));
	g_server_config.port = DFT_PORT;
	g_server_config.daemon = DFT_DM;
	g_server_config.mode = 0;
	g_server_config.active = 0;
	g_server_config.threadnum = DFT_THREAD;
	g_server_config.devtype = DEV_TYPE_CMD;
	g_server_config.interval = 60;
	
	//reading configuration from stdin
	char *cvalue = NULL;
       	int c;
     	char ec;
	int error = 0;
       
	while ((c = getopt (argc, argv, "p:dsat:T:i:h")) != -1)
	{
         	switch (c)
           	{
			//listen port
           		case 'p':
				g_server_config.port = atoi (optarg);
             			if (g_server_config.port == 0)
				{
					printf ("error port value\n");
					error = 1;
				}
				//printf ("server port:%s\n", optarg);	
             			break;
			case 'i':
				g_server_config.interval = atoi (optarg);
				if (g_server_config.interval <= 0)
				{
					g_server_config.interval = 60;
				}
			//daemon flag
           		case 'd':
				g_server_config.daemon = 1;
             			//printf ("daemon on\n");
             			break;
			case 'a': //update task activly
				g_server_config.active = 1;
				break;
			//run with popen or sytem
           		case 's':
				g_server_config.mode = 1;
             			//cvalue = optarg;
				//printf ("mode:%s\n", cvalue);
             			break;
			//thread number
			case 't':
				//printf ("thread number:%s\n", optarg);
				cvalue = optarg;
				g_server_config.threadnum = atoi (cvalue);
				if (g_server_config.threadnum == 0)
				{
					printf ("error thread number\n");
					error = 1;
				}
				break;
			//server type, command server or stat server	
			case 'T':
				cvalue = optarg;

				if (strcmp(cvalue, "stat") == 0)
				{
					g_server_config.devtype = DEV_TYPE_STAT;
				}else if (strcmp(cvalue, "cmd") == 0)
				{
					g_server_config.devtype = DEV_TYPE_CMD;
				}else
				{
					printf ("error value for -T [stat|cmd]\n");
					error = 1;
				}	
				break;
			case '?':
				ec = optopt;
				error = 1;
				printf ("invalid option '%c'\n", ec);
				break;
			case ':':
				error  = 1;
				printf ("option '%c' need a parameter\n", optopt);
				break;	
			case 'h':
				printf ("usage:%s [-d] [-s] [-p port]  [-t threadnumber] [-T stat|cmd] [-i syslog interval]\n", argv[0]);
				printf ("\t-d\t--daemon\t run as a daemon\n");
				printf ("\t-s\t--system\t run command with system(use open as a default method)\n");
				printf ("\t-a\t--active\t update task activly\n");
				error = 1;
				break;
			default:
				printf ("unknow opt:%s\n", optarg);
				error = 1;
				break;
		}

	}
		
	if (error > 0)
	{
		return 0;
	}
	if (!g_server_config.daemon)
	{
		//printf ("cmserver config: port = %u, thread number = %d, run with system:%s\n",
		//	g_server_config.port, g_server_config.threadnum, g_server_config.mode ? "yes":"no");
	}
	g_server_config.auth_on = atoi (get_conf("cmtools/general/auth", "1"));
	return 1;
}
