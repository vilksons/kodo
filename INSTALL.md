# INSTALLATION

## Supported Platforms:
- Linux, Darwin, Windows, macOS, and Android/iPhone via:
  - Docker (Windows/macOS): [https://www.docker.com/](https://www.docker.com/)
  - WSL2 (Windows): [https://learn.microsoft.com/en-us/windows/wsl/install](https://learn.microsoft.com/en-us/windows/wsl/install)
  - MSYS2 (Windows): [https://www.msys2.org/](https://www.msys2.org/)
  - mingw-w64 (Windows/macOS): [https://www.mingw-w64.org/](https://www.mingw-w64.org/)
  - Cygwin (Windows): [https://www.cygwin.com/](https://www.cygwin.com/)
  - Babun (Windows): [https://github.com/babun/babun](https://github.com/babun/babun)
  - Termux (Android/Emulator): [https://termux.dev/en/](https://termux.dev/en/)
  - UserLAnd (Android/Emulator): [https://github.com/CypherpunkArmory/UserLAnd](https://github.com/CypherpunkArmory/UserLAnd)
  - Linux Deploy (Android/Emulator): [https://github.com/meefik/linuxdeploy](https://github.com/meefik/linuxdeploy)
  - iSH (iPhone): [https://github.com/ish-app/ish](https://github.com/ish-app/ish)
  - a-Shell (iPhone): [https://github.com/holzschu/a-shell](https://github.com/holzschu/a-shell)

## INSTALL `kodo/workspace` SCRIPT

- Download via GNU/Wget:
  ```bash
  wget -q -O workspace https://raw.githubusercontent.com/vilksons/kodo/refs/heads/kodo/Scripts/workspace
  ```

- Download via cURL:
  ```bash
  curl -L -o workspace https://raw.githubusercontent.com/vilksons/kodo/refs/heads/kodo/Scripts/workspace
  ```

## INSTALL REQUIRED PACKAGES

Full list available at:  
[https://github.com/vilksons/kodo/wiki/Required-Packages](https://github.com/vilksons/kodo/wiki/Required-Packages)

## INSTALLATION STEPS

1. Open the Terminal.
2. Make the `workspace` script executable:
   ```bash
   chmod +x workspace
   ```
3. Run the script:
   ```bash
   bash ./workspace
   ```

## Docker Usage

### (MacOS)
```bash
docker run -it ubuntu
docker run -v ~/Downloads:/mnt/downloads -it ubuntu  # allowed downloads directory
```
- `apt update`  # refresh repository
- `apt upgrade`  # (optional) upgrade packages if needed
- `dpkg --add-architecture i386`
- `apt update`  # refresh repository
- `apt install libc6:i386 libstdc++6:i386`  # x86 support for x64 (needed)
- `apt install python3 wget curl grep awk sed coreutils yq jq aria2 unzip tar`  # required packages for kodo/workspace

```bash
cd /path/to/kodo/  # cd to kodo/workspace
chmod +x workspace  # recommended +x : 755
./workspace  # run the kodo/workspace
```

### (Linux/WSL2)
```bash
sudo usermod -aG docker $USER  # add docker user to your Linux
newgrp docker  # add new group for docker
sudo systemctl start docker  # start docker
sudo systemctl enable docker  # enable docker
```

```bash
docker run -it ubuntu
docker run -v ~/Downloads:/mnt/downloads -it ubuntu  # allowed downloads directory
```
- `apt update`  # refresh repository
- `apt upgrade`  # (optional) upgrade packages if needed
- `dpkg --add-architecture i386`
- `apt update`  # refresh repository
- `apt install libc6:i386 libstdc++6:i386`  # x86 support for x64 (needed)
- `apt install python3 wget curl grep awk sed coreutils yq jq aria2 unzip tar`  # required packages for kodo/workspace

```bash
cd /path/to/kodo/  # cd to kodo/workspace
chmod +x workspace  # recommended +x : 755
./workspace  # run the kodo/workspace
```

## Additional Docker Commands

- Check all running/previously run docker containers:
  ```bash
  docker ps -a
  ```

- Start a docker image:
  ```bash
  docker start <container-names>
  ```

- Open a session in a running container:
  ```bash
  docker exec -it <container-names> /bin/bash  # /bin/bash or /bin/sh
  ```

- Stop a docker container:
  ```bash
  docker stop <container-names>
  ```

- Remove a docker container:
  ```bash
  docker rm -f <container-names>  # -f for forcing
  ```

- Remove all docker containers:
  ```bash
  docker rm -f $(docker ps -a -q)  # -f for forcing
  ```