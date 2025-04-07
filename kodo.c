/*
 * Project Name: Kodo Toolchain
 * Project File: Kodo/kodo.c
 * Copyright (C) Kodo/Contributors
 *
 * This program is distributed under the terms of the GNU General Public License v2.0.
 * See the LICENSE file for details.
 *
 * Compile with GCC or CLANG
 * Required Library: build-essential, clang. libcurl4-openssl-dev, libncurses-dev, libarchive-dev
 * gcc -D_GNU_SOURCE -g -Os -s kodo.c utils.c package.c compiler.c server.c tomlc99/toml.c cJson/cJSON.c -o kodo -lm -lcurl -lncurses -lreadline -larchive
 * clang -D_GNU_SOURCE -g -Os -s kodo.c utils.c package.c compiler.c server.c tomlc99/toml.c cJson/cJSON.c -o kodo -lm -lcurl -lncurses -lreadline -larchive
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
#include <errno.h>
#include <math.h>
#include <limits.h>
#include <ncurses.h>
#include <dirent.h>
#include <time.h>
#include <signal.h>
#include <ftw.h>
#include <sys/file.h>
#include <curl/curl.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <archive.h>
#include <archive_entry.h>

#include "tomlc99/toml.h"
#include "cJson/cJSON.h"

static inline int kd_sys(const char *cmd) {
    return system(cmd);
}

#include "color.h"
#include "utils.h"
#include "package.h"
#include "compiler.h"
#include "kodo.h"
#include "server.h"

int kodo_title(
    const char *custom_title)
 {
    const char *title = custom_title ? custom_title : "Kodo Toolchain";
    printf("\033]0;%s\007", title);
    return 0;
}

struct struct_of {
    int (*title)(const char *);
};

struct struct_of init_kodo(void) {
    struct struct_of kodo;
    kodo.title = kodo_title;
    return kodo;
}

void _kodo_ () {
    struct struct_of kodo = init_kodo();
    kodo.title(NULL);

    char *ptr_cmds;

    using_history();

    printf("\033[4mWelcome to Kodo!\033[0m\n");

    signal(SIGINT, _kodo_);

    char *__vcommands__[] = 
    /* valid commands. */
        {
            "exit", "clear", "kill", "title", "help",
            "gamemode", "pawncc", "compile", "running",
            "debug", "stop", "restart"
        };
    int num_cmds = 
        sizeof(__vcommands__) / 
            sizeof(__vcommands__[0]);

    while (1) {
        ptr_cmds = 
            readline("user:~$ ");

        char *malloc_dynamic_str = 
            (char *)malloc(256 * sizeof(char));  /* Allocating 256 bytes of memory */
        if (malloc_dynamic_str == NULL) {
            printf_color(COL_RED, "Memory allocation failed!\n");
            break;  /* Exit the loop if memory allocation fails */
        }

        if (ptr_cmds == NULL) {
            _kodo_();
        }

        if (strlen(ptr_cmds) > 0) {
            add_history(ptr_cmds);
        }

        int c_distance = 
            INT_MAX;
        char *c_command = 
            NULL;

        /* Iterate through available commands to find the closest match */
        for (int i = 0; i < num_cmds; i++) {
            /* Calculate the distance between the input command and the current command */
            int distance = call_kom_undefined_rate(ptr_cmds, __vcommands__[i]);
            if (distance < c_distance) {
                c_distance = distance;
                c_command = __vcommands__[i];
            }
        }

        if (strncmp(ptr_cmds, "help", 4) == 0) {
            kodo_title("Kodo Toolchain | @ help");

            char *arg =
                ptr_cmds + 4;
            while (*arg == ' ') arg++;

            if (strlen(arg) == 0) {
                println("usage: help | help [<cmds>]");
                println("cmds:");
                println(" clear, exit, kill, title");
                println(" gamemode, pawncc");
                println(" compile, running, debug");
                println(" stop, restart");
            } else if (strcmp(arg, "exit") == 0) {
                println("exit: exit from Kodo. | \
Usage: \"exit\"");
            } else if (strcmp(arg, "clear") == 0) {
                println("clear: clear screen Kodo. | \
Usage: \"clear\"");
            } else if (strcmp(arg, "kill") == 0) {
                println("kill: kill - restart terminal Kodo. | \
Usage: \"kill\"");
            } else if (strcmp(arg, "title") == 0) {
                println("title: set-title Terminal Kodo. | \
Usage: \"title\" | [<args>]");
            } else if (strcmp(arg, "compile") == 0) {
                println("compile: compile your project. | \
Usage: \"compile\" | [<args>]");
            } else if (strcmp(arg, "running") == 0) {
                println("running: running your project. | \
Usage: \"running\" | [<args>]");
            } else if (strcmp(arg, "debug") == 0) {
                println("debug: debugging your project. | \
Usage: \"debug\" | [<args>]");
            } else if (strcmp(arg, "stop") == 0) {
                println("stop: stopped server task. | \
Usage: \"stop\"");
            } else if (strcmp(arg, "restart") == 0) {
                println("restart: restart server task. | \
Usage: \"restart\"");
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
                call_download_pawncc("linux");
            } else if (platform == 'W' || platform == 'w') {
                call_download_pawncc("windows");
            } else {
                printf("Invalid platform selection. use C^ to exit.\n");
                goto ret_pcc;
            }

            continue;
        } else if (strcmp(ptr_cmds, "gamemode") == 0) {
            kodo_title("Kodo Toolchain | @ gamemode");

            char platform;

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
                call_download_samp("linux");
            } else if (platform == 'W' || platform == 'w') {
                call_download_samp("windows");
            } else {
                printf("Invalid platform selection. use C^ to exit.\n");
                goto ret_gm;
            }

            break;
        } else if (strcmp(ptr_cmds, "clear") == 0) {
            kodo_title("Kodo Toolchain | @ clear");

            clear:
                kd_sys("clear");
        } else if (strcmp(ptr_cmds, "exit") == 0) {
            kodo_title("Kodo Toolchain | @ exit");

            printf("exit\n");

            char
                *ptr_sigexit;
            ptr_sigexit =
                readline("user:~$ ");

            while (1) {
                if (strcmp(ptr_sigexit, "exit") == 0) {
                    exit(1);
                } else {
                    break;
                }
            }
        } else if (strcmp(ptr_cmds, "kill") == 0) {
            kodo_title("Kodo Toolchain | @ kill");

            goto clear;
            _kodo_();
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
        
            char *arg = ptr_cmds + 7;
            while (*arg == ' ') arg++;
        
            const char *ptr_pawncc;
            int __kodo_os__ = system_os();
            if (__kodo_os__ == 1) 
                ptr_pawncc="pawncc.exe";
            else if (__kodo_os__ == 0)
                ptr_pawncc="pawncc";
        
            int find_pawncc = call_find_for_pawncc(".", ptr_pawncc);
            if (find_pawncc == 1) {
                char * _compiler_ = malloc(4096);
                
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
            
                toml_table_t *kom_compiler = toml_table_in(config, "compiler");
                if (kom_compiler) {
                    toml_datum_t option_val = toml_string_in(kom_compiler, "option");
                    if (option_val.ok) {
                        kd_compiler_opt = option_val.u.s;
                    }

                    toml_datum_t output_val = toml_string_in(kom_compiler, "output");
                    if (output_val.ok) {
                        kd_gamemode_output = output_val.u.s;
                    }
            
                    toml_array_t *include_paths = toml_array_in(kom_compiler, "include_path");
                    if (include_paths) {
                        int array_size = toml_array_nelem(include_paths);
                        char all_paths[1024] = {0};
            
                        for (int i = 0; i < array_size; i++) {
                            toml_datum_t path_val = toml_string_at(include_paths, i);
                            if (path_val.ok) {
                                if (i > 0) {
                                    strcat(all_paths, " ");
                                }
                                snprintf(all_paths + strlen(all_paths), sizeof(all_paths) - strlen(all_paths), "-i\"%s\"", path_val.u.s);
                            }
                        }
            
                        char kd_gamemode_rate[120];
                        if (*arg == '\0') {
                            toml_datum_t kodo_gmodes = toml_string_in(kom_compiler, "input");
                            if (kodo_gmodes.ok) {
                                kd_gamemode_input = kodo_gmodes.u.s;
                            }
                            snprintf(_compiler_, 4096, "%s %s \"%s\" -o\"%s\" \"%s\"", pf_found[0], all_paths, kd_gamemode_input, kd_gamemode_output, kd_compiler_opt);
                        } else {
                            char *arg1 = strtok(arg, " ");
                            snprintf(_compiler_, 4096, "%s %s \"%s\" -o\"%s\" \"%s\"", pf_found[0], all_paths, arg1, kd_gamemode_output, kd_compiler_opt);
                        }
                        printf("\n");
            
                        if (_compiler_) {
                            int ret = kd_sys(_compiler_);

                            if (ret != 0) {
                                printf_error("Err detected from compiler..");
                            }
                        }
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
                            call_download_pawncc("windows");
                        } else if (__kodo_os__ == 0) {
                            call_download_pawncc("linux");
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

                char *arg = ptr_cmds + 7;
                while (*arg == ' ') arg++;

                char *format_prompt = malloc(1000);
                size_t format_size = 1000;
            
                const char *ptr_samp = NULL;
                const char *ptr_openmp = NULL;

                int find_for_samp =
                    0;
                int find_for_omp =
                    0;

                int __kodo_os__ = system_os();
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

                FILE
                    *file_s =
                        fopen(ptr_samp, "r");
                FILE
                    *file_m =
                        fopen(ptr_openmp, "r");
                if (file_s)
                    /* if "ptr_samp" is found */
                    find_for_samp=1;
                if (file_m)
                    /* if "ptr_openmp" is found */
                    find_for_omp=1;

                if (find_for_samp == 1) {
                    if (*arg == '\0') {
                        const char *srv_log_samp = "server_log.txt";
                        FILE *__samp_log = fopen(srv_log_samp, "r");

                        if (__samp_log) {
                            remove(srv_log_samp);
                        }

                        printf_color(COL_YELLOW, "running...");
                        usleep(500000);

                        snprintf(format_prompt, 1000, "chmod 777 %s", ptr_samp);
                        kd_sys(format_prompt);
                        snprintf(format_prompt, 1000, "./%s", ptr_samp);
                        kd_sys(format_prompt);

                        printf_color(COL_YELLOW, "Press enter to print logs..");
                        getchar();

                        if (__samp_log) {
                            snprintf(format_prompt, 1000, "cat %s", srv_log_samp);
                            kd_sys(format_prompt);
                        }
                    } else {
                        char *arg1 = strtok(arg, " ");
                        call_server_samp(arg1, ptr_samp);
                    }
                } else if (find_for_omp == 1) {
                    if (*arg == '\0') {
                        const char *srv_log_omp = "log.txt";
                        FILE *__omp_log = fopen(srv_log_omp, "r");

                        if (__omp_log) {
                            remove(srv_log_omp);
                        }

                        printf_color(COL_YELLOW, "running...");
                        usleep(500000);

                        snprintf(format_prompt, 1000, "chmod 777 %s", ptr_openmp);
                        kd_sys(format_prompt);
                        snprintf(format_prompt, 1000, "./%s", ptr_openmp);
                        kd_sys(format_prompt);

                        printf_color(COL_YELLOW, "Press enter to print logs..");
                        getchar();

                        if (__omp_log) {
                            snprintf(format_prompt, 1000, "cat %s", srv_log_omp);
                            kd_sys(format_prompt);
                        }
                    } else {
                        char *arg1 = strtok(arg, " ");
                        call_server_openmp(arg1);
                    }
                } else if (!find_for_omp || !find_for_samp) {
                    printf_error("samp-server/open.mp server not found!");

                    char *ptr_sigA;
                    ptr_sigA = readline("install now? [Y/n]: ");

                    while (1) {
                        if (strcmp(ptr_sigA, "Y") == 0 || strcmp(ptr_sigA, "y") == 0) {
                            if (__kodo_os__ == 1) {
                                call_download_samp("windows");
                            } else if (__kodo_os__ == 0) {
                                call_download_samp("linux");
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

                if (format_prompt) {
                    free(format_prompt);
                }
        } else if (strcmp(ptr_cmds, "stop") == 0) {
            kodo_title("Kodo Toolchain | @ stop");

            call_server_stop_tasks();
            continue;
        } else if (strcmp(ptr_cmds, "restart") == 0) {
            call_server_stop_tasks();

            usleep(500000);

            goto _running_
                ;
        } else if (strcmp(ptr_cmds, "kodo") == 0) {
            printf("Hello there!\n");
        } else if (strcmp(ptr_cmds, c_command) != 0 && c_distance <= 1) {
            kodo_title("Kodo Toolchain | @ undefined");
            println("Did you mean: '%s'?", c_command);
            continue;
        } else {
            if (strlen(ptr_cmds) > 0) {
                kodo_title("Kodo Toolchain | @ not found");
                println("%s not found!", ptr_cmds);
            }
        }

        if (ptr_cmds) free(ptr_cmds);
        free(malloc_dynamic_str);
    }
}

int main(void) {
    /* main is not using. */
    kom_toml_data();
    _kodo_();
    return 0;
}