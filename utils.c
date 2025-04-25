#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <math.h>
#include <limits.h>
#include <ncurses.h>
#include <dirent.h>
#include <time.h>
#include <ftw.h>
#include <sys/file.h>
#include <curl/curl.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <stddef.h>
#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <archive.h>
#include <archive_entry.h>

#include "tomlc99/toml.h"

#include "color.h"
#include "utils.h"
#include "kodo.h"

const char *kodo_os = NULL;
const char *server_or_debug = NULL;
const char *kd_compiler_opt = NULL;
const char *kd_gamemode_input = NULL;
const char *kd_gamemode_output = NULL;

int kodo_title(const char *__title)
{
    const char *title = __title ? __title : "Kodo Toolchain";
    printf("\033]0;%s\007", title);
    return 0;
}
    
struct struct_of init_kodo(void) {
    struct struct_of kodo;
    kodo.title = kodo_title;
    return kodo;
}

void printf_color(
                  const char *color, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    printf("%s", color);
    vprintf(format, args);
    printf("%s", COL_DEFAULT);

    va_end(args);
}

void println(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

void printf_succes(const char *format, ...) {
    printf_color(COL_YELLOW, "succes: ");
    printf_color(COL_DEFAULT, "%s\n", format);
}
void printf_info(const char *format, ...) {
    printf_color(COL_YELLOW, "info: ");
    printf_color(COL_DEFAULT, "%s\n", format);
}
void printf_warning(const char *format, ...) {
    printf_color(COL_GREEN, "warning: ");
    printf_color(COL_DEFAULT, "%s\n", format);
}
void printf_error(const char *format, ...) {
    printf_color(COL_RED, "error: ");
    printf_color(COL_DEFAULT, "%s\n", format);
}
void printf_crit(const char *format, ...) {
    printf_color(COL_RED, "crit: ");
    printf_color(COL_DEFAULT, "%s\n", format);
}

int kodo_is_windows(void) {
    const char *__win__[] = { "/c/windows/System32", "/windows/System32" };
    for (int i = 0; i < 3; i++) if (access(__win__[i], F_OK) == 0) return 1;
    if (getenv("WSL_INTEROP") != NULL) return 1;
    return 0;
}

const char* kodo_detect_os(void) {
    if (kodo_is_windows()) {
        return "windows";
    }
    struct utsname buf;
    if (uname(&buf) == 0) {
        if (strstr(buf.sysname, "Linux") != NULL) {
            return "linux";
        }
    }
    return "Unknown's";
}

int signal_system_os(void) {
    if (strcmp(kodo_os, "windows") == 0)
        return 1;
    else if (strcmp(kodo_os, "linux") == 0)
        return 0;
    
    return 0;
}

int kodo_toml_data(void)
{
    const char *fname =
        "kodo.toml";
    FILE
        *toml_files;

    toml_files = fopen(fname, "r");
    if (toml_files != NULL) {
        fclose(toml_files);
    } else {
        toml_files = fopen(fname, "w");
        if (toml_files != NULL) {
            const char *os_type = kodo_detect_os();
            fprintf(toml_files, "[general]\n");
            fprintf(toml_files, "os = \"%s\"\n", os_type);
            fprintf(toml_files, "[compiler]\n");
            fprintf(toml_files, "option = \"-;+ -(+ -d3\"\n");
            fprintf(toml_files, "include_path = [\"sample1\", \"sample2\"]\n");
            fprintf(toml_files, "input = \"mymodes.pwn\"\n");
            fprintf(toml_files, "output = \"mymodes.amx\"\n");
            fclose(toml_files);
        }
    }

    FILE *__fp = fopen(fname, "r");
    if (!__fp) {
        printf_error("Can't __read file %ss\n", fname);
    }

    char errbuf[256];
    toml_table_t *config = toml_parse_file(__fp, errbuf, sizeof(errbuf));
    fclose(__fp);

    if (!config) {
        printf_error("parsing TOML: %s\n", errbuf);
    }

    toml_table_t *_kodo_general = toml_table_in(config, "general");
    if (_kodo_general) {
        toml_datum_t os_val = toml_string_in(_kodo_general, "os");
        if (os_val.ok) {
            kodo_os = os_val.u.s;
        }
    }

    return 0;
}

int kodo_cmds_check(const char *_str1, const char *_str2)
{
    int len1 = strlen(_str1);
    int len2 = strlen(_str2);
    
    int **dist;
    dist = (int **)malloc((len1 + 1) * sizeof(int *));
    for (int i = 0; i <= len1; i++) {
        dist[i] = (int *)malloc((len2 + 1) * sizeof(int));
    }

    for (int i = 0; i <= len1; i++) for (int j = 0; j <= len2; j++) {
        if (i == 0) {
            dist[i][j] = j;
        } else if (j == 0) {
            dist[i][j] = i;
        } else {
            int cost = (_str1[i - 1] == _str2[j - 1]) ? 0 : 1;
            dist[i][j] = fmin(
                fmin(dist[i - 1][j] + 1,
                    dist[i][j - 1] + 1),
                dist[i - 1][j - 1] + cost
            );
        }
    }

    int kodo_res = dist[len1][len2];
    
    for (int i = 0; i <= len1; i++) {
        free(dist[i]);
    }
    free(dist);

    return kodo_res;
}

int arch_copy_data(
              struct archive *ar, struct archive *aw)
{
    int __read;
    const void
        *__buff;
    size_t
        size;
    la_int64_t
        offset;

    for (;;) {
        __read = archive_read_data_block(ar, &__buff, &size, &offset);
        if (__read == ARCHIVE_EOF)
            return ARCHIVE_OK;
        if (__read != ARCHIVE_OK)
            return __read;
        __read = archive_write_data_block(aw, __buff, size, offset);
        if (__read != ARCHIVE_OK) {
            printf_error("Write error: %s\n", archive_error_string(aw));
            return __read;
        }
    }
}

int kodo_extract_tar_gz(const char *tar_files) {
    struct archive
        *__arch =
            archive_read_new();
    struct archive
        *__ext =
            archive_write_disk_new();
    struct archive_entry
        *__entry;
    int
        __read;

    archive_read_support_format_tar(__arch);
    archive_read_support_filter_gzip(__arch);

    __read = archive_read_open_filename(__arch, tar_files, 10240);
    if (__read != ARCHIVE_OK) {
        printf_crit("Can't resume. sys can't write/open file %s", tar_files);
        _kodo_(0);
    }

    while (archive_read_next_header(__arch, &__entry) == ARCHIVE_OK) {
        archive_write_header(__ext, __entry);
        arch_copy_data(__arch, __ext);
        archive_write_finish_entry(__ext);
    }

    archive_read_close(__arch);
    archive_read_free(__arch);
    archive_write_close(__ext);
    archive_write_free(__ext);

    return 0;
}

int kodo_extract_zip(
                const char *zip_path, const char *__dest_path)
{
    struct archive
        *__arch;
    struct archive
        *__ext;
    struct archive_entry
        *__entry;
    int
        __read;

    __arch = archive_read_new();
    archive_read_support_format_zip(__arch);
    archive_read_support_filter_all(__arch);

    if ((__read = archive_read_open_filename(__arch, zip_path, 10240))) {
        printf_crit("Can't resume. sys can't write/open file %s", archive_error_string(__arch));
        _kodo_(0);
    }

    __ext = archive_write_disk_new();
    archive_write_disk_set_options(__ext, ARCHIVE_EXTRACT_TIME);
    archive_write_disk_set_standard_lookup(__ext);

    int has_error = 0x0;

    while (archive_read_next_header(__arch, &__entry) == ARCHIVE_OK) {
        const char *__cur_file = archive_entry_pathname(__entry);

        char ext_full_path[128];
        snprintf(ext_full_path, sizeof(ext_full_path), "%s/%s", __dest_path, __cur_file);
        archive_entry_set_pathname(__entry, ext_full_path);

        __read = archive_write_header(__ext, __entry);
        if (__read != ARCHIVE_OK) {
            if (!has_error) {
                printf_error("during extraction: %s\n", archive_error_string(__ext));
                has_error = 0x1;
            }
        } else {
            const void *__buff;
            size_t size;
            la_int64_t offset;

            while (1) {
                __read = archive_read_data_block(__arch, &__buff, &size, &offset);
                if (__read == ARCHIVE_EOF)
                    break;
                if (__read < ARCHIVE_OK) {
                    if (!has_error) {
                        printf_error("reading block from archive: %s\n", archive_error_string(__arch));
                        has_error = 0x1;
                    }
                }
                __read = archive_write_data_block(__ext, __buff, size, offset);
                if (__read < ARCHIVE_OK) {
                    if (!has_error) {
                        printf_error("writing block to destination: %s\n", archive_error_string(__ext));
                        has_error = 0x1;
                    }
                }
            }
        }
    }

    archive_read_close(__arch);
    archive_read_free(__arch);
    archive_write_close(__ext);
    archive_write_free(__ext);

    return 0;
}

size_t write_file(void *ptr,
                  size_t size,
                  size_t nmemb,
                  FILE *stream
) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}
int progress_callback(void *ptr,
                      double dltotal,
                      double dlnow,
                      double ultotal, 
                      double ulnow
) {
    if (dltotal > 0 && dlnow <= 1000.0) {
        printf("\rDownloading ...");
        fflush(stdout);
    }
    return 0;
}

void kodo_download_file(const char *url,
                        const char *fname
) {
    CURL
        *__curl;
    CURLcode
        __res;
    FILE
        *__fp;

    __fp = fopen(fname, "wb");
    if (__fp == NULL) {
        printf_crit("failed to open file for writing in 'kodo_download_file'");
        _kodo_(0);
    }

    curl_global_init(CURL_GLOBAL_DEFAULT);
    __curl =
        curl_easy_init();

    if (__curl) {
        curl_easy_setopt(__curl, CURLOPT_URL, url);
        curl_easy_setopt(__curl, CURLOPT_WRITEFUNCTION, write_file);
        curl_easy_setopt(__curl, CURLOPT_WRITEDATA, __fp);
        curl_easy_setopt(__curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(__curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
        curl_easy_setopt(__curl, CURLOPT_NOPROGRESS, 0L);

        __res = curl_easy_perform(__curl);

        if (__res != CURLE_OK) {
            printf_crit("failed to download the file: %s\n", curl_easy_strerror(__res));
            curl_easy_cleanup(__curl);
            curl_global_cleanup();
            fclose(__fp);
            _kodo_(0);
        }

        printf("\nDownload completed successfully.\n");

        if (strstr(fname, ".tar.gz")) {
            kodo_extract_tar_gz(fname);
        }
        else if (strstr(fname, ".zip")) {
            char zip_of_pos[120];

            size_t len = strlen(fname);
            if (len > 4 && strncmp(fname + len - 4, ".zip", 4) == 0) {
                strncpy(zip_of_pos, fname, len - 4);
                zip_of_pos[len - 4] = '\0';
            } else {
                strcpy(zip_of_pos, fname);
            }
        
            kodo_extract_zip(fname, zip_of_pos);
        }

        fclose(__fp);
        curl_easy_cleanup(__curl);
    } else {
        fprintf(stderr, "[err]: Failed to initialize curl session\n");
    }

    curl_global_cleanup();
}