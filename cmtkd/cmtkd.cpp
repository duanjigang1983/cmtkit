#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <Ice/Ice.h>
#include "cmtkp.h"
#include "cmconfig.h"
#include "MessageHandler.h"
#include "cmutils.h"

using namespace std;
using namespace cmtkp;

Ice::CommunicatorPtr g_ic;
extern cm_server_config_t g_server_config;
extern int init_syslog ();
extern int syslog_msg(const char * szMsg,...);
int t_main(int argc, char* argv[]);

int main(int argc, char* argv[])
{

	if ( parse_options (argc, argv) <= 0 )
	{
		return 0;
	}
	
	if ( g_server_config.daemon )
	{
		daemon_func(0,0);
	}
	return t_main (argc, argv);	
}
int t_main(int argc, char* argv[])
{
		//parse program options from its parameter	
		setuid(0);
		init_syslog ();
		load_plugin (g_server_config.plugin_dir);//added by duanjigang@2011-11-13
		adjust_path ();
		//show_plist ();
		
		try
		{
			char szProxy[128] = {0};
			sprintf(szProxy, "tcp -p %u:udp -p %u", g_server_config.port, g_server_config.port); // protocol and port
			Ice::PropertiesPtr props = Ice::createProperties(argc, argv);
			props->setProperty("Ice.MessageSizeMax", "10240");
			Ice::InitializationData id;
			id.properties = props;
			g_ic = Ice::initialize(id);

			Ice::ObjectAdapterPtr adapter =
			g_ic->createObjectAdapterWithEndpoints("cmtkp", szProxy);
			CMessageHandler * pH = new CMessageHandler;
			
			if (pH->init_server() <= 0)
			{
				printf ("init server failed\n");
				goto ret;
			}
			
			Ice::ObjectPtr object = pH;
			adapter->add(object, g_ic->stringToIdentity("cmtkp"));
			adapter->activate();
			syslog_msg ("INFO [cmserver star running]");	
			g_ic->waitForShutdown(); //
		}
		catch (const Ice::Exception & e)
		{
			cerr << e << endl;
		} catch (const char * msg)
		{
			cout << msg << endl;
		}
	ret:
		return 0;
}

