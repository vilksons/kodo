#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
 
#ifndef UTILS_H
#define UTILS_H

#define SEF_MAX_PATH_COUNT 28
#define SEF_MAX_PATH_SIZE 1024

extern int kodo_sef_count;
extern char kodo_sef_found[SEF_MAX_PATH_COUNT][SEF_MAX_PATH_SIZE];

extern const char *kodo_os;
extern const char *kd_compiler_opt;
extern const char *kd_gamemode_input;
extern const char *kd_gamemode_output;
extern const char *server_or_debug;

struct struct_of { int (*title)(const char *); };
struct struct_of init_kodo(void);

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
int kodo_extract_tar_gz(const char *fname);
int kodo_extract_zip(const char *zip_path, const char *dest_path);
size_t write_file(void *ptr, size_t size, size_t nmemb, FILE *stream);
int progress_callback(void *ptr, double dltotal, double dlnow, double ultotal, double ulnow);
void kodo_download_file(const char *url, const char *fname);

#endif