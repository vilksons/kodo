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

inline void null_variables(void) { KodoConfig kodo_config = {0}; }
inline int kodo_sys(const char *cmd) { return system(cmd); }

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

inline void handle_sigint(int sig)
{
        println("Exit?, You only exit with use a \"exit\"");
        kodo_main(0);
}

inline int kodo_title(const char *__title)
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
        /*
         * The function kodo_detect_os attempts to detect the current operating system at runtime
         * and returns a string representing the OS name.
         *
         * Detailed behavior:
         * 
         * 1. Initialize a char pointer `os` with the default value "unknown".
         *    - This acts as a fallback in case detection fails.
         *
         * 2. Check if the environment variable "OS" exists and contains "Windows_NT",
         *    OR if the "WSL_INTEROP" or "WSL_DISTRO_NAME" environment variables are set:
         *    - These environment variables indicate a Windows or WSL (Windows Subsystem for Linux) environment.
         *    - If any of these conditions are true, immediately return "windows".
         *
         * 3. If not detected as Windows/WSL:
         *    - Use `uname()` to retrieve system information into a `utsname` structure.
         *    - If `uname` succeeds and `sys_info.sysname` contains "Linux", set `os` to "linux".
         *
         * 4. Finally, return the detected OS string ("windows", "linux", or "unknown").
         *
         * Notes:
         * - The detection method uses both environment variables and system calls for broader compatibility.
         * - `getenv` is used multiple times without storing its result; for better performance, it could be called once and cached.
         * - `uname` is only available on POSIX systems (Linux/macOS/Unix); if compiled on Windows without WSL, it may fail.
         * - Minor redundancy: when returning `os="windows"`, the assignment to `os` is technically unnecessary if directly returning the string.
         */

        char *os;
        os="unknown";

        if (getenv("OS") && strstr(getenv("OS"), "Windows_NT") ||
            getenv("WSL_INTEROP") || getenv("WSL_DISTRO_NAME")) {
                return os="windows";
        }

        struct utsname sys_info;
        if (!uname(&sys_info)) if (strstr(sys_info.sysname, "Linux")) os="linux";

        return os;
}

int signal_system_os(void) {
        if (strcmp(kodo_config.kodo_os, "windows") == 0)
                return 0x01;
        else if (strcmp(kodo_config.kodo_os, "linux") == 0)
                return 0x00;
        
        return 0;
}

int kodo_toml_data(void)
{
        /*
         * The function kodo_toml_data is responsible for handling the "kodo.toml" configuration file.
         * It ensures the file exists, generates a default configuration if it doesn't, and then reads
         * and parses the contents of the TOML file into the application's configuration.
         *
         * Here are the detailed steps:
         *
         * 1. Attempt to open "kodo.toml" in read mode ("r").
         *    - If the file exists and opens successfully, immediately close it; no changes are needed.
         *    - If the file does not exist (fopen returns NULL), proceed to create it.
         *
         * 2. If creating a new file:
         *    - Open "kodo.toml" in write mode ("w").
         *    - If the file opens successfully:
         *        - Detect the current operating system using `kodo_detect_os()`.
         *        - Write a default TOML configuration into the file. This includes:
         *          - A [general] section specifying the detected OS.
         *          - A [compiler] section specifying:
         *              - Compiler options.
         *              - Include paths as an array of strings.
         *              - Default input and output file names.
         *        - After writing, close the file.
         *
         * 3. Re-open "kodo.toml" in read mode to process it.
         *    - If opening fails, an error is printed and the process likely stops (via `printf_error`).
         *
         * 4. Parse the opened TOML file into a `toml_table_t` structure using `toml_parse_file`.
         *    - If parsing fails, an error is printed containing details from the error buffer.
         *
         * 5. If parsing succeeds:
         *    - Access the "general" table section from the parsed TOML.
         *    - From "general", retrieve the value associated with the "os" key.
         *    - If successful, assign the OS value to the global `kodo_config.kodo_os`.
         *
         * 6. Finally, return 0 to indicate successful execution.
         *
         * Notes:
         * - The function ensures a fallback mechanism: if the configuration file is missing, it will generate
         *   a default one with sensible settings, preventing failures in later parts of the program.
         * - Error handling is minimal but direct: errors on file operations or TOML parsing are immediately reported.
         * - Memory management caution: `toml_parse_file` might allocate resources that should eventually be freed.
         *   (In the current code snippet, freeing the `config` object is not shown.)
         */

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
                if (os_val.ok) kodo_config.kodo_os = os_val.u.s;
        }

        return 0;
}

int kodo_sef_fdir(const char *sef_path,
                  const char *sef_name)
{
        char path_buff[SEF_PATH_SIZE];
        struct dirent *entry;
        struct stat statbuf;
        DIR *dir;

        if ((dir = opendir(sef_path)) == NULL) {
                perror("opendir");
                kodo_main(0);
        }

        while ((entry = readdir(dir)) != NULL) {
                if (entry->d_name[0] == '.' && 
                   (entry->d_name[1] == '\0' || 
                   (entry->d_name[1] == '.' && entry->d_name[2] == '\0')))
                        continue;

                snprintf(path_buff, sizeof(path_buff), "%s/%s", sef_path, entry->d_name);

                if (entry->d_type == DT_DIR) {
                        if (kodo_sef_fdir(path_buff, sef_name)) {
                                closedir(dir);
                                return 1;
                        }
                } else if (entry->d_type == DT_REG) {
                        if (strcmp(entry->d_name, sef_name) == 0) {
                                strncpy(kodo_config.kodo_sef_found[kodo_config.kodo_sef_count],
                                        path_buff, SEF_PATH_SIZE);
                                kodo_config.kodo_sef_count++;
                                closedir(dir);
                                return 1;
                        }
                } else {
                        if (stat(path_buff, &statbuf) == -1)
                                continue;
                        if (S_ISDIR(statbuf.st_mode)) {
                                if (kodo_sef_fdir(path_buff, sef_name)) {
                                        closedir(dir);
                                        return 1;
                                }
                        } else if (S_ISREG(statbuf.st_mode) && strcmp(entry->d_name, sef_name) == 0) {
                                strncpy(kodo_config.kodo_sef_found[kodo_config.kodo_sef_count],
                                        path_buff,
                                        SEF_PATH_SIZE);
                                kodo_config.kodo_sef_count++;
                                closedir(dir);
                                return 1;
                        }
                }
        }

        closedir(dir);
        return 0;
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

static int arch_copy_data(struct archive *ar, struct archive *aw) {
        int a_read;
        const void *a_buff;
        size_t size;
        la_int64_t offset;
    
        while (1) {
            a_read = archive_read_data_block(ar, &a_buff, &size, &offset);
            if (a_read == ARCHIVE_EOF) return ARCHIVE_OK;
            if (a_read != ARCHIVE_OK) {
                printf("Read error: %s\n", archive_error_string(ar));
                return a_read;
            }
            a_read = archive_write_data_block(aw, a_buff, size, offset);
            if (a_read != ARCHIVE_OK) {
                printf("Write error: %s\n", archive_error_string(aw));
                return a_read;
            }
        }
}

int kodo_extract_archive(const char *tar_files) {
        struct archive *archive_write = archive_write_disk_new();
        struct archive *archives = archive_read_new();
        struct archive_entry *entry;
        int a_read;
        
        archive_write_disk_set_options(archive_write,
                ARCHIVE_EXTRACT_TIME |
                ARCHIVE_EXTRACT_PERM |
                ARCHIVE_EXTRACT_ACL |
                ARCHIVE_EXTRACT_FFLAGS |
                ARCHIVE_EXTRACT_UNLINK |
                ARCHIVE_EXTRACT_SECURE_SYMLINKS |
                ARCHIVE_EXTRACT_SECURE_NODOTDOT |
                ARCHIVE_EXTRACT_NO_OVERWRITE_NEWER);
            
        archive_read_support_format_all(archives);
        archive_read_support_filter_all(archives);
        
        a_read = archive_read_open_filename(archives, tar_files, 1024 * 1024);
        if (a_read != ARCHIVE_OK) {
                printf_error("Can't open file: %s\n", archive_error_string(archives));
                archive_read_free(archives);
                archive_write_free(archive_write);
                return -1;
        }
    
        while (1) {
            a_read = archive_read_next_header(archives, &entry);
            if (a_read == ARCHIVE_EOF) break;
            if (a_read != ARCHIVE_OK) {
                        printf_error("header: %s\n",
                                archive_error_string(archives));
                        break;
            }
    
            a_read = archive_write_header(archive_write, entry);
            if (a_read != ARCHIVE_OK) {
                        printf_error("header: %s\n",
                                archive_error_string(archive_write));
                        break;
            }
    
            if (archive_entry_size(entry) > 0) {
                a_read = arch_copy_data(archives, archive_write);
                if (a_read != ARCHIVE_OK) {
                        printf_error("data: %s\n",
                                archive_error_string(archives));
                        break;
                }
            }
    
            archive_write_finish_entry(archive_write);
        }
    
        archive_read_close(archives);
        archive_read_free(archives);
        archive_write_close(archive_write);
        archive_write_free(archive_write);
    
        return (a_read == ARCHIVE_EOF) ? 0 : -1;
}

void kodo_extract_zip(const char *zip_path, const char *__dest_path)
{
        struct archive *archives;
        struct archive *archive_write;
        struct archive_entry *entry;
        int a_read;

        archives = archive_read_new();
        archive_read_support_format_zip(archives);
        archive_read_support_filter_all(archives);

        if ((a_read = archive_read_open_filename(archives, zip_path, 1024 * 1024))) {
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
                                printf_error("during extraction: %s\n",
                                        archive_error_string(archive_write));
                                has_error = 0x1;
                                break;
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
                                        printf_error("reading block from archive: %s\n",
                                                archive_error_string(archives));
                                        has_error = 0x2;
                                        break;
                                }
                        }
                        a_read = archive_write_data_block(archive_write, a_buff, size, offset);
                        if (a_read < ARCHIVE_OK) {
                                if (!has_error) {
                                        printf_error("writing block to destination: %s\n",
                                                archive_error_string(archive_write));
                                        has_error = 0x3;
                                        break;
                                }
                        }
                    }
                }
        }

        archive_read_close(archives);
        archive_read_free(archives);
        archive_write_close(archive_write);
        archive_write_free(archive_write);
}

static size_t write_file(void *ptr,
                  size_t size,
                  size_t nmemb,
                  FILE *stream
) {
        size_t written = fwrite(ptr, size, nmemb, stream);
        return written;
}
static int progress_callback(void *ptr,
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

static void install_pawncc_now(void) {
        /*
         * install_pawncc_now - Install or move the pawncc-related binaries into a usable structure.
         *
         * Workflow Overview:
         * ------------------
         * 1. Detect operating system type (Linux or Windows).
         * 2. Search the current directory (".") for:
         *    - pawncc
         *    - pawncc.exe
         *    - pawndisasm
         *    - pawndisasm.exe
         *    - libpawnc.so (Linux only)
         *
         * 3. Find or create a destination directory:
         *    - Prefer using an existing "pawno" or "qawno" directory.
         *    - If neither exists, create "pawno".
         *
         * 4. Move (`kodo_sef_wmv`) the found binaries to the destination directory:
         *    - Move both pawncc and pawncc.exe if they exist.
         *    - Move both pawndisasm and pawndisasm.exe if they exist.
         *
         * 5. On Linux/Termux:
         *    - Move `libpawnc.so` to an appropriate library folder:
         *      - Prefer `/usr/local/lib32/` if it exists.
         *      - Otherwise, use `/usr/local/lib/`.
         *      - Special handling for Termux environments (`/data/data/com.termux/...`).
         *    - After moving:
         *      - Run `ldconfig` (with `sudo` if available) to update library cache.
         *      - Update `LD_LIBRARY_PATH` environment variable to include the new lib directory.
         *
         * 6. Print "apply finished!" in yellow color.
         *
         * 7. Call `kodo_main(0)` to continue/reset program flow.
         *
         * Key behaviors and decisions:
         * -----------------------------
         * - Checks for both `.exe` and non-`.exe` versions to support Windows/Linux automatically.
         * - Prefers not to overwrite unless necessary.
         * - Tries to make the system immediately able to run pawncc without manual steps.
         * - Careful with directory detection and library paths.
         *
         * Assumptions:
         * ------------
         * - `kodo_sef_fdir`, `kodo_sef_wmv`, `kodo_sys`, `kodo_main`, `printf_color`, `printf_error` are properly defined elsewhere.
         * - `kodo_config.kodo_sef_found` and `kodo_config.kodo_sef_count` are populated correctly.
         * - `COL_YELLOW` is a constant for color printing.
         * - The environment provides access to `stat`, `sleep`, `mkdir`, and common C standard functions.
         */

        int __kodo_os__ = signal_system_os();
        int find_pawncc_exe = kodo_sef_fdir(".", "pawncc.exe"),
            find_pawncc = kodo_sef_fdir(".", "pawncc"),
            find_pawndisasm_exe = kodo_sef_fdir(".", "pawndisasm.exe"),
            find_pawndisasm = kodo_sef_fdir(".", "pawndisasm");

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

        char pawncc_dest_path[1024], pawncc_exe_dest_path[1024],
             pawndisasm_dest_path[1024], pawndisasm_exe_dest_path[1024];

        for (int i = 0; i < kodo_config.kodo_sef_count; i++) {
                if (strstr(kodo_config.kodo_sef_found[i], "pawncc") && strstr(kodo_config.kodo_sef_found[i], "pawncc.exe") == NULL) {
                        snprintf(pawncc_dest_path, sizeof(pawncc_dest_path), "%s",
                                kodo_config.kodo_sef_found[i]);
                } if (strstr(kodo_config.kodo_sef_found[i], "pawncc.exe") && strstr(kodo_config.kodo_sef_found[i], "pawncc") &&
                        strstr(kodo_config.kodo_sef_found[i], "pawncc.exe") == strstr(kodo_config.kodo_sef_found[i], "pawncc")) {
                        snprintf(pawncc_exe_dest_path, sizeof(pawncc_exe_dest_path), "%s",
                                kodo_config.kodo_sef_found[i]);
                }
                if (strstr(kodo_config.kodo_sef_found[i], "pawndisasm") && strstr(kodo_config.kodo_sef_found[i], "pawndisasm.exe") == NULL) { 
                        snprintf(pawndisasm_dest_path, sizeof(pawndisasm_dest_path), "%s",
                                kodo_config.kodo_sef_found[i]);
                } if (strstr(kodo_config.kodo_sef_found[i], "pawndisasm.exe") && strstr(kodo_config.kodo_sef_found[i], "pawndisasm") &&
                        strstr(kodo_config.kodo_sef_found[i], "pawndisasm.exe") == strstr(kodo_config.kodo_sef_found[i], "pawndisasm")) {
                        snprintf(pawndisasm_exe_dest_path, sizeof(pawndisasm_exe_dest_path), "%s",
                                kodo_config.kodo_sef_found[i]);
                }
        }
        
        if (find_pawncc_exe && find_pawncc) {
                snprintf(str_dest_path, sizeof(str_dest_path), "%s/pawncc.exe", dest_path);
                kodo_sef_wmv(pawncc_exe_dest_path, str_dest_path);
                snprintf(str_dest_path, sizeof(str_dest_path), "%s/pawncc", dest_path);
                kodo_sef_wmv(pawncc_dest_path, str_dest_path);
        } else if (find_pawncc_exe) {
                snprintf(str_dest_path, sizeof(str_dest_path), "%s/pawncc.exe", dest_path);
                kodo_sef_wmv(pawncc_exe_dest_path, str_dest_path);
        } else if (find_pawncc) {
                snprintf(str_dest_path, sizeof(str_dest_path), "%s/pawncc", dest_path);
                kodo_sef_wmv(pawncc_dest_path, str_dest_path);
        }
        if (find_pawndisasm_exe && find_pawndisasm) {
                snprintf(str_dest_path, sizeof(str_dest_path), "%s/pawndisasm.exe", dest_path);
                kodo_sef_wmv(pawndisasm_exe_dest_path, str_dest_path);
                snprintf(str_dest_path, sizeof(str_dest_path), "%s/pawndisasm", dest_path);
                kodo_sef_wmv(pawndisasm_dest_path, str_dest_path);
        } else if (find_pawndisasm_exe) {
                snprintf(str_dest_path, sizeof(str_dest_path), "%s/pawndisasm.exe", dest_path);
                kodo_sef_wmv(pawndisasm_exe_dest_path, str_dest_path);
        } else if (find_pawndisasm) {
                snprintf(str_dest_path, sizeof(str_dest_path), "%s/pawndisasm", dest_path);
                kodo_sef_wmv(pawndisasm_dest_path, str_dest_path);
        }

        if (__kodo_os__ == 0x00) {
                char *str_lib_path = NULL;
                char str_full_dest_path[1502];

                int find_libpawnc = kodo_sef_fdir(".", "libpawnc.so");
                char libpawnc_dest_path[1024];
                for (int i = 0; i < kodo_config.kodo_sef_count; i++) {
                        if (strstr(kodo_config.kodo_sef_found[i], "libpawnc.so")) {
                                snprintf(libpawnc_dest_path, sizeof(libpawnc_dest_path), "%s", kodo_config.kodo_sef_found[i]);
                                break;
                        }
                }

                struct stat st;
                int lib_or_lib32 = 0;
                if (stat("/usr/local/lib32", &st) == 0 && S_ISDIR(st.st_mode)) { 
                        lib_or_lib32=2;
                        str_lib_path="/usr/local/lib32";
                } else if (stat("/data/data/com.termux/files/usr/local/lib/", &st) == 0 && S_ISDIR(st.st_mode))
                        str_lib_path="/data/data/com.termux/files/usr/local/lib/";
                else if (stat("/usr/local/lib", &st) == 0 && S_ISDIR(st.st_mode)) { 
                        lib_or_lib32=1;
                        str_lib_path="/usr/local/lib";
                } else printf_error("Can't found ../usr/local/lib!");

                if (find_libpawnc == 1) {
                        snprintf(str_full_dest_path, sizeof(str_full_dest_path), "%s/libpawnc.so", str_lib_path);
                        kodo_sef_wmv(libpawnc_dest_path, str_full_dest_path);
                }

                if (strcmp(str_lib_path, "/usr/local/lib") == 0) {
                        int sys_sudo = system("which sudo > /dev/null 2>&1");
                        if (sys_sudo == 0) kodo_sys("sudo ldconfig");
                        else kodo_sys("ldconfig");

                        if (lib_or_lib32 == 1) {
                                const char *old_path_lib = getenv("LD_LIBRARY_PATH");
                                char new_path_lib[1024];
                                if (old_path_lib) snprintf(new_path_lib, sizeof(new_path_lib),
                                        "/usr/local/lib:%s",
                                        old_path_lib);
                                else snprintf(new_path_lib, sizeof(new_path_lib),
                                        "/usr/local/lib");
                                setenv("LD_LIBRARY_PATH", new_path_lib, 1);
                        } else if (lib_or_lib32 == 2) {
                                const char *old_path_lib32 = getenv("LD_LIBRARY_PATH");
                                char new_path_lib32[1024];
                                if (old_path_lib32) snprintf(new_path_lib32, sizeof(new_path_lib32),
                                        "/usr/local/lib32:%s",
                                        old_path_lib32);
                                else snprintf(new_path_lib32, sizeof(new_path_lib32),
                                        "/usr/local/lib32");
                                setenv("LD_LIBRARY_PATH", new_path_lib32, 1);
                        }
                } else if (strcmp(str_lib_path, "/data/data/com.termux/files/usr/local/lib/") == 0) {
                        const char *old_path_lib_tr = getenv("LD_LIBRARY_PATH");
                        char new_path_lib_tr[1024];
                        if (old_path_lib_tr) snprintf(new_path_lib_tr, sizeof(new_path_lib_tr),
                                "/data/data/com.termux/files/usr/local/lib:%s",
                                old_path_lib_tr);
                        else snprintf(new_path_lib_tr, sizeof(new_path_lib_tr), 
                                "/data/data/com.termux/files/usr/local/lib");
                        setenv("LD_LIBRARY_PATH", new_path_lib_tr, 1);
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

                if (strstr(fname, ".tar") || strstr(fname, ".tar.gz"))
                        kodo_extract_archive(fname);
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

                if (kodo_config.init_ipc == 1) {
                        kodo_config.init_ipc=0;

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
