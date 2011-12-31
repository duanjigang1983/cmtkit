#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "cmconfig.h"
/*
char*	get_conf(const char* path, const char* dft)
{
	static char szret[256] = {0};
	char szpath[512] = {0}, szpath1[512] = {0};
	memset (szret, 0, sizeof(szret));
	if (strlen(path) > 512)
	{
		printf ("path too long %u\n", (unsigned int)strlen(path));
		sprintf (szret, "%s", dft);
		return szret;
	}	
	sprintf (szpath, "/etc/%s", path);
	int len = strlen(szpath);
	int i = 0;
	for (i = 0; i < len; i++)
	{
		if (szpath[i] == '.') szpath[i] = '/';
	}
	//printf ("%s:%d\n", szpath, strlen(szpath));
	if (access(szpath, R_OK))
	{
		char szdir[512] = {0};
		strcpy (szpath1, szpath);
		sprintf (szdir, "%s", dirname(szpath1));
		mkdir_rec(szdir);
		FILE * fp  = fopen (szpath, "w");
		if (fp)
		{
			fprintf (fp, "%s", dft);
			fclose (fp);
		}
		sprintf (szret, "%s", dft);
		return szret;
	}
	int ok = 1;
	FILE * fp = fopen (szpath, "r");
	if (fp)
	{
		if (fgets(szret, sizeof(szret), fp))
		{
			fclose (fp);
			return szret;
		}else
		{
			ok = 0;
		}
	}else ok = 0;
	
	if (!ok)
	{

		FILE * fp  = fopen (szpath, "w");
		if (fp)
		{
			fprintf (fp, "%s", dft);
			fclose (fp);
		}
		sprintf (szret, "%s", dft);
	}
	return szret;
}*/
int 	mkdir_rec(const char* path)
{
	char szpath[512] = {0};
	sprintf (szpath, "%s", path);
	int len = strlen(szpath);
	for (int  i = 1; i < len; i++)
	{
		if (szpath[i] == '/')
		{
			szpath[i] = '\0';
			mkdir (szpath, 0777);
			szpath[i] = '/';
		}
	}
	mkdir (szpath, 0777);
	return 1;
}
int daemon_func (int nochdir, int noclose)
{
	pid_t pid;
	pid = fork ();
	if (pid < 0)
	{
		perror ("fork");
		return -1;
	}
	if (pid != 0)
	exit (0);
	pid = setsid();
	if (pid < -1)
	{
		perror ("setsid");
		return -1;
	}
	if (! nochdir)chdir ("/");
	if (! noclose)
	{
		int fd = 0;
		fd = open ("/dev/null", O_RDWR, 0);
		if (fd != -1)
		{
			dup2 (fd, STDIN_FILENO);
			dup2 (fd, STDOUT_FILENO);
			dup2 (fd, STDERR_FILENO);
			if (fd > 2)close (fd);
		}
	}
	
	umask (0027);		
	return 0;
}
char * find_bin (const char* name)
{
	static char ret[512] = {0};
	char * p = NULL, * q = NULL;
	char path[2048] = {0};
	unsigned char find = 0;
	memset (ret, 0, 512);
	char * szfind = getenv ("PATH");
	if (!szfind) return NULL;
	strcpy (path, szfind);
	p = path;
	while (p && !find)
	{
		q = strchr (p, ':');
		if (q) *q = '\0';	
		memset (ret, 0, 512);
		sprintf (ret, "%s/%s", p, name);
		if (!access(ret, F_OK))
		{
			find = 1;
			break;
		}
		if(!q) break;
		p = q +1;	
	}
	if (find) return ret;
	return NULL;
} 

