
#ifndef _MSG_HD_H_
#define _MSG_HD_H_

#include <Ice/Ice.h>
#include "cmtkp.h"
#include "cmconfig.h"

using namespace std;
using namespace cmtkp;
#define MAX_CLIENT 10000
//ICE class to handle message
class CMessageHandler: public CmdMessageHandler
{
public:
	CMessageHandler();
	virtual ~CMessageHandler();
	//command and file transfer message handle
    	::cmtkp::CommandMessage ProcessMessage(const ::cmtkp::CommandMessage& msg, const Ice::Current &);
    	int ProcessMagicMessage(const ::cmtkp::magicmsg& msg, const Ice::Current &);
    	
	//tell cient to udate or change config file
	::cmtkp::command ProcessCmd(const ::cmtkp::command& cmd, const Ice::Current &);
    	
	//config update method
	::cmtkp::response ProcessNotify(const ::cmtkp::command& cmd, const Ice::Current &);

	
	//handle command message
	int	handle_cmd_message(const ::cmtkp::CommandMessage& msg, ::cmtkp::CommandMessage& msg1);
	//handle result report message
	int	handle_report_message(const ::cmtkp::CommandMessage& msg, ::cmtkp::CommandMessage& msg1);
	//handle file transfer message
	int	handle_file_message(const ::cmtkp::CommandMessage& msg, ::cmtkp::CommandMessage& msg1);
	int	handle_fetch_message(const::cmtkp::CommandMessage& msg, ::cmtkp::CommandMessage& msg1);
	int	init_server(void);
private:
	int	init_cmd_server (void);//init command server
	int	init_stat_server (void); //init stat server
	int	handle_conf_message (const ::cmtkp::command& cmd);
	ip_struct_t	m_iplist[MAX_CLIENT];
	int		m_ipnum;
	void		init_auth_info(void);
};
#endif

