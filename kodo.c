/*
 * Project Name: Kodo Toolchain
 * Copyright (C) Kodo/Contributors
 *
 * This program is distributed under the terms of the GNU General Public License v2.0.
 * See the LICENSE file for details.
 *
 * Compile with GCC or CLANG
 * Required Library: build-essential, clang. libcurl4-openssl-dev, libncurses-dev, libarchive-dev
 * 
 * (default compile) -> gcc -D_GNU_SOURCE -g -Os -s kodo.c utils.c package.c server.c \
    tomlc99/toml.c cJson/cJSON.c -o kodo \
        -lm -lcurl -lncurses -lreadline -larchive
 * (with checking) -> gcc -D_GNU_SOURCE -g -fsanitize=address -fno-omit-frame-pointer -Os \
    -s kodo.c utils.c package.c server.c \
        tomlc99/toml.c cJson/cJSON.c -o kodo \
            -lm -lcurl -lncurses -lreadline -larchive
 * (default compile) -> clang -D_GNU_SOURCE -g -Os -s kodo.c utils.c package.c server.c \
    tomlc99/toml.c cJson/cJSON.c -o kodo \
        -lm -lcurl -lncurses -lreadline -larchive
    (with checking) -> clang -D_GNU_SOURCE -g -fsanitize=address -fno-omit-frame-pointer -Os \
    -s kodo.c utils.c package.c server.c \
        tomlc99/toml.c cJson/cJSON.c -o kodo \
            -lm -lcurl -lncurses -lreadline -larchive
 
 * If you want to open the code in the terminal without running it manually using the terminal, you can run the following commands.
    * nano ~/.bashrc - or zsh
    * put this at the bottom: alias kodo='/path/to/program/of/kodo'
    * exit with CTRL+X and Yes and Enter.
    * run the `kodo` command anywhere in Terminal. it will automatically run the Kodo Program according to where the program path location has been set.
 *
 */

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <limits.h>
#include <ncurses.h>
#include <dirent.h>
#include <time.h>
#include <signal.h>
#include <ftw.h>
#include <sys/file.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <archive.h>
#include <archive_entry.h>

#include "tomlc99/toml.h"

#include "color.h"
#include "utils.h"
#include "package.h"
#include "kodo.h"
#include "server.h"

int kodo_fork_sys(const char *cmd) {
    FILE *fp = popen(cmd, "r");
    if (fp == NULL) {
        perror("Error:");
        return -1;
    }
    char kd_sys_buff[256];
    while (fgets(kd_sys_buff, sizeof(kd_sys_buff), fp) != NULL)
        printf("%s", kd_sys_buff);

    int status = pclose(fp);
    return status;
}

void handle_sigint(int sig)
{
    println("Exit?, You only exit with use a \"exit\"");
    _kodo_(0);
}

void _kodo_(int sig_unused) {
    (void)sig_unused;

    struct struct_of kodo = init_kodo();
    kodo.title(NULL);

    char *home = getenv("HOME");
    if (home != NULL) {
        char history_file[512];
        snprintf(history_file, sizeof(history_file), "%s/.history", home);
        read_history(history_file);
    }
    
    using_history();

    signal(SIGINT, handle_sigint);

    char *__vcommands__[] = { "exit", "clear", "kill", "title", "help",
                              "gamemode", "pawncc", "compile", "running",
                              "debug", "stop", "restart" };
    int num_cmds = sizeof(__vcommands__) / sizeof(__vcommands__[0]);

    while (1) {
        char *ptr_cmds = readline("kodo:~$ ");
        if (ptr_cmds == NULL) {
            break;
        }

        char *malloc_dynamic_str = 
            (char *)malloc(256 * sizeof(char));
        if (malloc_dynamic_str == NULL) {
            printf_color(COL_RED, "Memory allocation failed!\n");
            break;
        }
        if (strlen(ptr_cmds) > 0) {
            add_history(ptr_cmds);

            if (home != NULL) {
                char history_file[512];
                snprintf(history_file, sizeof(history_file), "%s/.history", home);
                write_history(history_file);
            }
        }
        int c_distance = 
            INT_MAX;
        char *c_command = 
            NULL;

        for (int i = 0; i < num_cmds; i++) {
            int dist_kodo_cmds;
            dist_kodo_cmds = kodo_cmds_check(ptr_cmds, __vcommands__[i]);
            if (dist_kodo_cmds < c_distance) {
                c_distance = dist_kodo_cmds;
                c_command = __vcommands__[i];
            }
        }

        if (strncmp(ptr_cmds, "help", 4) == 0) {
            kodo_title("Kodo Toolchain | @ help");

            static char *arg;
                arg = ptr_cmds + 7;
            while (*arg == ' ') arg++;

            if (strlen(arg) == 0) {
                println("usage: help | help [<cmds>]");
                println("cmds:");
                println(" clear, exit, kill, title");
                println(" gamemode, pawncc");
                println(" compile, running, debug");
                println(" stop, restart");
            } else if (strcmp(arg, "exit") == 0) {
                println("exit: exit from Kodo. | Usage: \"exit\"");
            } else if (strcmp(arg, "clear") == 0) {
                println("clear: clear screen Kodo. | Usage: \"clear\"");
            } else if (strcmp(arg, "kill") == 0) {
                println("kill: kill - restart terminal Kodo. | Usage: \"kill\"");
            } else if (strcmp(arg, "title") == 0) {
                println("title: set-title Terminal Kodo. | Usage: \"title\" | [<args>]");
            } else if (strcmp(arg, "compile") == 0) {
                println("compile: compile your project. | Usage: \"compile\" | [<args>]");
            } else if (strcmp(arg, "running") == 0) {
                println("running: running your project. | Usage: \"running\" | [<args>]");
            } else if (strcmp(arg, "debug") == 0) {
                println("debug: debugging your project. | Usage: \"debug\" | [<args>]");
            } else if (strcmp(arg, "stop") == 0) {
                println("stop: stopped server task. | Usage: \"stop\"");
            } else if (strcmp(arg, "restart") == 0) {
                println("restart: restart server task. | Usage: \"restart\"");
            } else {
                println("help not found for: '%s'", arg);
            }
            
            continue;
        } else if (strcmp(ptr_cmds, "pawncc") == 0) {
            kodo_title("Kodo Toolchain | @ pawncc");

            char platform;

            ret_pcc:
                println("Select platform:");
                println("[L/l] Linux");
                println("[W/w] Windows");
                printf(">> ");

            if (scanf(" %c", &platform) != 1) {
                return;
            }

            signal(SIGINT, _kodo_);

            if (platform == 'L' || platform == 'l') {
                kodo_download_pawncc("linux");
            } else if (platform == 'W' || platform == 'w') {
                kodo_download_pawncc("windows");
            } else {
                printf("Invalid platform selection. use C^ to exit.\n");
                goto ret_pcc;
            }

            continue;
        } else if (strcmp(ptr_cmds, "gamemode") == 0) {
            kodo_title("Kodo Toolchain | @ gamemode");

            static
                char platform = 0;

            ret_gm:
                println("Select platform:");
                println("[L/l] Linux");
                println("[W/w] Windows");
                printf(">> ");

            if (scanf(" %c", &platform) != 1) {
                return;
            }

            signal(SIGINT, _kodo_);

            if (platform == 'L' || platform == 'l') {
                kodo_download_samp("linux");
            } else if (platform == 'W' || platform == 'w') {
                kodo_download_samp("windows");
            } else {
                printf("Invalid platform selection. use C^ to exit.\n");
                goto ret_gm;
            }

            break;
        } else if (strcmp(ptr_cmds, "clear") == 0) {
            kodo_title("Kodo Toolchain | @ clear");

            clear:
                kodo_fork_sys("clear");
        } else if (strcmp(ptr_cmds, "exit") == 0) {
            exit(1);
        }
         else if (strcmp(ptr_cmds, "kill") == 0) {
            kodo_title("Kodo Toolchain | @ kill");

            goto clear;
            _kodo_(0);
        } else if (strncmp(ptr_cmds, "title", 5) == 0) {
            char *arg = ptr_cmds + 6;
            while (*arg == ' ') arg++;
        
            if (*arg == '\0') {
                println("usage: title [<title>]");
            } else {
                printf("\033]0;%s\007", arg);
            }
        
            continue;
        } else if (strncmp(ptr_cmds, "compile", 7) == 0) {
            kodo_title("Kodo Toolchain | @ compile");
            static char *arg;
            arg = ptr_cmds + 7;
            while (*arg == ' ') arg++;
        
            static char *compile_arg1;
            compile_arg1 = strtok(arg, " ");
        
            const char *ptr_pawncc;
            int __kodo_os__ = signal_system_os();
            if (__kodo_os__ == 1) 
                ptr_pawncc = "pawncc.exe";
            else if (__kodo_os__ == 0)
                ptr_pawncc = "pawncc";

            char kodo_c_output_f_container[520];
            int format_size_c_f_container = sizeof(kodo_c_output_f_container);
        
            int find_pawncc = kodo_sef_fdir(".", ptr_pawncc);

            if (find_pawncc == 1) {
                static char *_compiler_ = NULL;
                static size_t format_size_compiler = 276;

                if (_compiler_ == NULL) {
                    _compiler_ = malloc(format_size_compiler);
                    if (!_compiler_) {
                        printf_error("Memory allocation failed for _compiler_!\n");
                        return;
                    }
                }
        
                const char *fname = "kodo.toml";
                FILE *__fp = fopen(fname, "r");
                if (!__fp) {
                    printf_error("Can't read file %s\n", fname);
                }
        
                char errbuf[256];
                toml_table_t *config = toml_parse_file(__fp, errbuf, sizeof(errbuf));
                fclose(__fp);
        
                if (!config) {
                    printf_error("parsing TOML: %s\n", errbuf);
                }
        
                toml_table_t *kodo_compiler = toml_table_in(config, "compiler");
                if (kodo_compiler) {
                    toml_datum_t option_val = toml_string_in(kodo_compiler, "option");
                    if (option_val.ok) {
                        kd_compiler_opt = option_val.u.s;
                    }
        
                    toml_datum_t output_val = toml_string_in(kodo_compiler, "output");
                    if (output_val.ok) {
                        kd_gamemode_output = output_val.u.s;
                    }
        
                    toml_array_t *include_paths = toml_array_in(kodo_compiler, "include_path");
                    if (include_paths) {
                        int array_size = toml_array_nelem(include_paths);
                        char all_paths[256] = {0};
        
                        for (int i = 0; i < array_size; i++) {
                            toml_datum_t path_val = toml_string_at(include_paths, i);
                            if (path_val.ok) {
                                if (i > 0) {
                                    strcat(all_paths, " ");
                                }
                                snprintf(all_paths + strlen(all_paths), sizeof(all_paths) - strlen(all_paths), "-i\"%s\"", path_val.u.s);
                            }
                        }
        
                        static char kd_gamemode[56];
                        if (*arg == '\0') {
                            toml_datum_t kodo_gmodes = toml_string_in(kodo_compiler, "input");
                            if (kodo_gmodes.ok) {
                                kd_gamemode_input = kodo_gmodes.u.s;
                            }
                            
                            int find_gamemodes = kodo_sef_fdir(".", kd_gamemode_input);

                            if (find_gamemodes == 1) {
                                char* container_output = strdup(kodo_sef_found[1]);
                                char* f_last_slash_container = strrchr(container_output, '/');
                                if (f_last_slash_container != NULL && *(f_last_slash_container + 1) != '\0') {
                                    *(f_last_slash_container + 1) = '\0';
                                }

                                snprintf(kodo_c_output_f_container, format_size_c_f_container, "%s%s",
                                    container_output, kd_gamemode_output);

                                kd_gamemode_input=strdup(kodo_sef_found[1]);

                                snprintf(_compiler_, format_size_compiler, "%s %s \"%s\" -o\"%s\" \"%s\"",
                                    kodo_sef_found[0],
                                    all_paths,
                                    kd_gamemode_input,
                                    kodo_c_output_f_container,
                                    kd_compiler_opt);
                            } else {
                                println("Can't locate %s", kd_gamemode_input);
                                continue;
                            }
                        } else {
                            int find_gamemodes_arg1 = kodo_sef_fdir(".", compile_arg1);

                            if (find_gamemodes_arg1 == 1) {
                                char* container_output = strdup(kodo_sef_found[1]);
                                char* f_last_slash_container = strrchr(container_output, '/');
                                if (f_last_slash_container != NULL && *(f_last_slash_container + 1) != '\0') {
                                    *(f_last_slash_container + 1) = '\0';
                                }

                                snprintf(kodo_c_output_f_container, format_size_c_f_container, "%s%s",
                                    container_output, kd_gamemode_output);

                                compile_arg1=strdup(kodo_sef_found[1]);

                                snprintf(_compiler_, format_size_compiler, "%s %s \"%s\" -o\"%s\" \"%s\"",
                                    kodo_sef_found[0],
                                    all_paths,
                                    compile_arg1,
                                    kodo_c_output_f_container,
                                    kd_compiler_opt);
                            } else {
                                println("Can't locate %s", compile_arg1);
                                continue;
                            }
                        }
                        
                        if (_compiler_) 
                            kodo_fork_sys(_compiler_);
                    }
        
                    if (_compiler_)
                        free(_compiler_);
                }
            } else {
                printf_error("pawncc not found!");
        
                char *ptr_sigA;
                ptr_sigA = readline("install now? [Y/n]: ");
        
                while (1) {
                    if (strcmp(ptr_sigA, "Y") == 0 || strcmp(ptr_sigA, "y") == 0) {
                        if (__kodo_os__ == 1) {
                            kodo_download_pawncc("windows");
                        } else if (__kodo_os__ == 0) {
                            kodo_download_pawncc("linux");
                        }
                        break;
                    } else if (strcmp(ptr_sigA, "N") == 0 || strcmp(ptr_sigA, "n") == 0) {
                        break;
                    } else {
                        printf("Invalid input. Please type Y/y to install or N/n to cancel.\n");
                        free(ptr_sigA);
                        ptr_sigA = readline("install now? [Y/n]: ");
                    }
                }
        
                free(ptr_sigA);
            }
        } else if (strncmp(ptr_cmds, "running", 7) == 0 || strncmp(ptr_cmds, "debug", 7) == 0) {
            _running_:
                if (strcmp(ptr_cmds, "debug") == 0) {
                    server_or_debug="debug";
                    kodo_title("Kodo Toolchain | @ debug");    
                } else {
                    kodo_title("Kodo Toolchain | @ running");
                }

                static char *arg;
                    arg = ptr_cmds + 7;
                while (*arg == ' ') arg++;

                char *arg1 = strtok(arg, " ");

                static char *format_prompt = NULL;
                static size_t format_size = 126;

                if (format_prompt == NULL) {
                    format_prompt = malloc(format_size);
                }

                static const char *ptr_samp = NULL;
                static const char *ptr_openmp = NULL;

                static int find_for_samp
                    = 0x0;
                static int find_for_omp
                    = 0x0;

                static int __kodo_os__;
                    __kodo_os__ = signal_system_os();
                
                if (__kodo_os__ == 1) {
                    /* windows */
                    ptr_samp="samp-server.exe";
                    ptr_openmp="omp-server.exe";
                }
                else if (__kodo_os__ == 0) {
                    /* linux */
                    ptr_samp="samp03svr";
                    ptr_openmp="omp-server";
                }

                static FILE
                    *file_s = NULL;
                if (file_s == NULL)
                    fopen(ptr_samp, "r");
                FILE
                    *file_m = NULL;
                if (file_m == NULL)
                    fopen(ptr_samp, "r");

                if (file_s)
                    find_for_samp=0x1;
                if (file_m)
                    find_for_omp=0x1;

                if (find_for_samp == 0x1) {
                    if (*arg == '\0') {
                        const char *srv_log_samp = "server_log.txt";
                        static
                            FILE *__samp_log = NULL;
                        if (__samp_log == NULL)
                            fopen(srv_log_samp, "r");

                        if (__samp_log) {
                            remove(srv_log_samp);
                        }

                        printf_color(COL_YELLOW, "running...");
                        usleep(500000);

                        snprintf(format_prompt, 126, "chmod 777 %s", ptr_samp);
                        kodo_fork_sys(format_prompt);
                        snprintf(format_prompt, 126, "./%s", ptr_samp);
                        kodo_fork_sys(format_prompt);

                        printf_color(COL_YELLOW, "Press enter to print logs..");
                        getchar();

                        if (__samp_log) {
                            snprintf(format_prompt, 126, "cat %s", srv_log_samp);
                            kodo_fork_sys(format_prompt);
                        }
                    } else {
                        kodo_server_samp(arg1, ptr_samp);
                    }
                } else if (find_for_omp == 0x1) {
                    if (*arg == '\0') {
                        const char *srv_log_omp = "log.txt";
                        static
                            FILE *__omp_log = NULL;
                        if (__omp_log == NULL)
                            fopen(srv_log_omp, "r");

                        if (__omp_log) {
                            remove(srv_log_omp);
                        }

                        printf_color(COL_YELLOW, "running...");
                        usleep(500000);

                        snprintf(format_prompt, 126, "chmod 777 %s", ptr_openmp);
                        kodo_fork_sys(format_prompt);
                        snprintf(format_prompt, 126, "./%s", ptr_openmp);
                        kodo_fork_sys(format_prompt);

                        printf_color(COL_YELLOW, "Press enter to print logs..");
                        getchar();

                        if (__omp_log) {
                            snprintf(format_prompt, 126, "cat %s", srv_log_omp);
                            kodo_fork_sys(format_prompt);
                        }
                    } else {
                        kodo_server_openmp(arg1);
                    }
                } else if (!find_for_omp || !find_for_samp) {
                    printf_error("samp-server/open.mp server not found!");

                    char *ptr_sigA;
                    ptr_sigA = readline("install now? [Y/n]: ");

                    while (1) {
                        if (strcmp(ptr_sigA, "Y") == 0 || strcmp(ptr_sigA, "y") == 0) {
                            if (__kodo_os__ == 1) {
                                kodo_download_samp("windows");
                            } else if (__kodo_os__ == 0) {
                                kodo_download_samp("linux");
                            }
                            break;
                        } else if (strcmp(ptr_sigA, "N") == 0 || strcmp(ptr_sigA, "n") == 0) {
                            break;
                        } else {
                            printf("Invalid input. Please type Y/y to install or N/n to cancel.\n");
                            free(ptr_sigA);
                            ptr_sigA = readline("install now? [Y/n]: ");
                        }
                    }

                    free(ptr_sigA);
                }

                if (format_prompt) free(format_prompt);

        } else if (strcmp(ptr_cmds, "stop") == 0) {
            kodo_title("Kodo Toolchain | @ stop");
            kodo_server_stop_tasks();
            continue;
        } else if (strcmp(ptr_cmds, "restart") == 0) {
            kodo_server_stop_tasks();

            usleep(500000);

            goto _running_
                ;
        } else if (strcmp(ptr_cmds, "kodo") == 0) {
            printf("Hello there!\n");
        } else if (strcmp(ptr_cmds, c_command) != 0 && c_distance <= 1) {
            kodo_title("Kodo Toolchain | @ undefined");
            println("Did you mean: '%s'?", c_command);
        } else {
            if (strlen(ptr_cmds) > 0) {
                kodo_title("Kodo Toolchain | @ not found");
                println("%s not found!", ptr_cmds);
            }
        }

        if (ptr_cmds) free(ptr_cmds);
        if (malloc_dynamic_str) free(malloc_dynamic_str);
    }
}

int main(void) {
    /* main is not using. */
    kodo_toml_data();
    _kodo_(0);
    return 0;
}