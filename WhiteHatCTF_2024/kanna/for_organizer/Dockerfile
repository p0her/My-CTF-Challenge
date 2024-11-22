FROM ubuntu:22.04
RUN apt-get update
RUN apt-get install socat -y
RUN apt-get install libseccomp-dev -y
RUN useradd -d /home/kanna kanna -s /bin/bash
RUN mkdir /home/kanna

COPY ./main /home/kanna/main
COPY ./flag /flag

COPY ./start.sh /start.sh

RUN chown root:kanna /flag
RUN chmod 440 /flag
RUN chmod +x /start.sh
