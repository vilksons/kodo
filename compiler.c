/*
 * Project Name: Kodo Toolchain
 * Project File: Kodo/compiler.c
 * Copyright (C) Kodo/Contributors
 *
 * This program is distributed under the terms of the GNU General Public License v2.0.
 * See the LICENSE file for details.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int call_find_for_pawncc(const char *__d_path, const char *__t_file) {
    DIR *__dir;
    struct dirent *__entry;
    struct stat __statbuf;
    int find_found_rate = 0;

    if ((__dir = opendir(__d_path)) == NULL) {
        perror("opendir");
        _kodo_();
    }

    while ((__entry = readdir(__dir)) != NULL && pf_count < MAX_FIND_PATH) {
        if (strcmp(__entry->d_name, ".") == 0 || strcmp(__entry->d_name, "..") == 0)
            continue;

        char __full_path[MAX_FIND_PATH_LENGTH];
        snprintf(__full_path, sizeof(__full_path), "%s/%s", __d_path, __entry->d_name);

        if (stat(__full_path, &__statbuf) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(__statbuf.st_mode)) {
            find_found_rate += call_find_for_pawncc(__full_path, __t_file);
        } 
        // if the __entry is the target file, store its path and increment the count
        else if (strcmp(__entry->d_name, __t_file) == 0) {
            strncpy(pf_found[pf_count], __full_path, MAX_FIND_PATH_LENGTH);
            pf_count++;
            find_found_rate++;
        }
    }

    // close the directory
    closedir(__dir);
    return find_found_rate; 
}

#include "utils.h"
#include "compiler.h"
