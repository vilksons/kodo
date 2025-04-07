/*
 * Project Name: Kodo Toolchain
 * Project File: Kodo/server.c
 * Copyright (C) Kodo/Contributors
 *
 * This program is distributed under the terms of the GNU General Public License v2.0.
 * See the LICENSE file for details.
 *
 */

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

void call_server_stop_tasks(void)
{
    char *format_sys = malloc(1000);
    size_t format_size = 1000;

    snprintf(format_sys, 1000, "pkill -9 -f \"%s\"", "samp-server.exe");

    int ret_deb = system(format_sys);
    if (ret_deb != 0) {}

    snprintf(format_sys, 1000, "pkill -9 -f \"%s\"", "samp03svr");

    ret_deb = system(format_sys);
    if (ret_deb != 0) {}

    snprintf(format_sys, 1000, "pkill -9 -f \"%s\"", "omp-server.exe");

    ret_deb = system(format_sys);
    if (ret_deb != 0) {}

    snprintf(format_sys, 1000, "pkill -9 -f \"%s\"", "omp-server");

    ret_deb = system(format_sys);
    if (ret_deb != 0) {}

    if (format_sys) {
        free(format_sys);
    }
}

void call_server_samp(const char *arg1, const char *ptr_server) {
    char *format_sys = malloc(1000);
    size_t format_size = 1000;

    const char *config_file = "server.cfg";

    snprintf(format_sys, 1000, "cp %s .%s.bak", config_file, config_file);
    int ret = system(format_sys);
    if (ret == -1) {
        perror("system failed");
    }

    char bak_fname[256];
    snprintf(bak_fname, sizeof(bak_fname), ".%s.bak", config_file);

    FILE *in = fopen(bak_fname, "r");
    if (!in) {
        printf_error("Failed opening input file");
    }

    FILE *out = fopen(config_file, "w");
    if (!out) {
        printf_error("Failed opening output file");
        fclose(in);
    }

    int found_rate = 0;
    char line[512];

    while (fgets(line, sizeof(line), in)) {
        if (strncmp(line, "gamemode0 ", 10) == 0) {
            fprintf(out, "gamemode0 %s\n", arg1);
            found_rate = 1;
        } else {
            fputs(line, out);
        }
    }

    if (!found_rate) {
        fprintf(out, "gamemode0 %s\n", arg1);
    }

    fclose(in);
    fclose(out);

    snprintf(format_sys, 1000, "chmod 777 %s", ptr_server);
    int ret2 = system(format_sys);
    if (ret2 == -1) {
        perror("system failed");
    }
    snprintf(format_sys, 1000, "./%s", ptr_server);
    int ret3 = system(format_sys);
    if (ret3 == -1) {
        perror("system failed");
    }
    
    const char *srv_log_samp = "server_log.txt";
    FILE *file = fopen(srv_log_samp, "r");

    snprintf(format_sys, 1000, "cat %s", srv_log_samp);
    int ret4 = system(format_sys);
    if (ret4 == -1) {
        perror("system failed");
    }

    remove(config_file);
    snprintf(format_sys, 1000, "mv .%s.bak %s", config_file, config_file);
    int ret5 = system(format_sys);
    if (ret5 == -1) {
        perror("system failed");
    }

    if (strcmp(server_or_debug, "debug") == 0) {
        server_or_debug=NULL;
        
        snprintf(format_sys, 1000, "pkill -9 -f \"%s\"", "samp-server.exe");

        int ret_deb = system(format_sys);
        if (ret_deb != 0) {}

        snprintf(format_sys, 1000, "pkill -9 -f \"%s\"", "samp03svr");

        ret_deb = system(format_sys);
        if (ret_deb != 0) {}
    }

    if (format_sys) {
        free(format_sys);
    }

    _kodo_();
}

void call_server_openmp(const char *arg1) {
    const char *config_file = "config.json";
    const char *backup_file = ".config.json.bak";

    char cmd[512];
    snprintf(cmd, sizeof(cmd), "cp %s %s", config_file, backup_file);
    if (system(cmd) == -1) {
        printf_error("Failed to create backup");
    }

    FILE *fp = fopen(config_file, "r");
    if (!fp) {
        printf_error("Failed to open config.json");
    }

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *data = (char *)malloc(len + 1);
    if (!data) {
        fclose(fp);
        printf_error("Memory allocation failed");
    }

    size_t read_len = fread(data, 1, len, fp);
    if (read_len != len) {
        printf_error("Failed to read full file (read %zu of %ld bytes)", read_len, len);
        free(data);
        fclose(fp);
    }

    data[len] = '\0';
    fclose(fp);

    cJSON *root = cJSON_Parse(data);
    free(data);

    if (!root) {
        printf_error("Error before: [%s]", cJSON_GetErrorPtr());
        return;
    }

    cJSON *pawn = cJSON_GetObjectItem(root, "pawn");
    if (!pawn) {
        printf_error("\"pawn\" object not found");
        cJSON_Delete(root);
        return;
    }

    cJSON_DeleteItemFromObject(pawn, "main_scripts");
    cJSON *main_scripts = cJSON_CreateArray();
    
    char script_entry[256];
    snprintf(script_entry, sizeof(script_entry), "%s 1", arg1);
    cJSON_AddItemToArray(main_scripts, cJSON_CreateString(script_entry));
    cJSON_AddItemToObject(pawn, "main_scripts", main_scripts);

    fp = fopen(config_file, "w");
    if (!fp) {
        printf_error("Failed to open config.json for writing");
        cJSON_Delete(root);
    }

    char *new_data = cJSON_Print(root);
    if (new_data) {
        fputs(new_data, fp);
        free(new_data);
    }

    fclose(fp);
    cJSON_Delete(root);

    const char *srv_log_samp = "log.txt";
    FILE *file = fopen(srv_log_samp, "r");

    char *format_sys = malloc(1000);
    size_t format_size = 1000;


    printf_color(COL_YELLOW, "Press enter to print logs..");
    getchar();

    if (file) {
        snprintf(format_sys, 1000, "cat %s", srv_log_samp);
        int ret = system(format_sys);
        if (ret == -1) {
            printf_error("system failed");
        }
    }

    remove(config_file);
    snprintf(cmd, sizeof(cmd), "mv %s %s", backup_file, config_file);
    if (system(cmd) == -1) {
        printf_error("Failed to restore original config");
    }

    if (strcmp(server_or_debug, "debug") == 0) {
        server_or_debug=NULL;
        
        snprintf(format_sys, 1000, "pkill -9 -f \"%s\"", "omp-server.exe");

        int ret_deb = system(format_sys);
        if (ret_deb != 0) {}

        snprintf(format_sys, 1000, "pkill -9 -f \"%s\"", "omp-server");

        ret_deb = system(format_sys);
        if (ret_deb != 0) {}
    }

    if (format_sys) {
        free(format_sys);
    }

    _kodo_();
}
