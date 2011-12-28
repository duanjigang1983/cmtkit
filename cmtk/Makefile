#CFLAG=-c -Wall  -I/usr/local/include  -ftemplate-depth-128 -I./ -D__LINUX__ -g -I/usr/local/Ice
CFLAG=-c -Wall -I./ -D__LINUX__ -g -I/usr/local/Ice-3.3.1/include
CLI_OBJS=*.o
#DFLAG=-lIce -lIceUtil -lpthread -L/usr/local/Ice-3.3.1/lib
DFLAG=-L/lib/nptl -lrt -lIce -lIceUtil -lpthread -L/usr/local/Ice-3.3.1/lib
CC=c++
default:
	$(CC) $(CFLAG)   cm_function.cpp
	$(CC) $(CFLAG)   cm_init.cpp
	$(CC) $(CFLAG)   cm_init_dev.cpp
	$(CC) $(CFLAG)   cm_init_task.cpp
	$(CC) $(CFLAG)   cm_result.cpp
	$(CC) $(CFLAG)   cm_thread.cpp
	$(CC) $(CFLAG)   cmtkp.cpp
	$(CC) $(CFLAG)   cm_utils.cpp
	$(CC) $(CFLAG)   getopt.cpp
	$(CC) $(CFLAG)   IniHelper.cpp
	$(CC) $(CFLAG)   main.cpp
	$(CC) $(CFLAG)   NetHelper.cpp
	$(CC) -o cmtk  $(CLI_OBJS) $(DFLAG)    
clean:
	rm *.o  cmtk 

install:
	install -v cmtk  /usr/local/bin/cmtk
	#cp -rf  cmtools  /etc/
