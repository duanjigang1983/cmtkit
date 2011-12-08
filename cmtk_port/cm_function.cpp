#include "cmconfig.h"
#include "ColorDefine.h"
#include "constdef.h"
extern Ice::CommunicatorPtr g_ic;

int send_cmd2host (cmdev_t* dev, const cm_client_config_t * config)
{
	int nRet = RET_SUCCES;
	char szip[20] = {0};
	bool connected = false;
	STR_IP (szip, dev->dev_ip);
	CommandMessage msg; // request message
	//printf ("send command:%s-%d\n", __FILE__, __LINE__);
	for (unsigned int i = 0; i < config->ipnum; i++)
	{
		msg.head.srchost.push_back (config->iplist[i].ip);
	}
	
	try
	{
		
		char szproxy [PROXY_LEN] = {0}; // what we use to identify a connection
		sprintf (szproxy, "cmtkp:tcp -h %s -p %u -t %u", szip,  config->remote_port, config->time_out * 1000);
		
		
		//Ice::ObjectPrx  base = config->ic->stringToProxy(szproxy);
		Ice::ObjectPrx  base =  g_ic->stringToProxy(szproxy);
		cmtkp::CmdMessageHandlerPrx  client = CmdMessageHandlerPrx::checkedCast(base);
	//	printf ("send command:%s-%d\n", __FILE__, __LINE__);
		if(!client)
		{
			nRet = RET_CONNECT_FAILED;
			goto ret;
		}
		connected 		=	true;
		msg.head.msgtype 	= 	MSG_TYPE_CMD;
		//msg.head.hostaddr 	= 	g_client_config.local_addr;
		//msg.head.hostport 	= 	g_client_config.local_port;
		msg.head.timestamp	=	time(0);
		msg.head.taskid		= 	GETPID;	
		msg.head.commandid	=	time(0);
		//msg.head.localaddr	=	dev->dev_ip;
		msg.head.runmode	=	config->run_mode;//added by djg@2011-03-09
		msg.cmd 		= 	config->command; // get file name
		//msg.head.username	=	g_client_config.username;
		//msg.head.login		=	g_client_config.login;
		
		
		// try to request the file
	//	printf ("send command:%s-%d\n", __FILE__, __LINE__);
		CommandMessage ret_msg = client->ProcessMessage(msg);
		//printf ("send command:%s-%d\n", __FILE__, __LINE__);
		dev->stret.clear();
		if (RUN_REAL == config->run_mode)	
		{
			for (unsigned int i = 0; i < ret_msg.filedata.size(); i++)
			{
				dev->stret.push_back(ret_msg.filedata[i]);
			}
			/*
			for (unsigned int i = 0; i < ret_msg.result.size(); i++)
			{
				
				dev->retmsg.push_back(ret_msg.result[i]);	
			}*/
			
		}
	}
	catch(const Ice::Exception & ex)
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
//	printf ("send command:%s-%d\n", __FILE__, __LINE__);
	return nRet;
}
//send file to host
int send_file2host (cmdev_t* dev, const cm_client_config_t * config)
{
	int nRet = RET_SUCCES;
	bool connected = false;
	char szip[20] = {0};
	printf ("sendfile:%s-%d\n", __FILE__, __LINE__);
	if (!dev)
	{
		printf ("invalid parameter[%s:%d]\n",__FILE__, __LINE__);
		exit(0);
	}
	STR_IP (szip, dev->dev_ip);
	
	
	for (unsigned int i = 0; i < config->ipnum; i++)
	{
		config->file_msg->head.srchost.push_back (config->iplist[i].ip);
	}
	
	
	try
	{
		char szproxy [PROXY_LEN] = {0}; // what we use to identify a connection
		sprintf (szproxy, "cmtkp:tcp -h %s -p %u -t %u", szip, config->remote_port, config->time_out * 1000);
		//Ice::ObjectPrx  base = config->ic->stringToProxy(szproxy);
	printf ("sendfile:%s-%d\n", __FILE__, __LINE__);
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
		config->file_msg->head.timestamp	=	time(0);
		config->file_msg->head.taskid		= 	getpid();	
		config->file_msg->head.commandid	=	time(0);
		//config->file_msg->head.localaddr	=	dev->dev_ip;
		char index[20] = {0};
		sprintf (index, "%lu", dev->dev_index);
		config->file_msg->head.index = string (index);		
		// try to request the file
		printf ("sendfile:%s-%d\n", __FILE__, __LINE__);
		CommandMessage ret_msg = client->ProcessMessage(*(config->file_msg));
		printf ("sendfile:%s-%d\n", __FILE__, __LINE__);
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
	return nRet;
	
	return 1;
}
//fetch file from host
int fetch_file_from_host (cmdev_t *dev, const cm_client_config_t * config)
{
	int nRet = RET_SUCCES;
	bool connected = false;
	char szip[20] = {0};
	CommandMessage g_fetch_msg, ret_msg; 
	
	if (!dev)
	{
		printf ("invalid parameter[%s:%d]\n",__FILE__, __LINE__);
		exit(0);
	}
	STR_IP (szip, dev->dev_ip);
	
	g_fetch_msg.head.msgtype = MSG_TYPE_DOWN_FILE;
	for (unsigned int i = 0; i < config->ipnum; i++)
	{
		g_fetch_msg.head.srchost.push_back (config->iplist[i].ip);
	}
	
	try
	{
		char szproxy [PROXY_LEN] = {0}; // what we use to identify a connection
		char index[20] = {0};
		g_fetch_msg.filedata.clear();	
		sprintf (szproxy, "cmtkp:tcp -h %s -p %u -t %u", szip, config->remote_port, config->time_out * 1000);
		
	
		//Ice::ObjectPrx  base = config->ic->stringToProxy(szproxy);
		Ice::ObjectPrx  base = g_ic->stringToProxy(szproxy);
		cmtkp::CmdMessageHandlerPrx  client = CmdMessageHandlerPrx::checkedCast(base);
		if(!client)
		{
			
			nRet = RET_CONNECT_FAILED;
			goto ret;
		}
		connected = true;
		//added by duanjigang1983@2011-11-01 --start
		g_fetch_msg.head.remotefile	= 	config->remote_file;
		g_fetch_msg.head.localfile	=	config->local_file;		
		//added by duanjigang1983@2011-11-01 --finish
		g_fetch_msg.head.timestamp	=	time(0);
		g_fetch_msg.head.taskid		= 	GETPID;	
		g_fetch_msg.head.commandid	=	time(0);
		//g_fetch_msg.head.localaddr	=	dev->dev_ip;
		
		
		sprintf (index, "%lu", dev->dev_index);
		g_fetch_msg.head.index = string (index);
		
		ret_msg = client->ProcessMessage(g_fetch_msg);
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
		//printf ("%s:%d------------------\n", __FILE__, __LINE__);
		sprintf (szfile, "%s", my_dirname (szdir));
		//printf ("%s:%d------------------\n", __FILE__, __LINE__);
	
		///mkdir_rec (my_dirname(szfile));
		memset (szfile, 0, 256);
		sprintf (szfile, "%s.%s", g_fetch_msg.head.localfile.c_str(), dev->dev_name);
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
#ifdef __LINUX__
			chmod (szfile, 0777);
			chown  (szfile, 0, 0);
#endif
		}
		//printf ("fetching file '%s' success,size %ld, dest name '%s'\n", 
		//g_fetch_msg.head.remotefile.c_str(), ret_msg.head.filesize, g_fetch_msg.head.localfile.c_str());
	}
	return nRet;
}

