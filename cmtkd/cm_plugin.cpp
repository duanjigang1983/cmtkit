#include "cm_plugin.h"
#include "cmconfig.h"
#include "IniHelper.h"
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include "ColorDefine.h"

static cm_plugin_pkg_t * plist = NULL ;
static cm_plugin_t * g_find_plugin = NULL;

extern cm_server_config_t g_server_config;
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
				char user_plugin[64] = {0};
				char name_plugin[64] = {0};
				char store_plugin[256] = {0};
				char desc_plugin [100] = {0};
				unsigned short enable = 0;
				cm_plugin_t * np = NULL;
				if (strcmp(sc.title, "general") == 0) continue;
                		for ( i = 0; i < sc.entry_number; i++)
                		{

					//plugin name
					if (strncasecmp(sc.entry_list[i].var_name, "name", 4) == 0)
                        		{
                                		sprintf (name_plugin, "%s", sc.entry_list[i].var_value);
                                		continue;
                       		 	}

					//plugin description
					if (strncasecmp(sc.entry_list[i].var_name, "desc", 4) == 0)
                        		{
                                		sprintf (desc_plugin, "%s", sc.entry_list[i].var_value);
                                		continue;
                       		 	}
					//run user
					if (strncasecmp(sc.entry_list[i].var_name, "user", 4) == 0)
                        		{
                                		sprintf (user_plugin, "%s", sc.entry_list[i].var_value);
                                		continue;
                       		 	}
					//whether it is a link of a system command
					if (strncasecmp(sc.entry_list[i].var_name, "cmdpath", 7) == 0)
                        		{
                                		sprintf (store_plugin, "%s", sc.entry_list[i].var_value);
                                		continue;
                       		 	}

					//siwth on or off
					if (strncasecmp(sc.entry_list[i].var_name, "enable", 6) == 0)
                        		{
                                		enable = atoi(sc.entry_list[i].var_value);
                                		continue;
                       		 	}
				}//end of each var
				if (!enable) continue;	
				if (strlen(desc_plugin) == 0) continue;
				if (strlen (name_plugin) == 0) continue;
				if (strlen(user_plugin) == 0) strcpy (user_plugin, user_pkg);
				if (strlen(store_plugin) == 0) 
				sprintf (store_plugin, "%s/%s", pdir->path, name_plugin);
				
				np = (cm_plugin_t *) malloc (sizeof(cm_plugin_t));
				if (!np)
				{
					printf ("%s-%d:malloc(sizeof(cm_plugin_t))failed\n", __FILE__, __LINE__);
					continue;
				}
				memset (np, 0, sizeof(cm_plugin_t));
				np->store = (char*)malloc(sizeof(char)*strlen(store_plugin)+1);
				if (!np->store)
				{
					printf ("%s-%d:malloc(cm_plugin_t->store)failed\n", __FILE__, __LINE__);
					free (np);
					continue;

				}
				np->next = NULL;
				memset (np->store, 0, sizeof(char)*strlen(store_plugin)+1);
				strncpy (np->store, store_plugin, strlen(store_plugin));	
				unsigned int len = strlen (name_plugin);
				if (len >= sizeof(np->name))
				len = sizeof(np->name);
				
				strncpy (np->name, name_plugin, len);
				len = strlen (desc_plugin);
				if (len >= sizeof(np->desc));
				len = sizeof(np->desc);
				strncpy (np->desc, desc_plugin, len);

				len = strlen (user_plugin);
				if (len >= sizeof(np->user))
				len  = sizeof(np->user);
				strncpy (np->user, user_plugin, len);

				if (!pdir->plist)
				{
					pdir->plist = np; 
					pdir->pnum = 1;
				}else
				{
					np->next = pdir->plist;
					pdir->plist = np;
					pdir->pnum++;
				}
				//printf (GREEN"%s-%s-%s-%s"NONE"\n", name_plugin, desc_plugin, user_plugin, store_plugin);
			}

		}while (false);	
		pdir = pdir->next;
	}
	return 1;
}

void show_plist (void)
{
	cm_plugin_pkg_t * pdir = plist;
	while (pdir)
	{
		do
		{
			cm_plugin_t * pp = pdir->plist;
			if (pdir->pnum == 0) break;
			printf ("%s:\n", pdir->path);
			while (pp)
			{
				do
				{
					printf ("name:"GREEN"%s"NONE",desc:"GREEN"%s"NONE",user:"GREEN"%s"NONE",store:"GREEN"%s"NONE"\n",
					pp->name, pp->desc, pp->user, pp->store);
				}while (false);
				pp = pp->next;
			}
			
		}while (false);
		pdir = pdir->next;	
	}	
}
int  search_cmd (const char* szcmd)
{
	int nRet = -1;
	char szbuf[1024] = {0};
	char szdir[1024] = {0};	
	char * end = 0;
	unsigned int plen = strlen(g_server_config.plugin_dir);
	g_find_plugin = NULL;
	strcpy (szbuf, szcmd);
	char * pdir = dirname (szbuf);
	if (!pdir) return -1;
	strcpy (szdir, pdir);
	memset (szbuf, 0, 1024);
	strcpy (szbuf, szcmd);
	end = szbuf + strlen(szdir);
	while (end && !isspace(*end)) end++;
	if (!end) end = szbuf + strlen(szbuf);
	cm_plugin_pkg_t * pkg = plist;
	while (pkg)
	{
		do
		{
			const char* pstart = pkg->path + plen;
			cm_plugin_t * pp = pkg->plist;
			if (strcmp(pstart, szdir)) break;
			while(pp)
			{
				do
				{
					end = '\0';
					const char * name = pstart + strlen(szdir)+1;
					if (strcmp(name, pp->name))break;			
					g_find_plugin = pp; return 1;		
				}while (false);
				pp = pp->next;
			}
		}while (false);
		pkg = pkg->next;
	}

	
	return nRet;
}
