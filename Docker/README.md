## How to Usage?
### run this in Docker/dockerfile this area. and don't forget for linux: `sudo usermod -aG docker $USER`
                                                                | `newgrp docker`
                                                                | `sudo systemctl enable docker`
                                                                | `sudo systemctl start docker` 
```bash
$ docker build -t docker_usage .
$ docker run --rm -it docker_usage
```