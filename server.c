/*
 * Project Name: Kodo Toolchain
 * Project File: cKodo/server.c
 * Copyright (C) cKodo/Contributors
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

void process_system(const char *_from_, const char *_for_) {
    if (_from_) {
        errno = 0;
        int ret = system(_from_);
        if (ret == -1) {
            printf_error("system() failed for %s: %s",
                         _for_, strerror(errno));
        } else if (ret != 0) {
            printf_error("Command failed for %s (exit code: %d)",
                         _for_, WEXITSTATUS(ret));
        }
    }

    /* void */
}

void call_server_stop_tasks(void)
{
    char *format_sys = malloc(126);
    size_t format_size = 126;

    snprintf(format_sys, 126, "pkill -9 -f \"%s\"", "samp-server.exe");
    process_system(format_sys, format_sys);

    snprintf(format_sys, 126, "pkill -9 -f \"%s\"", "samp03svr");
    process_system(format_sys, format_sys);

    snprintf(format_sys, 126, "pkill -9 -f \"%s\"", "omp-server.exe");
    process_system(format_sys, format_sys);

    snprintf(format_sys, 126, "pkill -9 -f \"%s\"", "omp-server");
    process_system(format_sys, format_sys);

    if (format_sys) {
        free(format_sys);
    }

    /* void */
}

void call_server_samp(const char *_args_,
                      const char *ptr_server)
{
    char *format_sys = malloc(256);
    size_t format_size = 256;

    const char *config_file = "server.cfg";

    snprintf(format_sys, 256, "cp %s .%s.bak", config_file, config_file);
    process_system(format_sys, format_sys);

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

    int found_gmodes = 0x0;
    char line_gmodes[26];

    while (fgets(line_gmodes, sizeof(line_gmodes), in)) {
        if (strncmp(line_gmodes, "gamemode0 ", 10) == 0) {
            fprintf(out, "gamemode0 %s\n", _args_);
            found_gmodes = 0x1;
        } else {
            fputs(line_gmodes, out);
        }
    }

    if (!found_gmodes) {
        fprintf(out, "gamemode0 %s\n", _args_);
    }

    fclose(in);
    fclose(out);

    snprintf(format_sys, 256, "chmod 777 %s", ptr_server);
    process_system(format_sys, format_sys);

    snprintf(format_sys, 256, "./%s", ptr_server);
    process_system(format_sys, format_sys);
    
    const char *srv_log_samp = "server_log.txt";
    FILE *__samp_log = fopen(srv_log_samp, "r");

    if (__samp_log) {
        snprintf(format_sys, 256, "cat %s", srv_log_samp);
        process_system(format_sys, format_sys);
    }

    snprintf(format_sys, 256, "cat %s", srv_log_samp);
    process_system(format_sys, format_sys);

    remove(config_file);
    snprintf(format_sys, 256, "mv .%s.bak %s", config_file, config_file);
    process_system(format_sys, format_sys);

    if (strcmp(server_or_debug, "debug") == 0) {
        server_or_debug = NULL;
        
        snprintf(format_sys, 256, "pkill -9 -f \"%s\"", "samp-server.exe");
        process_system(format_sys, format_sys);

        snprintf(format_sys, 256, "pkill -9 -f \"%s\"", "samp03svr");
        process_system(format_sys, format_sys);
    }

    if (format_sys) {
        free(format_sys);
    }

    _kodo_();

    /* void */
}

void call_server_openmp(const char *_args_) {
    const char
        *config_file
            ="config.json";
    const char
        *backup_file
            =".config.json.bak";

    char cmd_s[126];
    snprintf(cmd_s, sizeof(cmd_s), "cp %s %s", config_file, backup_file);
    process_system(cmd_s, cmd_s);

    FILE *__fp = fopen(config_file, "r");
    if (!__fp) {
        printf_error("Failed to open config.json");
    }

    fseek(__fp, 0, SEEK_END);
    long len = ftell(__fp);
    fseek(__fp, 0, SEEK_SET);

    char *data = (char *)malloc(len + 1);
    if (!data) {
        fclose(__fp);
        printf_error("Memory allocation failed");
    }

    size_t read_len = fread(data, 1, len, __fp);
    if (read_len != len) {
        printf_error("Failed to read full file (read %zu of %ld bytes)", read_len, len);
        free(data);
        fclose(__fp);
    }

    data[len] = '\0';
    fclose(__fp);

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
    snprintf(script_entry, sizeof(script_entry), "%s 1", _args_);
    cJSON_AddItemToArray(main_scripts, cJSON_CreateString(script_entry));
    cJSON_AddItemToObject(pawn, "main_scripts", main_scripts);

    __fp = fopen(config_file, "w");
    if (!__fp) {
        printf_error("Failed to open config.json for writing");
        cJSON_Delete(root);
    }

    char *new_data = cJSON_Print(root);
    if (new_data) {
        fputs(new_data, __fp);
        free(new_data);
    }

    fclose(__fp);
    cJSON_Delete(root);

    const char *svr_log_omp = "log.txt";
    FILE *__omp_log = fopen(svr_log_omp, "r");

    char *format_sys = malloc(126);
    size_t format_size = 126;

    printf_color(COL_YELLOW, "Press enter to print logs..");
    getchar();

    if (__omp_log) {
        snprintf(format_sys, 126, "cat %s", svr_log_omp);
        process_system(format_sys, format_sys);
    }

    remove(config_file);
    snprintf(cmd_s, sizeof(cmd_s), "mv %s %s", backup_file, config_file);
    process_system(cmd_s, cmd_s);

    if (strcmp(server_or_debug, "debug") == 0) {
        server_or_debug = NULL;
        
        snprintf(format_sys, 126, "pkill -9 -f \"%s\"", "omp-server.exe");
        process_system(format_sys, format_sys);

        snprintf(format_sys, 126, "pkill -9 -f \"%s\"", "omp-server");
        process_system(format_sys, format_sys);
    }

    if (format_sys) {
        free(format_sys);
    }

    _kodo_();

    /* void */
}
