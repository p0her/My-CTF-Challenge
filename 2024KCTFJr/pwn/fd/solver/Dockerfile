FROM ubuntu:22.04

RUN apt -y update
RUN apt -y upgrade
RUN apt install -y socat
RUN groupadd -r fd && useradd -r -g fd fd

COPY ./server/main /home/fd/main
COPY ./server/flag /flag

WORKDIR /home/fd
RUN chmod 440 /flag
RUN chmod 550 main

RUN chown -R root:fd /home/fd
RUN chown root:fd /flag
RUN ulimit -n 65536
USER fd

CMD socat TCP-L:8080,reuseaddr,fork, EXEC:"./main"