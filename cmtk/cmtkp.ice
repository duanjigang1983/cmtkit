#ifndef CMTKP
#define CMTKP

module cmtkp
{
	sequence<string>  StringArray; //������
	sequence<StringArray>StringArray2;//
	sequence <byte> byteArray; //for files
	sequence<long> ipArray;
	struct msghead //��Ϣͷ
	{
		string		index;
		long		hostaddr;	//������ַ
		ipArray		srchost;
		int		hostport;
		int		taskid;	//�߳�ID
		int 		msgtype;	//��Ϣ����
		long		commandid;	//����ID�����ڷ���ʶ��
		long		timestamp;	//����ʱ��
		string		file;
		string		dstfile;
		string		filemd5;
		long		filesize;
		int		nret;
		int		runmode;//1 real-time 2 background
		string remotefile;
		string localfile;
	};
	struct CommandMessage
	{
		msghead 	head; //������Ϣ
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
	interface CmdMessageHandler //�������Ϳͻ���֮��ĵ��ýӿ���
	{
		idempotent int	 ProcessMagicMessage (magicmsg msg);
		idempotent CommandMessage ProcessMessage(CommandMessage msg); //ʵ�ʵ��ýӿ�
	};
};
#endif
