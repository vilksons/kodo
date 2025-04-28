#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stddef.h>

#include "kodo.h"
#include "utils.h"
#include "package.h"

typedef struct {
    char key;
    const char *name;
    const char *linux_url;
    const char *linux_file;
    const char *windows_url;
    const char *windows_file;
} VersionInfo;

void
kodo_download_pawncc(const char *platform) {
        char selection, version_selection;
        char url[300], fname[256];

        const char *versions[] = {
            "3.10.10", "3.10.9"
        };

        printf(":: Do you want to continue downloading PawnCC? (Yy/Nn)\n>> ");
        if (scanf(" %c", &selection) != 1) {
            return;
        }
        if (selection != 'Y' && selection != 'y') {
            kodo_main(0);
            return;
        }

        printf("Select the PawnCC version to download:\n");
        for (int i = 0; i < 3 && i < sizeof(versions)/sizeof(versions[0]); i++) {
            printf("[%c/%c] PawnCC %s\n", 'A'+i, 'a'+i, versions[i]);
        }
        
        printf(">> ");
        if (scanf(" %c", &version_selection) != 1) {
            return;
        }
        int index = (version_selection >= 'A' && version_selection <= 'Z') ? version_selection - 'A' :
             (version_selection >= 'a' && version_selection <= 'z') ? version_selection - 'a' :
             -1;

        if (index < 0 || index >= 3) {
            printf("Invalid selection.\n");
            return;
        } else {
            const char *archive_write = strcmp(platform, "linux") == 0 ? "tar.gz" : "zip";

            sprintf(url, "https://github.com/pawn-lang/compiler/releases/download/v%s/pawnc-%s-%s.%s",
                    versions[index], versions[index], platform, archive_write);
            sprintf(fname, "pawnc-%s-%s.%s", versions[index], platform, archive_write);
    
            initialize_ipawncc=1;
    
            kodo_download_file(url, fname);
        }
}

void
kodo_download_samp(const char *platform) {
        char sel_c;
        printf(":: Do you want to continue downloading SA-MP? (Yy/Nn): ");
        if (scanf(" %c", &sel_c) != 1) {
            return;
        }
        if (sel_c != 'Y' && sel_c != 'y') {
            kodo_main(0);
            return;
        }
        
        VersionInfo versions[] = {
            { 'A', "SA-MP 0.3.DL R1", 
                "https://github.com/KrustyKoyle/files.sa-mp.com-Archive/raw/refs/heads/master/samp03DLsvr_R1.tar.gz",
                "samp03DLsvr_R1.tar.gz",
                "https://github.com/KrustyKoyle/files.sa-mp.com-Archive/raw/refs/heads/master/samp03DL_svr_R1_win32.zip",
                "samp03DL_svr_R1_win32.zip"
            },
            { 'B', "SA-MP 0.3.7 R3", 
                "https://github.com/KrustyKoyle/files.sa-mp.com-Archive/raw/refs/heads/master/samp037svr_R3.tar.gz",
                "samp037svr_R3.tar.gz",
                "https://github.com/KrustyKoyle/files.sa-mp.com-Archive/raw/refs/heads/master/samp037_svr_R3_win32.zip",
                "samp037_svr_R3_win32.zip"
            },
            { 'C', "SA-MP 0.3.7 R2-2-1", 
                "https://github.com/KrustyKoyle/files.sa-mp.com-Archive/raw/refs/heads/master/samp037svr_R2-2-1.tar.gz",
                "samp037svr_R2-2-1.tar.gz",
                "https://github.com/KrustyKoyle/files.sa-mp.com-Archive/raw/refs/heads/master/samp037_svr_R2-1-1_win32.zip",
                "samp037_svr_R2-2-1_win32.zip"
            },
            { 'D', "SA-MP 0.3.7 R2-1-1", 
                "https://github.com/KrustyKoyle/files.sa-mp.com-Archive/raw/refs/heads/master/samp037svr_R2-1.tar.gz",
                "samp037svr_R2-1.tar.gz",
                "https://github.com/KrustyKoyle/files.sa-mp.com-Archive/raw/refs/heads/master/samp037_svr_R2-1-1_win32.zip",
                "samp037_svr_R2-1-1_win32.zip"
            },
            { 'E', "OPEN.MP v1.4.0.2779",
                "https://github.com/openmultiplayer/open.mp/releases/download/v1.4.0.2779/open.mp-linux-x86.tar.gz",
                "open.mp-linux-x86.tar.gz",
                "https://github.com/openmultiplayer/open.mp/releases/download/v1.4.0.2779/open.mp-win-x86.zip",
                "open.mp-win-x86.zip"
            },
            { 'F', "OPEN.MP v1.3.1.2748",
                "https://github.com/openmultiplayer/open.mp/releases/download/v1.3.1.2748/open.mp-linux-x86.tar.gz",
                "open.mp-linux-x86.tar.gz",
                "https://github.com/openmultiplayer/open.mp/releases/download/v1.3.1.2748/open.mp-win-x86.zip",
                "open.mp-win-x86.zip"
            },
            { 'G', "OPEN.MP v1.2.0.2670",
                "https://github.com/openmultiplayer/open.mp/releases/download/v1.2.0.2670/open.mp-linux-x86.tar.gz",
                "open.mp-linux-x86.tar.gz",
                "https://github.com/openmultiplayer/open.mp/releases/download/v1.2.0.2670/open.mp-win-x86.zip",
                "open.mp-win-x86.zip"
            },
            { 'H', "OPEN.MP v1.1.0.2612",
                "https://github.com/openmultiplayer/open.mp/releases/download/v1.1.0.2612/open.mp-linux-x86.tar.gz",
                "open.mp-linux-x86.tar.gz",
                "https://github.com/openmultiplayer/open.mp/releases/download/v1.1.0.2612/open.mp-win-x86.zip",
                "open.mp-win-x86.zip"
            }
        };

        printf("Select the SA-MP version to download:\n");
        for (int i = 0; i < sizeof(versions)/sizeof(versions[0]); i++) {
            printf("[%c/%c] %s\n", versions[i].key, versions[i].key + 32, versions[i].name);
        }

        printf(">> ");
        char version_choice;
        if (scanf(" %c", &version_choice) != 1) {
            return;
        }

        VersionInfo *chosen = NULL;
        for (int i = 0; i < sizeof(versions)/sizeof(versions[0]); i++) {
            if (version_choice == versions[i].key || version_choice == versions[i].key + 32) {
                chosen = &versions[i];
                break;
            }
        }

        if (!chosen) {
            printf("Invalid selection\n");
            kodo_main(0);
            return;
        } else {
            const char *url = strcmp(platform, "linux") == 0 ? chosen->linux_url : chosen->windows_url;
            const char *fname = strcmp(platform, "linux") == 0 ? chosen->linux_file : chosen->windows_file;

            kodo_download_file(url, fname);
        }
}