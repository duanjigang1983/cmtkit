#ifndef CMTKP
#define CMTKP

module cmtkp
{
	sequence<string>  StringArray; //流数组
	sequence<StringArray>StringArray2;//
	sequence <byte> byteArray; //for files
	sequence<long> ipArray;
	struct msghead //消息头
	{
		string		index;
		long		hostaddr;	//主机地址
		ipArray		srchost;
		int		hostport;
		int		taskid;	//线程ID
		int 		msgtype;	//消息类型
		long		commandid;	//命令ID，用于反馈识别
		long		timestamp;	//创建时间
		//long		localaddr;
		//added by djg@2011-03-08 --start
		string		file;
		string		dstfile;
		string		filemd5;
		long		filesize;
		int		nret;
		int		runmode;//1 real-time 2 background
		string remotefile;
		string localfile;
		//added by duanjigang1983@2011-11-01 finish
	};
	struct CommandMessage
	{
		msghead 	head; //控制信息
		string		cmd;
		StringArray	result;
		byteArray	filedata;
		byteArray	fileinfo;
	};
	struct command
	{
		long		host;
		int		port;
		int		cmdtype;
		int		cmdid;
		int		pid;
		long		clientaddr;
		long		clientid;
		byteArray	data;
	};
	struct response
	{
		string filename;
		byteArray filedata;
		string infoname;
		byteArray infodata;
		int	more;
	};

	struct magicmsg
	{
		int 	type;
		byteArray data;
	};
	interface CmdMessageHandler //服务器和客户端之间的调用接口类
	{
		idempotent int	 ProcessMagicMessage (magicmsg msg);
		idempotent CommandMessage ProcessMessage(CommandMessage msg); //实际调用接口
	};
};
#endif
