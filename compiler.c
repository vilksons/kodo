#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "kodo.h"
#include "utils.h"
#include "compiler.h"

char pcc_found[PCC_MAX_PATH_COUNT][PCC_MAX_PATH_SIZE];
int pcc_count = 0;

int execute_sfpawncc(const char *pawncc_path,
					 const char *pawncc_name)
{
	char path_buff[PCC_MAX_PATH_SIZE];
    struct dirent *entry;
    struct stat statbuf;
    int found = 0;
    DIR *dir;
    if ((dir = opendir(pawncc_path)) == NULL) {
        perror("opendir");
        _kodo_(0);
    }
    while ((entry = readdir(dir)) != NULL && pcc_count < PCC_MAX_PATH_COUNT) {
        if (!strcmp(entry->d_name, ".") ||
			!strcmp(entry->d_name, ".."))
            continue;
        snprintf(path_buff, sizeof(path_buff), "%s/%s",
			pawncc_path, entry->d_name);
        if (stat(path_buff, &statbuf) == -1) {
            perror("stat");
            continue;
        }
		if (S_ISDIR(statbuf.st_mode)) {
            found += execute_sfpawncc(path_buff, pawncc_name);
        } else if (strcmp(entry->d_name, pawncc_name) == 0) {
            strncpy(pcc_found[pcc_count], path_buff, PCC_MAX_PATH_SIZE);
            pcc_count++;
            found++;
        }
    }

    closedir(dir);
    return found; 
}