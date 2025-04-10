/*
 * Project Name: Kodo Toolchain
 * Project File: cKodo/server.h
 * Copyright (C) cKodo/Contributors
 *
 * This program is distributed under the terms of the GNU General Public License v2.0.
 * See the LICENSE file for details.
 *
 */

#ifndef SERVER_H
#define SERVER_H

void call_server_stop_tasks(void);
void call_server_samp(const char *_args_, const char *ptr_server);
void call_server_openmp(const char *_args_);

#endif