#ifndef _CONST_DEF_H_
#define _CONST_DEF_H_
#define MSG_TYPE_STAT 1
#define MSG_TYPE_CMD 2
#define MSG_TYPE_FILE 3
#define PROXY_LEN 128


#define RET_CONNECT_FAILED -1
#define RET_RUNCMD_FAILED -2
#define RET_SUCCES 1
#define RET_DIR "result"
#define INFO_DIR "/usr/local/cmserver/info"
#define DFT_LOGIN "none"
enum CMD_TYPE
{
	CMD_TYPE_NOTIFY = 1,
	CMD_TYPE_CONF = 2
};



//static const char* str_msg_type[] = {"unkonw message type", "stat", "command", "file"};
#endif
