#ifndef _UTILS_H_
#define _UTILS_H_

#ifdef __LINUX__
#include <sys/socket.h>
#include <libgen.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#endif

#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <Ice/Ice.h>
#include <sys/types.h>
#include "cmdhelper.h"
#include "constdef.h"
#include "cmconfig.h"
using namespace std;
using namespace cmdhelper;

extern int 	init_parameters(int argc, char* argv[]);
extern int 	init_devlist(void);
extern int 	init_tasks(void);
extern void 	print_thread_list(void);
extern int 	init_ice(int argc, char* argv[]);
extern int 	start_tasks(void);
extern void *	work_thread_func(void *arg);
extern bool	run_over(void);
extern void	show_results(void);
extern char *	get_username(uid_t uid);

#endif
