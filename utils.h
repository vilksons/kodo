#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
 
#ifndef UTILS_H
#define UTILS_H

#define SEF_PATH_COUNT 28
#define SEF_PATH_SIZE 1024

typedef struct {
    int init_ipc;
    int kodo_sef_count;
    char kodo_sef_found[SEF_PATH_COUNT][SEF_PATH_SIZE];
    const char *kodo_os;
    const char *server_or_debug;
    const char *kd_compiler_opt;
    const char *kd_gamemode_input;
    const char *kd_gamemode_output;
} KodoConfig;

KodoConfig kodo_config = {
    .init_ipc = 0,
    .kodo_sef_count = 0,
    .kodo_sef_found = { {0} },
    .kodo_os = NULL,
    .server_or_debug = NULL,
    .kd_compiler_opt = NULL,
    .kd_gamemode_input = NULL,
    .kd_gamemode_output = NULL
};

extern KodoConfig kodo_config;

struct struct_of { int (*title)(const char *); };
struct struct_of init_kodo(void);

void reset_variables(void);
int kodo_sys(const char *cmd);
void handle_sigint(int sig);
int kodo_title(const char *__title);
void printf_color(const char *color, const char *format, ...);
void println(const char* fmt, ...);
void printf_succes(const char *format, ...);
void printf_info(const char *format, ...);
void printf_warning(const char *format, ...);
void printf_error(const char *format, ...);
void printf_crit(const char *format, ...);
const char* kodo_detect_os(void);
int signal_system_os(void);
int kodo_toml_data(void);
int kodo_cmds_check(const char *_str1, const char *_str2);
int kodo_sef_fdir(const char *sef_path, const char *sef_name);
int kodo_sef_wcopy(const char *c_src, const char *c_dest);
int kodo_sef_wmv(const char *c_src, const char *c_dest);
int kodo_sef_wmwrm(const char *c_src, const char *c_dest);
int kodo_sef_wcopy(const char *c_src, const char *c_dest);
int kodo_extract_archive(const char *tar_files);
void kodo_extract_zip(const char *zip_path, const char *dest_path);
size_t write_file(void *ptr, size_t size, size_t nmemb, FILE *stream);
int progress_callback(void *ptr, double dltotal, double dlnow, double ultotal, double ulnow);
void kodo_download_file(const char *url, const char *fname);

#endif