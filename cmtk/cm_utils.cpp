#include "cmconfig.h"
#include "os_independent.h"


char * my_dirname (char * path)
{
	char * p = path, * q = path;
	while ((p = strchr(q, '/')))
	{
		q = p + 1;
	}
	if (*(q-1) == '/') *(q - 1) = '\0';
	return path;
}


char* my_basename (char* path)
{
	char * p = path, *q = path;
	
	while ((p = strchr (q, '/')))
	{
		q = p + 1;
	}
	
	if (*q && !*(q + 1)) 
	{
		*q-- = '\0';
		while ((q != path) && (*q != '/')) q--;
	}

	if (*q == '/') q++;
	return q;
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
#ifdef __LINUX__
			mkdir (szpath, 0777);
#else
			_mkdir (szpath);
#endif
			szpath[i] = '/';
		}
	}
	#ifdef __LINUX__
	mkdir (szpath, 0777);
#else
		mkdir (szpath);	
#endif
	return 1;
}
int 	show_version (int argc, char* argv[])
{
	printf ("%s version list:\n", argv[0]);
	printf ("\t v1.0 2011-10-29:create version by duanjigang1983\n");
	printf ("\t v1.1 2011-11-02:adding file fetching interface by duanjigang1983\n");
	printf ("\t v1.2 2011-11-26:simplify source code by duanjigang1983\n");
	printf ("\t v1.3 2011-12-08:porting to windows by duanjiang1983\n");
	return 0;;
}



