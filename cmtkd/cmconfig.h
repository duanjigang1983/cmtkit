#ifndef _CM_CONFIG_H_
#define _CM_CONFIG_H_

#define DFT_PORT 43699
#define DFT_DM   0
#define DFT_MODE 1
#define DFT_THREAD 10
#define DEV_TYPE_CMD 1
#define DEV_TYPE_STAT 2
#define CM_IP_LIST	"/etc/cmserver.lst"
#define CMTKD_CONF 	"/etc/cmtk/cmtkd_config.ini"

#include <sys/types.h>
#include <Ice/Ice.h>
#include <string>
#include "NetHelper.h"

#include <stdarg.h>
#include "cmdhelper.h"
#ifdef __LINUX__
#include <pthread.h>
#endif
#define STR_IP(str, n) sprintf((str), "%u.%u.%u.%u", (n)  & 0xff, ((n)>> 8) & 0xff, ((n)>>16) & 0xff, ((n)>>24) & 0xff)
using namespace std;
using namespace cmdhelper;

//define struct of configuration for cmserver
typedef struct
{
	unsigned short 	port;
	unsigned short 	daemon;
	unsigned short 	mode; // 1 for popen 2 for system
	unsigned short	threadnum;//number of working threads
	unsigned short	devtype;	//1 for master 2 for slave
	unsigned short  active; //update task active
	unsigned short	interval;// heart beat interval
	unsigned short  auth_on;//
}cm_server_config_t;

#define CONF_FILE 128 //config file name length
#define CONF_DIR 128 //directory name length
#define TASKNUM 20  // default number of tasks running
#define MODE_CMD 1  //command message mode for server
#define MODE_FILE 2 //file transfer message mode for server

#define RUN_REAL 1 //run real time
#define RUN_BACK 2 //run background
#define CMD_LEN 256 //max length of each command

#define MAX_TASK_NUM 100 //max number of tasks
#define MAX_DEV 5000   //max device number
#define MAX_DEV_THREAD 500  //max device(s) for each thread or task
#define HOST_NAME 64
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
	::cmdhelper::StringArray retmsg;//result of command
}cmdev_t;
//function to handle message for each device
typedef int(*host_func_t)(cmdev_t * dev);
//define for device list
typedef struct
{
	cmdev_t dev_list[MAX_DEV]; //device list
	int   dev_num; //device number
}dev_list_t;
//thread status, maybe useless
enum status 
{
	status_ready = 1,
	status_working = 2,
	status_over = 3
};

//task define for server host 
typedef struct _task_t 
{
	unsigned int 	cmdid; 			//command id
	unsigned int 	heart_time_out;		//time out for heart beat message 
	unsigned int  	total_time_out; 	//total time out for this task
	unsigned int 	status; 		//status of this task node
	unsigned int	result;			//result of this task
	unsigned int	stat_server_addr;	// address of stat server
	unsigned int 	local_addr;
	unsigned int	taskid;
	unsigned short 	stat_server_port;	// port of stat server	
	time_t 		born_time;		//time when this task node is born
	time_t 		run_time;		//start run time
	time_t 		over_time;		//run over time

	struct _task_t * next;			//where next node is stored
	//define for command and result
	string   cmd_data;			//what command to run
	::cmdhelper::StringArray cmd_ret;	//what the command runs for ?
}task_t;

//define for task list
typedef struct
{
	task_t * list; //task list
	#ifdef __LINUX__
	pthread_mutex_t mutex; //mutex
	#else
	int		mutex;
	#endif
}task_list_t;

typedef struct
{
	char nothing[10];
	unsigned int ipaddr;
	int rand_num1;
	char 	user[64];
	char	hostname[64];
}ip_struct_t;

char*	get_conf(const char* path, const char* dft);
extern int 	mkdir_rec(const char* path);
#define TASK_POOL_SIZE 200 //size of task pool
#endif
