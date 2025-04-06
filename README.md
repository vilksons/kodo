# KODO TOOLCHAIN

## WIKI GUIDE
Essential references to understand and use Kodo:
- [Compiler Options](https://github.com/vilksons/kodo/wiki/Compiler-Option)
- [Required Packages](https://github.com/vilksons/kodo/wiki/Required-Packages)
- [PawnCC Installation](https://github.com/vilksons/kodo/wiki/PawnCC-Installation)
- [CODE OF CONDUCT](https://github.com/vilksons/kodo/wiki/CODE-OF-CONDUCT)

## WHAT IS KODO?
- [Kodo](https://github.com/vilksons/kodo/tree/kodo)  
  Kodo is an SA:MP-specific toolchain designed for debugging, compiling, and package management.

## WHAT IS CKODO?
- [cKodo](https://github.com/vilksons/kodo/tree/ckodo)  
  cKodo is a Kodo Toolchain built on the C language without any connection to Shell/Bash.

## KEEPING UP-TO-DATE
Always update using:
```
sync
```

## PREREQUISITES

### INSTALLATION
**Supported Platforms:**
- Linux, Darwin, Windows, macOS, and Android/iPhone via:
  - [Docker](https://www.docker.com/)
  - [WSL/WSL2](https://learn.microsoft.com/en-us/windows/wsl/install)
  - [MSYS2](https://www.msys2.org/)
  - [mingw-w64](https://www.mingw-w64.org/)
  - [Cygwin](https://www.cygwin.com/)
  - [Babun](https://github.com/babun/babun)
  - [Termux](https://termux.dev/en/)
  - [UserLAnd](https://github.com/CypherpunkArmory/UserLAnd)
  - [Linux Deploy](https://github.com/meefik/linuxdeploy)
  - [iSH](https://github.com/ish-app/ish)
  - [a-Shell](https://github.com/holzschu/a-shell)

### INSTALL `kodo/workspace` SCRIPT
- **Via GNU/Wget:**
```bash
wget -q --show-progress -O workspace https://raw.githubusercontent.com/vilksons/kodo/refs/heads/kodo/Scripts/workspace
```
- **Via cURL:**
```bash
curl -L -o workspace https://raw.githubusercontent.com/vilksons/kodo/refs/heads/kodo/Scripts/workspace
```

### INSTALL REQUIRED PACKAGES
Full list available at:  
[Required Packages](https://github.com/vilksons/kodo/wiki/Required-Packages)

### INSTALLATION STEPS
```bash
1. Open the Terminal.
2. chmod +x workspace
3. bash ./workspace
```

---

## Docker Usage

### MacOS
```bash
docker run -it ubuntu
docker run -v ~/Downloads:/mnt/downloads -it ubuntu

apt update
apt upgrade # optional
dpkg --add-architecture i386
apt update
apt install libc6:i386 libstdc++6:i386
apt install python3 wget curl grep awk sed coreutils yq jq aria2 unzip tar 

cd /path/to/kodo/
chmod +x workspace
./workspace
```

### Linux/WSL2
```bash
sudo usermod -aG docker $USER
newgrp docker
sudo systemctl start docker
sudo systemctl enable docker

docker run -it ubuntu
docker run -v ~/Downloads:/mnt/downloads -it ubuntu

apt update
apt upgrade
dpkg --add-architecture i386
apt update
apt install libc6:i386 libstdc++6:i386
apt install python3 wget curl grep awk sed coreutils yq jq aria2 unzip tar

cd /path/to/kodo/
chmod +x workspace
./workspace
```

### Additional Docker Commands
```bash
docker ps -a
docker start <container-name>
docker exec -it <container-name> /bin/bash
docker stop <container-name>
docker rm -f <container-name>
docker rm -f $(docker ps -a -q)
```

---

## USAGE

### FILE RENAMING
Rename `yourmode.pwn` to something like `yourmode.io.pwn` for clarity.

### COMPILATION
```bash
compile .                      # Default compilation
compile yourmode.pwn           # Specific file compilation
compile yourmode.pwn opt1 opt2 opt3  # With options
```

### STARTING & DEBUGGING
```bash
running yourmode.pwn
debug yourmode.pwn
```

---

## Configuration `kodo.json`

| Key               | Description                                                                 |
|--------------------|-----------------------------------------------------------------------------|
| `amx_flags`        | Compiler option flags. See [Compiler Options](https://github.com/vilksons/kodo/wiki/Compiler-Option) |
| `include_paths`    | Directory path for additional includes used in compilation.                |
| `exclude_paths`    | Paths to exclude from compilation.                                          |
| `samp_log`         | SA-MP server log file path.                                                 |
| `samp_config`      | SA-MP configuration file (for Zapman).                                      |
| `samp_init`        | SA-MP main server file (for Zapman).                                        |
| `samp_executable`  | Path to the SA-MP server executable.                                       |
| `include_dir`      | Directory for includes managed by the package manager (Zapman).             |
| `plugins_dir`      | Directory where `.dll` and `.so` plugin files are stored (Zapman).          |
| `bot_token`        | API key from [Groq Console](https://console.groq.com/keys).                |
| `bot_model`        | AI model from [Groq Console](https://console.groq.com/).                   |
| `bot_profile`      | Additional chatbot configuration info.                                     |
| `remcache_auto`    | Automatically remove `.cache`, table history, etc. (`true`/`false`).       |
| `repository`       | Repository source for sync. (`stable` or `testing`).                       |

### Example for `include_paths`:
If your script uses `includes/`, set:
```json
"include_paths": "includes"
```

### Example Directory Structure:
```
samp-server/
├── gamemodes/
│   └── mygamemode.pwn
├── includes/
│   └── customFile.pwn
└── server.cfg
```

---

## PACKAGE MANAGER - Zapman

### Modes
- Kodo Mode (default)
- Zapman Mode (with package management)

### Switch Modes
```bash
zapman    # Enter Zapman Mode
exit     # Exit Zapman Mode
```

### Manage Packages
```bash
install github/samp-incognito/samp-streamer-plugin
remove streamer
autoinstall
```

### Zapman CONFIGURATION

#### zapman.toml:
```toml
[package]
urls = [
  "github/user/repository",
  "gitlab/user/repository",
  "sourceforge/user/repository"
]
```

#### zapman.json:
```json
{
  "package": [
    "github/user/repository",
    "gitlab/user/repository",
    "sourceforge/user/repository"
  ]
}
```

---

## SECURITY

Zapman uses:
- Double SHA256 hashing (e.g., for bot tokens)
- Encrypted cache
- aria2 for fast, secure downloads
- Smart validation for `.so`, `.dll`, `.inc` files
- Cleanup for unknown or unnecessary install files

---

## FINAL NOTES
- Keep updated using:
```bash
sync
# or
syncc
```
- [Kodo Wiki](https://github.com/vilksons/kodo/wiki)
- [GitHub Discussions](https://github.com/vilksons/kodo/discussions)
- [Report Bugs](https://github.com/vilksons/kodo/issues)
```