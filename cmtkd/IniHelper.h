// IniHelper.h: interface for the CIniHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIHELPER_H__E176C25B_C58B_4765_B150_4A8E42898ED4__INCLUDED_)
#define AFX_INIHELPER_H__E176C25B_C58B_4765_B150_4A8E42898ED4__INCLUDED_
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <ctype.h>
#define VAR_NAME 64
#define VAR_VAL  128
#define SEC_TITLE 64
#define MAX_VAR   200
#define MAX_SEC	   500
//define a variable
struct var_entry
{
	char var_name[VAR_NAME];
	char var_value[VAR_VAL];
};
//define a section
struct section
{
	char title[SEC_TITLE];
	var_entry entry_list[MAX_VAR];
	int		entry_number;
};
class CIniHelper  
{
public:
	int ReadInt(const char* szSec, const char* szName, int nDefault);
	char* ReadString(const char* szSec, const char* szName, const char* szDefault);
	int ErrorOccurence( );
	int ReadSection(section & pSec);
	CIniHelper(const char*  file);
	~CIniHelper();
private:
	FILE * m_fp ; //file pointer
	int  m_Error; // error flag
	long m_pos;
	char m_strRet[VAR_VAL];
	int  m_nRet;
};
#ifdef _WIN32  
static int strcasecmp(const char *s1, const char *s2)
{
	return strcmp(s1, s2);
}
static int strncasecmp(const char *s1, const char *s2, size_t n)
{
	if( s1 == NULL) return -1;
	else if(s2 == NULL)return 1; 
	return strnicmp(s1, s2, n);
}
#endif
#endif 
