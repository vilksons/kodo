#ifndef COMPILER_H
#define COMPILER_H

#define PCC_MAX_PATH_COUNT 28
#define PCC_MAX_PATH_SIZE 128

extern char pcc_found[PCC_MAX_PATH_COUNT][PCC_MAX_PATH_SIZE];
int execute_sfpawncc(const char *__d_path, const char *__t_file);

#endif
