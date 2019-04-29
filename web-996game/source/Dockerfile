FROM node:boron

# Create app directory
RUN apt-get update
RUN apt-get -y upgrade
RUN mkdir -p /usr/src/app
# Install app dependencies
WORKDIR /usr/src/app
COPY phaserquest/package.json /usr/src/app/
RUN npm install
RUN npm install pm2 -g
# Bundle app source
WORKDIR /
RUN useradd -m ctf
COPY processes.yml /root/processes.yml
COPY phaserquest/ /usr/src/app
COPY flag /flag
COPY readflag /readflag
RUN chmod 555 /readflag
RUN chmod u+s /readflag
RUN chmod 500 /flag
RUN chmod -R 555 /usr/src/app
RUN chmod -R 500 /root/processes.yml
WORKDIR /usr/src/app

EXPOSE 10081
