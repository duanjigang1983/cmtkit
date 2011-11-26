#include "utils.h"

//need some input from stdin
//such as 
//	-p 12345 --remote port is 12345
//	-h 192.168.0.1	-- remote host is 192.168.0.1
//	-P 123  --local stat server port for collect return infor
//	-H 127.0.0.1 --address of local stat server
//	-f ip.conf when there is a command for many host to run, we store the address list into a config file
//	--added by djg@2011-03-08
//	-u update a file
extern dev_list_t g_dev_list;
int main (int argc, char* argv[])
{

	int nRet = 0;
	
	//init parameter
	if (init_parameters (argc, argv) <= 0)
	{
		printf ("%s-%d:init_parameters failed\n", __FILE__, __LINE__);
		return 1;
	}

	//init device list
	if ((nRet = init_devlist ( )) <= 0)
	{
		printf ("%s-%d:init_devlist failed\n", __FILE__, __LINE__);
		return 1;
	}

	//init tasks, load command or file content
	if (init_tasks() <= 0)
	{
		printf ("%s-%d:init_tasks failed\n", __FILE__, __LINE__);
		return 1;
	}
	
	#if 0
		print_thread_list();
	#endif
	init_ice (argc, argv );

		
	
	//init ice object
	
	if (start_tasks ( ) <= 0)
	{
		printf ("%s-%d:start tasks failed\n", __FILE__, __LINE__);
		return 1;
	}
	//waitting all tasks to run over	
	while (!run_over ())
	{
		usleep(10);
	}
	//print all results
	show_results ( );
	return 1;
}
