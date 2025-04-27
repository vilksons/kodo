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

int kodo_sef_count = 0;
char kodo_sef_found[SEF_MAX_PATH_COUNT][SEF_MAX_PATH_SIZE];

int initialize_ipawncc = 0;

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
        va_list args;
        va_start(args, format);
        printf_color(COL_YELLOW, "succes: ");
        vprintf(format, args);
        printf("\n");
        va_end(args);
}

void printf_info(const char *format, ...) {
        va_list args;
        va_start(args, format);
        printf_color(COL_YELLOW, "info: ");
        vprintf(format, args);
        printf("\n");
        va_end(args);
}

void printf_warning(const char *format, ...) {
        va_list args;
        va_start(args, format);
        printf_color(COL_GREEN, "warning: ");
        vprintf(format, args);
        printf("\n");
        va_end(args);
}

void printf_error(const char *format, ...) {
        va_list args;
        va_start(args, format);
        printf_color(COL_RED, "error: ");
        vprintf(format, args);
        printf("\n");
        va_end(args);
}

void printf_crit(const char *format, ...) {
        va_list args;
        va_start(args, format);
        printf_color(COL_RED, "crit: ");
        vprintf(format, args);
        printf("\n");
        va_end(args);
}

const char* kodo_detect_os(void) {
        char *os;
        os="unknown";

        if (getenv("OS") && strstr(getenv("OS"), "Windows_NT") ||
            getenv("WSL_INTEROP") || getenv("WSL_DISTRO_NAME"))
                os="windows";

        struct utsname sys_info;
        if (!uname(&sys_info)) if (strstr(sys_info.sysname, "Linux")) os="linux";

        return os;
}

int signal_system_os(void) {
        if (strcmp(kodo_os, "windows") == 0)
                return 0x01;
        else if (strcmp(kodo_os, "linux") == 0)
                return 0x00;
        
        return 0;
}

int kodo_toml_data(void)
{
        const char *fname = "kodo.toml";
        FILE *toml_files;

        toml_files = fopen(fname, "r");
        if (toml_files != NULL)
                fclose(toml_files);
        else {
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
        if (!procc_f) printf_error("Can't a_read file %ss\n", fname);

        char errbuf[256];
        toml_table_t *config = toml_parse_file(procc_f, errbuf, sizeof(errbuf));
        fclose(procc_f);

        if (!config) printf_error("error parsing TOML: %s\n", errbuf);

        toml_table_t *_kodo_general = toml_table_in(config, "general");
        if (_kodo_general) {
                toml_datum_t os_val = toml_string_in(_kodo_general, "os");
                if (os_val.ok) kodo_os = os_val.u.s;
        }

        return 0;
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
            if (stat(path_buff, &statbuf) == -1)
                    continue;
            if (S_ISDIR(statbuf.st_mode))
                found += kodo_sef_fdir(path_buff, sef_name);
            else if (strcmp(entry->d_name, sef_name) == 0) {
                    strncpy(kodo_sef_found[kodo_sef_count], path_buff, SEF_MAX_PATH_SIZE);
                    kodo_sef_count++;
                    found++;
            }
        }

        closedir(dir);
        return found; 
}

int kodo_sef_wmv(const char *c_src,
                 const char *c_dest)
{
        FILE *src_FILE = fopen(c_src, "rb");
        if (src_FILE == NULL)
            return 1;
        
        FILE *dest_FILE = fopen(c_dest, "wb");
        if (dest_FILE == NULL) {
            fclose(src_FILE);
            return 1;
        }
    
        char src_buff[520];
        size_t bytes;
        while ((bytes = fread(src_buff, 1, sizeof(src_buff), src_FILE)) > 0)
            fwrite(src_buff, 1, bytes, dest_FILE);
    
        fclose(src_FILE);
        fclose(dest_FILE);
    
        return 0;
}

int kodo_sef_wmwrm(const char *c_src,
                   const char *c_dest)
{
        FILE *src_FILE = fopen(c_src, "rb");
        if (src_FILE == NULL)
            return 1;
        
        FILE *dest_FILE = fopen(c_dest, "wb");
        if (dest_FILE == NULL) {
            fclose(src_FILE);
            return 1;
        }
    
        char src_buff[520];
        size_t bytes;
        while ((bytes = fread(src_buff, 1, sizeof(src_buff), src_FILE)) > 0)
            fwrite(src_buff, 1, bytes, dest_FILE);
    
        fclose(src_FILE);
        fclose(dest_FILE);
        
        if (remove(c_src) != 0)
                return 1; 

        return 0;
}

int kodo_sef_wcopy(const char *c_src,
                   const char *c_dest)
{
        FILE *src_FILE = fopen(c_src, "rb");
        if (src_FILE == NULL)
                return 1;
        FILE *dest_FILE = fopen(c_dest, "wb");
        if (dest_FILE == NULL)
                return 1;

        char src_buff[520];
        size_t bytes;
        while ((bytes = fread(src_buff, 1, sizeof(src_buff), src_FILE)) > 0)
                fwrite(src_buff, 1, bytes, dest_FILE);

        fclose(src_FILE);
        fclose(dest_FILE);

        return 0;
}

int arch_copy_data(
              struct archive *ar, struct archive *aw)
{
        int a_read;
        const void *a_buff;
        size_t size;
        la_int64_t offset;

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
        struct archive *archive_write = archive_write_disk_new();
        struct archive *archives = archive_read_new();
        struct archive_entry *entry;
        int a_read;

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
        struct archive *archives;
        struct archive *archive_write;
        struct archive_entry *entry;
        int a_read;

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
                snprintf(ext_full_path, sizeof(ext_full_path), "%s/%s",
                        __dest_path, __cur_file);
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
                                        kodo_main(0);
                                }
                        }
                        a_read = archive_write_data_block(archive_write, a_buff, size, offset);
                        if (a_read < ARCHIVE_OK) {
                                if (!has_error) {
                                        printf_error("writing block to destination: %s\n", archive_error_string(archive_write));
                                        has_error = 0x1;
                                        kodo_main(0);
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

void install_pawncc_now(void) {
        int __kodo_os__ = signal_system_os();
        int find_pawncc_exe = kodo_sef_fdir(".", "pawncc.exe");
        int find_pawncc = kodo_sef_fdir(".", "pawncc");
        int find_pawndisasm_exe = kodo_sef_fdir(".", "pawndisasm.exe");
        int find_pawndisasm = kodo_sef_fdir(".", "pawndisasm");

        int dir_pawno=0, dir_qawno=0;

        char *dest_path = NULL;
        char str_dest_path[256];

        struct stat st;
        if (stat("pawno", &st) == 0 && S_ISDIR(st.st_mode)) {
                dir_pawno=1;
                dest_path="pawno";
        } if (stat("qawno", &st) == 0 && S_ISDIR(st.st_mode)) {
                dir_qawno=1;
                dest_path="qawno";
        }
        if (!dir_pawno && !dir_qawno)
                if (mkdir("pawno", 0755) == 0)
                        dest_path="pawno";
        
        sleep(2);

        char str_finstall[1502];

        if (find_pawncc_exe == 1 && find_pawncc == 1) {
                snprintf(str_finstall, sizeof(str_finstall), "%s", kodo_sef_found[0]);
                snprintf(str_dest_path, sizeof(str_dest_path), "%s/pawncc.exe", dest_path);
                kodo_sef_wmv(str_finstall, str_dest_path);
                snprintf(str_finstall, sizeof(str_finstall), "%s", kodo_sef_found[1]);
                snprintf(str_dest_path, sizeof(str_dest_path), "%s/pawncc", dest_path);
                kodo_sef_wmv(str_finstall, str_dest_path);
        } else if (find_pawncc_exe == 1) {
                snprintf(str_finstall, sizeof(str_finstall), "%s", kodo_sef_found[0]);
                snprintf(str_dest_path, sizeof(str_dest_path), "%s/pawncc.exe", dest_path);
                kodo_sef_wmv(str_finstall, str_dest_path);
        } else if (find_pawncc == 1) {
                snprintf(str_finstall, sizeof(str_finstall), "%s", kodo_sef_found[0]);
                snprintf(str_dest_path, sizeof(str_dest_path), "%s/pawncc", dest_path);
                kodo_sef_wmv(str_finstall, str_dest_path);
        }

        if (find_pawndisasm_exe == 1 && find_pawndisasm == 1) {
                snprintf(str_finstall, sizeof(str_finstall), "%s", kodo_sef_found[3]);
                snprintf(str_dest_path, sizeof(str_dest_path), "%s/pawndisasm.exe", dest_path);
                kodo_sef_wmv(str_finstall, str_dest_path);
                snprintf(str_finstall, sizeof(str_finstall), "%s", kodo_sef_found[4]);
                snprintf(str_dest_path, sizeof(str_dest_path), "%s/pawndisasm", dest_path);
                kodo_sef_wmv(str_finstall, str_dest_path);
        } else if (find_pawndisasm_exe == 1) {
                snprintf(str_finstall, sizeof(str_finstall), "%s", kodo_sef_found[3]);
                snprintf(str_dest_path, sizeof(str_dest_path), "%s/pawndisasm.exe", dest_path);
                kodo_sef_wmv(str_finstall, str_dest_path);
        } else if (find_pawndisasm == 1) {
                snprintf(str_finstall, sizeof(str_finstall), "%s", kodo_sef_found[4]);
                snprintf(str_dest_path, sizeof(str_dest_path), "%s/pawndisasm", dest_path);
                kodo_sef_wmv(str_finstall, str_dest_path);
        }

        if (__kodo_os__ == 0x00) {
                char *str_lib_path = NULL;
                char str_full_dest_path[1502];

                int find_libpawnc = kodo_sef_fdir(".", "libpawnc.so");

                struct stat st;
                if (stat("/usr/local/lib", &st) == 0 && S_ISDIR(st.st_mode)) {
                        str_lib_path="/usr/local/lib";
                }
                if (stat("/data/data/com.termux/files/usr/local/lib/", &st) == 0 && S_ISDIR(st.st_mode)) {
                        str_lib_path="/data/data/com.termux/files/usr/local/lib/";
                }

                snprintf(str_finstall, sizeof(str_finstall), "%s", kodo_sef_found[5]);

                if (find_libpawnc == 1) {
                        snprintf(str_full_dest_path, sizeof(str_full_dest_path), "%s/libpawnc.so", str_lib_path);
                        kodo_sef_wmv(str_finstall, str_full_dest_path);
                }

                if (strcmp(str_lib_path, "/usr/local/lib") == 0) {
                        int sys_sudo = system("sudo");
                        if (sys_sudo == 0) kodo_sys("sudo ldconfig");
                        else kodo_sys("ldconfig");

                        kodo_sys("export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH");
                        kodo_sys("source ~/.bashrc");

                        FILE *procc_f;
                        char buff[520];
                        int found = 0;

                        procc_f = popen("ldconfig -p | grep libpawnc", "r");
                        if (procc_f == NULL) {
                                perror("popen failed");
                                kodo_main(0);
                        }
                        while (fgets(buff, sizeof(buff), procc_f) != NULL) found = 1;

                        pclose(procc_f);
                        if (!found)  {
                                printf_error("libpawnc NOT found.\n");
                                struct stat st;
                                if (stat("/usr/local/lib32", &st) == 0 && S_ISDIR(st.st_mode)) { 
                                        printf_info("moving libpawnc.so to /usr/local/lib32");

                                        int find_libpawnc_new = kodo_sef_fdir("/usr/local/lib", "libpawnc.so");

                                        if (find_libpawnc_new == 1) kodo_sef_wmwrm(kodo_sef_found[6], "/usr/local/lib32");

                                        if (sys_sudo == 0) kodo_sys("sudo ldconfig");
                                        else kodo_sys("ldconfig");

                                        kodo_sys("export LD_LIBRARY_PATH=/usr/local/lib32:$LD_LIBRARY_PATH");
                                        kodo_sys("source ~/.bashrc");
                                }
                        }
                } else if (strcmp(str_lib_path, "/data/data/com.termux/files/usr/local/lib/") == 0) {
                        kodo_sys("export LD_LIBRARY_PATH=/data/data/com.termux/files/usr/local/lib:$LD_LIBRARY_PATH");
                        kodo_sys("source ~/.bashrc");
                }
        }

        printf_color(COL_YELLOW, "apply finished!\n");

        kodo_main(0);
}

void kodo_download_file(const char *url,
                        const char *fname
) {
        CURL *__curl = curl_easy_init();
        CURLcode __res;
        FILE *procc_f;

        procc_f = fopen(fname, "wb");
        if (procc_f == NULL)
                printf_crit("failed to open file for writing in 'kodo_download_file'");

        curl_global_init(CURL_GLOBAL_DEFAULT);

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
                        } else strcpy(zip_of_pos, fname);
                    
                        kodo_extract_zip(fname, zip_of_pos);
                }

                fclose(procc_f);
                curl_easy_cleanup(__curl);

                if (initialize_ipawncc == 1) {
                        initialize_ipawncc=0;

                        char *ptr_sigA;
                        ptr_sigA = readline("apply pawncc now? [Y/n]: ");
            
                        while (1) {
                                if (ptr_sigA == NULL || strlen(ptr_sigA) == 0 || strcmp(ptr_sigA, "Y") == 0 || strcmp(ptr_sigA, "y") == 0) {
                                        install_pawncc_now();
                                } else if (strcmp(ptr_sigA, "N") == 0 || strcmp(ptr_sigA, "n") == 0) {
                                        break;
                                } else break;
                        }

                        if (ptr_sigA)
                                free(ptr_sigA);
                }
        } else fprintf(stderr, "[err]: Failed to initialize curl session\n");

        curl_global_cleanup();

        kodo_main(0);
}