#ifndef _CM_PLUGIN_H
#define _CM_PLUGIN_H
#define PLUGIN_DIR 256
#define PLUGIN_NAME 64
#define PLUGIN_PATH 256
#define PLUGIN_USER 64
typedef struct _plugin_t
{
	char name[PLUGIN_NAME];
	char path[PLUGIN_PATH];
	char user[PLUGIN_USER];
}cm_plugin_t;

#endif
