#include "cmconfig.h"
#include "constdef.h"

static int load_file2msg(const char * szfile, CommandMessage * file_msg, const cm_client_config_t * config);
int init_tasks(cm_client_config_t * config, 
			   CommandMessage * file_msg, 
			   thread_pool_t  * thread_pool, dev_list_t * dev_list)
{
	unsigned int index_task = 0;	
	
	//load file to transfer
	
	file_msg->filedata.clear();	
	if (MODE_UPLOAD_FILE == config->mode)
	{
		if (load_file2msg(config->file, file_msg, config) <= 0)
		{
			printf ("load file '%s' failed\n", config->file);
			return 0;
		}	
	}
	
	// task too much
	if (config->tasknum > MAX_TASK_NUM)
	{
		config->tasknum = MAX_TASK_NUM;
	}
	
	//in case of no -n input
	if (0 == config->tasknum)
	{
		config->tasknum = TASKNUM;
	}
	
	//init thread pool
	for (index_task = 0; index_task < config->tasknum; index_task++)
	{
		thread_pool->thread[index_task].dev_num = 0;;
		thread_pool->thread[index_task].over = 0;
		
	}
	
	//in case of thread wast
	if (dev_list->dev_num < config->tasknum)
	{
		config->tasknum = dev_list->dev_num;
	}
	
	thread_pool->th_num =  config->tasknum;
	
	int i = 0;
	index_task = 0;
	
	for (i = 0; i < dev_list->dev_num; i++)
	{
		int dev_index = thread_pool->thread[index_task].dev_num;
		thread_pool->thread[index_task].dev_list[dev_index] = &dev_list->dev_list[i];		
		//printf ("%d---%d--%u\n", index_task, i, dev_list->dev_list[i].dev_ip);
		/*printf ("<devaddr:%p=devindex:%lu--%p=%lu>\n", 
		&dev_list->dev_list[i], 
		dev_list->dev_list[i].dev_index, 
		thread_pool->thread[index_task].dev_list[dev_index],
		thread_pool->thread[index_task].dev_list[dev_index]->dev_index);
		*/
		  thread_pool->thread[index_task].dev_num++;
		index_task = (index_task +  1) % config->tasknum;
	}
	return 1;
}

int load_file2msg(const char * szfile, CommandMessage * file_msg, const cm_client_config_t * config)
{
	if (!szfile) return 0;
	//struct stat file_stat;
	char szbuf[8192] = {0};
	unsigned  int nRead = 0;
	char base[128] = {0};
	file_msg->filedata.clear();	
		
	int nFd = open (szfile, O_RDONLY);
	if (nFd == -1) return -1;
	unsigned int size = 0;
	while ((nRead = read (nFd, szbuf, 8190)) > 0)
	{
		size += nRead;
		//printf(szbuf);
		for (unsigned int i = 0; i < nRead; i++)
		{
			file_msg->filedata.push_back(szbuf[i]);
		}
		memset (szbuf, 0, nRead);
	}
	close (nFd);
	file_msg->head.filesize 	= size;
	//---added by djg@2011-03-11 --start
	//file_msg->head.stmode		= file_stat.st_mode;
	//file_msg->head.uid		= file_stat.st_uid;
	//file_msg->head.gid		= file_stat.st_gid;
	//---added by djg@2011-03-11 --end
	sprintf (base, "%s", szfile);
	string str = string (my_basename(base));	
	file_msg->head.file 		= str;
	file_msg->head.dstfile 		= string(config->dst_file);
	file_msg->head.msgtype 	= MSG_TYPE_UP_FILE;
	
	//--added by duanjigang@2011-08-01 for login and user auth --start
	//file_msg->head.username = string (config.username);	
	//file_msg->head.login = string (g_client_config.login);	
	//--added by duanjigang@2011-08-01 for login and user auth --end
	
	
	//--added by djg@2011-03-30 -for file information --start
	char info_file[512] = {0};
	sprintf (info_file, "%s.info", szfile);	
#ifdef _WIN32
		if (access (info_file, _A_RDONLY))
#else
	if (access (info_file, R_OK))
#endif
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
	file_msg->fileinfo.clear();
	while ((nRead = read (nFd, szbuf, 8190)) > 0)
	{
		size += nRead;
		for (unsigned int i = 0; i < nRead; i++)
		{
			file_msg->fileinfo.push_back(szbuf[i]);
		}
		memset (szbuf, 0, nRead);
	}
	close (nFd);
	
	//--added by djg@2011-03-30 -for file information --end
ret:
	return 1;
}

