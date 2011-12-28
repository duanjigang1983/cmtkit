#include "cmconfig.h"
#include "os_independent.h"
#include "IniHelper.h"

extern int	opterr;		/* if error message should be printed */
extern int optind;		/* index into parent argv vector */
extern int optopt;			/* character checked for validity */
extern int optreset;		/* reset getopt */
extern char	*optarg;

static void print_usage(int argc, char* argv[]);
host_func_t g_host_func = 0;

//init network for windows
static int init_network (void);
//send command to host 
extern int send_cmd2host (cmdev_t* dev, const cm_client_config_t * config);
//send file to host
extern int send_file2host (cmdev_t* dev, const cm_client_config_t * config);
//fetch file from host
extern int fetch_file_from_host (cmdev_t *dev, const cm_client_config_t * config);

extern int getopt(int nargc, char* const * nargv, const char* ostr);

Ice::CommunicatorPtr g_ic;

int init_ice(int argc, char* argv[])
{
	Ice::PropertiesPtr props = Ice::createProperties(argc, argv);
	props->setProperty("Ice.MessageSizeMax", "10240");
	Ice::InitializationData id;
	id.properties = props;
	g_ic = Ice::initialize(id);
	return 1;
}

int init_parameters(int argc, char* argv[], cm_client_config_t * client_config)
{
	int c;
	char ec; 
	int error = 0;
	//	uid_t uid = 0;
	//char * szRet = 0;
	//char *login = 0;
	struct hostent *host  = 0;
	int len = 0;
	
	if (argc < 2)
	{
		print_usage(argc, argv);
		return 0;
	}
	
	if(init_network() <= 0)
	{
		return -1;
	}
	
	memset (client_config, 0, sizeof(cm_client_config_t));
	client_config->remote_port = DFT_PORT;
	client_config->remote_addr = 0;
//	client_config->local_port = 0;
	//client_config->local_addr = 0; 
	client_config->mode = 0;
	client_config->time_out = 2;
	client_config->color = 0;
	client_config->silent = 0;
	client_config->run_mode = RUN_REAL;
	
	while ((c = getopt (argc, argv, "p:h:f:c:n:u:d:t:bsxvl:r:")) != -1) 
	{   
		switch (c) 
		{   
			//remote port
		case 'p':
			client_config->remote_port = atoi (optarg);
			if (client_config->remote_port == 0)
			{
				printf ("error remote port value\n");
				error = 1;
			}
			break;
		case 's':
			client_config->silent = 1;
			break;	
		case 'x':
			client_config->color = 1;
			break;
		case 'h':
			
			host = gethostbyname(optarg);
			if (!host)
			{
				printf ("gethostbyname[%s] failed\n", optarg);
				error = 1;
				break;
			}
			client_config->remote_addr = inet_addr(inet_ntoa(*((struct in_addr *)host->h_addr)));
			len = strlen (optarg);                        
			if (len > HOST_NAME) len = HOST_NAME;
			strncpy (client_config->remote_host_name, optarg, len);	
			if (client_config->remote_addr == 0)
			{
				printf ("error remote addr value\n");
				error = 1;
			}
			break;
		case 't':
			client_config->time_out = atoi(optarg);
			if (client_config->time_out == 0)
			{
				printf ("error time out value\n");
				error = 1;
			}
			break;
		case 'n':
			client_config->tasknum = atoi(optarg);
			if (client_config->tasknum == 0)
			{
				printf ("error task number value\n");
				error = 1;
			}
			break;
		case 'f':
			//printf ("<%s>\n", optarg);
			if (strlen (optarg) < CONF_FILE)
			{
				//client_config->conf = (char*)malloc(128);
				memset (client_config->conf, 0, 128);
				strcpy (client_config->conf,  optarg);
				//strcpy (client_config->conf, optarg);
			}else
			
				{
				printf ("config file name too long(%u)max(%d)\n", 
					(unsigned int)strlen(optarg), 
					CONF_FILE);
				error = 1;
			}
			break; 
		case 'c':
			if (client_config->mode)
			{
				printf ("do not set duplicate task with '-c', already set\n");
				error = 1;
				break;
			}
			client_config->mode = MODE_CMD;
			strcpy(client_config->command, optarg);
			break;
		case 'u':
			
			if (client_config->mode)
			{
				printf ("do not set duplicate task with '-u', already set\n");
				error = 1;
				break;
			}
			
			strcpy(client_config->file, optarg);
			client_config->mode = MODE_UPLOAD_FILE;
			break;
		case 'd':
			strcpy(client_config->dst_file, optarg);
			break;
		case 'b':
			client_config->run_mode = RUN_BACK;
		case '?':
			ec = optopt;
			error = 1;
			printf ("invalid option '%c'\n", ec);
			break;
		case ':':
			error  = 1;
			printf ("option '%c' need a parameter\n", optopt);
			break;
			
		case 'v':
			show_version (argc, argv);
			return 0;	
		case 'r': 
			
			if (client_config->mode)
			{
				printf ("do not set duplicate task with '-r', already set\n");
				error = 1;
				break;
			}
			client_config->mode = MODE_DOWNLOAD_FILE;
			strcpy(client_config->remote_file, optarg);
			break;
			
		case 'l': 
			strcpy(client_config->local_file, optarg);
			break;
		default:
			printf ("unknow opt:%s\n", optarg);
			error = 1;
			break;
           }
	}
	
	//check options
	if ((0 == client_config->remote_addr)&&(strlen(client_config->conf)==0))
	{
		printf ("invalid remote host or ip config file\n");
		error = 1;
		goto ret;
	}else
		if (strlen(client_config->conf) > 0)
		{
			
#ifdef __LINUX__
			if (access(client_config->conf, R_OK))
#else
				if (access(client_config->conf, _A_RDONLY))
#endif
				{
					printf ("invalid ip config file '%s'\n", client_config->conf);
					error = 1;
					goto ret;
				}
				
		}
		
		
		if (0 == client_config->remote_port)
		{
			char szconf[256] = {0};
			char szbuf[256] = {0};
			sprintf (szbuf, "%s", argv[0]);
			#ifdef _WIN32	
			char * p = NULL;
			p = szbuf +  strlen(szbuf);
			while(*p != '\\')
			{
				*p = '\0';
				p--;
			}
			*p = '\0';
			sprintf (szconf, "%s\\conf\cmtk_config.ini", szbuf);
			#else
			sprintf (szconf, "%s", CMTK_CONF);
			#endif	
			CIniHelper helper(CMTK_CONF);
			if(helper.ErrorOccurence())
			{
				printf ("invalid remote port\n");
				error = 1;
				goto ret;
			}
			
			client_config->remote_port = helper.ReadInt("general", "port", DFT_PORT);
			
			if (client_config->remote_port == 0)
			{
				printf ("invalid remote port\n");
				error = 1;
				goto ret;
			}
		}
		
		//check for command data	
		if ((strlen(client_config->command) == 0)&&(MODE_CMD == client_config->mode))
		{
			printf ("invalid command to run\n");
			error = 1;
			goto ret;
			
		}
		if ((client_config->remote_addr > 0) && (strlen(client_config->conf) > 0 ))
		{
			printf ("your will specify the remote host(s) with either '-h' or '-f', but not both of them\n");
			error = 1;
			goto ret;
		}
		//check mode
		//if ((g_client_config.mode != MODE_CMD) && (g_client_config.mode != MODE_UPLOAD_FILE))
		if ((client_config->mode != MODE_CMD) && 
			(client_config->mode != MODE_UPLOAD_FILE)&&
			(client_config->mode != MODE_DOWNLOAD_FILE))
		{
			printf ("please specify the command type with '-c command' or '-u update files' or '-r remote file'\n");
			error = 1;
			goto ret;
		}
		
		switch (client_config->mode)
		{
		case MODE_CMD://run command	
			g_host_func = send_cmd2host;
			break;
		case MODE_UPLOAD_FILE://upload file
			
			//check source file
#ifdef _WIN32
			if (access(client_config->file, _A_RDONLY))
#else
				if (access(client_config->file, R_OK))
#endif
				{
					printf ("source file '%s' of upload does not exist\n", client_config->file);
					error = 1;
					goto ret;
				}
				//check dest path
				if (strlen(client_config->dst_file) == 0)
				{
					printf ("please specify the dest file with '-d file_path' when trying to upload\n");
					error = 1;
					goto ret;
				}
				g_host_func = send_file2host;
				break;
		case MODE_DOWNLOAD_FILE:
		default://download file
			g_host_func = fetch_file_from_host;
			if (!strlen(client_config->remote_file) || !strlen(client_config->local_file))
			{
				printf ("please specify local file and remote file when fetching a file\n");
				error = 1;
				goto ret;
			}
			break;
		}
ret:
		
		if (error)
			print_usage(argc, argv);
		//else
		//	 printf("remote host:%u\n", client_config->remote_addr);
		return 1 - error;
}
static opt_help_t opt_list [] =
{
	{ "-p", "remote port to connect", 1},
	{ "-h", "remotee host to connect", 1},
	{ "-f", "config file", 1},
	{ "-c", "command to run", 1},
	{ "-n", "task number", 1},
	{ "-u", "file path to update", 1},
	{ "-d", "dest file path to save", 1},
	{ "-s", "run command silent", 1},
	{ "-t", "time out value ", 1},
	{ "-v", "show cmtk version list", 1},
	//added by duanjigang@2011-11-01 for fetching file --start
	{ "-r", "remote file to download", 1},	
	{ "-l", "local file  to save", 1}	
	//added by duanjigang@2011-11-01 for fetching file --finish
};
void print_usage(int argc, char* argv[])
{
	
	//added by duanjigang@2011-10-29 --start
	printf ("usage:%s\n", argv[0]);
	for (unsigned int i = 0; i < sizeof(opt_list)/sizeof(opt_help_t); i++)
	{
		opt_help_t * oh = opt_list + i;
		if (oh->show)
		{
			printf ("\t[%5s\t%-25s]\n", oh->opt, oh->desc);
		}
	}
	//added by duanjigang@2011-10-29 --end
	
}
int init_network (void)
{
#ifdef _WIN32
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData)) 
	{
		printf("Error: WSAStartup failed.\n");
		return -11;
	}
#endif
	return 1;
	
}
