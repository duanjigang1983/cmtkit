#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <time.h>
#include "cmconfig.h"
#include "IniHelper.h"


int facility = LOG_LOCAL5;
int level = LOG_INFO; 
int log_on = 1;
int interval = 60;
int init_syslog()
{
	//openlog(se_merge_config.syslog_ident, LOG_PID, LOG_USER);
	char szbuf[128] = {0};

	CIniHelper helper(CMTK_CONF);
	if(helper.ErrorOccurence())
	{
		return 1;
	}

	log_on 		= 	helper.ReadInt("syslog", 	"on", 		1);	
	interval 	= 	helper.ReadInt("syslog",	"interval", 	60);	
	if (interval < 60) interval = 60;
	sprintf (szbuf, "%s",  helper.ReadString ("syslog",	"facility", 	"local5"));
	if (!strcmp(szbuf, "local1"))
	{
		facility = LOG_LOCAL1;
	}else
	if (!strcmp(szbuf, "local2"))
	{
		facility = LOG_LOCAL2;
	}else
	if (!strcmp(szbuf, "local3"))
	{
		facility = LOG_LOCAL3;
	}else
	if (!strcmp(szbuf, "local4"))
	{
		facility = LOG_LOCAL4;
	}else
	if (!strcmp(szbuf, "local5"))
	{
		facility = LOG_LOCAL5;
	}else
	if (!strcmp(szbuf, "local6"))
	{
		facility = LOG_LOCAL6;
	}else
	if (!strcmp(szbuf, "local7"))
	{
		facility = LOG_LOCAL7;
	}else facility = LOG_LOCAL5;


	memset (szbuf, 0, sizeof(szbuf));
	sprintf (szbuf, "%s",  get_conf ("cmtools/syslog/leve", "info"));
	
	if (!strcmp(szbuf, "emerg"))
	{
		level = LOG_EMERG;
	}else
	if (!strcmp(szbuf, "alert"))
	{
		level = LOG_ALERT;
	}else
	if (!strcmp(szbuf, "crit"))
	{
		level = LOG_CRIT;
	}else
	if (!strcmp(szbuf, "err"))
	{
		level = LOG_ERR;
	}else
	if (!strcmp(szbuf, "debug"))
	{
		level = LOG_DEBUG;
	}else
	if (!strcmp(szbuf, "warning"))
	{
		level = LOG_WARNING;
	}else
	if (!strcmp(szbuf, "notice"))
	{
		level = LOG_NOTICE;
	}else
	level = LOG_INFO;

	if (log_on)
	//openlog("CMTOOLS", LOG_PID, LOG_LOCAL5);
	openlog("CMTOOLS", LOG_PID, facility);
	return 1;
}
int syslog_msg(const char * szMsg,...)
{
	
	if (!log_on) return 1;
	//struct tm *tm1 = NULL;
	//char stime[30] = {0};
	time_t tmp_time = time(0);
	/*
	tm1 = gmtime (&tmp_time);
	if(tm1 == NULL) 
	{
		return -1;
	}
	sprintf (stime, 
			"%04d-%02d-%02d %02d:%02d:%02d", 
			tm1->tm_year + 1900,
			(tm1->tm_mon + 1)%12, 
			tm1->tm_mday, 
			(tm1->tm_hour + 8)%24, 
			tm1->tm_min, 
			tm1->tm_sec);
	*/
	char szline[4096] = {0};

	sprintf (szline, "%ld ", tmp_time);
	va_list args;
	va_start(args, szMsg);
	vsprintf(szline + strlen(szline), szMsg, args);
	va_end(args);	
	
	//syslog(LOG_LOCAL5|LOG_INFO, szline);
	syslog(facility|level, "%s", szline);
	printf("sysloging:%s\n", szline);
	return 1;
}
int close_syslog(void)
{
	if (log_on)
	closelog();
	return 1;
}
