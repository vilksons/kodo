#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stddef.h>
#include <unistd.h>

#include "cJson/cJSON.h"

#include "kodo.h"
#include "color.h"
#include "utils.h"
#include "server.h"

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
    kodo_sys(format_sys);

    if (format_sys) {
        free(format_sys);
    }

    /* void */
}

void
kodo_server_samp(const char *gamemode_arg,
                 const char *server_bin)
{
    FILE *serv_in = NULL, *serv_out = NULL;
    int found_gamemode = 0;
    char cmd_buf[256];
    char g_line[64];

    snprintf(cmd_buf, sizeof(cmd_buf), "cp %s .%s.bak", "server.cfg", "server.cfg");
    kodo_sys(cmd_buf);

    serv_in = fopen(".server.cfg.bak", "r");
    if (!serv_in) {
        printf_error("failed to open backup config");
        _kodo_(0);
    }
    serv_out = fopen("server.cfg", "w");
    if (!serv_out) {
        printf_error("failed to write new config");
        fclose(serv_in);
        _kodo_(0);
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

#ifdef _WIN32
    char cmd_path[128];
    snprintf(cmd_path, sizeof(cmd_path), "%s", server_bin);
    kodo_sys(cmd_path);
#else
    chmod(server_bin, 0777);
    char cmd_path[128];
    snprintf(cmd_path, sizeof(cmd_path), "./%s", server_bin);
    kodo_sys(cmd_path);
#endif

    sleep(2);

    printf_color(COL_YELLOW, "Press enter to print logs..");
    getchar();

    FILE *server_log = fopen("server_log.txt", "r");

    if (server_log) {
        if (!server_log) {
            return;
        }
        int ch;
        while ((ch = fgetc(server_log)) != EOF) {
            putchar(ch);
        }
        fclose(server_log);
    }

    remove("server.cfg");
    rename(".server.cfg.bak", "server.cfg");

    kodo_sys(cmd_buf);

    if (server_or_debug &&
        !strcmp(server_or_debug, "debug")) {
        server_or_debug = NULL;
        kodo_sys("pkill -9 -f \"samp-server.exe\" && pkill -9 -f \"samp03svr\"");
    }

    _kodo_(0);
}

void
kodo_server_openmp(const char *gamemode_arg,
                   const char *server_bin)
{
    cJSON *root = NULL, *pawn = NULL, *main_scripts = NULL;
    FILE *fp = NULL;
    char *json_data = NULL;
    char cmd[128];
    char cmd_buf[256];

    snprintf(cmd, sizeof(cmd), "cp %s %s", "config.json", ".config.json.bak");
    kodo_sys(cmd);

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

    pawn = cJSON_GetObjectItem(root, "pawn");
    cJSON_DeleteItemFromObject(pawn, "main_scripts");

    main_scripts = cJSON_CreateArray();
    snprintf(cmd, sizeof(cmd), "%s 1", gamemode_arg);
    cJSON_AddItemToArray(main_scripts, cJSON_CreateString(cmd));
    cJSON_AddItemToObject(pawn, "main_scripts", main_scripts);

    fp = fopen("config.json", "w");
    char *main_njson = cJSON_Print(root);
    fputs(main_njson, fp);
    free(main_njson);
    fclose(fp);

#ifdef _WIN32
    char cmd_path[128];
    snprintf(cmd_path, sizeof(cmd_path), "%s", server_bin);
    kodo_sys(cmd_path);
#else
    chmod(server_bin, 0777);
    char cmd_path[128];
    snprintf(cmd_path, sizeof(cmd_path), "./%s", server_bin);
    kodo_sys(cmd_path);
#endif

    sleep(2);

    printf_color(COL_YELLOW, "Press enter to print logs..");
    getchar();

    FILE *server_log = fopen("log.txt", "r");

    if (server_log) {
        if (!server_log) {
            return;
        }
        int ch;
        while ((ch = fgetc(server_log)) != EOF) {
            putchar(ch);
        }
        fclose(server_log);
    }

    remove("config.json");
    rename(".config.json.bak", "config.json");

    kodo_sys(cmd);

    if (server_or_debug &&
        !strcmp(server_or_debug, "debug")) {
        server_or_debug = NULL;
        kodo_sys("pkill -9 -f \"omp-server.exe\" && pkill -9 -f \"omp-server\"");
    }

    cJSON_Delete(root);
    free(json_data);
}
