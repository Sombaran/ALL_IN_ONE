#include <stdio.h>
#include "glib.h"

#define MIN_ARGS 4

static void usage();

int main(int argc, char *argv[]) {
	char *file = NULL;
	char *group = NULL;
	char *key = NULL;
	char *value = NULL;
	char *current_value = NULL;
	GKeyFile *key_file = NULL;
	GKeyFileFlags flags;
	
	if(argc < MIN_ARGS) {
		usage();
		goto l_err_args;
	}
	
	file = argv[1];
	group = argv[2];
	key = argv[3];
	if(argc > MIN_ARGS) {
		value = argv[4];
	}
	
	key_file = g_key_file_new(); //get an empty struct
	flags = G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS;
	if(g_key_file_load_from_file (key_file, file, flags, NULL) == 0) {
		g_key_file_free(key_file);
		printf("[Error] Could not read config file: %s\n", file);
		goto l_err;
	}
	
	if(g_key_file_has_key(key_file, group, key, NULL)) {
		current_value = g_key_file_get_value(key_file, group, key, NULL);
		printf("Found:\n\tgroup: %s key: %s value: %s\n", group, key, current_value);
	} else {
		printf("[Warning] group: %s key: %s NOT found\n", group, key);
	}
	
	if(value != NULL) {
		printf("Setting:\n\tgroup: %s key: %s value: %s\n", group, key, value);
		g_key_file_set_value(key_file, group, key, value);
		if(g_key_file_save_to_file(key_file, file, NULL) == 0) {
			printf("[Error] Could not write to config file: %s\n", file);
		}
	}
	
	g_key_file_free(key_file);
	printf("\nDone!\n");
	return 0;
l_err_args:
	return -1;
l_err:
	return -2;
}

static void usage() {
	printf("Usage:\n\tzwscene_config <config_file> <config_group> <config_key> [config_value]\n");
	printf("\tif no value is provided then the current value of config_key is returned\n");
}
