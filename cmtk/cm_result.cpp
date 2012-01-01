#include "cmconfig.h"
#include "constdef.h"

void show_result_silent (dev_list_t * devlist, const cm_client_config_t * config);
void show_result_obvious (dev_list_t * devlist, const cm_client_config_t * config);


const char * error_msg(int errcode)
{
	switch (errcode)
	{
	case RET_SUCCES: return "success";
	case RET_CONNECT_FAILED: return "connect host failed";
	case RET_RUNCMD_FAILED: return "run command failed";
	default: return "unkown error";
	}
}

void show_result (dev_list_t * devlist, const cm_client_config_t * config)
{
	config->silent ? show_result_silent(devlist, config) : show_result_obvious(devlist, config); 
}

void show_result_obvious (dev_list_t * devlist, const cm_client_config_t * config)
{
	int i = 0;
	if (config->mode & MODE_CMD)
	{
		printf ("run command [%s]:\n", config->command);
	}
	
	if (config->mode & MODE_UPLOAD_FILE)
	{
		printf ("upload file [%s] to destination [%s]:\n", config->file, config->dst_file);
	}
	
	if (config->mode & MODE_DOWNLOAD_FILE)
	{
		printf ("download from [%s] to local [%s]:\n", config->remote_file, config->local_file);
	}
	
	
	for (i = 0; i < devlist->dev_num; i++)
	{
		char szip[20] = {0};
		cmdev_t *p = devlist->dev_list + i;
		STR_IP (szip, p->dev_ip);	
			printf ("(%s):[%s]\n", p->dev_name,  szip);
		
		if (config->mode & MODE_CMD)
		{
			for (unsigned int j = 0; j < p->stret.size(); j++)
				printf("%c", p->stret[j]);
			

			if (!p->stret.size() && (p->nret <= 0)) printf ("%s\n", error_msg(p->nret));
		}else if (config->mode & MODE_UPLOAD_FILE)
		{
			if (p->nret > 0) printf ("success\n");
			//else printf ("failed[%d]\n", p->nret);
			else
			for (unsigned int j = 0; j < p->stret.size(); j++)
				printf("%c", p->stret[j]);

		}else
		{
			if (p->nret > 0) printf ("success\n");
			else
			//else printf ("failed[%d]\n", p->nret);
		for (unsigned int j = 0; j < p->stret.size(); j++)
				printf("%c", p->stret[j]);

		}
		//printf ("\n");
	}
	
}

void show_result_silent (dev_list_t * devlist, const cm_client_config_t * config)
{
	int i = 0;
	char szpath[64] = {0};
	unsigned int pid = GETPID;
	mkdir_rec(RET_DIR);
	sprintf (szpath, "%s/%u", RET_DIR, pid);
	mkdir_rec(szpath);
	
	for (i = 0; i < devlist->dev_num; i++)
	{
		char szip[20] = {0};
		char szfile[100] = {0};
		cmdev_t *p = devlist->dev_list + i;
		STR_IP (szip, p->dev_ip);	
		sprintf (szfile, "%s/%u_%u.txt", szpath, p->dev_ip, i);
		printf ("[result_%u]\n", i);
		{
			printf ("index = %u\n", i+1);
			printf ("dev_ip = %s\n", szip);
			printf ("dev_name = %s\n", p->dev_name);
			printf ("save_path = %s\n", szfile);
			printf ("return =  %d\n", p->nret);
			printf ("start = %lu\n", p->start_time);
			printf ("finish = %lu\n\n", p->finish_time);
		}
		if ((config->mode & MODE_CMD) || (config->mode & MODE_DOWNLOAD_FILE))
		{
			FILE * fp = fopen (szfile, "w");
			if (fp)
			{
				//int size = p->retmsg.size();
				int size = p->stret.size();
				/*
				for (j = 0; j < size; j++)
				{
					fprintf (fp, "%s", p->retmsg[j].c_str());
				}*/
				for (unsigned int j = 0; j < p->stret.size(); j++)
					fprintf(fp, "%c", p->stret[j]);
				if (!size && (p->nret <= 0)) fprintf (fp, "%s\n", error_msg(p->nret));
				fclose (fp);
				chmod(szfile, 0666);
			}
		}else
		{
			FILE * fp = fopen (szfile, "w");
			if (fp)
			{
				fprintf (fp, "%s", error_msg(p->nret));
				fclose (fp);
			}
		}
	}
}
