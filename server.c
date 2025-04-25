#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stddef.h>

#include "cJson/cJSON.h"

#include "kodo.h"
#include "color.h"
#include "utils.h"
#include "server.h"

void kodo_process_system(const char *__process) {
	if (__process) {
		errno = 0;
		int ret = system(__process);
		if (ret == -1) {
			printf_error("system() failed for %s: %s",
				__process, strerror(errno));
		} else if (ret != 0) {
			printf_error("Command failed for %s (exit code: %d)",
				__process, WEXITSTATUS(ret));
		}
	}
}

void kodo_server_stop_tasks(void)
{
	char *format_sys = malloc(126);
	size_t format_size = 126;

	snprintf(format_sys, 126, "pkill -9 -f \"%s\" && pkill -9 -f \"%s\" && pkill -9 -f \"%s\" && pkill -9 -f \"%s\"",
			"samp-server.exe", // one - samp windows
			"samp03svr", // two - samp linux 
			"omp-server.exe", // theree - omp windows
			"omp-server" // four - omp linux
	);
	kodo_process_system(format_sys);

	if (format_sys) {
		free(format_sys);
	}

	/* void */
}

void
kodo_server_samp(const char *gamemode_arg,
				 const char *server_bin)
{
	FILE *serv_in = NULL, *serv_out = NULL, *serv_log = NULL;
	int found_gamemode = 0;
	char cmd_buf[256];
	char g_line[64];

	snprintf(cmd_buf, sizeof(cmd_buf), "cp %s .%s.bak", "server.cfg", "server.cfg");
	kodo_process_system(cmd_buf);

	serv_in = fopen(".server.cfg.bak", "r");
	if (!serv_in) {
		printf_error("failed to open backup config");
		goto serv_out;
	}
	serv_out = fopen("server.cfg", "w");
	if (!serv_out) {
		printf_error("failed to write new config");
		fclose(serv_in);
		goto serv_out;
	}

	while (fgets(g_line, sizeof(g_line), serv_in)) {
		if (!strncmp(g_line, "gamemode0 ", 10)) {
			fprintf(serv_out, "gamemode0 %s\n", gamemode_arg);
			found_gamemode = 1;
		} else {
			fputs(g_line, serv_out);
		}
	}

	if (!found_gamemode)
		fprintf(serv_out, "gamemode0 %s\n", gamemode_arg);

	fclose(serv_in);
	fclose(serv_out);

	snprintf(cmd_buf, sizeof(cmd_buf), "chmod 777 %s", server_bin);
	kodo_process_system(cmd_buf);
	snprintf(cmd_buf, sizeof(cmd_buf), "./%s", server_bin);
	kodo_process_system(cmd_buf);

	serv_log = fopen("server_log.txt", "r");
	if (serv_log) {
		fclose(serv_log);
		kodo_process_system("cat server_log.txt");
	}

	remove("server.cfg");
	snprintf(cmd_buf, sizeof(cmd_buf), "mv .%s.bak %s", "server.cfg", "server.cfg");
	kodo_process_system(cmd_buf);

	if (!strcmp(server_or_debug, "debug")) {
		server_or_debug = NULL;
		kodo_process_system("pkill -9 -f \"samp-server.exe\" && pkill -9 -f \"samp03svr\"");
	}

serv_out:
	_kodo_(0);
}

void
kodo_server_openmp(const char *gamemode_arg)
{
	cJSON *root = NULL, *pawn = NULL, *main_scripts = NULL;
	FILE *fp = NULL;
	char *json_data = NULL;
	char cmd[128];

	snprintf(cmd, sizeof(cmd), "cp %s %s", "config.json", ".config.json.bak");
	kodo_process_system(cmd);

	fp = fopen("config.json", "r");
	if (!fp) {
		printf_error("failed to open config.json");
		return;
	}

	fseek(fp, 0, SEEK_END);
	long file_len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	json_data = malloc(file_len + 1);
	fread(json_data, 1, file_len, fp);
	json_data[file_len] = '\0';
	fclose(fp);

	root = cJSON_Parse(json_data);
	if (!root) {
		printf_error("JSON parse error: [%s]", cJSON_GetErrorPtr());
		free(json_data);
		return;
	}

	pawn = cJSON_GetObjectItem(root, "pawn"); cJSON_DeleteItemFromObject(pawn, "main_scripts");

	main_scripts = cJSON_CreateArray();
	snprintf(cmd, sizeof(cmd), "%s 1", gamemode_arg);
	cJSON_AddItemToArray(main_scripts, cJSON_CreateString(cmd));
	cJSON_AddItemToObject(pawn, "main_scripts", main_scripts);

	fp = fopen("config.json", "w");
	char *new_json = cJSON_Print(root);
	fputs(new_json, fp);
	free(new_json);
	fclose(fp);

	printf_color(COL_YELLOW, "Press enter to print logs..");
	getchar();
	snprintf(cmd, sizeof(cmd), "cat log.txt");
	kodo_process_system(cmd);

	remove("config.json");
	snprintf(cmd, sizeof(cmd), "mv %s %s", ".config.json.bak", "config.json");
	kodo_process_system(cmd);

	if (!strcmp(server_or_debug, "debug")) {
		server_or_debug = NULL;
		kodo_process_system("pkill -9 -f \"omp-server.exe\" && pkill -9 -f \"omp-server\"");
	}

	cJSON_Delete(root);
	free(json_data);
}
