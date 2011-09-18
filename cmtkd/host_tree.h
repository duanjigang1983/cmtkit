#ifndef _HOST_TREE
#define _HOST_TREE
typedef struct _auth_t
{
	char * szuser;
	int length;	
	unsigned int iplist[256];
	unsigned int ipnum;
	struct _auth_t * next;
}auth_t;
extern int init_auth_list (void);
extern bool find_auth_info (unsigned int addr, const char* szuser, const char* szlogin);
extern int add_auth_info (unsigned int addr, const char* user);
#endif
