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
pthread_mutex_t  g_index_mutex;
unsigned int 	 g_index_cmd = 0;

extern int syslog_msg(const char * szMsg,...);
//global configure for server
cm_server_config_t g_server_config;
extern int interval;
extern int GetNetip (unsigned int *);
pthread_t g_tid_heartbt;

int	run_cmd(const string & cmd, ::cmtkp::CommandMessage& ret);
int	run_limited_cmd(const string & cmd, ::cmtkp::byteArray & result);
int 	run_unlimited_cmd (const string & cmd, ::cmtkp::CommandMessage& ret);

extern Ice::CommunicatorPtr g_ic;
CMessageHandler::CMessageHandler()
{

}
CMessageHandler::~CMessageHandler()
{

}


//void *update_thread_func(void *arg);
void *heartbt_thread_func(void *arg);

int	CMessageHandler::init_cmd_server (void)
{
	int nRet = 1;
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

	if (pthread_create (&g_tid_heartbt, 0, heartbt_thread_func, NULL))
	{
		printf ("create heart beat thread failed\n");
		return -1;
	}else
	{
		printf ("create heartbeat thread succsss\n");
	}	
	return init_cmd_server();
}



int	CMessageHandler::handle_cmd_message(const ::cmtkp::CommandMessage& msg, 
	::cmtkp::CommandMessage& ret)
{
	ret.result.clear();
	//ret.head.hostaddr 	= 	msg.head.localaddr;
	ret.head.hostport 	= 	2011;
	ret.head.commandid 	= 	msg.head.commandid;
	ret.head.taskid 	= 	msg.head.taskid;
        ret.head.msgtype 	= 	msg.head.msgtype;
	ret.head.timestamp 	= 	time (0);
	//ret.head.localaddr 	= 	msg.head.hostaddr;
	ret.cmd			=	msg.cmd;
	//ret.head.nret		=	g_server_config.limited ? run_limited_cmd (ret.cmd, ret.result):run_cmd (ret.cmd, ret);	
	ret.head.nret		=	g_server_config.limited ? run_limited_cmd (ret.cmd, ret.result):run_unlimited_cmd (ret.cmd, ret);	
	//ret.head.nret		=	g_server_config.limited ? run_limited_cmd (ret.cmd, ret.result):run_cmd (ret.cmd, ret.result);	
	return 1;
}

int run_unlimited_cmd (const string & cmd, ::cmtkp::CommandMessage& ret)
{
	char szfile[128] = {0};
	char szerr[256] = {0};
	char szcmd[1024] = {0};
	unsigned int index = 0;
	unsigned int cmd_len = cmd.size();
	char * tt = 0;

	ret.result.clear();	
	mkdir("/tmp", 0666);
	mkdir("/tmp/dont_remove", 0666);
	if (cmd.size() > 1024)
	{
		sprintf (szerr, "command too long(%u)", (unsigned int)cmd.size());
		for (unsigned int i = 0; i < strlen(szerr); i++)
		ret.result.push_back (szerr[i]);
		return 1;
	}
	strncpy (szcmd, cmd.c_str(), cmd_len);
	tt = szcmd + strlen(szcmd) - 1;
	while (tt && isspace(*tt))
	{
		*tt = '\0';
		tt--;
	}
	pthread_mutex_lock (&g_index_mutex);
	index = g_index_cmd;
	g_index_cmd++;
	pthread_mutex_unlock (&g_index_mutex);
	sprintf (szfile, "/tmp/dont_remove/%u.data", index);

	//try to find the command
	int pid = fork();
	if (!pid)
	{
		char * pcmd = szcmd;
		char * szname = NULL;
		char  name[1024] = {0};
		char ** argv_list = NULL;
		char ** argvs = NULL;
		int32_t ntok = 0;
		int32_t i = 0;
		int32_t size = 0;

		int nFd =  open (szfile, O_CREAT|O_RDWR, 0644);	
		if (nFd == -1)
		{
			printf ("%s-%d:open file '%s' for writing failed\n", __FILE__, __LINE__, szfile);
			exit (0);
		}
		dup2(nFd, 2);
		dup2(nFd, 1);
		
		pcmd  = strchr (szcmd, ' ');
		char * tail = pcmd;
		while (pcmd && isspace(*pcmd)) pcmd++;		
		if (tail)
		{
			strncpy (name, szcmd, tail - szcmd);
		}else
		{
			strcpy (name, szcmd);
		}
		if (pcmd && isspace(*(pcmd - 1)))
		{	
			ntok = makeargv (pcmd, &argv_list);
		}else  
		{
			ntok = 0;
		}
		if (ntok < 0 )
		{
			printf ("error parameter:[%s]\n", pcmd);
			exit (0);
		}
		size = (ntok > 0) ? ntok : 0; 
		argvs  = (char**)malloc (sizeof(char*)*(size + 2));
		if (!argvs)
		{
			printf ("malloc failed\n");
			exit (0);
		}
		memset (argvs, 0, sizeof(char*)*(size + 2));
		argvs[0] = name;
	
		for (i = 0; i < size; i++)	
		argvs [i + 1] = argv_list [i];
	
		argvs [size + 1] = NULL;
		
		//if can not find this file, just search it in the path
		if (access (name, F_OK))
		{	
			//printf ("acces:[%s] failed\n", name);

			szname  = find_bin (name);
			if (!szname)
			{
				fprintf (stderr, "can not find command:%s\n", name);
				exit (0);
			}
		}else szname = name;
		if (access (szname, X_OK))
		{
			printf ("can not run :%s\n", szname);
			perror("");
			exit (0);
		}
		
		//printf ("EXE:%s\n", szname);
		if (execvp(szname,  argvs) < 0)
		{
			perror("Error on execv:");
		}
		//close (fd[1]);
		close (nFd);
		free(argvs);	
		if (ntok >= 0)
		free_makeargv (argv_list);
		exit(0);
		}else
		{
			wait (NULL);
		}
	//getting result
	char szline[4096] = {0};
		
	//ret.filedata.clear();
	ret.result.clear();
	int nFd = open (szfile, O_RDONLY);
	if (nFd == -1)
	{
		sprintf (szerr, "open file '%s' for reading failed\n", szfile);
		for (unsigned int i = 0; i < strlen(szerr); i++)
		ret.result.push_back (szerr[i]);
                return -1;
	}
	
	unsigned int size = 0;
	unsigned int nread = 0;
	while ( (nread = read (nFd, szline, 4196)) > 0 )
	{
		size += nread;
		for (unsigned int i = 0; i < nread; i++)
		{
			//ret.filedata.push_back (szline[i]);
			ret.result.push_back (szline[i]);
		}
		memset (szline, 0, nread);
	}
	close (nFd);
	unlink (szfile);
	return 1;
}

int	run_cmd(const string & cmd, ::cmtkp::CommandMessage& ret)
{
	//printf ("get one command:%s\n", cmd.c_str());
	char szfile[50] = {0};
	char szshell[50] = {0};
	char szcmd[1124] = {0};	
	char szerr[512] = {0};

	if (cmd.length() == 0)
	{
		return 0;
	}	
	ret.result.clear();	
	mkdir("/tmp", 0666);
	mkdir("/tmp/dont_remove", 0666);
	if (cmd.size() > 1024)
	{
		sprintf (szcmd, "command too long(%u)", (unsigned int)cmd.size());
		for (unsigned int i = 0; i < strlen(szcmd); i++)
		ret.result.push_back (szcmd[i]);
		return 1;
	}
	unsigned int index = 0;
	pthread_mutex_lock (&g_index_mutex);
	index = g_index_cmd;
	g_index_cmd++;
	pthread_mutex_unlock (&g_index_mutex);
	sprintf (szfile, "/tmp/dont_remove/%u.data", index);
	sprintf (szshell, "/tmp/dont_remove/%u.sh", index);
	FILE * f1 = fopen (szshell, "w");
	if (!f1)
	{
		syslog_msg ("%s-%d:open file '%s' for writing failed\n",__FILE__, __LINE__, szshell); 
		sprintf (szerr, "%s", "run command exception(create temp shell file failed)");
		for (unsigned int i = 0; i < strlen(szcmd); i++)
		ret.result.push_back (szerr[i]);
		return 1;
	}
	fprintf (f1, "%s", cmd.c_str());
	fclose (f1);
	pid_t pid = 0;	
	if ((pid = fork()) == 0)
	{
		
		char szexe[64] = {0};
		sprintf (szexe, "%s", "/bin/bash");
		char * const szparam[] = {szexe, szshell, NULL};
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
	
	unlink (szshell);
	char szline[4096] = {0};
		
	ret.filedata.clear();
	int nFd = open (szfile, O_RDONLY);
	if (nFd == -1)
	{
		sprintf (szerr, "open file '%s' for reading failed\n", szfile);
		for (unsigned int i = 0; i < strlen(szerr); i++)
		ret.result.push_back (szerr[i]);
                return -1;
	}
	
	unsigned int size = 0;
	unsigned int nread = 0;
	while ( (nread = read (nFd, szline, 4196)) > 0 )
	{
		size += nread;
		for (unsigned int i = 0; i < nread; i++)
		{
			ret.filedata.push_back (szline[i]);
		}
		memset (szline, 0, nread);
	}
	close (nFd);
	unlink (szfile);
	return 1;
}

//handle config update message
int	CMessageHandler::handle_conf_message (const ::cmtkp::command& cmd)
{
	return 1;
}

int CMessageHandler::ProcessMagicMessage(const ::cmtkp::magicmsg& msg, const Ice::Current &)
{
	printf ("magic message type:%d\n", msg.type);
	return 1;
}

//handle message
::cmtkp::CommandMessage CMessageHandler::ProcessMessage(const ::cmtkp::CommandMessage& msg, 
const Ice::Current & )
{
		#if 0	
		printf ("message type: %s\n", str_msg_type[MSG_TYPE_CMD%4]);
		#endif
		::cmtkp::CommandMessage ret_msg;
		unsigned int  size = msg.head.srchost.size();
		//printf ("===recv message from:");
		
		//int find = -1;
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
				/*if (find_auth_info(msg.head.srchost[i]))
				{
					find = 1;
					find_addr = msg.head.srchost[i];
					break;
				}*/
			}
			//printf ("\n");
			/*
			if (find == -1 )
			{
				char szip[20] = {0};
				STR_IP(szip, find_addr);
				syslog_msg ("COMMAND %s refused %d USER %s LOGIN %s", 
				szip, msg.head.msgtype, 
				msg.head.username.c_str(), msg.head.login.c_str());
				ret_msg.result.push_back("you are  not authorized to run command on this host\n");
				goto ret;
			}*/
		}//end of if auth_on
		
		STR_IP(szip, find_addr);

		
	printf ("recving message:%s\n", msg.cmd.c_str());	
	if (MSG_TYPE_CMD == msg.head.msgtype)
	{
		syslog_msg ("COMMAND %s accept [%s] %d", 
		szip, msg.cmd.c_str(), msg.head.msgtype);
		handle_cmd_message (msg, ret_msg);
	}else
	if (MSG_TYPE_UP_FILE == msg.head.msgtype)
	{
		//dedicate to xuejian only for command
		#if 1
		handle_file_message (msg, ret_msg);
		syslog_msg ("FILE %s SRCFILE %s DSTFILE %s TYPE %d", 
		szip, msg.head.file.c_str(), msg.head.dstfile.c_str(),
		msg.head.msgtype);
		#endif
	}else 
	if (MSG_TYPE_DOWN_FILE == msg.head.msgtype)
	{
		#if 1
		int ret = handle_fetch_message (msg, ret_msg);
		ret_msg.head.nret = ret;
		syslog_msg ("FETCH %s REMOTEFILE %s LOCALFILE %s TYPE %d", 
		szip, msg.head.remotefile.c_str(), msg.head.localfile.c_str(), msg.head.msgtype);
		#endif
	}else
	{
		ret_msg.head.nret = -1;
	}
	//ret:
	return ret_msg;
}



int	CMessageHandler::handle_file_message(const ::cmtkp::CommandMessage& msg, 
	::cmtkp::CommandMessage& ret)
{
	printf ("CMessageHandler::handle_file_message %s\n", msg.head.index.c_str());
	char file[512] = {0};
	char dstfile[512] = {0};
	char dstfile_ok[530] = {0};
	char dstinfo[520] = {0};
	char szerr[256] = {0};
	if (msg.head.file.size() == 0)
	{
		sprintf (szerr, "%s", "invalid file name to upload\n");
		for (unsigned int i = 0; i < strlen (szerr); i++)
		ret.result.push_back(szerr[i]);
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
	sprintf (file, "%s", dirname(szdir));
	mkdir_rec(file);
	memset (file, 0, 512);
	
	sprintf (file, "%s.back", ret.head.dstfile.c_str());
	//sprintf (file, "%s/%s", ret.head.dir.c_str(), ret.head.file.c_str());
	sprintf (dstfile, "%s", ret.head.dstfile.c_str());	
	int nFd = open (file, O_WRONLY|O_CREAT|O_TRUNC);	
	if (nFd == -1)
	{

		sprintf(szerr, "create file '%s'' failed\n", file);
		for (unsigned int i = 0; i < strlen(szerr); i++)
		ret.result.push_back(szerr[i]);
		ret.head.nret = -1;
		return 1;

	}
	printf("create file '%s'' success\n", file);

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
		sprintf (szerr, "%s", "file size not equal\n");
		for (unsigned int i = 0; i < strlen(szerr); i++)
		ret.result.push_back(szerr[i]);
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
		sprintf (szerr,  "rename new file '%s' to old file '%s' failed\n", file, dstfile);
		for (unsigned int i = 0; i < strlen(szerr); i++)
		ret.result.push_back (szerr[i]);
		ret.head.nret = -1;
		return 1;
	}	
	//change mode
	//char szmod[30] = {0};
	//sprintf (szmod, "%lld", msg.head.stmode);
	//if (chmod (dstfile, msg.head.stmode))
	if (chmod (dstfile, 511))
	{
		sprintf (szerr,  "chmod '%s' 511 failed\n", dstfile);
		for (unsigned int i = 0; i < strlen(szerr); i++)
		ret.result.push_back (szerr[i]);
		printf ("change mode of file [%s] failed\n", dstfile);
		ret.head.nret = -1;
		return 1;
	}
	
	//change owner
	/*
	if (chown (dstfile, msg.head.uid, msg.head.gid))
	{
		ret.result.push_back ("chown failed");
		printf ("change own of file [%s] failed\n", dstfile);
		ret.head.nret = -1;
		return 1;

	}*/
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
			sprintf (szerr, "create infor file '%s' failed\n", dstinfo);
			for (unsigned int i = 0; i < strlen (szerr); i++)
			ret.result.push_back(szerr[i]);
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
/*
void *update_thread_func(void *arg)
{
	//printf ("%ld update thread start running\n", (long int)time(0));
		while (true)
	{
		int sleep_interval = 5;
		
		::cmtkp::command * pcmd = 0;	
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
	::cmtkp::response res;
	::cmtkp::command cmd;
	cmd.clientaddr = g_myaddr;
	STR_IP (szip, g_host);

	try
	{
		char szproxy [PROXY_LEN] = {0}; // what we use to identify a connection

		sprintf (szproxy, "cmtkp:tcp -h %s -p %u -t 2000", szip, g_port);
		Ice::ObjectPrx  base = g_ic->stringToProxy(szproxy);
		cmtkp::CmdMessageHandlerPrx  client = 
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
*/
//added by duanjigang1983@2011-11-02 --start
int	CMessageHandler::handle_fetch_message(const ::cmtkp::CommandMessage& msg, 
	::cmtkp::CommandMessage& ret)
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
		sprintf (szmsg, "can not stat file '%s'\n", szfile);
		for (unsigned int i = 0; i < strlen(szmsg); i++)
		ret.result.push_back (szmsg[i]);
		return -1;	

	}

	if (file_stat.st_size > 10000000 )
	{
		sprintf (szmsg, "remote file too large '%ld(KB)' > 10240(KB)\n", file_stat.st_size/1024);
		for (unsigned int i = 0; i < strlen(szmsg); i++)
		ret.result.push_back (szmsg[i]);
		return -1;	
	}	

	if (access(szfile, R_OK))
	{
		sprintf (szmsg, "can not read file '%s'\n", szfile);
		for (unsigned int i = 0; i < strlen(szmsg); i++)
		ret.result.push_back (szmsg[i]);
		return -1;	
	}
	int nFd = open (szfile, O_RDONLY);
	if (nFd == -1)
	{
		sprintf (szmsg, "open file '%s' for read failed\n", szfile);
		for (unsigned int i = 0; i < strlen(szmsg); i++)
		ret.result.push_back (szmsg[i]);
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
	return 1;
}
//added by duanjigang@2011-11-20 1:08 --start
int	run_limited_cmd(const string & cmd, ::cmtkp::byteArray & result1)
{
	char szcmd[1024] = {0};
	static int g_index = 0;
	char result[512] = {0};
	//char* tail = 0;
	int index = 0;
	char szline [1024] = {0};
	pthread_mutex_lock (&g_index_mutex);
	if (++g_index > 100) g_index = 0;
	index = g_index;
	pthread_mutex_unlock (&g_index_mutex);
	
	char szfile[512] = {0};	

	printf ("%s-%d:run limited cmd:%s\n", __FILE__, __LINE__, cmd.c_str());
	
	if (cmd.length() == 0)
	{
		printf ("EXIT:%s-%d\n", __FILE__,__LINE__);
		sprintf(result, "%s", "empty command to run");
		for (unsigned int i = 0; i < strlen(result); i++)
		result1.push_back (result[i]);
		return 0;
	}	
	if (cmd.size() > 1024)
	{
		printf ("EXIT:%s-%d\n", __FILE__,__LINE__);
		sprintf (result, "command too long(%u)", (unsigned int)cmd.size());
		for (unsigned int i = 0; i < strlen(result); i++)
		result1.push_back (result[i]);
		return 0;
	}
	strncpy (szcmd, cmd.c_str(), cmd.size());
	//tail = szcmd + (cmd.size() - 1);
	
	char * tt = szcmd + strlen(szcmd) - 1;
	while (tt && isspace(*tt))
	{
		*tt = '\0';
		tt--;
	}

	sprintf (szfile, "/tmp/cmtkit_%d.data", index);
	if (access(szfile, W_OK))
	{
		unlink (szfile);
	}
	int nret = run_limit_cmd (szcmd, szfile);
	//result.clear();
	if (nret <= 0)
	{
		sprintf(result, "can not find command:'%s'\n", szcmd);
		for (unsigned int i = 0; i < strlen(result); i++)
		result1.push_back (result[i]);
		return -1;
	}
	FILE * fp = fopen (szfile, "r");
	if (!fp)
	{
		printf ("EXIT:%s-%d\n", __FILE__,__LINE__);
		sprintf(result, "open result file '%s' for reading failed\n", szfile);
		for (unsigned int i = 0; i < strlen(result); i++)
		result1.push_back (result[i]);
		return -1;
	}	
	
	while (fgets(szline, 1024, fp))
	{
		for (unsigned int i = 0; i < strlen(szline); i++)
		result1.push_back (szline[i]);
		memset (szline, 0, 1024);
	}
	fclose (fp);
	unlink (szfile);
	return nret;
}
//added by duanjigang@2011-11-20 1:08 --finish
