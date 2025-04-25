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

int kodo_sef_count = 0; char kodo_sef_found[SEF_MAX_PATH_COUNT][SEF_MAX_PATH_SIZE];

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

void printf_color(const char *color,
                  const char *format, ...)
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

const char* kodo_detect_os(void) {
        const char *__win__[] = { "/c/windows/System32", "/windows/System32" };
        for (int i = 0; i < 3; i++) if (access(__win__[i], F_OK) == 0) return "windows";
        if (getenv("WSL_INTEROP") != NULL) return "windows";

        struct utsname buf;
        if (uname(&buf) == 0) {
            if (strstr(buf.sysname, "Linux") != NULL) {
                return "linux";
            }
        }

        return "unknowns";
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

        FILE *procc_f = fopen(fname, "r");
        if (!procc_f) {
            printf_error("Can't a_read file %ss\n", fname);
        }

        char errbuf[256];
        toml_table_t *config = toml_parse_file(procc_f, errbuf, sizeof(errbuf));
        fclose(procc_f);

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
        for (int i = 0; i <= len1; i++) 
            dist[i] = (int *)malloc((len2 + 1) * sizeof(int));

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
        
        for (int i = 0; i <= len1; i++)
            free(dist[i]);

        free(dist);

        return kodo_res;
}

int kodo_sef_fdir(const char *sef_path,
                  const char *sef_name)
{
        char path_buff[SEF_MAX_PATH_SIZE];
        struct dirent *entry;
        struct stat statbuf;
        int found = 0;
        DIR *dir;

        if ((dir = opendir(sef_path)) == NULL) {
            perror("opendir");
            kodo_main(0);
        }
        
        while ((entry = readdir(dir)) != NULL && kodo_sef_count < SEF_MAX_PATH_COUNT) {
            if (!strcmp(entry->d_name, ".") ||
                !strcmp(entry->d_name, ".."))
                continue;
            snprintf(path_buff, sizeof(path_buff), "%s/%s",
                sef_path, entry->d_name);
            if (stat(path_buff, &statbuf) == -1) {
                continue;
            }
            if (S_ISDIR(statbuf.st_mode)) {
                found += kodo_sef_fdir(path_buff, sef_name);
            } else if (strcmp(entry->d_name, sef_name) == 0) {
                strncpy(kodo_sef_found[kodo_sef_count], path_buff, SEF_MAX_PATH_SIZE);
                kodo_sef_count++;
                found++;
            }
        }

        closedir(dir);
        return found; 
}

int arch_copy_data(
              struct archive *ar, struct archive *aw)
{
        int a_read;
        const void
            *a_buff;
        size_t
            size;
        la_int64_t
            offset;

        for (;;) {
            a_read = archive_read_data_block(ar, &a_buff, &size, &offset);
            if (a_read == ARCHIVE_EOF)
                return ARCHIVE_OK;
            if (a_read != ARCHIVE_OK)
                return a_read;
            a_read = archive_write_data_block(aw, a_buff, size, offset);
            if (a_read != ARCHIVE_OK) {
                printf_error("Write error: %s\n", archive_error_string(aw));
                return a_read;
            }
        }
}

int kodo_extract_tar_gz(const char *tar_files) {
        struct archive
            *archives =
                archive_read_new();
        struct archive
            *archive_write =
                archive_write_disk_new();
        struct archive_entry
            *entry;
        int
            a_read;

        archive_read_support_format_tar(archives);
        archive_read_support_filter_gzip(archives);

        a_read = archive_read_open_filename(archives, tar_files, 10240);
        if (a_read != ARCHIVE_OK) {
            printf("Can't resume. sys can't write/open file %s\n", tar_files);
            kodo_main(0);
        }

        while (archive_read_next_header(archives, &entry) == ARCHIVE_OK) {
            archive_write_header(archive_write, entry);
            arch_copy_data(archives, archive_write);
            archive_write_finish_entry(archive_write);
        }

        archive_read_close(archives);
        archive_read_free(archives);
        archive_write_close(archive_write);
        archive_write_free(archive_write);

        return 0;
}

int kodo_extract_zip(
                const char *zip_path, const char *__dest_path)
{
        struct archive
            *archives;
        struct archive
            *archive_write;
        struct archive_entry
            *entry;
        int
            a_read;

        archives = archive_read_new();
        archive_read_support_format_zip(archives);
        archive_read_support_filter_all(archives);

        if ((a_read = archive_read_open_filename(archives, zip_path, 10240))) {
            printf("Can't resume. sys can't write/open file %s\n", archive_error_string(archives));
            kodo_main(0);
        }

        archive_write = archive_write_disk_new();
        archive_write_disk_set_options(archive_write, ARCHIVE_EXTRACT_TIME);
        archive_write_disk_set_standard_lookup(archive_write);

        int has_error = 0x0;

        while (archive_read_next_header(archives, &entry) == ARCHIVE_OK) {
            const char *__cur_file = archive_entry_pathname(entry);

            char ext_full_path[128];
            snprintf(ext_full_path, sizeof(ext_full_path), "%s/%s", __dest_path, __cur_file);
            archive_entry_set_pathname(entry, ext_full_path);

            a_read = archive_write_header(archive_write, entry);
            if (a_read != ARCHIVE_OK) {
                if (!has_error) {
                    printf_error("during extraction: %s\n", archive_error_string(archive_write));
                    has_error = 0x1;
                }
            } else {
                const void *a_buff;
                size_t size;
                la_int64_t offset;

                while (1) {
                    a_read = archive_read_data_block(archives, &a_buff, &size, &offset);
                    if (a_read == ARCHIVE_EOF)
                        break;
                    if (a_read < ARCHIVE_OK) {
                        if (!has_error) {
                            printf_error("reading block from archive: %s\n", archive_error_string(archives));
                            has_error = 0x1;
                        }
                    }
                    a_read = archive_write_data_block(archive_write, a_buff, size, offset);
                    if (a_read < ARCHIVE_OK) {
                        if (!has_error) {
                            printf_error("writing block to destination: %s\n", archive_error_string(archive_write));
                            has_error = 0x1;
                        }
                    }
                }
            }
        }

        archive_read_close(archives);
        archive_read_free(archives);
        archive_write_close(archive_write);
        archive_write_free(archive_write);

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
            *procc_f;

        procc_f = fopen(fname, "wb");
        if (procc_f == NULL) {
            printf_crit("failed to open file for writing in 'kodo_download_file'");
            kodo_main(0);
        }

        curl_global_init(CURL_GLOBAL_DEFAULT);
        __curl =
            curl_easy_init();

        if (__curl) {
            curl_easy_setopt(__curl, CURLOPT_URL, url);
            curl_easy_setopt(__curl, CURLOPT_WRITEFUNCTION, write_file);
            curl_easy_setopt(__curl, CURLOPT_WRITEDATA, procc_f);
            curl_easy_setopt(__curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(__curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
            curl_easy_setopt(__curl, CURLOPT_NOPROGRESS, 0L);

            __res = curl_easy_perform(__curl);

            if (__res != CURLE_OK) {
                printf_crit("failed to download the file: %s\n", curl_easy_strerror(__res));
                curl_easy_cleanup(__curl);
                curl_global_cleanup();
                fclose(procc_f);
                kodo_main(0);
            }

            printf("\nDownload completed successfully.\n");

            if (strstr(fname, ".tar.gz"))
                kodo_extract_tar_gz(fname);
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

            fclose(procc_f);
            curl_easy_cleanup(__curl);
        } else {
            fprintf(stderr, "[err]: Failed to initialize curl session\n");
        }

        curl_global_cleanup();

        kodo_main(0);
}