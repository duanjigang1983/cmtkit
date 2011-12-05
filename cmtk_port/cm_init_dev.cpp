#include "cmconfig.h"
int init_devlist(dev_list_t * dev_list, const cm_client_config_t *config)
{
	char szline[128] = {0};
	FILE * fp = 0;
	dev_list->dev_num = 0; 
	//just single host 
	if (config->remote_addr > 0)
	{
		dev_list->dev_list[0].dev_ip = config->remote_addr;
		strncpy (dev_list->dev_list[0].dev_name, config->remote_host_name, strlen(config->remote_host_name));
		dev_list->dev_num = 1;
		return 1;	
	}
	
	//ip list is stored in a txt file, try to open
	fp = fopen (config->conf, "r");
	if (!fp)
	{
		printf ("%s-%d:open file '%s' failed\n", __FILE__, __LINE__, config->conf);
		return -1;
	}
	
	//read each line to parse a host addr and store it
	while (fgets(szline, 128, fp))
	{
		int len = 0;
		int dev_index = 0;
		struct hostent * host = 0;
		if (strlen(szline) <= strlen("\n")) continue;
		if (szline[0] == '#') continue;
		
		len = strlen (szline);
		
		if (szline[len - 1] == '\n')
		{
			szline[len-1] = '\0';
		}
		
		//we ought to convert an domain name to ip addr --added by cme@2011-06-04 --start
		host = gethostbyname(szline);
		
		if (!host) continue;
		dev_index = dev_list->dev_num;
		dev_list->dev_list[dev_index].dev_ip = inet_addr(inet_ntoa(*((struct in_addr *)host->h_addr)));
		int slen = strlen (szline);
		if (slen > HOST_NAME) slen = HOST_NAME;
		strncpy (dev_list->dev_list[dev_index].dev_name, szline, slen);
		//printf ("%s-%u-%s\n", szline, dev_list->dev_list[dev_index].dev_ip, dev_list->dev_list[dev_index].dev_name);
		//get ip addr	
		if (!dev_list->dev_list[dev_index].dev_ip)
		{
			continue;
		}
	
		dev_list->dev_list[dev_list->dev_num].dev_index = dev_index + 1;
		dev_list->dev_num++;
		//check overflow	
		if (dev_list->dev_num >= MAX_DEV)
		{
			printf ("too many host to load [>%d]\n", MAX_DEV);
			break;
		}
		memset (szline, 0, 128);
	}
	fclose (fp);
	//return number of devices
	return dev_list->dev_num ;
}
