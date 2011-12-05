#ifndef _CM_CONFIG_H_
#define _CM_CONFIG_H_

#define DFT_PORT 43670
#define DFT_DM   0
#define DFT_MODE 1
#define DFT_THREAD 10
#define DEV_TYPE_CMD 1
#define DEV_TYPE_STAT 2
#define CM_IP_LIST	"/etc/cmserver.lst"
#define CMTK_CONF	"/etc/cmtk/cmtk_config.ini"
#define CONF_FILE 128 //config file name length
#define CONF_DIR 128 //directory name length
#define TASKNUM 20  // default number of tasks running
#define MODE_CMD 1  //command message mode for server
#define MODE_UPLOAD_FILE 2 //file transfer message mode for server
#define MODE_DOWNLOAD_FILE 4 //file upload added @2011-11-01

#define RUN_REAL 1 //run real time
#define RUN_BACK 2 //run background
#define CMD_LEN 256 //max length of each command

#define MAX_TASK_NUM 100 //max number of tasks
#define MAX_DEV 5000   //max device number
#define MAX_DEV_THREAD 500  //max device(s) for each thread or task
#define HOST_NAME 64

#include <sys/types.h>
#include <Ice/Ice.h>
#include <string>
#include "NetHelper.h"

#include <stdarg.h>
#include "cmtkp.h"
#ifdef __LINUX__
#include <pthread.h>
#endif
#define STR_IP(str, n) sprintf((str), "%u.%u.%u.%u", (n)  & 0xff, ((n)>> 8) & 0xff, ((n)>>16) & 0xff, ((n)>>24) & 0xff)
using namespace std;
using namespace cmtkp;


#define MAX_LOCAL_ADDR 16
#define USER_NAME 64
typedef struct 
{
	char		conf[CONF_FILE];	//where the ip list stored when we want to run a batch command
	unsigned short 	remote_port;		//which port to connect 
	unsigned short 	forbid_root;		//forbid root login or run flag
	unsigned short	tasknum;		//how many tasks are running
	unsigned short  color;			//print with color
	unsigned int	remote_addr;		//same as above
	char		remote_host_name[HOST_NAME];
	unsigned int	time_out;		//added @2011-05-18
	char		command[CMD_LEN];	//which command to run
	char		file[CONF_FILE];	//file to upload
	char		dst_file[CONF_DIR];		//where to upload the file on dest box
	char		mode;			//1 for cmd and 2 for file
	char		run_mode;		//1 for real time and 2 for background
	char		silent;			//1 store result in files or print it to stdout
	device_t	iplist[MAX_LOCAL_ADDR];
	unsigned int 	ipnum;
	//added by duanjigang@2011-08-01 --start
	char 		username[USER_NAME];
	char		login	[USER_NAME];
	//added by duanjigang@2011-08-01 --end
	//added by duanjigang@2011-08-18 --start
	unsigned short	forbidroot;
	//added by duanjigang@2011-08-18 --end
	
	//added by duanjigang1983@2011-11-01 --start for file fecth 
	char		remote_file [CONF_DIR]; //which to download
	char		local_file  [CONF_DIR]; //which to save
	//Ice::CommunicatorPtr  ic;
	CommandMessage * file_msg; 
	//added by duanjigang1983@2011-11-01 --finish for file fetch
}cm_client_config_t;

//device information
typedef  struct
{
	unsigned long index;
	unsigned int dev_ip;  	//device ip
	char		dev_name[HOST_NAME];
	unsigned long dev_index;
	bool	 over;		//is it run over ?
	int 	 nret;		//return value > 0 success else failed
	time_t	start_time;	//when it start to runing..
	time_t	finish_time;	//when it finish running..
	//::cmtkp::StringArray retmsg;//result of command
	::cmtkp::byteArray stret;
}cmdev_t;


//define for each task or thread
typedef struct 
{
	time_t		start_time; //when it began run
	time_t 		finish_time; //when it run over
	#ifdef __LINUX__
	pthread_t 	tid; //thread id or task id
	#else
	int		tid;
	#endif
	cmdev_t * 	dev_list[MAX_DEV_THREAD]; //all device(s) for this thread to serve
	int	  	dev_num;//number of device		
	unsigned char		over;//whether this thread run over ?
	int 		index; //index of this thread or task 
	cm_client_config_t * config;
}thread_t;

//define for thread pool entry
typedef struct 
{
	thread_t  thread[MAX_TASK_NUM]; //thread list 
	int	  th_num;//thread number
}thread_pool_t;

//define for device list
typedef struct
{
	cmdev_t dev_list[MAX_DEV]; //device list
	int   dev_num; //device number
}dev_list_t;


typedef struct
{
	char nothing[10];
	unsigned int ipaddr;
	int rand_num1;
	char 	user[64];
	char	hostname[64];
}ip_struct_t;

//added by duanjigang1983@gmail.com @2011-10-29 --start
typedef struct 
{
	char opt[10];
	char desc[30];
	unsigned char show;
}opt_help_t;
//added by duanjigang1983@gmail.com @2011-10-29 --end
//function to handle message for each device
typedef int(*host_func_t)(cmdev_t * dev, const cm_client_config_t * config);
extern 	int 	getopt(int nargc, char* const * nargv, const char* ostr);
extern 	int 	mkdir_rec(const char* path);
extern 	int 	init_parameters(int argc, char* argv[], cm_client_config_t * client_config);
extern 	int 	init_devlist(dev_list_t * dev_list, const cm_client_config_t *config);
extern 	int 	init_tasks(cm_client_config_t * config, CommandMessage * file_msg, 
			   thread_pool_t  * thread_pool, dev_list_t * dev_list);
extern 	void 	print_thread_list(const thread_pool_t  * thread_pool);
extern 	int	init_ice(int argc, char* argv[]);
extern 	int	GetNetip(unsigned int *ip);
extern 	int 	start_tasks(thread_pool_t  * thread_pool,  cm_client_config_t * config);
extern 	unsigned char	thread_run_over(const  thread_pool_t * thread_pool);
extern	void 	show_result (dev_list_t * devlist, const cm_client_config_t * config);
extern 	char * 	my_dirname (char * path);
extern 	char * 	my_basename (char * path);
extern 	int 	show_version (int argc, char* argv[]);
#define TASK_POOL_SIZE 200 //size of task pool
#endif
