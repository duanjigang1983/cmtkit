#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "cmconfig.h"

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
}
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

