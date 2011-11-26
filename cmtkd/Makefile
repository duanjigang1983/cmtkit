#CFLAG=-c -Wall  -I/usr/local/include  -ftemplate-depth-128 -I./ -D__LINUX__ -g
CFLAG=-c -Wall -I./ -D__LINUX__ -g -I/usr/local/Ice-3.3.1/include
SERVER_OBJS=MessageHandler.o cmtkp.o cmutils.o syslog_utils.o IniHelper.o NetHelper.o cm_utils.o host_tree.o cm_plugin.o
DFLAG=-lIce -lIceUtil -lpthread -L/usr/local/Ice-3.3.1/lib
CC=c++
OBJS=cmtkd
default:
	$(CC) $(CFLAG)   cmtkd.cpp
	$(CC) $(CFLAG)   cmutils.cpp
	$(CC) $(CFLAG)   host_tree.cpp
	$(CC) $(CFLAG)   MessageHandler.cpp
	$(CC) $(CFLAG)   cmtkp.cpp
	$(CC) $(CFLAG)   syslog_utils.cpp
	$(CC) $(CFLAG)   NetHelper.cpp
	$(CC) $(CFLAG)   IniHelper.cpp
	$(CC) $(CFLAG)   cm_utils.cpp
	$(CC) $(CFLAG)   cm_plugin.cpp
	$(CC) -o cmtkd  cmtkd.o $(SERVER_OBJS) $(DFLAG)    
clean:
	rm *.o cmtkd

install:
	install -v cmtkd     /usr/local/bin/cmtkd
	install -v cmtkd_config.init /etc/cmtk/
