#ifndef _CM_PLUGIN_H
#define _CM_PLUGIN_H
#define PLUGIN_DIR 256
#define PLUGIN_NAME 64
#define PLUGIN_DESC 100
#define PLUGIN_PATH 256
#define PLUGIN_USER 64
#define CTL_FILE "cmtk.ini"
typedef struct _plugin_t
{
	char name[PLUGIN_NAME];
	char desc[PLUGIN_DESC];
	char * store;
	char user[PLUGIN_USER];
	struct _plugin_t * next;
}cm_plugin_t;

typedef struct _cm_plugin_pkg_t
{
	char path [PLUGIN_PATH];
	unsigned int pnum;
	cm_plugin_t * plist;
	struct _cm_plugin_pkg_t * next;
	char finish;
}cm_plugin_pkg_t;

extern int  load_plugin (const char* szpath);
extern void show_plist (void);
extern void adjust_path (void);
extern int  run_limit_cmd (const char* szline, const char* szfile);
#endif
