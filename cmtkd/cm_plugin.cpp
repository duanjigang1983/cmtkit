#include "cm_plugin.h"
#include "IniHelper.h"
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "ColorDefine.h"

static cm_plugin_pkg_t * plist = NULL ;

static void free_plist (void)
{
	cm_plugin_pkg_t * p = plist;
	while (p)
	{
		cm_plugin_pkg_t * pq = p;
		cm_plugin_t * hd = p->plist;
		while (hd)
		{
			cm_plugin_t * q = hd;
			hd = hd->next;
			free (q);
		}
		p = p->next;
		free (pq);
	}
}
int load_plugin (const char* szpath)
{
	unsigned int new_dir = 0;
	cm_plugin_pkg_t * pdir =  plist;
	if (plist) 
	{
		free_plist ();
	}
	plist = NULL;
	plist = (cm_plugin_pkg_t*) malloc (sizeof(cm_plugin_pkg_t));
	if (!plist)
	{
		printf ("%s-%d:malloc cm_plugin_pkg_t failed\n", __FILE__, __LINE__);
		return -1;
	}
	memset (plist, 0, sizeof(cm_plugin_pkg_t));
	plist->finish = 0;
        strcpy (plist->path, szpath);	
	plist->plist = 0;
	plist->pnum = 0;
	plist->next = NULL;

	//load all dirs in plugin directory..
	do
	{
		cm_plugin_pkg_t * p =  plist;
		new_dir = 0;
	
		while (p)
		{
			do
			{
				DIR * dir = 0;    
                		struct dirent* drt = 0;
				//already check over
				if (p->finish) break;	
				//try to open dir
				if( (dir = opendir (p->path))== NULL )
                		{		   
                        		printf ("%s-%d:open dir '%s' failed\n", __FILE__, __LINE__, p->path);
                        		break;
               			}
				 
				while ((drt = readdir(dir)) != NULL)
                		{
                        		cm_plugin_pkg_t * ndir = NULL;
                        		if ((strcmp(drt->d_name, ".") == 0) || (strcmp(drt->d_name, "..") == 0))
                        		{
                                		continue;
                        		}
					
					if (drt->d_type != DT_DIR)
                        		{
                                		continue;
                        		}
					ndir = (cm_plugin_pkg_t*)malloc(sizeof(cm_plugin_pkg_t));
					if (!ndir)
					{
						printf ("%s-%d:malloc(cm_plugin_pkg_t) failed\n",__FILE__, __LINE__);
						continue;
					}
					ndir->finish = 0;
					ndir->plist = NULL;
					ndir->pnum = 0;
					sprintf (ndir->path, "%s/%s", p->path, drt->d_name);
					printf ("adding:"YELLOW"%s"NONE"\n", ndir->path);
					ndir->next = plist;
					plist = ndir;		
					new_dir++;
				}
				
			}while (false);
			p->finish = 1;	
			p = p ->next;
		}
		

	}while (new_dir > 0);

	//load all plugins in all dirs
	pdir = plist;		
	while (pdir)
	{
		char szini[256] = {0};
		char user_pkg [64] = {0};
		sprintf (szini, "%s/%s", pdir->path, CTL_FILE);
		do
		{
			section sc;
			char user_plugin[64] = {0};
			char name_plugin[64] = {0};
			char store_plugin[256] = {0};
			if (access (szini, R_OK))
			{
				printf ("can not find file "RED"'%s'"NONE"\n", szini);
				break;
			}
			printf ("load plugin from "GREEN"'%s'"NONE"\n", szini);
			//reading cmtk.ini
			
			CIniHelper helper (szini);
        		if (helper.ErrorOccurence())
        		{   
                		printf("%s-%d:open file '%s' failed\n", __FILE__, __LINE__, szini); 
                		break;
        		}   
			//construct user
			sprintf (user_pkg, "%s", helper.ReadString ("general", "user", "root"));
			while (helper.ReadSection(sc))
			{
				int i = 0;
				if (strcmp(sc.title, "general") == 0) continue;
                		for ( i = 0; i < sc.entry_number; i++)
                		{
					if (strncasecmp(sc.entry_list[i].var_name, "name", 4) == 0)
                        		{
                                		sprintf (name_plugin, "%s", sc.entry_list[i].var_value);
                                		continue;
                       		 	}
				}	
			}

		}while (false);	
		pdir = pdir->next;
	}
	return 1;
}

