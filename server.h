#ifndef SERVER_H
#define SERVER_H

void kodo_server_stop_tasks(void);
void
kodo_server_samp(const char *gamemode_arg,
                 const char *server_bin);
void
kodo_server_openmp(const char *gamemode_arg);

#endif