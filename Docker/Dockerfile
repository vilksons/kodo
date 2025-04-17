FROM ubuntu:latest

WORKDIR /mnt

RUN apt update && \
    apt install -y curl sudo && \
    curl -L -o workspace https://raw.githubusercontent.com/vilksons/kodo/refs/heads/kodo/Scripts/workspace && \
    chmod +x workspace

CMD ["./workspace"]
