#include "cm_plugin.h"
#include "cmconfig.h"
#include "IniHelper.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include "ColorDefine.h"

static cm_plugin_pkg_t * plist = NULL ;

void free_makeargv(char **argv);
int32_t get_userid (const char* name, uid_t * uid);
int32_t makeargv(const char *s, char ***argvp);
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

void adjust_path (void)
{
	cm_plugin_pkg_t * pdir = plist;
	while (pdir)
	{
		do
		{
			char szline[1024] = {0};
			sprintf (szline, pdir->path);
			memset (pdir->path, 0, sizeof(pdir->path));
			if (strlen(szline) > strlen(g_server_config.plugin_dir))
			strcpy (pdir->path, szline + strlen(g_server_config.plugin_dir)+1);
			
		}while(false);
		pdir = pdir->next;
	}
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
			printf ("[%s]:\n", pdir->path);
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
int  run_limit_cmd (const char* szline, const char* szfile)
{
	cm_plugin_pkg_t * pdir = plist;
	cm_plugin_t * prun = NULL;
	unsigned int len = 0;
	unsigned char find = 0;
	char szbuf[1024] = {0};
	char szdir[512] = {0};
	char szname[256] = {0};
	if (!szline)
	{
		printf ("EXIT:%s-%d\n", __FILE__,__LINE__);
		 return 0;
	}
	strcpy (szbuf, szline);
	len = strlen (szbuf);
	unsigned int dir_len = 0;
	if (1)
	{
		char * bl = strchr (szbuf, ' ');
		if (bl) *bl = '\0';
		char *tmpdir = dirname (szbuf);
		if (tmpdir)
		{
			strcpy (szdir, tmpdir);
			if (szdir[0] == '.') memset(szdir, 0, sizeof(szdir));
		}
	}
	
	dir_len = strlen (szdir);
	memset (szbuf, '\0', sizeof(szbuf));
	strcpy (szbuf, szline);
	//strcat (szbuf, "\0");
	char * pstart = szbuf + strlen(szdir);
	while (pstart && *pstart && *pstart == '/')
	 pstart++;
	char * finish = pstart;
	unsigned char loop = 1;
	while (finish && !isspace(*finish) && *finish)finish++;
	
	while (pdir && loop)
	{
		printf ("loop:%s-%u-%u\n", pdir->path, dir_len, (unsigned)strlen(pdir->path));
		do
		{
			if (dir_len != strlen(pdir->path))break;
			cm_plugin_t * pp = pdir->plist;
			printf ("in package:%s\n", pdir->path);
			while (pp && loop)
			{
				do
				{
					if (strlen(pp->name) != (unsigned int)(finish - pstart)) break;
					if (strncmp(pp->name, pstart, (unsigned int)(finish - pstart))) break;
					printf ("<%s-%s-%u>\n", pp->name, pstart, (unsigned int)(finish - pstart));
					find = 1;
					prun = pp;
					loop = 0;
					break;					
				}while (false);
				pp = pp->next;
			}											
			
		}while (false);
		pdir = pdir->next;
	}
	//find plugin
	if (prun)
	{
		uid_t uid = 0;
		strcpy (szname, prun->store);
		if( get_userid (prun->user, &uid) <= 0)
        	{
                	printf ("get user id of '%s' failed\n", prun->user);
                	return -1;
        	}
		int pid = fork();
		if (!pid)
		{
			const char * p = pstart;
			char  name[128] = {0};
			char ** argv_list = NULL;
			char ** argvs = NULL;
			int32_t ntok = 0;
			int32_t i = 0;
			int32_t size = 0;

			int nFd =  open (szfile, O_CREAT|O_RDWR, 0644);	
			if (nFd == -1)
			{
				printf ("%s-%d:open file '%s' for writing failed\n", __FILE__, __LINE__, szfile);
				exit (0);
			}
			dup2(nFd, 2);
			dup2(nFd, 1);

			//char* envp[]  = {"env", NULL};
			if (setuid(uid))
			{
				printf ("setuid(%u) failed\n", uid);
				exit (0);
			}
			while (!isspace(*p) && *p)
			{
				p++;
			}	
			//dup2 (1, 2);
			sprintf (name, "%s", basename(szbuf));
			if (*p)
			{	
				//printf("[%s]\n", p);
				ntok = makeargv (p, &argv_list);
			    	/*
				for (i = 0; i < ntok; i++)
				{
					printf ("token[%d]=%s[%u]\n", i, argv_list[i], strlen(argv_list[i]));
				}*/
			}else  
			{
				//printf ("================no parameter======\n");
				ntok = 0;
			}
			if (ntok < 0 )
			{
				printf ("error parameter:[%s]\n", p);
				exit (0);
			}
			size = (ntok > 0) ? ntok : 0; 
			argvs  = (char**)malloc (sizeof(char*)*(size + 2));
			if (!argvs)
			{
				printf ("malloc failed\n");
				exit (0);
			}
			memset (argvs, 0, sizeof(char*)*(size + 2));
			argvs[0] = name;
		
			for (i = 0; i < size; i++)	
			argvs [i + 1] = argv_list [i];
		
			argvs [size + 1] = NULL;

			//memset (szbuf, 0, sizeof(szbuf));
			//if (execvp(szname, argvs) < 0)
			if (access (szname, X_OK))
			{
				perror("");
				exit (0);
			}
			//printf ("EXE:%s\n", szname);
			if (execvp(szname,  argvs) < 0)
			{
				perror("Error on execv:");
			}
			//close (fd[1]);
			close (nFd);
			free(argvs);
			if (ntok >= 0)
			free_makeargv (argv_list);
			exit(0);
		}else
		{
			wait (NULL);
		}
	}else
	{
		printf ("can not find command:%s-%d\n", __FILE__,__LINE__);
		return -1;
	}
	return 1;
}
int32_t makeargv(const char *s, char ***argvp)
{
        const char *snew;
        const char *pch;
        size_t size;
        char *t;
        char *pnext;
        char quote = 0;
        int32_t numtokens;
        int32_t i;

        if ((s == NULL) || (argvp == NULL))
        {
		//printf ("%s-%d\n", __FILE__, __LINE__);
                errno = EINVAL;
                return -1;
        }

        /* snew is real start of the string, tricky? */
        snew = s + strspn(s, " \t");
        size = strlen(snew) + 1;

        if ((t = (char*)malloc(size)) == NULL)
	{
		//printf ("%s-%d\n", __FILE__, __LINE__);
                return -1;
	}
	for (pch = s, pnext = t, numtokens = 1; *pch; pch++)
        {
                switch (*pch) 
		{
                	case '\\':
                        	++pch;
                        	if (*pch == '\0') break;
                        		switch (*pch) 
					{
                                		default:
                                		*pnext++ = *pch;
                                		break;
                                        }
                         	break;
                	case '"':
                	case '\'':
                        	quote = quote ? '\0' : *pch;
                        	break;
                        case ' ':
                        case '\t':
                        
				if (quote) 
				{	
					*pnext++ = *pch;
         			} else
				{
          				// skip continuing white-space 
          				if (pnext > t && *(pnext - 1) != '\0') 
					{
            					*pnext++ = '\0';
            					++numtokens;
          				}
        			}
        		break;
      			default:
        			*pnext++ = *pch;
        		break;
    		}
  	}

	if (quote) 
	{
		
		//printf ("%s-%d\n", __FILE__, __LINE__);
    		return -1;
  	}

  	// create argument array for ptrs to the tokens 
  	if ((*argvp = (char**)malloc((numtokens + 1) * sizeof(char *))) == NULL) 
	{
   		int error = errno;
    		free(t);
    		errno = error;
		//printf ("%s-%d\n", __FILE__, __LINE__);
    		return -1;
  	}

  	
	for (i = 0, pnext = t; i < numtokens; i++, pnext = pnext + strlen(pnext) + 1)
    	{
		*((*argvp) + i) = pnext;
  	}
	*((*argvp) + numtokens) = NULL;

  	return numtokens;
}
void free_makeargv(char **argv) 
{
        if (argv)
        {
                if (*argv != NULL)
                free(*argv);
                free(argv);
        }
}

int32_t get_userid (const char* name, uid_t * uid)
{
	char szline[1024] = {0};
	u_int8_t find = 0;
	FILE * fp = fopen ("/etc/passwd", "r");
	if (!fp) return -1;
	
	while (fgets (szline ,1024, fp))
	{
		char fname[128] = {0},szuid[10] = {0};
		char * p1, * p2, *p3;

		p1 = strchr (szline, ':');
		if (!p1)continue;
		p2 = strchr (p1+1, ':');
		if (!p2)continue;
		p3 = strchr (p2+1, ':');
		if (!p3) continue;
		strncpy (fname, szline, p1-szline);
		if (strcmp(name, fname)) continue;
		strncpy(szuid, p2 + 1, p3-p2-1);
		//printf ("===%s-%d\n", uid, strlen(uid));
		*uid = atoi(szuid);
		find  = 1;
		break;	
	}
	return find ? 1:-1;
}

