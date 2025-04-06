/*
 * Project Name: Kododo Toolchain
 * Project File: Kododo/utils.h
 * Copyright (C) Kododo/Contributors
 *
 * This program is distributed under the terms of the GNU General Public License v2.0.
 * See the LICENSE file for details.
 *
 */
 
#ifndef UTILS_H
#define UTILS_H

int kom_toml_data(void);
extern const char *kodo_os;
extern const char *kd_compiler_opt;
extern const char *kd_gamemode_name;
int system_os(void);
int call_kom_undefined_rate(const char *str1, const char *str2);
void printf_color(const char *color, const char *format, ...);
void println(const char* fmt, ...);
void printf_succes(const char *format, ...);
void printf_info(const char *format, ...);
void printf_warning(const char *format, ...);
void printf_error(const char *format, ...);
void printf_crit(const char *format, ...);
int call_extract_tar_gz(const char *fname);
int call_extract_zip(const char *zip_path, const char *dest_path);
size_t write_file(void *ptr, size_t size, size_t nmemb, FILE *stream);
int progress_callback(void *ptr, double dltotal, double dlnow, double ultotal, double ulnow);
void call_download_file(const char *url, const char *fname);

#define MAX_FIND_PATH 100
#define MAX_FIND_PATH_LENGTH 1024

extern char pf_found[MAX_FIND_PATH][MAX_FIND_PATH_LENGTH];
extern int pf_count;
int call_kodo_find_file(const char *__d_path, const char *__t_file);

#endif