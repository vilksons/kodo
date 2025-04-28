/*
 * Project Name: Kodo Toolchain
 * Copyright (C) Kodo/Contributors
 *
 * This program is distributed under the terms of the GNU General Public License v2.0.
 * See the LICENSE file for details.
 *
 * Compile with GCC or CLANG
 * Required Library: -lm -lcurl -lncurses -lreadline -larchive
 * 
 * (default compile) - build with GCC, Make, Makefile.
 * ~ build with `make`
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
#include <math.h>
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

void kodo_main(int sig_unused) {
        (void)sig_unused;
        signal(SIGINT, handle_sigint);
        kodo_title(NULL);
        kodo_toml_data();
        using_history();
        reset_variables();
        while (1) {
            char* pattern_COMMANDS = readline("kodo:~$ ");
            if (pattern_COMMANDS == NULL) break;
            if (strlen(pattern_COMMANDS) > 0) add_history(pattern_COMMANDS);
            const char* commands_ok[] = {
                "exit", "clear", "kill", "title", "help",
                "gamemode", "pawncc", "compile", "running",
                "debug", "stop", "restart"
            };
            int c_distance = INT_MAX;
            const char* c_command = NULL;
            int for_num_cmds;
            for_num_cmds = sizeof(commands_ok) / sizeof(commands_ok[0]);
            for (int i = 0; i < for_num_cmds; i++) {
                const char* _str1 = pattern_COMMANDS;
                const char* _str2 = commands_ok[i];
                int len1 = strlen(_str1);
                int len2 = strlen(_str2);
                int** dist = (int**)malloc((len1 + 1) * sizeof(int*));
                for (int i = 0; i <= len1; i++)
                    dist[i] = (int*)malloc((len2 + 1) * sizeof(int));
                for (int i = 0; i <= len1; i++) for (int j = 0; j <= len2; j++) {
                    if (i == 0)
                        dist[i][j] = j;
                    else if (j == 0)
                        dist[i][j] = i;
                    else {
                        int cost = (_str1[i - 1] == _str2[j - 1]) ? 0 : 1;
                        dist[i][j] = fmin(
                            fmin(dist[i - 1][j] + 1, dist[i][j - 1] + 1),
                            dist[i - 1][j - 1] + cost
                        );
                    }
                }
                int of_dist_kodo_cmds = dist[len1][len2];
                if (of_dist_kodo_cmds < c_distance) {
                    c_distance = of_dist_kodo_cmds;
                    c_command = commands_ok[i];
                }
                for (int i = 0; i <= len1; i++)
                    free(dist[i]);
                free(dist);
            }
        
            if (strncmp(pattern_COMMANDS, "help", 4) == 0) {
                kodo_title("Kodo Toolchain | @ help");

                static char *arg;
                    arg = pattern_COMMANDS + 4;
                while (*arg == ' ') arg++;

                if (strlen(arg) == 0) {
                    println("usage: help | help [<cmds>]");
                    println("cmds:");
                    println(" clear, exit, kill, title");
                    println(" gamemode, pawncc");
                    println(" compile, running, debug");
                    println(" stop, restart");
                } else if (strcmp(arg, "exit") == 0) { println("exit: exit from Kodo. | Usage: \"exit\"");
                } else if (strcmp(arg, "clear") == 0) { println("clear: clear screen Kodo. | Usage: \"clear\"");
                } else if (strcmp(arg, "kill") == 0) { println("kill: kill - restart terminal Kodo. | Usage: \"kill\"");
                } else if (strcmp(arg, "title") == 0) { println("title: set-title Terminal Kodo. | Usage: \"title\" | [<args>]");
                } else if (strcmp(arg, "compile") == 0) { println("compile: compile your project. | Usage: \"compile\" | [<args>]");
                } else if (strcmp(arg, "running") == 0) { println("running: running your project. | Usage: \"running\" | [<args>]");
                } else if (strcmp(arg, "debug") == 0) { println("debug: debugging your project. | Usage: \"debug\" | [<args>]");
                } else if (strcmp(arg, "stop") == 0) { println("stop: stopped server task. | Usage: \"stop\"");
                } else if (strcmp(arg, "restart") == 0) { println("restart: restart server task. | Usage: \"restart\"");
                } else println("help not found for: '%s'", arg);
                
                continue;
            } else if (strcmp(pattern_COMMANDS, "pawncc") == 0) {
                kodo_title("Kodo Toolchain | @ pawncc");
                static
                    char platform = 0;
                ret_pcc:
                    println("Select platform:");
                    println("[L/l] Linux");
                    println("[W/w] Windows");
                    printf(">> ");

                if (scanf(" %c", &platform) != 1)
                    return;

                signal(SIGINT, kodo_main);

                if (platform == 'L' || platform == 'l')
                    kodo_download_pawncc("linux");
                else if (platform == 'W' || platform == 'w')
                    kodo_download_pawncc("windows");
                else {
                    printf("Invalid platform selection. use C^ to exit.\n");
                    goto ret_pcc;
                }
            } else if (strcmp(pattern_COMMANDS, "gamemode") == 0) {
                kodo_title("Kodo Toolchain | @ gamemode");
                static
                    char platform = 0;
                ret_gm:
                    println("Select platform:");
                    println("[L/l] Linux");
                    println("[W/w] Windows");
                    printf(">> ");

                if (scanf(" %c", &platform) != 1)
                    return;

                signal(SIGINT, kodo_main);

                if (platform == 'L' || platform == 'l')
                    kodo_download_samp("linux");
                else if (platform == 'W' || platform == 'w')
                    kodo_download_samp("windows");
                else {
                    printf("Invalid platform selection. use C^ to exit.\n");
                    goto ret_gm;
                }
            } else if (strcmp(pattern_COMMANDS, "clear") == 0) {
                kodo_title("Kodo Toolchain | @ clear");
                clear:
                    kodo_sys("clear");
                continue;
            } else if (strcmp(pattern_COMMANDS, "exit") == 0) {
                exit(1);
            } else if (strcmp(pattern_COMMANDS, "kill") == 0) {
                kodo_title("Kodo Toolchain | @ kill");
                kodo_sys("clear");
                kodo_main(0);
            } else if (strncmp(pattern_COMMANDS, "title", 5) == 0) {
                char *arg = pattern_COMMANDS + 6;
                while (*arg == ' ') arg++;
                if (*arg == '\0') {
                    println("usage: title [<title>]");
                } else {
                    printf("\033]0;%s\007", arg);
                }
                continue;
            } else if (strncmp(pattern_COMMANDS, "compile", 7) == 0) {
                kodo_title("Kodo Toolchain | @ compile");
                static char *arg;
                arg = pattern_COMMANDS + 7;
                while (*arg == ' ') arg++;
            
                static char *compile_arg1;
                compile_arg1 = strtok(arg, " ");
            
                const char *ptr_pawncc;
                int __kodo_os__ = signal_system_os();
                if (__kodo_os__ == 0x01) 
                    ptr_pawncc = "pawncc.exe";
                else if (__kodo_os__ == 0x00)
                    ptr_pawncc = "pawncc";

                FILE *procc_f = NULL;
                char error_compiler_check[100];
                char kodo_c_output_f_container[128];
                int format_size_c_f_container = sizeof(kodo_c_output_f_container);
            
                int find_pawncc = kodo_sef_fdir(".", ptr_pawncc);
                if (find_pawncc == 1) {
                    static char *_compiler_ = NULL;
                    static size_t format_size_compiler = 2048;

                    if (_compiler_ == NULL) {
                        _compiler_ = malloc(format_size_compiler);
                        if (!_compiler_) {
                            printf_error("Memory allocation failed for _compiler_!\n");
                            return;
                        }
                    }
            
                    FILE *procc_f = fopen("kodo.toml", "r");
                    if (!procc_f)
                        printf_error("Can't read file %s\n", "kodo.toml");
            
                    char errbuf[256];
                    toml_table_t *config = toml_parse_file(procc_f, errbuf, sizeof(errbuf));
                    fclose(procc_f);
            
                    if (!config)
                        printf_error("error parsing TOML: %s\n", errbuf);
            
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
                            char all_paths[250] = {0};
            
                            for (int i = 0; i < array_size; i++) {
                                toml_datum_t path_val = toml_string_at(include_paths, i);
                                if (path_val.ok) {
                                    if (i > 0)
                                        strcat(all_paths, " ");
                                    snprintf(all_paths + strlen(all_paths), sizeof(all_paths) - strlen(all_paths), "-i\"%s\"", path_val.u.s);
                                }
                            }
            
                            static char kd_gamemode[56];
                            if (*arg == '\0' || arg == ".") {
                                toml_datum_t kodo_gmodes = toml_string_in(kodo_compiler, "input");
                                if (kodo_gmodes.ok) {
                                    kd_gamemode_input = kodo_gmodes.u.s;
                                }
                                
                                int find_gamemodes = kodo_sef_fdir("gamemodes/", kd_gamemode_input);
                                if (find_gamemodes == 1) {
                                    char* container_output = strdup(kodo_sef_found[1]);
                                    char* f_last_slash_container = strrchr(container_output, '/');
                                    if (f_last_slash_container != NULL && *(f_last_slash_container + 1) != '\0')
                                        *(f_last_slash_container + 1) = '\0';

                                    snprintf(kodo_c_output_f_container, format_size_c_f_container, "%s%s",
                                        container_output, kd_gamemode_output);

                                    kd_gamemode_input=strdup(kodo_sef_found[1]);

                                    struct timespec start, end;
                                    double compiler_dur;

                                    snprintf(_compiler_, format_size_compiler, "%s %s \"%s\" -o\"%s\" \"%s\" > .kd_compiler.log 2>&1",
                                        kodo_sef_found[0],
                                        all_paths,
                                        kd_gamemode_input,
                                        kodo_c_output_f_container,
                                        kd_compiler_opt);

                                    clock_gettime(CLOCK_MONOTONIC, &start);
                                
                                    kodo_sys(_compiler_);
                                    
                                    clock_gettime(CLOCK_MONOTONIC, &end);

                                    procc_f = fopen(".kd_compiler.log", "r");
                                    if (procc_f) {
                                        int ch;
                                        while ((ch = fgetc(procc_f)) != EOF) {
                                            putchar(ch);
                                        }
                                    }
                                    while (fscanf(procc_f, "%s", error_compiler_check) != EOF) {
                                        if (strcmp(error_compiler_check, "error") == 0) {
                                            FILE *c_output;
                                            c_output = fopen(kodo_c_output_f_container, "r");
                                            if (c_output)
                                                remove(kodo_c_output_f_container);
                                            break;
                                        }
                                    }
                                
                                    fclose(procc_f);

                                    compiler_dur = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
                                    printf("[Finished in %.3fs]\n", compiler_dur);
                                    if (_compiler_)
                                        free(_compiler_);
                                } else {
                                    printf_color(COL_RED, "Can't locate: ");
                                    printf("%s\n", kd_gamemode_input);
                                    continue;
                                }
                            } else {
                                int find_gamemodes_arg1 = kodo_sef_fdir("gamemodes/", compile_arg1);
                                if (find_gamemodes_arg1 == 1) {
                                    char* container_output = strdup(kodo_sef_found[1]);
                                    char* f_last_slash_container = strrchr(container_output, '/');
                                    if (f_last_slash_container != NULL && *(f_last_slash_container + 1) != '\0')
                                        *(f_last_slash_container + 1) = '\0';

                                    snprintf(kodo_c_output_f_container, format_size_c_f_container, "%s%s",
                                        container_output, kd_gamemode_output);

                                    compile_arg1=strdup(kodo_sef_found[1]);

                                    struct timespec start, end;
                                    double compiler_dur;

                                    snprintf(_compiler_, format_size_compiler, "%s %s \"%s\" -o\"%s\" \"%s\" > .kd_compiler.log 2>&1",
                                        kodo_sef_found[0],
                                        all_paths,
                                        compile_arg1,
                                        kodo_c_output_f_container,
                                        kd_compiler_opt);
                                    
                                    clock_gettime(CLOCK_MONOTONIC, &start);
                                    
                                    kodo_sys(_compiler_);
                                    
                                    clock_gettime(CLOCK_MONOTONIC, &end);

                                    procc_f = fopen(".kd_compiler.log", "r");
                                    if (procc_f) {
                                        int ch;
                                        while ((ch = fgetc(procc_f)) != EOF) {
                                            putchar(ch);
                                        }
                                    }
                                    while (fscanf(procc_f, "%s", error_compiler_check) != EOF) {
                                        if (strcmp(error_compiler_check, "error") == 0) {
                                            FILE *c_output;
                                            c_output = fopen(kodo_c_output_f_container, "r");
                                            if (c_output)
                                                remove(kodo_c_output_f_container);
                                            break;
                                        }
                                    }
                                
                                    fclose(procc_f);

                                    compiler_dur = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
                                    printf("[Finished in %.3fs]\n", compiler_dur);
                                    
                                    if (_compiler_)
                                        free(_compiler_);
                                } else {
                                    printf_color(COL_RED, "Can't locate: ");
                                    printf("%s\n", compile_arg1);
                                    continue;
                                }
                            }
                        }
                    }
                } else {
                    printf_error("pawncc not found!");
            
                    char *ptr_sigA;
                    ptr_sigA = readline("install now? [Y/n]: ");
            
                    while (1) {
                        if (strcmp(ptr_sigA, "Y") == 0 || strcmp(ptr_sigA, "y") == 0) {
                            if (__kodo_os__ == 0x01) {
                                kodo_download_pawncc("windows");
                            } else if (__kodo_os__ == 0x00) {
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

                    if (ptr_sigA)
                        free(ptr_sigA);
                }
            } else if (strncmp(pattern_COMMANDS, "running", 7) == 0 || strncmp(pattern_COMMANDS, "debug", 7) == 0) {
                _running_:
                if (strcmp(pattern_COMMANDS, "debug") == 0) {
                    server_or_debug="debug";
                    kodo_title("Kodo Toolchain | @ debug");    
                } else {
                    kodo_title("Kodo Toolchain | @ running");
                }

                static char *arg;
                    arg = pattern_COMMANDS + 7;
                while (*arg == ' ') arg++;
                char *arg1 = strtok(arg, " ");

                static char *format_prompt = NULL;
                static size_t format_size = 126;
                if (format_prompt == NULL) 
                    format_prompt = malloc(format_size);

                static const char *ptr_samp = NULL;
                static int find_for_samp
                    = 0x0;
                static const char *ptr_openmp = NULL;
                static int find_for_omp
                    = 0x0;

                static int __kodo_os__;
                    __kodo_os__ = signal_system_os();
                
                if (__kodo_os__ == 0x01) {
                    ptr_samp="samp-server.exe"; ptr_openmp="omp-server.exe";
                }
                else if (__kodo_os__ == 0x00) {
                    ptr_samp="samp03svr"; ptr_openmp="omp-server";
                }
                
                FILE *file_s = fopen(ptr_samp, "r");
                if (file_s)
                    find_for_samp=0x1;
                FILE *file_m = fopen(ptr_openmp, "r");
                if (file_m)
                    find_for_omp=0x1;

                if (find_for_samp == 0x1) {
                    if (*arg == '\0' || arg == ".") {
                        FILE *server_log = fopen("server_log.txt", "r");
                        if (server_log)
                            remove("server_log.txt");

                        printf_color(COL_YELLOW, "running..\n");

                        char snprintf_ptrS[128];
                        chmod(ptr_samp, 0777);
                        snprintf(snprintf_ptrS, sizeof(snprintf_ptrS), "./%s", ptr_samp);
                        int running_FAIL = kodo_sys(snprintf_ptrS);
                        if (running_FAIL == 0) {
                            sleep(2);

                            printf_color(COL_YELLOW, "Press enter to print logs..");
                            getchar();

                            FILE *server_log = fopen("server_log.txt", "r");
                            if (!server_log)
                                printf_error("Can't found server_log.txt!");
                            else {
                                int ch;
                                while ((ch = fgetc(server_log)) != EOF) {
                                    putchar(ch);
                                }
                                fclose(server_log);
                            }
                        } else {
                            printf_color(COL_RED, "running failed! ");
                            printf("need help?, go here: https://github.com/vilksons/kodo/issues\n");
                        }
                        continue;
                    } else {
                        printf_color(COL_YELLOW, "running..\n");
                        kodo_server_samp(arg1, ptr_samp);
                    }
                } else if (find_for_omp == 0x1) {
                    if (*arg == '\0' || arg == ".") {
                        FILE *server_log = fopen("log.txt", "r");

                        if (server_log) 
                            remove("log.txt");

                        printf_color(COL_YELLOW, "running..\n");

                        char snprintf_ptrS[128];
                        chmod(ptr_openmp, 0777);
                        snprintf(snprintf_ptrS, sizeof(snprintf_ptrS), "./%s", ptr_openmp);
                        int running_FAIL = kodo_sys(snprintf_ptrS);
                        if (running_FAIL == 0) {
                            sleep(2);

                            printf_color(COL_YELLOW, "Press enter to print logs..");
                            getchar();

                            FILE *server_log = fopen("log.txt", "r");
                            if (!server_log)
                                printf_error("Can't found log.txt!");
                            else {
                                int ch;
                                while ((ch = fgetc(server_log)) != EOF) {
                                    putchar(ch);
                                }
                                fclose(server_log);
                            }
                        } else {
                            printf_color(COL_RED, "running failed! ");
                            printf("need help?, go here: https://github.com/vilksons/kodo/issues\n");
                        }
                        continue;
                    } else {
                        printf_color(COL_YELLOW, "running..\n");
                        kodo_server_openmp(arg1, ptr_openmp);
                    }
                } else if (!find_for_omp || !find_for_samp) {
                    printf_error("samp-server/open.mp server not found!");

                    char *ptr_sigA;
                    ptr_sigA = readline("install now? [Y/n]: ");

                    while (1) {
                        if (strcmp(ptr_sigA, "Y") == 0 || strcmp(ptr_sigA, "y") == 0) {
                            if (__kodo_os__ == 0x01) {
                                kodo_download_samp("windows");
                            } else if (__kodo_os__ == 0x00) {
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
                if (format_prompt)
                    free(format_prompt);
            } else if (strcmp(pattern_COMMANDS, "stop") == 0) {
                kodo_title("Kodo Toolchain | @ stop");
                kodo_server_stop_tasks();
                continue;
            } else if (strcmp(pattern_COMMANDS, "restart") == 0) {
                kodo_server_stop_tasks();
                sleep(2);
                goto _running_
                    ;
            } else if (strcmp(pattern_COMMANDS, "kodo") == 0) {
                printf("Hello There!\n");
            } else if (strcmp(pattern_COMMANDS, c_command) != 0 && c_distance <= 1) {
                kodo_title("Kodo Toolchain | @ undefined");
                println("Did you mean: \"%s\"?", c_command);
            } else {
                if (strlen(pattern_COMMANDS) > 0) {
                    kodo_title("Kodo Toolchain | @ not found");
                    println("%s not found!", pattern_COMMANDS);
                }
            }

            if (pattern_COMMANDS) free(pattern_COMMANDS);
        }
}

int main(void) { kodo_main(0); return 0; }
