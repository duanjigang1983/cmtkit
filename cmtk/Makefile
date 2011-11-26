#CFLAG=-c -Wall  -I/usr/local/include  -ftemplate-depth-128 -I./ -D__LINUX__ -g -I/usr/local/Ice
CFLAG=-c -Wall -I./ -D__LINUX__ -g -I/usr/local/Ice-3.3.1/include
CLI_OBJS=cmtkp.o utils.o NetHelper.o syslog_utils.o  cm_utils.o IniHelper.o
DFLAG=-lIce -lIceUtil -lpthread -L/usr/local/Ice-3.3.1/lib
CC=c++
default:
	$(CC) $(CFLAG)   cmtk.cpp
	$(CC) $(CFLAG)   utils.cpp
	$(CC) $(CFLAG)   cmtkp.cpp
	$(CC) $(CFLAG)   syslog_utils.cpp
	$(CC) $(CFLAG)   NetHelper.cpp
	$(CC) $(CFLAG)   IniHelper.cpp
	$(CC) $(CFLAG)   cm_utils.cpp
	$(CC) -o cmtk cmtk.o $(CLI_OBJS) $(DFLAG)    
clean:
	rm *.o  cmtk 

install:
	install -v cmtk  /usr/local/bin/cmtk
	#cp -rf  cmtools  /etc/
