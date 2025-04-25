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

int
execute_sfpawncc(const char *pawncc_path,
				const char *pawncc_name)
{
	char path_buf[PCC_MAX_PATH_SIZE];
	struct dirent *entry;
	struct stat statbuf;
	int pcc_count = 0;
	int found = 0;
	DIR *dir;
	int len;

	dir = opendir(pawncc_path);
	if (!dir) perror("opendir failed"); return 0;

	while ((entry = readdir(dir)) && pcc_count < PCC_MAX_PATH_COUNT) {
		if (!strcmp(entry->d_name, "."))
			continue;
		len = snprintf(path_buf, sizeof(path_buf), "%s/%s",
			pawncc_path, entry->d_name);
		if (len < 0 || len >= sizeof(path_buf)) {
			fprintf(stderr, "Path too long: %s/%s\n",
				pawncc_path, entry->d_name);
			continue;
		} if (stat(path_buf, &statbuf)) {
			perror("stat failed");
			continue;
		} if (S_ISDIR(statbuf.st_mode)) {
			found += execute_sfpawncc(path_buf, pawncc_name);
			continue;
		} if (!strcmp(entry->d_name, pawncc_name)) {
			strncpy(pcc_found[pcc_count], path_buf,
				PCC_MAX_PATH_SIZE - 1);
			pcc_found[pcc_count][PCC_MAX_PATH_SIZE - 1] = '\0';
			pcc_count++;
			found++;
		}
	}

	closedir(dir);
	return found;
}
