#include "utils.h"
#include "constdef.h"
#include "cmconfig.h"
#include "IniHelper.h"
#include "cmconfig.h"
#include "ColorDefine.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>

extern int	GetNetip(unsigned int *ip);
long int g_command_id = 0;
cm_client_config_t g_client_config;
dev_list_t g_dev_list;
CommandMessage g_file_msg; 
thread_pool_t g_thread_pool;
Ice::CommunicatorPtr g_ic; 
host_func_t g_host_func = 0;

//send command to host 
int send_cmd2host (cmdev_t* dev);
//send file to host
int send_file2host (cmdev_t* dev);
//fetch file from host
int fetch_file_from_host (cmdev_t *dev);

int load_file2msg(char * szfile);
const char * error_msg(int errcode);
static void print_usage(int argc, char* argv[]);
int init_parameters(int argc, char* argv[])
{
	int c;
        char ec; 
        int error = 0;
	uid_t uid = 0;
	char * szRet = 0;
	char *login = 0;

	if (argc < 2)
	{
		print_usage(argc, argv);
		return 0;
	}
	
	memset (&g_client_config, 0, sizeof(g_client_config));
	g_client_config.remote_port = 0;
	g_client_config.remote_addr = 0;
	g_client_config.local_port = 0;
	g_client_config.local_addr = 0; 
	g_client_config.mode = 0;
	g_client_config.time_out = 2;
	g_client_config.color = 0;
	g_client_config.auth = 0;
	g_client_config.silent = 0;//added @2011-03-28
	//added by djg@2011-03-09--start
	g_client_config.run_mode = RUN_REAL;
	//added by djg@2011-03-09--end
	struct hostent *host  = 0;
	int len = 0;

	while ((c = getopt (argc, argv, "p:h:P:H:f:c:n:u:d:t:bsaxvl:r:")) != -1) 
       {   
                switch (c) 
                {   
                        //remote port
                        case 'p':
                                g_client_config.remote_port = atoi (optarg);
                                if (g_client_config.remote_port == 0)
                                {
                                        printf ("error remote port value\n");
                                        error = 1;
                                }
                                //printf ("server port:%s\n", optarg);  
                                break;
			///added @2011-03-28 --start
			//run command in silent
			case 's':
				g_client_config.silent = 1;
				break;
			///added @2011-03-28 --end
			
			//--added @2011-05-05 --start for get task actively
			case 'a':
				g_client_config.auth  = 1;//auth flag
				break;
			//--added @2011-05-05 --finish for get task actively
			//remote host

		
			//--added by jigang.djg@2011-06-08 --start
			case 'x':
				g_client_config.color = 1;
				break;
			//--added by jigang.djg@2011-06-08 --end
			
                        case 'h':

				host = gethostbyname(optarg);
				if (!host)
				{
					printf ("gethostbyname[%s] failed\n", optarg);
					error = 1;
					break;
				}
				g_client_config.remote_addr = inet_addr(inet_ntoa(*((struct in_addr *)host->h_addr)));
        			len = strlen (optarg);                        
				if (len > HOST_NAME) len = HOST_NAME;
				strncpy (g_client_config.remote_host_name, optarg, len);
				//g_client_config.remote_addr = inet_addr(optarg);
				
                                if (g_client_config.remote_addr == 0)
                                {
                                        printf ("error remote addr value\n");
                                        error = 1;
                                }
                                break;
			//////--added by djg@2011-05-18 -- for time out start
			case 't':
                                g_client_config.time_out = atoi(optarg);
				if (g_client_config.time_out == 0)
				{
					printf ("error time out value\n");
					error = 1;
				}
				break;
			//////--added by djg@2011-05-18 -- for time out finish
			//task number
			case 'n':
                                g_client_config.tasknum = atoi(optarg);
                                if (g_client_config.tasknum == 0)
                                {
                                        printf ("error task number value\n");
                                        error = 1;
                                }
                                break;
			//local host
                        case 'H':
                                g_client_config.local_addr = inet_addr(optarg);
                                if (g_client_config.local_addr == 0)
                                {
                                        printf ("error local addr value\n");
                                        error = 1;
                                }
                                break;

			//local port
				
				case 'P':
                                g_client_config.local_port = atoi(optarg);
                                if (g_client_config.local_port == 0)
                                {
                                        printf ("error local port value\n");
                                        error = 1;
                                }
                                break;

			//config file of ip list
                        case 'f':
                                if (strlen (optarg) < CONF_FILE)
				{
					strcpy (g_client_config.conf, optarg);
				}else
				{
					printf ("config file name too long(%u)max(%d)\n", 
					(unsigned int)strlen(optarg), 
					CONF_FILE);
					error = 1;
				}
                                break;
			//command 
			case 'c':
				if (g_client_config.mode)
				{
					printf ("do not set duplicate task with '-c', already set\n");
					error = 1;
					break;
				}
				g_client_config.mode = MODE_CMD;
				strcpy(g_client_config.command, optarg);
				//memcpy(g_client_config.command, optarg, strlen(optarg));
                                break;
			//update file name
			case 'u':

				if (g_client_config.mode)
				{
					printf ("do not set duplicate task with '-u', already set\n");
					error = 1;
					break;
				}

				strcpy(g_client_config.file, optarg);
				g_client_config.mode = MODE_UPLOAD_FILE;
				break;

			//destination directory
			case 'd':
				strcpy(g_client_config.dst_file, optarg);
				break;
			//--added by djg@2011-03-09 --start
			//run mode real-time or background
			case 'b':
			g_client_config.run_mode = RUN_BACK;//
			//--added by djg@2011-03-09 --end
			case '?':
                                ec = optopt;
                                error = 1;
                                printf ("invalid option '%c'\n", ec);
                                break;
                        case ':':
                                error  = 1;
                                printf ("option '%c' need a parameter\n", optopt);
                                break;
			//added version by duanjigang@2011-10-29 13:50 start
			case 'v':
				show_version (argc, argv);
				return 0;	
			//added version by duanjigang@2011-10-29 13:50 finish

			//added file fetch by duanjigang1983@2011-11-01 --start
			case 'r': //retmote file to fetch

				if (g_client_config.mode)
				{
					printf ("do not set duplicate task with '-r', already set\n");
					error = 1;
					break;
				}
				g_client_config.mode = MODE_DOWNLOAD_FILE;
				strcpy(g_client_config.remote_file, optarg);
				break;

			case 'l': //local file to save
				strcpy(g_client_config.local_file, optarg);
				break;
			//added file fetch by duanjigang1983@2011-11-01 -finish
                        default:
                                printf ("unknow opt:%s\n", optarg);
                                error = 1;
                                break;
                }
	}

	//check options
	if ((0 == g_client_config.remote_addr)&&(strlen(g_client_config.conf)==0))
	{
		printf ("invalid remote host or ip config file\n");
		error = 1;
		goto ret;
	}else
	if (strlen(g_client_config.conf) > 0)
	{
		if (access(g_client_config.conf, R_OK))
		{
			printf ("invalid ip config file '%s'\n", g_client_config.conf);
			error = 1;
			goto ret;
		}
	}
	
	
	if (0 == g_client_config.remote_port)
	{

		CIniHelper helper(CMTK_CONF);
		if(helper.ErrorOccurence())
		{
			printf ("invalid remote port\n");
			error = 1;
			goto ret;
		}

		g_client_config.remote_port = helper.ReadInt("general", "port", 43699);
		
		if (g_client_config.remote_port == 0)
		{
			printf ("invalid remote port\n");
			error = 1;
			goto ret;
		}
			}

	if (!g_client_config.auth)
	{
		CIniHelper helper(CMTK_CONF);
		if(helper.ErrorOccurence())
		{
			;
		}else
		{
			g_client_config.auth = helper.ReadInt("general", "auth", 0);
		}
	}

	if (!g_client_config.forbid_root)
	{
		CIniHelper helper(CMTK_CONF);
		if(helper.ErrorOccurence())
		{
			;
		}else
		{
			g_client_config.forbidroot = helper.ReadInt("general", "forbidroot", 0);
		}
	}
	//chech for local stat server addr
	if ((0 == g_client_config.local_addr)&&(MODE_CMD == g_client_config.mode)&&(RUN_BACK == g_client_config.run_mode))
	{
		printf ("invalid local addr\n");
		error = 1;
		goto ret;
	}

	//check for local stat server port
      if ((0 == g_client_config.local_port)&&(MODE_CMD == g_client_config.mode)&&(RUN_BACK == g_client_config.run_mode))
	{
		printf ("invalid local port\n");
		error = 1;
		goto ret;
	}
	
	//check for command data	
	if ((strlen(g_client_config.command) == 0)&&(MODE_CMD == g_client_config.mode))
	{
		printf ("invalid command to run\n");
		error = 1;
		goto ret;

	}
	if ((g_client_config.remote_addr > 0) && (strlen(g_client_config.conf) > 0 ))
	{
		printf ("your will specify the remote host(s) with either '-h' or '-f', but not both of them\n");
		error = 1;
		goto ret;
	}
	//check mode
	//if ((g_client_config.mode != MODE_CMD) && (g_client_config.mode != MODE_UPLOAD_FILE))
	if ((g_client_config.mode != MODE_CMD) && 
		(g_client_config.mode != MODE_UPLOAD_FILE)&&
		(g_client_config.mode != MODE_DOWNLOAD_FILE))
	{
		printf ("please specify the command type with '-c command' or '-u update files' or '-r remote file'\n");
		error = 1;
		goto ret;
	}

	switch (g_client_config.mode)
	{
		case MODE_CMD://run command	
			g_host_func = send_cmd2host;
			break;
		case MODE_UPLOAD_FILE://upload file
			if (access(g_client_config.file, R_OK))
			{
				printf ("invalid update file '%s'\n", g_client_config.file);
				error = 1;
				goto ret;
			}
			g_host_func = send_file2host;
			break;
		case MODE_DOWNLOAD_FILE:
		default://download file
			g_host_func = fetch_file_from_host;
			if (!strlen(g_client_config.remote_file) || !strlen(g_client_config.local_file))
			{
				printf ("please specify local file and remote file when fetching a file\n");
				error = 1;
				goto ret;
			}
			break;
	}
	//check file
	//added by duanjigang@2011-08-01 for username and login infor --start
	uid = getuid();	
	szRet = get_username(uid);
	if (szRet == NULL)
	{
		printf ("%s-%d:get username of uid %u failed\n", __FILE__, __LINE__, uid);
		error = 1;
		goto ret;
	}
	sprintf (g_client_config.username, "%s", szRet);
	login = getlogin();
	if (login)
	sprintf (g_client_config.login, "%s", login);
	else
	sprintf (g_client_config.login, "%s", DFT_LOGIN);
	if (0)
	printf ("user:%s-login:%s\n", g_client_config.username, g_client_config.login);
	

	//g_client_config.forbid_root  = atoi(get_conf ("cmtools/general/forbidroot", "0"));

	if (g_client_config.forbid_root && strcmp(g_client_config.login, DFT_LOGIN))
	{	
		if (strncmp(g_client_config.username, "root", 4) == 0)
		{
			printf ("%s is not allowed to run with account root\n", argv[0]);
			error	= 1;
			goto ret;
		}

		if (strncmp(g_client_config.login, "root", 4) == 0)
		{
			printf ("%s is not allowed to run with login name root\n", argv[0]);
			error	= 1;
			goto ret;
		}
	}
	//added by duanjigang@2011-08-01 for username and login infor --end
ret:
	if (error)
	print_usage(argc, argv);
	return 1 - error;
}

int init_devlist(void)
{
	char szline[128] = {0};
	FILE * fp = 0;

	g_dev_list.dev_num = 0; 
	//just single host 
	if (g_client_config.remote_addr > 0)
	{
		g_dev_list.dev_list[0].dev_ip = g_client_config.remote_addr;
		strncpy (g_dev_list.dev_list[0].dev_name, g_client_config.remote_host_name,
		strlen(g_client_config.remote_host_name));
		g_dev_list.dev_num = 1;
		return 1;	
	}

	//ip list is stored in a txt file, try to open
	fp = fopen (g_client_config.conf, "r");
	if (!fp)
	{
		printf ("open file '%s' failed\n", g_client_config.conf);
 		return -1;
	}

	//read each line to parse a host addr and store it
	while (fgets(szline, 128, fp))
	{
		int len = 0;
		char szip[80] = {0}, szindex[20] = {0};
		if (strlen(szline) <= strlen("\n")) continue;
		if (szline[0] == '#') continue;
		
		len = strlen (szline);
		
		if (szline[len - 1] == '\n')
		{
			szline[len-1] = '\0';
		}
		if (sscanf(szline, "%s\t%s", szip, szindex)== 2)
		{
			g_dev_list.dev_list[g_dev_list.dev_num].dev_index = strtoul (szindex, 0, 10);
			//printf ("%p============%lu===%s=\n", &g_dev_list.dev_list[g_dev_list.dev_num], 
			//g_dev_list.dev_list[g_dev_list.dev_num].dev_index, szindex);
		}
		//we ought to convert an domain name to ip addr --added by cme@2011-06-04 --start
		struct hostent *host = gethostbyname(szip);
		//g_dev_list.dev_list[g_dev_list.dev_num].dev_ip = inet_addr (szip);
		if (!host) continue;
                //printf("Host (%s) IP: %s\n", szip, inet_ntoa(*((struct in_addr *)host->h_addr)));
		g_dev_list.dev_list[g_dev_list.dev_num].dev_ip = inet_addr(inet_ntoa(*((struct in_addr *)host->h_addr)));
		int slen = strlen (szip);
		if (slen > HOST_NAME) slen = HOST_NAME;
		strncpy (g_dev_list.dev_list[g_dev_list.dev_num].dev_name, szip, slen);
		
		//get ip addr	
		if (!g_dev_list.dev_list[g_dev_list.dev_num].dev_ip)
		{
			 continue;
		}
		//add device number
		g_dev_list.dev_num++;
		//check overflow	
		if (g_dev_list.dev_num >= MAX_DEV)
		{
			printf ("too many host to load [>%d]\n", MAX_DEV);
			break;
		}
		memset (szline, 0, 128);
	}
	fclose (fp);
	//return number of devices
	return g_dev_list.dev_num ;
}

int init_tasks(void)
{
	unsigned int index_task = 0;	

	//load file to transfer
		
	g_file_msg.filedata.clear();	
	if (MODE_UPLOAD_FILE == g_client_config.mode)
	{
		if (load_file2msg(g_client_config.file) <= 0)
		{
			printf ("load file '%s' failed\n", g_client_config.file);
			return 0;
		}		
	}

	// task too much
	if (g_client_config.tasknum > MAX_TASK_NUM)
	{
		g_client_config.tasknum = MAX_TASK_NUM;
	}
	
	//in case of no -n input
	if (0 == g_client_config.tasknum)
	{
		g_client_config.tasknum = TASKNUM;
	}
	
	//init thread pool
	for (index_task = 0; index_task < g_client_config.tasknum; index_task++)
	{
		g_thread_pool.thread[index_task].dev_num = 0;;
		g_thread_pool.thread[index_task].over = false;
			
	}

	//in case of thread wast
	if (g_dev_list.dev_num < g_client_config.tasknum)
	{
		g_client_config.tasknum = g_dev_list.dev_num;
	}

	g_thread_pool.th_num =  g_client_config.tasknum;
	
	int i = 0;
	index_task = 0;

	for (i = 0; i < g_dev_list.dev_num; i++)
	{
		int dev_index = g_thread_pool.thread[index_task].dev_num;
		g_thread_pool.thread[index_task].dev_list[dev_index] = &g_dev_list.dev_list[i];		
		//printf ("<%p=%lu--%p=%lu>\n", &g_dev_list.dev_list[i], 
		//g_dev_list.dev_list[i].dev_index, g_thread_pool.thread[index_task].dev_list[dev_index],
		//g_thread_pool.thread[index_task].dev_list[dev_index]->dev_index);
		g_thread_pool.thread[index_task].dev_num++;
		index_task = (index_task +  1)%g_client_config.tasknum;
	}
	return 1;
}

int load_file2msg(char * szfile)
{
	if (!szfile) return 0;
	struct stat file_stat;
	char szbuf[8192] = {0};
	unsigned  int nRead = 0;
	char base[128] = {0};
	g_file_msg.filedata.clear();	
	
	if (stat (szfile, &file_stat))
	{
		printf ("fstat(%s) failed\n", szfile);
		return 0;
	}	

	int nFd = open (szfile, O_RDONLY);
	if (nFd == -1) return -1;
	unsigned int size = 0;
	while ((nRead = read (nFd, szbuf, 8190)) > 0)
	{
		size += nRead;
		for (unsigned int i = 0; i < nRead; i++)
		{
			g_file_msg.filedata.push_back(szbuf[i]);
		}
		memset (szbuf, 0, nRead);
	}
	close (nFd);
	g_file_msg.head.filesize 	= size;
	//---added by djg@2011-03-11 --start
	g_file_msg.head.stmode		= file_stat.st_mode;
	g_file_msg.head.uid		= file_stat.st_uid;
	g_file_msg.head.gid		= file_stat.st_gid;
	//---added by djg@2011-03-11 --end
	sprintf (base, "%s", basename(szfile));	
	g_file_msg.head.file 		= string (base);
	g_file_msg.head.dstfile 		= string(g_client_config.dst_file);
	g_file_msg.head.msgtype 	= MSG_TYPE_UP_FILE;

	//--added by duanjigang@2011-08-01 for login and user auth --start
	g_file_msg.head.username = string (g_client_config.username);	
	g_file_msg.head.login = string (g_client_config.login);	
	//--added by duanjigang@2011-08-01 for login and user auth --end


	//--added by djg@2011-03-30 -for file information --start
	char info_file[512] = {0};
	sprintf (info_file, "%s.info", szfile);	
	if (access (info_file, R_OK))
	{
		//printf ("accee file '%s' failed\n", info_file);
		goto ret;
	}
	
	nFd = open (info_file, O_RDONLY);
	if (nFd == -1)
	{
		printf ("open file '%s' for read failed\n", info_file);
		goto ret;
	}
	size = 0;
	g_file_msg.fileinfo.clear();
	while ((nRead = read (nFd, szbuf, 8190)) > 0)
	{
		size += nRead;
		for (unsigned int i = 0; i < nRead; i++)
		{
			g_file_msg.fileinfo.push_back(szbuf[i]);
		}
		memset (szbuf, 0, nRead);
	}
	close (nFd);

	//--added by djg@2011-03-30 -for file information --end
 	ret:
	return 1;
}
void print_thread_list(void)
{
	int i = 0;
	for (i = 0; i < g_thread_pool.th_num; i++)
	{
		printf ("task_%d has %d host(s):\n", i, g_thread_pool.thread[i].dev_num);
		int j = 0;
		for (j = 0; j < g_thread_pool.thread[i].dev_num; j++)
		{
			char szip[20] = {0};
			STR_IP(szip, g_thread_pool.thread[i].dev_list[j]->dev_ip);
			printf ("%s\n", szip);
		}
		printf ("\n");
	}	
}

int init_ice(int argc, char* argv[])
{
	Ice::PropertiesPtr props = Ice::createProperties(argc, argv);
	props->setProperty("Ice.MessageSizeMax", "10240");
	Ice::InitializationData id;
	id.properties = props;
	g_ic = Ice::initialize(id);
	//g_ic = Ice::initialize (argc, argv);
	return 1;
}

int start_tasks()
{
	int i = 0;
	int j = 0; 

	g_command_id = time (0);	
	g_client_config.ipnum = GetNetip(g_client_config.iplist);
	
	if (g_client_config.ipnum <= 0)
	{
		printf ("get ip list of local device failed\n");
		return -1;
	}
	
	srand(time(0));
	//init status of each node
	for (i = 0; i < g_thread_pool.th_num; i++)
	{
		g_thread_pool.thread[i].over = false;

		for (j = 0; j < g_thread_pool.thread[i].dev_num; j++)
		{
			g_thread_pool.thread[i].dev_list[j]->over = false;
		}
	}	
	//then, try to start all threads

	for (i = 0; i < g_thread_pool.th_num; i++)
	{
		g_thread_pool.thread[i].start_time = time (0);
		g_thread_pool.thread[i].index = i;

		if (pthread_create (&g_thread_pool.thread[i].tid, 0, work_thread_func, 
			(void*)&g_thread_pool.thread[i]))
		{
			printf ("creat thread_%d failed\n", i);
			return -1;	
		}
	}

	return 1;
}
void *work_thread_func(void *arg)
{
	
	pthread_detach(pthread_self());
	thread_t * pt = (thread_t*)arg;
	
	int i = 0;
	if (!pt)
	{
		goto ret;
	}
	//printf ("thread_%d running now\n", pt->index);
	
	
	for (i = 0; i < pt->dev_num; i++)
	{
		char szip[20] = {0};
		STR_IP (szip, pt->dev_list[i]->dev_ip);
		#if 0
		printf ("%p thread_%d sending command to host %s index %lu\n", 
		pt->dev_list[i],
		pt->index, szip, pt->dev_list[i]->dev_index);
		#endif
		pt->dev_list[i]->start_time = time(0);
		if (g_host_func)
		{
			pt->dev_list[i]->nret = g_host_func(pt->dev_list[i]);
		}else
		{
			printf ("invalid g_host_func\n");
		}
		pt->dev_list[i]->finish_time = time(0);
		pt->dev_list[i]->over = true;
		//usleep(rand()%10000);
	}
ret:
	pt->finish_time = time(0);
	pt->over = true;
	//printf ("thread_%d running over..\n", pt->index);
	pthread_exit(0);
}

bool	run_over(void)
{
	int i = 0;
	
	for (i = 0; i < g_thread_pool.th_num; i++)
	{
		if (!g_thread_pool.thread[i].over)	
		{
			return false;
		}
	}
	return true;
}
int send_cmd2host (cmdev_t* dev)
{
	
	int nRet = RET_SUCCES;
	char szip[20] = {0};
	bool connected = false;
	STR_IP (szip, dev->dev_ip);
	CommandMessage msg; // request message
	dev->retmsg.clear();

	for (unsigned int i = 0; i < g_client_config.ipnum; i++)
	{
		msg.head.srchost.push_back (g_client_config.iplist[i].ip);
	}

	try
	{
		
		char szproxy [PROXY_LEN] = {0}; // what we use to identify a connection
		sprintf (szproxy, "cmtkp:tcp -h %s -p %u -t %u", 
			szip, 
			g_client_config.remote_port, g_client_config.time_out * 1000);
		Ice::ObjectPrx  base = g_ic->stringToProxy(szproxy);
		cmtkp::CmdMessageHandlerPrx  client = 
		CmdMessageHandlerPrx::checkedCast(base);
                if(!client)
                {
			//throw "invalud proxy:ComPlusBasePrx::checkedCast(base)";
        		nRet = RET_CONNECT_FAILED;
			goto ret;
		}
		connected 		=	true;
		msg.head.msgtype 	= 	MSG_TYPE_CMD;
		msg.head.hostaddr 	= 	g_client_config.local_addr;
		msg.head.hostport 	= 	g_client_config.local_port;
		msg.head.timestamp	=	time(0);
		msg.head.taskid		= 	getpid();	
		msg.head.commandid	=	g_command_id;
		msg.head.localaddr	=	dev->dev_ip;
		msg.head.runmode	=	g_client_config.run_mode;//added by djg@2011-03-09
		msg.cmd 		= 	g_client_config.command; // get file name
		msg.head.username	=	g_client_config.username;
		msg.head.login		=	g_client_config.login;
		//printf ("command is [%s]\n", g_client_config.command);
		
		// try to request the file
		CommandMessage ret_msg = client->ProcessMessage(msg);
		
		
		if (RUN_REAL == g_client_config.run_mode)	
		{

			//if (ret_msg.head.nret > 0)
			if (1)
			{
				for (unsigned int i = 0; i < ret_msg.result.size(); i++)
				{

					dev->retmsg.push_back(ret_msg.result[i]);	
				}
			}
		
		}
	}
	catch(const Ice::Exception & ex)
        {
                //cerr << ex << endl;
		nRet = RET_CONNECT_FAILED;
		if (connected)
		{
			nRet = RET_RUNCMD_FAILED;
		}
		goto ret;
        }catch(const char* msg)
        {
                //cerr << msg << endl;
		nRet = RET_CONNECT_FAILED;
		if (connected)
		{
			nRet = RET_RUNCMD_FAILED;
		}

		goto ret;
	}
ret:
	return nRet;
}

int send_file2host (cmdev_t * dev)
{ 

	int nRet = RET_SUCCES;
	bool connected = false;
	char szip[20] = {0};
	if (!dev)
	{
		 printf ("invalid parameter[%s:%d]\n",__FILE__, __LINE__);
		 exit(0);
	}
	STR_IP (szip, dev->dev_ip);


	for (unsigned int i = 0; i < g_client_config.ipnum; i++)
	{
		g_file_msg.head.srchost.push_back (g_client_config.iplist[i].ip);
	}


	try
	{
		char szproxy [PROXY_LEN] = {0}; // what we use to identify a connection
		sprintf (szproxy, "cmtkp:tcp -h %s -p %u -t %u", 
		szip, g_client_config.remote_port,
		g_client_config.time_out * 1000);
		Ice::ObjectPrx  base = g_ic->stringToProxy(szproxy);
		cmtkp::CmdMessageHandlerPrx  client = 
		CmdMessageHandlerPrx::checkedCast(base);
                if(!client)
                {
			//throw "invalud proxy:ComPlusBasePrx::checkedCast(base)";
        		nRet = RET_CONNECT_FAILED;
			goto ret;
		}
		connected = true;
		g_file_msg.head.hostaddr 	= 	g_client_config.local_addr;
		g_file_msg.head.hostport 	= 	g_client_config.local_port;
		g_file_msg.head.timestamp	=	time(0);
		g_file_msg.head.taskid		= 	getpid();	
		g_file_msg.head.commandid	=	g_command_id;
		g_file_msg.head.localaddr	=	dev->dev_ip;
		char index[20] = {0};
		sprintf (index, "%lu", dev->dev_index);
		g_file_msg.head.index = string (index);		
		// try to request the file
		CommandMessage ret_msg = client->ProcessMessage(g_file_msg);
		//printf ("sending file message, index = %s\n", g_file_msg.head.index.c_str());
		nRet = ret_msg.head.nret;
	}catch(const Ice::Exception & ex)
        {
                //cerr << ex << endl;
		nRet = RET_CONNECT_FAILED;
		if (connected)
		{
			nRet = RET_RUNCMD_FAILED;
		}
		goto ret;
        }catch(const char* msg)
        {
                //cerr << msg << endl;
		nRet = RET_CONNECT_FAILED;
		if (connected)
		{
			nRet = RET_RUNCMD_FAILED;
		}

		goto ret;
	}
ret:
	return nRet;
}

void	show_result_front(void);
void	show_result_silent(void);
void	show_results(void)
{
	if (g_client_config.silent)
	{
		show_result_silent();
	}else
	{
		show_result_front();
	}
}
void	show_result_front(void)
{
	int i = 0;
	if (g_client_config.mode & MODE_CMD)
	{
		printf ("run command [%s]:\n", g_client_config.command);
	}

	if (g_client_config.mode & MODE_UPLOAD_FILE)
	{
		printf ("upload file [%s] to destination [%s]:\n", g_client_config.file, g_client_config.dst_file);
	}

	if (g_client_config.mode & MODE_DOWNLOAD_FILE)
	{
		printf ("download from [%s] to local [%s]:\n", g_client_config.remote_file, g_client_config.local_file);
	}


	for (i = 0; i < g_dev_list.dev_num; i++)
	{
		char szip[20] = {0};
		cmdev_t *p = g_dev_list.dev_list + i;
		STR_IP (szip, p->dev_ip);	
		
		if (g_client_config.color)
			printf ("("GREEN"%s"NONE"):["YELLOW"%s"NONE"]\n", p->dev_name,  szip);
		else
		printf ("(%s):[%s]\n", p->dev_name,  szip);

		if (g_client_config.mode & MODE_CMD)
		{
			int j = 0;
			int size = p->retmsg.size();
			for (j = 0; j < size; j++)
			{
				printf ("%s", p->retmsg[j].c_str());
			}
			if (!size && (p->nret <= 0)) printf ("%s\n", error_msg(p->nret));
		}else if (g_client_config.mode & MODE_UPLOAD_FILE)
		{
			if (p->nret > 0) printf ("success\n");
			else printf ("failed[%d]\n", p->nret);
		}else
		{
			if (p->nret > 0) printf ("success\n");
			else printf ("failed[%d]\n", p->nret);
		}
		printf ("\n");
	}

}

void	show_result_silent(void)
{
	int i = 0;
	mkdir (RET_DIR, 0777);
	char szpath[64] = {0};
	unsigned int pid = getpid();
	sprintf (szpath, "%s/%u", RET_DIR, pid);
	mkdir (szpath, 0777);

	for (i = 0; i < g_dev_list.dev_num; i++)
	{
		char szip[20] = {0};
		char szfile[100] = {0};
		cmdev_t *p = g_dev_list.dev_list + i;
		STR_IP (szip, p->dev_ip);	
		sprintf (szfile, "%s/%u_%u.txt", szpath, p->dev_ip, i);
		printf ("[result_%u]\n", i);
		if (g_client_config.color)
		{
			printf ("index = "GREEN"%u"NONE"\n", i+1);
			printf ("dev_ip = "GREEN"%s"NONE"\n", szip);
			printf ("dev_name = "GREEN"%s"NONE"\n", p->dev_name);
			printf ("save_path = "GREEN"%s"NONE"\n", szfile);
			printf ("return =  "GREEN"%d"NONE"\n", p->nret);
			printf ("start = "GREEN"%lu"NONE"\n", p->start_time);
			printf ("finish = "GREEN"%lu"NONE"\n\n", p->finish_time);
		}else
		{
			printf ("index = %u\n", i+1);
			printf ("dev_ip = %s\n", szip);
			printf ("dev_name = %s\n", p->dev_name);
			printf ("save_path = %s\n", szfile);
			printf ("return =  %d\n", p->nret);
			printf ("start = %lu\n", p->start_time);
			printf ("finish = %lu\n\n", p->finish_time);
		}
		if ((g_client_config.mode & MODE_CMD) || (g_client_config.mode & MODE_DOWNLOAD_FILE))
		{
			int j = 0;
			FILE * fp = fopen (szfile, "w");
			if (fp)
			{
				int size = p->retmsg.size();
				
				for (j = 0; j < size; j++)
				{
					fprintf (fp, "%s", p->retmsg[j].c_str());
				}
				if (!size && (p->nret <= 0)) fprintf (fp, "%s\n", error_msg(p->nret));
				fclose (fp);
				chmod(szfile, 0666);
			}
		}else
		{
			FILE * fp = fopen (szfile, "w");
			if (fp)
			{
				fprintf (fp, "%s", error_msg(p->nret));
				fclose (fp);
			}
			//printf ("%s\n", (p->nret > 0) ? "success": "failed");
		}
	}
}

const char * error_msg(int errcode)
{
	switch (errcode)
	{
		case RET_SUCCES: return "success";
		case RET_CONNECT_FAILED: return "connect host failed";
		case RET_RUNCMD_FAILED: return "run command failed";
		default: return "unkown error";
	}
}
char *	get_username(uid_t uid)
{
	static char szret[USER_NAME] = {0};
	char szline[512] = {0};
	FILE*  fp = fopen ("/etc/passwd", "r");
	bool find = false;
	if (!fp)
	{
		return NULL;
	}
	
	while (fgets(szline, sizeof(szline), fp))
	{
		char * p1 = 0, * p2=0, *p3 = 0;
		char szuid[20] = {0};
		uid_t tuid = 0;
		p1 = strchr (szline, ':');
		if (!p1)continue;
		p2 = strchr (p1+1, ':');
		if (!p2) continue;
		p3 = strchr (p2 + 1, ':');
		if (!p3) continue;	
		//copy data
		strncpy (szuid, p2+1, p3-p2-1);
		tuid = atoi (szuid);
		if (uid == tuid)
		{
			memset (szret, 0, sizeof(szret));
			strncpy (szret, szline, p1 - szline);
			find = true;
			break;	
		}
	}
	fclose(fp);
	if (!find) return NULL;
	return szret;
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
	{ "-a", "authorize user on", 1},
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
//added by duanjigang1983@2011-10-29 13:50
int 	show_version (int argc, char* argv[])
{
		printf ("%s version list:\n", argv[0]);
		printf ("\t v1.0 2011-10-29:create version by duanjigang1983\n");
		printf ("\t v1.1 2011-11-02:adding file fetching interface by duanjigang1983\n");
		printf ("\t v1.2 2011-11-26:simplify source code by duanjigang1983\n");
		return 0;;
}
//added by duanjigang1983@2011-11-01 --start
int fetch_file_from_host (cmdev_t *dev)
{

	int nRet = RET_SUCCES;
	bool connected = false;
	char szip[20] = {0};
	CommandMessage g_fetch_msg, ret_msg; 
	//printf ("fetch_file_from_host......................\n");
	if (!dev)
	{
		 printf ("invalid parameter[%s:%d]\n",__FILE__, __LINE__);
		 exit(0);
	}
	STR_IP (szip, dev->dev_ip);

	g_fetch_msg.head.msgtype = MSG_TYPE_DOWN_FILE;
	for (unsigned int i = 0; i < g_client_config.ipnum; i++)
	{
		g_fetch_msg.head.srchost.push_back (g_client_config.iplist[i].ip);
	}

	try
	{
		char szproxy [PROXY_LEN] = {0}; // what we use to identify a connection
		
		g_fetch_msg.filedata.clear();	
		sprintf (szproxy, "cmtkp:tcp -h %s -p %u -t %u", 
		szip, g_client_config.remote_port,
		g_client_config.time_out * 1000);
		//printf ("===%s\n", szproxy);
		Ice::ObjectPrx  base = g_ic->stringToProxy(szproxy);
		cmtkp::CmdMessageHandlerPrx  client = 
		CmdMessageHandlerPrx::checkedCast(base);
                if(!client)
                {
			//throw "invalud proxy:ComPlusBasePrx::checkedCast(base)";
        		nRet = RET_CONNECT_FAILED;
			goto ret;
		}
		connected = true;
		g_fetch_msg.head.hostaddr 	= 	g_client_config.local_addr;
		g_fetch_msg.head.hostport 	= 	g_client_config.local_port;
		//added by duanjigang1983@2011-11-01 --start
		g_fetch_msg.head.remotefile	= 	g_client_config.remote_file;
		g_fetch_msg.head.localfile	=	g_client_config.local_file;		
		//added by duanjigang1983@2011-11-01 --finish
		g_fetch_msg.head.timestamp	=	time(0);
		g_fetch_msg.head.taskid		= 	getpid();	
		g_fetch_msg.head.commandid	=	g_command_id;
		g_fetch_msg.head.localaddr	=	dev->dev_ip;

		g_fetch_msg.head.username = string (g_client_config.username);	
		g_fetch_msg.head.login = string (g_client_config.login);	

		char index[20] = {0};
		sprintf (index, "%lu", dev->dev_index);
		g_fetch_msg.head.index = string (index);		
		// try to request the file
		ret_msg = client->ProcessMessage(g_fetch_msg);
		//printf ("sending file message, index = %s\n", g_file_msg.head.index.c_str());
		nRet = ret_msg.head.nret;
	}catch(const Ice::Exception & ex)
        {
                cerr << ex << endl;
		nRet = RET_CONNECT_FAILED;
		if (connected)
		{
			nRet = RET_RUNCMD_FAILED;
		}
		goto ret;
        }catch(const char* msg)
        {
                cerr << msg << endl;
		nRet = RET_CONNECT_FAILED;
		if (connected)
		{
			nRet = RET_RUNCMD_FAILED;
		}

		goto ret;
	}
ret:
	if (ret_msg.head.nret > 0)
	{
		char szdir [256] = {0};
		char szfile [256] = {0};
		int nFd = 0;
		sprintf (szdir, "%s", g_fetch_msg.head.localfile.c_str());
		sprintf (szfile, "%s", dirname (szdir));
		mkdir_rec (szfile);
		memset (szfile, 0, 256);
		sprintf (szfile, "%s.%s", g_fetch_msg.head.localfile.c_str(), szip);
		//printf ("open file '%s'-%ld-%d\n", szfile, ret_msg.filedata.size(), ret_msg.head.nret);
		//open file for write 
		nFd = open (szfile, O_WRONLY | O_CREAT | O_TRUNC);
		if (nFd == -1)
		{
			ret_msg.result.push_back (string("open file '")+string(szfile)+"' for write failed");
			ret_msg.head.nret = -1;
			nRet = -1;
		}else
		{
			unsigned int size = ret_msg.filedata.size();
			char szline[8192] = {0};
			unsigned int ptr = 0;
			for (unsigned int i = 0; i < size; i++)
			{
				szline [ptr] = ret_msg.filedata[i];
		
				if (++ptr > 8000)
				{
					write (nFd, szline, ptr);
					ptr = 0;
				}
			}
			if (ptr > 0)
			{
				write (nFd, szline, ptr);
			}
			close (nFd);
			chmod (szfile, 0777);
			chown  (szfile, 0, 0);
		}
		//printf ("fetching file '%s' success,size %ld, dest name '%s'\n", 
		//g_fetch_msg.head.remotefile.c_str(), ret_msg.head.filesize, g_fetch_msg.head.localfile.c_str());
	}
	return nRet;


}

//added by duanjigang1983@2011-11-01 --finish
