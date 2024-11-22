FROM ubuntu:22.04
RUN apt-get update
RUN apt-get install socat -y
RUN apt-get install libseccomp-dev -y
RUN useradd -d /home/yuni yuni -s /bin/bash
RUN mkdir /home/yuni

COPY ./main /home/yuni/main
COPY ./flag-fc66ff0869d1b9afc9024e91b9af7e8b9bbe09e1 /flag-fc66ff0869d1b9afc9024e91b9af7e8b9bbe09e1

COPY ./start.sh /start.sh

RUN chown root:yuni /flag-fc66ff0869d1b9afc9024e91b9af7e8b9bbe09e1
RUN chmod 440 /flag-fc66ff0869d1b9afc9024e91b9af7e8b9bbe09e1
RUN chmod +x /start.sh
