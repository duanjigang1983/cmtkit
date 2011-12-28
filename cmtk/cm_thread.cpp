#include "cmconfig.h"

extern host_func_t g_host_func;
//工作线程函数体
#ifdef __LINUX__
void* 
#else
DWORD WINAPI
#endif
work_thread_func(
#ifdef _WIN32
		   LPVOID
#else
		   void*
#endif
data)
{
	
#ifdef _WIN32

#else
	pthread_detach(pthread_self());
#endif
	thread_t * pt = (thread_t*)data;
	
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
			pt->dev_list[i]->nret = g_host_func(pt->dev_list[i], pt->config);
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
#ifdef __LINUX__
	pthread_exit(0);
#endif
	
	return 0;
  }

int start_tasks(thread_pool_t  * thread_pool,  cm_client_config_t * config)
{
	int i = 0;
	int j = 0; 
	
	//g_command_id = time (0);	
	config->ipnum = GetNetip(config->iplist);
	
	if (config->ipnum <= 0)
	{
		printf ("get ip list of local device failed\n");
		return -1;
	}
	
	srand(time(0));
	//init status of each node
	for (i = 0; i < thread_pool->th_num; i++)
	{
		thread_pool->thread[i].over = false;
		thread_pool->thread[i].config = config;
		
		for (j = 0; j < thread_pool->thread[i].dev_num; j++)
		{
			thread_pool->thread[i].dev_list[j]->over = false;
		}
	}	
	//then, try to start all threads
	
	for (i = 0; i < thread_pool->th_num; i++)
	{
		thread_pool->thread[i].start_time = time (0);
		thread_pool->thread[i].index = i;
#ifdef _WIN32
		DWORD dwThreadID;
		if(CreateThread(NULL, 0, work_thread_func, (LPVOID)&thread_pool->thread[i], 
			0, &dwThreadID) == NULL)
#else
		if (pthread_create (&thread_pool->thread[i].tid, 0, work_thread_func, 
			(void*)&thread_pool->thread[i]))
#endif
		{
			printf ("creat thread_%d failed\n", i);
			return -1;	
		}else
		{
			//printf ("starting thread_%d success\n", i);
		}

	}
	return 1;
}

void print_thread_list(const thread_pool_t  * thread_pool)
{
	int i = 0;
	for (i = 0; i < thread_pool->th_num; i++)
	{
		//printf ("task_%d has %d host(s):\n", i, thread_pool->thread[i].dev_num);
		int j = 0;
		for (j = 0; j < thread_pool->thread[i].dev_num; j++)
		{
			char szip[20] = {0};
			STR_IP(szip, thread_pool->thread[i].dev_list[j]->dev_ip);
			printf ("%s(%s)\n", szip, thread_pool->thread[i].dev_list[j]->dev_name);
		}
		printf ("==========================\n");
	}	
}

unsigned char	thread_run_over(const  thread_pool_t * thread_pool)
{
	int i = 0;
	
	for (i = 0; i < thread_pool->th_num; i++)
	{
		if (!thread_pool->thread[i].over)	
		{
			return 0;
		}
	}
	return 1;
}

