/*
 * Project Name: Kodo Toolchain
 * Project File: Kodo/utils.c
 * Copyright (C) Kodo/Contributors
 *
 * This program is distributed under the terms of the GNU General Public License v2.0.
 * See the LICENSE file for details.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <errno.h>
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
#include "cJson/cJSON.h"

#include "color.h"
#include "utils.h"
#include "kodo.h"
#include "server.h"

const char
    /* variable (gvar) for kodo os checking */
    *kodo_os
    = NULL;
const char
    /* variable (gvar) for compiler option checking */
    *kd_compiler_opt
    = NULL;
const char
    /* variable (gvar) for compiler gamemode input */
    *kd_gamemode_input
    = NULL;
const char
    /* variable (gvar) for compiler gamemode output */
    *kd_gamemode_output
    = NULL;
const char
    /* variable (gvar) for server or debug checking */
    *server_or_debug
    = NULL;

int kodo_title(
    const char *custom_title)
{
    const char *title = custom_title ? custom_title : "Kodo Toolchain";
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
    /* collor message of printf */
    
    /* 
        * printf_color(COL_RED, "red text!");
        * printf_color(COL_GREEN, "green text!");
        * printf_color(COL_YELLOW, "yellow text!");
        * printf_color(COL_BLUE, "blue text!");
        * printf_color(MAGENTA, "magenta text!");
        * printf_color(CYAN, "cyan text!");
        * printf_color(COL_DEFAULT, "reset text!");
    */

    va_list args;
    va_start(args, format);

    printf("%s", color);
    vprintf(format, args);
    printf("%s", COL_DEFAULT);

    va_end(args);
}

void println(const char* fmt, ...) {
    /* print new line of printf */
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

void printf_succes(const char *format, ...) {
    /* printf with newline for succes msg */
    printf_color(COL_YELLOW, "succes: ");
    printf_color(COL_DEFAULT, "%s\n", format);
}

void printf_info(const char *format, ...) {
    /* printf with newline for info msg */
    printf_color(COL_YELLOW, "info: ");
    printf_color(COL_DEFAULT, "%s\n", format);
}

void printf_warning(const char *format, ...) {
    /* printf with newline for warning msg */
    printf_color(COL_GREEN, "warning: ");
    printf_color(COL_DEFAULT, "%s\n", format);
}

void printf_error(const char *format, ...) {
    /* printf with newline for error msg */
    printf_color(COL_RED, "error: ");
    printf_color(COL_DEFAULT, "%s\n", format);
}

void printf_crit(const char *format, ...) {
    /* printf with newline for crit msg */
    printf_color(COL_RED, "crit: ");
    printf_color(COL_DEFAULT, "%s\n", format);
}
    
int kom_is_windows(void) {
    const char *__win__[] = {
        "/c/windows/System32", "/mnt/c/windows/System32", "/windows/System32"
    };

    for (int i = 0; i < 3; i++) {
        if (access(__win__[i], F_OK) == 0) {
            return 1;
        }
    }

    if (getenv("WSL_INTEROP") != NULL) {
        /* only WSL and disable docker , docker using linux system */
        return 1;
    }

    return 0;
}

const char* kom_detect_os(void) {
    if (kom_is_windows()) {
        return "windows";
    }

    struct utsname buf;
    if (uname(&buf) == 0) {
        if (strstr(buf.sysname, "Linux") != NULL) {
            return "linux";
        }
    }

    return "unknown";
}

int signal_system_os(void) {
    if (strcmp(kodo_os, "windows") == 0)
        return 1;
    else if (strcmp(kodo_os, "linux") == 0)
        return 0;
    
    return 0;
}

int kom_toml_data(void)
{
    /* Define the filename and file pointer */
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
            const char *os_type = kom_detect_os();
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

    /* Reopen the file for reading */
    FILE *__fp = fopen(fname, "r");
    if (!__fp) {
        printf_error("Can't __read file %ss\n", fname);
    }

    /* Parse the TOML file */
    char errbuf[256];
    toml_table_t *config = toml_parse_file(__fp, errbuf, sizeof(errbuf));
    fclose(__fp);

    /* Check for parse errors */
    if (!config) {
        printf_error("parsing TOML: %s\n", errbuf);
    }

    /* Read the 'general' table from the parsed TOML data */
    toml_table_t *_kom_general = toml_table_in(config, "general");
    if (_kom_general) {
        /* Read the 'os' value from the table */
        toml_datum_t os_val = toml_string_in(_kom_general, "os");
        if (os_val.ok) {
            /* Store the value in the global variable and print it */
            kodo_os = os_val.u.s;
        }
    }

    return 0;
}

int call_kom_undefined_rate(
                         const char *str1, const char *str2)
{
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    
    /* Allocate 2D array for distance matrix */
    int **dist = (int **)malloc((len1 + 1) * sizeof(int *));
    for (int i = 0; i <= len1; i++) {
        dist[i] = (int *)malloc((len2 + 1) * sizeof(int));
    }

    /* Fill distance matrix */
    for (int i = 0; i <= len1; i++) {
        for (int j = 0; j <= len2; j++) {
            if (i == 0) {
                dist[i][j] = j;  /* cost of all insertions */
            } else if (j == 0) {
                dist[i][j] = i;  /* cost of all deletions */
            } else {
                int cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
                dist[i][j] = fmin(
                    fmin(dist[i - 1][j] + 1,        /* deletion */
                         dist[i][j - 1] + 1),       /* insertion */
                    dist[i - 1][j - 1] + cost       /* substitution */
                );
            }
        }
    }

    /* Final result is in the bottom-right cell */
    int kodo_res = dist[len1][len2];
    
    /* Free the distance matrix */
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
        /* Read a block of data from the source archive */
        __read = archive_read_data_block(ar, &__buff, &size, &offset);
        if (__read == ARCHIVE_EOF)
            return ARCHIVE_OK; /* Reached end of archive */
        if (__read != ARCHIVE_OK)
            return __read; /* Return error if __read fails */

        /* Write the block to the destination archive */
        __read = archive_write_data_block(aw, __buff, size, offset);
        if (__read != ARCHIVE_OK) {
            /* Print write error and return code */
            printf_error("Write error: %s\n", archive_error_string(aw));
            return __read;
        }
    }
}

int call_extract_tar_gz(const char *fname) {
    /* Create archive objects for reading and writing */
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

    /* Enable support for tar format and gzip compression */
    archive_read_support_format_tar(__arch);
    archive_read_support_filter_gzip(__arch);

    __read = archive_read_open_filename(__arch, fname, 10240);
    if (__read != ARCHIVE_OK) {
        printf_crit("Can't resume. i can't write/open the %s", fname);
        _kodo_();
    }

    /* Loop through each __entry in the archive */
    while (archive_read_next_header(__arch, &__entry) == ARCHIVE_OK) {
        archive_write_header(__ext, __entry);
        arch_copy_data(__arch, __ext);
        archive_write_finish_entry(__ext);
    }

    /* Clean up */
    archive_read_close(__arch);
    archive_read_free(__arch);
    archive_write_close(__ext);
    archive_write_free(__ext);

    return 0;
}

int call_extract_zip(
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

    /* Create and configure archive reader */
    __arch = archive_read_new();
    archive_read_support_format_zip(__arch);      /* Enable ZIP format */
    archive_read_support_filter_all(__arch);      /* Support all filters */

    /* Open the archive file */
    if ((__read = archive_read_open_filename(__arch, zip_path, 10240))) {
        printf_crit("Can't resume. i can't write/open the %s", archive_error_string(__arch));
        _kodo_();
    }

    /* Create and configure archive writer */
    __ext = archive_write_disk_new();
    archive_write_disk_set_options(__ext, ARCHIVE_EXTRACT_TIME);
    archive_write_disk_set_standard_lookup(__ext);

    /* Flag to ensure error messages are only printed once */
    int has_error = 0x0L;

    /* Extract each __entry */
    while (archive_read_next_header(__arch, &__entry) == ARCHIVE_OK) {
        const char *__cur_file = archive_entry_pathname(__entry);

        /* Construct full path for the destination file */
        char __full_path[4096];
        snprintf(__full_path, sizeof(__full_path), "%s/%s", __dest_path, __cur_file);
        archive_entry_set_pathname(__entry, __full_path);

        /* Write __entry header */
        __read = archive_write_header(__ext, __entry);
        if (__read != ARCHIVE_OK) {
            if (!has_error) { // Only print once
                printf_error("during extraction: %s\n", archive_error_string(__ext));
                has_error = 0x1L; // Set flag to true after first error
            }
        } else {
            /* Read and write file content block by block */
            const void *__buff;
            size_t size;
            la_int64_t offset;

            while (1) {
                __read = archive_read_data_block(__arch, &__buff, &size, &offset);
                if (__read == ARCHIVE_EOF)
                    break;
                if (__read < ARCHIVE_OK) {
                    if (!has_error) { // Only print once
                        printf_error("reading block from archive: %s\n", archive_error_string(__arch));
                        has_error = 0x1L;
                    }
                }
                __read = archive_write_data_block(__ext, __buff, size, offset);
                if (__read < ARCHIVE_OK) {
                    if (!has_error) { // Only print once
                        printf_error("writing block to destination: %s\n", archive_error_string(__ext));
                        has_error = 0x1L;
                    }
                }
            }
        }
    }

    /* Clean up */
    archive_read_close(__arch);
    archive_read_free(__arch);
    archive_write_close(__ext);
    archive_write_free(__ext);

    return 0;
}

/*
 * Callback for libcurl to write downloaded data into a file.
 */
size_t write_file(void *ptr,
                  size_t size,
                  size_t nmemb,
                  FILE *stream
) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

/*
 * Progress callback for libcurl to show download progress.
 */
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

void call_download_file(const char *url,
                   const char *fname
) {
    CURL
        *__curl;
    CURLcode
        __res;
    FILE
        *__fp;

    /* Open file for writing in binary mode */
    __fp = fopen(fname, "wb");
    if (__fp == NULL) {
        printf_crit("failed to open file for writing in 'call_download_file'");
        _kodo_();
    }

    /* Initialize libcurl globally */
    curl_global_init(CURL_GLOBAL_DEFAULT);
    __curl =
        curl_easy_init();

    if (__curl) {
        /* Set URL to download */
        curl_easy_setopt(__curl, CURLOPT_URL, url);

        /* Set write callback and file destination */
        curl_easy_setopt(__curl, CURLOPT_WRITEFUNCTION, write_file);
        curl_easy_setopt(__curl, CURLOPT_WRITEDATA, __fp);

        /* Follow redirects */
        curl_easy_setopt(__curl, CURLOPT_FOLLOWLOCATION, 1L);

        /* Set progress callback */
        curl_easy_setopt(__curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
        curl_easy_setopt(__curl, CURLOPT_NOPROGRESS, 0L);

        /* Perform the file download */
        __res = curl_easy_perform(__curl);

        if (__res != CURLE_OK) {
            /* Print error message on failure */
            printf_crit("failed to download the file: %s\n", curl_easy_strerror(__res));
            fclose(__fp);
            curl_easy_cleanup(__curl);
            curl_global_cleanup();
            _kodo_();
        }

        printf("\nDownload completed successfully.\n");

        /* Automatically extract archive if it's a tar.gz or zip file */
        if (strstr(fname, ".tar.gz")) {
            call_extract_tar_gz(fname);
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
        
            call_extract_zip(fname, zip_of_pos);
        }

        /* Close the file and clean up curl */
        fclose(__fp);
        curl_easy_cleanup(__curl);
    } else {
        /* Handle curl initialization failure */
        fprintf(stderr, "[err]: failed to initialize curl session\n");
    }

    /* Global cleanup for curl */
    curl_global_cleanup();
}