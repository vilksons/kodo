## How to Use

### Docker Usage  
Run the following commands to build and run the Docker container:

```bash
# Build the Docker image
docker build -t docker_usage .

# Run the Docker container
docker run --rm -it docker_usage
```

### Linux Setup (Optional)  
If you're on Linux and want to run Docker without `sudo`, set up Docker permissions with:

```bash
# Add your user to the docker group
sudo usermod -aG docker $USER

# Refresh your group membership
newgrp docker

# Enable Docker to start on boot
sudo systemctl enable docker

# Start the Docker service now
sudo systemctl start docker
```