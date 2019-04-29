From rastasheep/ubuntu-sshd:18.04
RUN apt update && apt-get -y dist-upgrade
RUN apt install -y qemu timelimit
RUN useradd -m pwn && echo "pwn:pwn" | chpasswd
