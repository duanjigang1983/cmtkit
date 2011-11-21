#include "MessageHandler.h"
#include "constdef.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>
#include <queue>
#include <sys/types.h>
#include <sys/wait.h>
#include "ColorDefine.h"
#include "NetHelper.h"
#include "host_tree.h"
#include "cm_plugin.h"

using namespace std;
typedef queue<command*> cmd_queue_t;
cmd_queue_t g_cmd_queue;
pthread_mutex_t  g_cmd_queue_mutex;
pthread_mutex_t  g_index_mutex;
unsigned int 	 g_index_cmd = 0;

extern int syslog_msg(const char * szMsg,...);
//global configure for server
cm_server_config_t g_server_config;
extern int interval;
//task queue for server
task_list_t g_task_list;

//task node pool
task_list_t g_task_pool;

//over task queue for report
task_list_t g_over_task_list;
//define for work thread
pthread_t * g_tid_work = 0;
void *work_thread_func(void *arg);
extern int GetNetip (unsigned int *);
//define for report thread
pthread_t g_tid_report;
pthread_t g_tid_update;
pthread_t g_tid_heartbt;
void *report_thread_func(void *arg);

int	run_cmd(const string & cmd, ::cmdhelper::StringArray & result);
int	run_limited_cmd(const string & cmd, ::cmdhelper::StringArray & result);
int report_task (const task_t * task);


extern Ice::CommunicatorPtr g_ic;
CMessageHandler::CMessageHandler()
{

}
CMessageHandler::~CMessageHandler()
{

}
static char* get_time_str(time_t * tm)
{
	struct tm * newtime;
	static char s[32]= {0};
 	if(tm == NULL)
	{
		time_t ct;
        	time(&ct);
        	newtime = gmtime(&ct);
	}else
	{
		newtime = gmtime(tm);
	}
	sprintf( s,"%04d-%02d-%02d %02d:%02d:%02d", 
	newtime->tm_year + 1900, 
				(newtime->tm_mon + 1)%12, 
				newtime->tm_mday,
         		(newtime->tm_hour + 8)%24, 
				newtime->tm_min, newtime->tm_sec );
		return s;
}
///////////////function to update client////////////////////

int do_notify_task (void);

/////////////////////////////////////

void *update_thread_func(void *arg);
void *heartbt_thread_func(void *arg);
int	CMessageHandler::init_cmd_server (void)
{
	int nRet = 1;
	//init task queue
	pthread_mutex_init (&g_task_list.mutex, 0);
	g_task_list.list = NULL;

	//init task pool
	pthread_mutex_init (&g_task_pool.mutex, 0);
	g_task_pool.list = NULL;

	//init over task list
	pthread_mutex_init (&g_over_task_list.mutex, 0);
	g_over_task_list.list = NULL;

	//init mutex for index 	
	pthread_mutex_init (&g_index_mutex, 0);
	//malloc resource for task pool
	for (unsigned int i = 0; i < TASK_POOL_SIZE; i++)
	{
		task_t * pt = new task_t;
		if (!pt)
		{
			printf ("malloc new task_t failed\n");
			nRet = -1;
			goto ret;	
		}
		pt->next = NULL;
		
		if (NULL == g_task_pool.list)
		{
			g_task_pool.list = pt;
		}else
		{
			pt->next = g_task_pool.list;
			g_task_pool.list = pt;
		}
		nRet++;
		
	}

	//init report thread
	if (pthread_create (&g_tid_report, 0, report_thread_func, NULL))
	{
		printf ("create report thread failed\n");
		nRet = -2;
		goto ret;
	}	
	
		
	//init work thread
	g_tid_work = new pthread_t[g_server_config.threadnum];
	if (!g_tid_work)
	{
		printf ("new pthread_t failed\n");
		nRet = -1;
		pthread_cancel (g_tid_report);
		goto ret;

	}
	unsigned int index [1000];
	for (unsigned int i = 0; i < g_server_config.threadnum; i++)
	{
		index[i] = i;
		if (pthread_create (&g_tid_work[i], 0, work_thread_func, &index[i]))
		{
			printf ("create work thread failed\n");
			nRet = -3;
			pthread_cancel (g_tid_report);
			goto ret;
		}
	}	
ret:
	return nRet;
}

int	CMessageHandler::init_stat_server (void)
{
	return 1;
}

void	CMessageHandler::init_auth_info(void)
{
	FILE * fp = fopen (CM_IP_LIST, "r");
	if (!fp) return;
	ip_struct_t node;
	
	while (fread (&node, sizeof(ip_struct_t), 1, fp) > 0)
	{
		add_auth_info (node.ipaddr, node.user);
	}

	fclose (fp);
}

int	CMessageHandler::init_server(void)
{
	mkdir_rec(INFO_DIR);
	init_auth_info();	

	//create update thread
	if (g_server_config.active)
	{
		pthread_mutex_init (&g_cmd_queue_mutex, 0);
		if (pthread_create (&g_tid_update, 0, update_thread_func, NULL))
		{
			printf ("create update thread failed\n");
			return -1;
		}else
		{
			printf ("create update thread succsss\n");
		}	
	}
	
	//create heart beat thread
	if (pthread_create (&g_tid_heartbt, 0, heartbt_thread_func, NULL))
	{
		printf ("create heart beat thread failed\n");
		return -1;
	}else
	{
		printf ("create heartbeat thread succsss\n");
	}	
	//printf ("init %s server\n", (g_server_config.devtype == DEV_TYPE_STAT) ? "stat" : "command");	
	return (DEV_TYPE_STAT ==  g_server_config.devtype) ? init_stat_server():init_cmd_server();
}

int	CMessageHandler::handle_report_message(const ::cmdhelper::CommandMessage& msg, 
	::cmdhelper::CommandMessage& ret)
{
	char szip[20] = {0};
	unsigned int i = 0;
	unsigned int addr = (unsigned int)msg.head.hostaddr;
	STR_IP (szip, addr);
	printf ("%s\n", szip);
	printf ("%s\n", msg.cmd.c_str());
	for ( i = 0; i < msg.result.size(); i++)
	{
		printf ("%s", msg.result[i].c_str());
	}
	return 1;
}

//command handling thread
int	CMessageHandler::handle_cmd_message(const ::cmdhelper::CommandMessage& msg, 
	::cmdhelper::CommandMessage& ret)
{
	/*
	if (RUN_BACK == msg.head.runmode)
	{
		task_t * p = 0;
		pthread_mutex_lock (&g_task_pool.mutex);
		//there are too many tasks for this server	
		if (NULL == g_task_pool.list)
		{
			ret.result.push_back ("too many task for this server");
			pthread_mutex_unlock (&g_task_pool.mutex);
			goto ret;
		}
		p = g_task_pool.list;
		//moving head to next pointer
		g_task_pool.list = p->next;
		pthread_mutex_unlock (&g_task_pool.mutex);

		p->status = status_ready;
		p->born_time = time(0);
		p->heart_time_out = 10;
		p->total_time_out = 120;
		p->result = 0;
		p->stat_server_addr 	= msg.head.hostaddr; //stat server addr
		p->taskid		= msg.head.taskid; //cmclient child task pid
		p->cmdid		= msg.head.commandid; //command id
		p->stat_server_port 	= msg.head.hostport; //stat server port
		p->local_addr		= msg.head.localaddr;//ip addr of self
		p->cmd_ret.clear();
		p->cmd_data = msg.cmd;

		//add this task to task queue
		pthread_mutex_lock (&g_task_list.mutex);
		p->next = NULL;
		if (NULL == g_task_list.list)
		{
			printf ("adding node at head\n");
			g_task_list.list = p;
		}else
		{
			printf ("adding node at tail\n");
			p->next = g_task_list.list;
			g_task_list.list = p;
		}
		pthread_mutex_unlock (&g_task_list.mutex);
	}
	else //run this command right now
	*/
	if(1)
	{
		ret.result.clear();
		ret.head.hostaddr 	= 	msg.head.localaddr;
		ret.head.hostport 	= 	2011;
		ret.head.commandid 	= 	msg.head.commandid;
		ret.head.taskid 	= 	msg.head.taskid;
                ret.head.msgtype 	= 	msg.head.msgtype;
		ret.head.timestamp 	= 	time (0);
		ret.head.localaddr 	= 	msg.head.hostaddr;
		ret.cmd			=	msg.cmd;
		//ret.head.nret		=	run_cmd (ret.cmd, ret.result);	
		ret.head.nret		=	run_limited_cmd (ret.cmd, ret.result);	
	}
	return 1;
}


int	run_cmd(const string & cmd, ::cmdhelper::StringArray & result)
{
	//printf ("get one command:%s\n", cmd.c_str());
	if (cmd.length() == 0)
	{
		return 0;
	}	
	char szfile[50] = {0};
	char szshell[50] = {0};
	char szcmd[1124] = {0};	
	mkdir("/tmp", 0666);
	mkdir("/tmp/cmtools", 0666);
	if (cmd.size() > 1024)
	{
		sprintf (szcmd, "command too long(%u)", (unsigned int)cmd.size());
		result.push_back (szcmd);
		return 1;
	}
	unsigned int index = 0;
	pthread_mutex_lock (&g_index_mutex);
	index = g_index_cmd;
	g_index_cmd++;
	pthread_mutex_unlock (&g_index_mutex);
	sprintf (szfile, "/tmp/cmtools/%u.data", index);
	sprintf (szshell, "/tmp/cmtools/%u.sh", index);
	FILE * f1 = fopen (szshell, "w");
	if (!f1)
	{
		result.push_back ("run command exception(create temp shell file failed)");
		return 1;
	}
	fprintf (f1, "%s", cmd.c_str());
	fclose (f1);
	//======================================================
	pid_t pid = 0;	
	if ((pid = fork()) == 0)
	{
		char szexe[64] = {0};
		sprintf (szexe, "%s", "/bin/bash");
		char * const szparam[] = {szexe, 
						szshell, 
						NULL};
		int nFd =  open (szfile, O_CREAT|O_RDWR, 0644);	
		dup2(nFd, 2);
		dup2(nFd, 1);
		execvp ("/bin/bash", szparam);
		close(nFd);
		exit(0);
	}else
	{
		wait(NULL);
	}	
	//================================================
	
	unlink (szshell);
	
	FILE * fp = fopen (szfile, "r");
	if (fp == NULL)
	{
		result.push_back ("run command exception(create temp data file failed)");
		return 1;
	}
	char szline[1024] = {0};
	while ( fgets(szline, sizeof(szline), fp))  
	{  
      		result.push_back(szline);
    	}  
    	fclose(fp); 
	unlink (szfile);
	return 1;
}

//handle config update message
int	CMessageHandler::handle_conf_message (const ::cmdhelper::command& cmd)
{
	return 1;
}

int CMessageHandler::ProcessMagicMessage(const ::cmdhelper::magicmsg& msg, const Ice::Current &)
{
	printf ("magic message type:%d\n", msg.type);
	return 1;
}

::cmdhelper::response  CMessageHandler::ProcessNotify(const ::cmdhelper::command& cmd, const Ice::Current &)
{
	::cmdhelper::response  res;
	char szip[20] = {0};
	unsigned int addr = (unsigned int) cmd.clientaddr;
	STR_IP (szip, addr);
	printf ("ProcessNotify for client %s\n", szip);
	return res;	
}
//handle message
::cmdhelper::command CMessageHandler::ProcessCmd(const ::cmdhelper::command& cmd, const Ice::Current &)
{
	::cmdhelper::command ret;
	::cmdhelper::command * new_cmd = 0;
	
	switch (cmd.cmdtype)
	{
		//notify this client to fetch task(s)
		case CMD_TYPE_NOTIFY:
		{
			new_cmd = new ::cmdhelper::command;
			//copy data
			new_cmd->host 		= cmd.host; //reply server
			new_cmd->port 		= cmd.port; //reply port
			new_cmd->cmdtype 	= cmd.cmdtype; //command type
			new_cmd->cmdid 		= cmd.cmdid; //command id
			new_cmd->pid		= cmd.pid; //process id
			new_cmd->clientaddr	= cmd.clientaddr; //client addr
			new_cmd->clientid	= cmd.clientid; //client id 
			unsigned size 		= cmd.data.size(); //data size
			new_cmd->data.clear();
			for (unsigned int i = 0; i < size; i++)
			{
				new_cmd->data.push_back (cmd.data[i]);
			}
			pthread_mutex_lock (&g_cmd_queue_mutex);
			g_cmd_queue.push (new_cmd);	
			printf ("pushing one command to queue ("GREEN"%u"NONE")\n", (unsigned int)g_cmd_queue.size());
			pthread_mutex_unlock (&g_cmd_queue_mutex);
			break;
		}
		//update conf message
		case CMD_TYPE_CONF:
		{
			break;
		}
		default:
		printf ("[%s:%d] error command type :%d\n", __FILE__, __LINE__, cmd.cmdtype);
		break;
	}
	
	return ret;
}
::cmdhelper::CommandMessage CMessageHandler::ProcessMessage(const ::cmdhelper::CommandMessage& msg, 
const Ice::Current & )
{
		::cmdhelper::CommandMessage ret_msg;
		#if 0	
		printf ("message type: %s\n", str_msg_type[MSG_TYPE_CMD%4]);
		#endif
		unsigned int  size = msg.head.srchost.size();
		//printf ("===recv message from:");
		
		int find = -1;
		char szip[20] = {0};
		unsigned int find_addr = 0;
		
		if (size > 0) find_addr = msg.head.srchost[0];
		if (g_server_config.auth_on)
		{		
			for (unsigned int i = 0; i < size; i++)
			{
				char szip[20] = {0};
				STR_IP (szip, (unsigned int )msg.head.srchost[i]);
				find_addr = msg.head.srchost[i];
				if (find_auth_info(msg.head.srchost[i], 
						msg.head.username.c_str(),
						msg.head.login.c_str()))
				{
					find = 1;
					find_addr = msg.head.srchost[i];
					break;
				}
			}
			//printf ("\n");
			if (find == -1 )
			{
				char szip[20] = {0};
				STR_IP(szip, find_addr);
				syslog_msg ("COMMAND %s refused %d USER %s LOGIN %s", 
				szip, msg.head.msgtype, 
				msg.head.username.c_str(), msg.head.login.c_str());
				ret_msg.result.push_back("you are  not authorized to run command on this host\n");
				goto ret;
			}
		}//end of if auth_on
		
		STR_IP(szip, find_addr);

		switch (g_server_config.devtype)
		{
			printf ("recving message:%s\n", msg.cmd.c_str());	
			case DEV_TYPE_CMD:
			if (MSG_TYPE_CMD == msg.head.msgtype)
			{
				syslog_msg ("COMMAND %s accept [%s] %d LOGIN %s USER %s", 
				szip, msg.cmd.c_str(), msg.head.msgtype,
				msg.head.login.c_str(), msg.head.username.c_str());
				
				handle_cmd_message (msg, ret_msg);
			}else
			if (MSG_TYPE_UP_FILE == msg.head.msgtype)
			{
				//dedicate to xuejian only for command
				#if 1
				handle_file_message (msg, ret_msg);
				syslog_msg ("FILE %s SRCFILE %s DSTFILE %s TYPE %d LOGIN %s USER %s", 
				szip, msg.head.file.c_str(), msg.head.dstfile.c_str(),
				msg.head.msgtype,
				msg.head.login.c_str(), msg.head.username.c_str());
				#endif
			}else 
			if (MSG_TYPE_DOWN_FILE == msg.head.msgtype)
			{
				#if 1
				int ret = handle_fetch_message (msg, ret_msg);
				ret_msg.head.nret = ret;
				syslog_msg ("FETCH %s REMOTEFILE %s LOCALFILE %s TYPE %d LOGIN %s USER %s", 
				szip, msg.head.remotefile.c_str(), msg.head.localfile.c_str(),
				msg.head.msgtype,
				msg.head.login.c_str(), msg.head.username.c_str());
				#endif
			}else
			{
				ret_msg.head.nret = -1;
			}
			break;
			case DEV_TYPE_STAT:
			handle_report_message (msg, ret_msg);
			break;
			default: ret_msg.head.nret = -1;
			break;
		}
	ret:
		return ret_msg;
}

//work thread
void *work_thread_func(void *arg)
{
	//unsigned int * index = (unsigned int*)arg;

	//printf ("working thread %u running\n", *index);
	pthread_detach(pthread_self());
	
	while (1)
	{
		task_t * task = NULL;
		
		pthread_mutex_lock (&g_task_list.mutex);
		
		if (NULL != g_task_list.list)
		{
			//printf ("task not null %p\n", g_task_list.list);
			task = g_task_list.list;
			g_task_list.list = g_task_list.list->next;
		}
		pthread_mutex_unlock (&g_task_list.mutex);
		
		if (NULL == task)
		{
			usleep (10);
			continue;
		}
		task->status = status_working;
		task->run_time = time(0);
		//run command
		task->result = run_cmd (task->cmd_data, task->cmd_ret);
		task->over_time = time (0);
		task->status = status_over;
		task->next =  NULL;
		
		//push over task to over task list
		pthread_mutex_lock (&g_over_task_list.mutex);
		//printf ("thread %u finishing task[%s]\n", *index, task->cmd_data.c_str());
		
		if (NULL == g_over_task_list.list) 	
		{
			g_over_task_list.list = task;
		}else
		{
			task->next = g_over_task_list.list;
			g_over_task_list.list = task;
		}
		pthread_mutex_unlock (&g_over_task_list.mutex);
	}
	pthread_exit(0);
	return 0;
}

//report thread
void *report_thread_func(void *arg)
{
	pthread_detach(pthread_self());
		
	while (1)
	{
		task_t * task = NULL;
		pthread_mutex_lock (&g_over_task_list.mutex);
		if (NULL != g_over_task_list.list)
		{
			task = g_over_task_list.list;
			g_over_task_list.list = g_over_task_list.list->next;
			task->next = NULL;
		}	
		pthread_mutex_unlock (&g_over_task_list.mutex);

		if (NULL == task)
		{
			usleep (10);
			continue;
		}
		
		printf ("task %p reporting result for command[%s]result=", 
			task, task->cmd_data.c_str());
		if (task->cmd_data.size() == 0)
		{
			printf ("error data\n");
			//exit(0);
		}
		for (unsigned int i = 0; i < task->cmd_ret.size(); i++)
		{
			printf ("%s", task->cmd_ret[i].c_str());
		}
		//printf ("\n");
		//report------------start
		if (report_task (task) <= 0)
		{
			//printf ("");
		}		
		//report------------finish

		//release task node
		pthread_mutex_lock (&g_task_pool.mutex);
		task->cmd_data = "";
		task->cmd_ret.clear();
		task->next = NULL;

		if (NULL == g_task_pool.list)
		{
			g_task_pool.list = task;
		}else
		{
			task->next = g_task_pool.list;
			g_task_pool.list = task;
		}	
		pthread_mutex_unlock (&g_task_pool.mutex);
		//sleep (1);
	}
	pthread_exit(0);
	return 0;

}
//report this task to stat server
int report_task (const task_t * task)
{
	CommandMessage msg;
	int nRet = 1;
	unsigned int i = 0;
	char szip[20] = {0};
	if (task->stat_server_addr == 0)
	{
		printf ("invalid task addr\n");
		return 1;
	}
	STR_IP (szip, task->stat_server_addr);

	try{
		char szproxy [PROXY_LEN] = {0}; // what we use to identify a connection
                sprintf (szproxy, "cmdhelper:tcp -h %s -p %u -t 1000", szip, task->stat_server_port);
                //sprintf (szproxy, "cmdhelper:tcp -h 10.10.10.128 -p %u -t 1000",  123);
                Ice::ObjectPrx  base = g_ic->stringToProxy(szproxy);
                cmdhelper::CmdMessageHandlerPrx  client = 
                CmdMessageHandlerPrx::checkedCast(base);
                if(!client)
                {   
                        //throw "invalud proxy:ComPlusBasePrx::checkedCast(base)";
                        nRet = -1;
                        goto ret;
                }   
		msg.head.hostaddr 	= 	task->local_addr;
		msg.head.hostport 	= 	2011;
		msg.head.commandid 	= 	task->cmdid;
		msg.head.taskid 	= 	task->taskid;
                msg.head.msgtype 	= 	MSG_TYPE_STAT;
		msg.head.timestamp 	= 	time (0);
		msg.head.localaddr 	= 	inet_addr (szip);
		
		//copying command
		msg.cmd = task->cmd_data;
		msg.result.clear();
		//coying result
		for ( i = 0; i < task->cmd_ret.size(); i++)
               	{
			msg.result.push_back (task->cmd_ret[i]);			
		}

		CommandMessage ret_msg = client->ProcessMessage(msg);
	}catch(const Ice::Exception & ex)
        {
                //cerr << ex << endl;
                nRet = -2;
                goto ret;
        }catch(const char* msg)
        {
                //cerr << msg << endl;
                nRet = -3;
                goto ret;
        }
ret:
	if (nRet < 0)
	{
		printf ("sending result to stat server %s:%u failed\n", szip, task->stat_server_port);
	}
	return nRet;
}

int	CMessageHandler::handle_file_message(const ::cmdhelper::CommandMessage& msg, 
	::cmdhelper::CommandMessage& ret)
{
	printf ("CMessageHandler::handle_file_message %s\n", msg.head.index.c_str());
	char file[512] = {0};
	char dstfile[512] = {0};
	char dstfile_ok[530] = {0};
	char dstinfo[520] = {0};
	if (msg.head.file.size() == 0)
	{
		ret.result.push_back("invalid file name");
		ret.head.nret = -1;
		return 1;
	}	
	ret.head.file = msg.head.file;
	if (msg.head.dstfile.size() == 0)
	{
		ret.head.dstfile = msg.head.file;
	}else
	{
		ret.head.dstfile = msg.head.dstfile;
	}
	char szdir[256] = {0};
	sprintf (szdir, "%s", ret.head.dstfile.c_str());
	//sprintf (file, "mkdir -pv %s", dirname(szdir));
	sprintf (file, "%s", dirname(szdir));
	mkdir_rec(file);
	memset (file, 0, 512);
	
	sprintf (file, "%s.back", ret.head.dstfile.c_str());
	//sprintf (file, "%s/%s", ret.head.dir.c_str(), ret.head.file.c_str());
	sprintf (dstfile, "%s", ret.head.dstfile.c_str());	
	int nFd = open (file, O_WRONLY|O_CREAT|O_TRUNC);	
	if (nFd == -1)
	{
		ret.result.push_back(string("create file '") + string(file)+string("' failed"));
		ret.head.nret = -1;
		return 1;

	}

	unsigned int size = msg.filedata.size();	
	char szline[8192] = {0};		
	unsigned int total_size = 0;
	int ptr = 0;
	for (unsigned int i = 0; i < size; i++)
	{
		szline[ptr] = msg.filedata[i];	
		if (++ptr >= 8000)
		{
			total_size += ptr;
			write (nFd, szline, ptr);
			ptr = 0;
		}
	}
	if (ptr > 0)
	{
		total_size += ptr;
		write (nFd, szline, ptr);
	}
	close (nFd);

	if (total_size != msg.head.filesize)
	{
		ret.result.push_back("file size not equal");
		ret.head.nret = -1;
		return 1;

	}
	if (unlink(dstfile))
	{
		printf ("unlink file [%s] failed\n", dstfile);
	}
	//rename 	
	if (rename (file, dstfile))
	{
		ret.result.push_back ("rename new file to old file failed");
		ret.head.nret = -1;
		return 1;
	}	
	//change mode
	char szmod[30] = {0};
	sprintf (szmod, "%ld", msg.head.stmode);
	//if (chmod (dstfile, msg.head.stmode))
	if (chmod (dstfile, strtoul(szmod,0, 8)))
	{
		ret.result.push_back ("chomd failed");
		printf ("change mode of file [%s] failed\n", dstfile);
		ret.head.nret = -1;
		return 1;
	}
	
	//change owner
	if (chown (dstfile, msg.head.uid, msg.head.gid))
	{
		ret.result.push_back ("chown failed");
		printf ("change own of file [%s] failed\n", dstfile);
		ret.head.nret = -1;
		return 1;

	}
	sprintf (dstfile_ok, "%s.ok", dstfile);
	FILE * fp = 0;
	if ((fp = fopen(dstfile_ok, "w")))
	{
		fclose(fp);
	}
	//adding code for file information @2011-03-30 --start
	if (msg.fileinfo.size() > 0)
	{
		char szcmd[256] = {0};
		//sprintf (dstinfo, "%s/%s.info", INFO_DIR, basename(dstfile));	
		sprintf (dstinfo, "%s/%s.info", dirname(dstfile), basename(dstfile));	
		nFd = open (dstinfo, O_WRONLY|O_CREAT|O_TRUNC);	
		if (nFd == -1)
		{
			ret.result.push_back(string("create infor file '") + string(dstinfo)+ string("' failed"));
			ret.head.nret = -1;
			return 1;
		}

		size = msg.fileinfo.size();	
		memset (szline, 0, sizeof(szline));	
		total_size = 0;
		ptr = 0;
		for (unsigned int i = 0; i < size; i++)
		{
			szline[ptr] = msg.fileinfo[i];	
			if (++ptr >= 8000)
			{
				total_size += ptr;
				write (nFd, szline, ptr);
				ptr = 0;
			}
		}
		if (ptr > 0)
		{
			total_size += ptr;
			write (nFd, szline, ptr);
		}
		/*	
		char tail[100] = {0};
		sprintf (tail, "index=%s\nhost=%lld\nport=%d\n",
		msg.head.index.c_str(), msg.head.hostaddr, msg.head.hostport);
		write (nFd, tail, strlen(tail));
		close (nFd);
		*/
		sprintf (szcmd, "%s.ok", dstinfo);
		FILE * fp = fopen (szcmd, "w");
		if (fp) fclose(fp);
		
	}
	
	//adding code for file information @2011-03-30 --end
	ret.head.nret = 1;
	return 1;
}

unsigned int   g_host  = 0;//reply host
unsigned short g_port = 0; //reply port
unsigned int   g_myaddr = 0;

void *heartbt_thread_func(void *arg)
{
	//syslog_msg ("INFO heart beat thread start running");
	device_t iplist[10];
	time_t stime;	
	char szmsg[1024] = {0};
	struct tm *tm1 = NULL;
	char strtime[30] = {0};
	time(&stime);
	tm1 = gmtime (&stime);
	if(tm1 == NULL) 
	{
		pthread_exit(0);
	}
	sprintf (strtime, 
			"%04d-%02d-%02d %02d:%02d:%02d", 
			tm1->tm_year + 1900,
			(tm1->tm_mon + 1)%12, 
			tm1->tm_mday, 
			(tm1->tm_hour + 8)%24, 
			tm1->tm_min, 
			tm1->tm_sec);

	while (1)
	{
		char hostname[64] = {0};
		 int num = GetNetip (iplist);
		
		if (!gethostname(hostname, sizeof(hostname)) && (num > 0))
		{
			int i = 0;
			time_t etime;
			memset (szmsg, 0, 1024);
			sprintf (szmsg, "HEARTBT %s [", hostname);
			
			for (i = 0; i < num; i++)	
			{
				char szip[20] = {0};
				char szip1[20] = {0};
				char mac[19] = {0};
				sprintf (mac, "%02X:%02X:%02X:%02X:%02X:%02X",
					iplist[i].mac[0],
					iplist[i].mac[1],
					iplist[i].mac[2],
					iplist[i].mac[3],
					iplist[i].mac[4],
					iplist[i].mac[5]);
				STR_IP (szip, iplist[i].ip);
				STR_IP (szip1, iplist[i].netmask);
				sprintf (szmsg + strlen(szmsg), "%s-%s-%s-%s%c", 
				iplist[i].name,  szip, mac, szip1, (i == num - 1)? ']':' ');
			}
		//	sprintf (szmsg + strlen(szmsg), "] ");
			time (&etime);
			sprintf (szmsg + strlen(szmsg), "%lu  %lu", 
			stime, etime - stime);
			syslog_msg (szmsg);
		}
		//sleep (g_server_config.interval);
		sleep (interval);
	}
	pthread_exit(0);
}
void *update_thread_func(void *arg)
{
	//printf ("%ld update thread start running\n", (long int)time(0));
		while (true)
	{
		int sleep_interval = 5;
		
		::cmdhelper::command * pcmd = 0;	
		pthread_mutex_lock (&g_cmd_queue_mutex);
		if (g_cmd_queue.size() > 0)
		{
			pcmd = g_cmd_queue.front();
			g_cmd_queue.pop();
			printf ("poping one command from queue ("GREEN"%u"NONE")\n", (unsigned int)g_cmd_queue.size());
			sleep_interval =  0;
		}	
		pthread_mutex_unlock (&g_cmd_queue_mutex);

		//try to run command
		if (pcmd) //如果取到命令了，就执行
		{
			char szip[20] = {0};
			STR_IP (szip, (unsigned int)pcmd->host);
			printf ("get a command:\nhost\t=\t"GREEN"%s:%u"NONE"\ncmdtype\t=\t"GREEN"%u"NONE"\ncmdid\t=\t"GREEN"%u"NONE"\npid=\t=\t"GREEN"%d"NONE"\nlength\t=\t"GREEN"%u"NONE"\n", 
			szip, (unsigned short)pcmd->port, (unsigned int)pcmd->cmdtype, 
			(unsigned int)pcmd->cmdid, pcmd->pid, (unsigned int)pcmd->data.size());
			//run task
			g_host = pcmd->host;
			g_port = pcmd->port;
			g_myaddr = pcmd->clientaddr;
			//release resource
			delete pcmd;	
		}else //否则，主动去服务器上取文件，执行
		{
			//request for config file  actively
		}
		
		if (!g_host || !g_port || !g_myaddr) goto LOOP;
		
		do_notify_task ();
	LOOP:
		sleep (sleep_interval);
	}
	return NULL;
}
int do_notify_task (void)
{
	char szip[20] = {0};
	int nRet = 1;
	::cmdhelper::response res;
	::cmdhelper::command cmd;
	cmd.clientaddr = g_myaddr;
	STR_IP (szip, g_host);

	try
	{
		char szproxy [PROXY_LEN] = {0}; // what we use to identify a connection

		sprintf (szproxy, "cmdhelper:tcp -h %s -p %u -t 2000", szip, g_port);
		Ice::ObjectPrx  base = g_ic->stringToProxy(szproxy);
		cmdhelper::CmdMessageHandlerPrx  client = 
		CmdMessageHandlerPrx::checkedCast(base);
                if(!client)
                {
			//throw "invalud proxy:ComPlusBasePrx::checkedCast(base)";
        		nRet = -1;
			goto ret;
		}
		
		res  = client->ProcessNotify(cmd);
		nRet = 1;
	}catch(const Ice::Exception & ex)
        {
                //cerr << ex << endl;
		nRet = -1;
		goto ret;
        }catch(const char* msg)
        {
		nRet = -1;
		goto ret;
	}
ret:
	printf (GREEN"%s"NONE" update from "GREEN"%s:%u"NONE""YELLOW"" "%s"NONE"\n", 
	get_time_str(0),
	szip, g_port, nRet > 0 ? " success" : " failed");
	return nRet;
}
//added by duanjigang1983@2011-11-02 --start
int	CMessageHandler::handle_fetch_message(const ::cmdhelper::CommandMessage& msg, 
	::cmdhelper::CommandMessage& ret)
{
	printf ("fetching file:%s\n", msg.head.remotefile.c_str());
	ret.filedata.clear();
	char szbuf [8192] = {0};
	struct stat file_stat;
	//try to stat file
	char szfile[256] = {0};
	char szmsg[512] = {0};
	sprintf (szfile, "%s",  msg.head.remotefile.c_str());

	if (stat (szfile, &file_stat))
	{
		sprintf (szmsg, "can not read file '%s'", szfile);
		ret.result.push_back (szmsg);
		return -1;	

	}

	if (file_stat.st_size > 5000000 )
	{
		sprintf (szmsg, "remote file too large '%ld(KB)'", file_stat.st_size/1024);
		ret.result.push_back (szmsg);
		return -1;	
	}	

	if (access(szfile, R_OK))
	{
		sprintf (szmsg, "can not read file '%s'", szfile);
		ret.result.push_back (szmsg);
		return -1;	
	}
	int nFd = open (szfile, O_RDONLY);
	if (nFd == -1)
	{
		sprintf (szmsg, "open file '%s' for read failed", szfile);
		ret.result.push_back (szmsg);
                return -1;
	}
	
	unsigned int size = 0;
	unsigned int nread = 0;
	while ( (nread = read (nFd, szbuf, 8192)) > 0 )
	{
		size += nread;
		for (unsigned int i = 0; i < nread; i++)
		{
			ret.filedata.push_back (szbuf[i]);
		}
		memset (szbuf, 0, nread);
	}
	close (nFd);
	ret.head.filesize	= size;
	ret.head.stmode		= file_stat.st_mode;
	ret.head.uid		= file_stat.st_uid;
	ret.head.gid		= file_stat.st_gid;
	return 1;
}
//added by duanjigang@2011-11-20 1:08 --start
int	run_limited_cmd(const string & cmd, ::cmdhelper::StringArray & result)
{
	char szcmd[1024] = {0};
	static int g_index = 0;
	int index = 0;
	char szline [1024] = {0};
	pthread_mutex_lock (&g_index_mutex);
	if (++g_index > 100) g_index = 0;
	index = g_index;
	pthread_mutex_unlock (&g_index_mutex);
	char szfile[512] = {0};	
	if (cmd.length() == 0)
	{
		return 0;
	}	
	if (cmd.size() > 1024)
	{
		sprintf (szcmd, "command too long(%u)", (unsigned int)cmd.size());
		result.push_back (szcmd);
		return 0;
	}
	strncpy (szcmd, cmd.c_str(), cmd.size());
	sprintf (szfile, "/tmp/cmtkit_%d.data", index);
	if (access(szfile, W_OK))
	{
		unlink (szfile);
	}
	int nret = run_limit_cmd (szcmd, szfile);
	result.clear();
	FILE * fp = fopen (szfile, "r");
	if (!fp)
	{
		result.push_back ("open result file '"+string(szfile)+"' for reading failed");
		return -1;
	}	
	
	while (fgets(szline, 1024, fp))
	{
		result.push_back (szline);
		memset (szline, 0, 1024);
	}
	fclose (fp);
	unlink (szfile);
	return nret;
}
//added by duanjigang@2011-11-20 1:08 --finish
