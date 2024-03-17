FROM mcr.microsoft.com/devcontainers/cpp:debian-12

WORKDIR /workspace/unix-network-learning
RUN sudo apt install ninja-build -y
# RUN ["sudo", "wget", "-P", "/app", "https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-linux.zip"]
# RUN ["sudo", "wget", "-P", "/app", "https://github.com/clangd/clangd/releases/download/17.0.3/clangd-linux-17.0.3.zip"]
RUN sudo wget -P /app https://github.com/clangd/clangd/releases/download/17.0.3/clangd-linux-17.0.3.zip 
RUN sudo wget -P /app https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-linux.zip
#     && sudo unzip /app/ninja-linux.zip\
#      &&  sudo cp /app/ninja /usr/bin

ENV PATH="$PATH:/app/clangd_17.0.3/bin"