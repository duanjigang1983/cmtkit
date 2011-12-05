#include "cmtkp.h"
#include "cmconfig.h"
#include "os_independent.h"

int main( int argc, char* argv[])
{
	dev_list_t * dev_list = new dev_list_t;
	cm_client_config_t * client_config = new cm_client_config_t;
	 
	thread_pool_t * thread_pool = new thread_pool_t;
	memset (client_config, 0, sizeof(cm_client_config_t));
	//parse parameters and init config for cmtk
	if (init_parameters(argc, argv, client_config) <= 0)
	{
		return -1;
	}
	client_config->file_msg =  new CommandMessage;
	//init the ice object
	init_ice(argc, argv);
	//load devices from config file
	if (init_devlist (dev_list, client_config) <= 0)
	{
		return -1;
	}
	
	//assign device of dev_list to each thread in thread pool
	if (init_tasks(client_config, client_config->file_msg, thread_pool, dev_list) <= 0)
	{
		return -1;
	}
	//print the distribution of all devices on each thread of thread_pool 
	#if 0
	print_thread_list(thread_pool);
	#endif
	//start all thread
	start_tasks(thread_pool, client_config);

	//waiting for thread to run over
	while (!thread_run_over(thread_pool))
	{
		SLEEPM(10);
	}

	//then show result
	show_result (dev_list, client_config);
	
	return 0;
}
