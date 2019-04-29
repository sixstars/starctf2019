FROM ubuntu:18.04

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/mirrors.ustc.edu.cn/g" /etc/apt/sources.list
RUN apt-get update
RUN apt-get -y install software-properties-common
RUN add-apt-repository -y ppa:ondrej/php
RUN apt-get update
RUN apt-get -y upgrade
RUN apt-get -y install tzdata
RUN apt-get -y install vim
RUN apt-get -y install apache2
RUN apt-cache search "php" | grep "php7.3"| awk '{print $1}'| xargs apt-get -y install
RUN service --status-all | awk '{print $4}'| xargs -i service {} stop

RUN rm /var/www/html/index.html
COPY randomstack.php /var/www/html/index.php
COPY sandbox.php /var/www/html/sandbox.php
RUN chmod 755 -R /var/www/html/
COPY flag /flag
COPY readflag /readflag
RUN chmod 555 /readflag
RUN chmod u+s /readflag
RUN chmod 500 /flag
COPY ./run.sh /run.sh
COPY ./php.ini /etc/php/7.3/apache2/php.ini
RUN chmod 700 /run.sh

CMD ["/run.sh"]