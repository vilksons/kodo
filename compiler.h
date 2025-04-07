/*
 * Project Name: Kodo Toolchain
 * Project File: Kodo/compiler.h
 * Copyright (C) Kodo/Contributors
 *
 * This program is distributed under the terms of the GNU General Public License v2.0.
 * See the LICENSE file for details.
 *
 */

#ifndef COMPILER_H
#define COMPILER_H

#define MAX_FIND_PATH 100
#define MAX_FIND_PATH_LENGTH 1024

extern char pf_found[MAX_FIND_PATH][MAX_FIND_PATH_LENGTH];
extern int pf_count;
int call_find_for_pawncc(const char *__d_path, const char *__t_file);

#endif