FROM ubuntu:22.04@sha256:77906da86b60585ce12215807090eb327e7386c8fafb5402369e421f44eff17e

RUN apt -y update
RUN apt -y upgrade
RUN apt install -y socat
RUN groupadd -r overflow-tutorial && useradd -r -g overflow-tutorial overflow-tutorial

COPY ./server/main /home/overflow-tutorial/main
COPY ./server/flag /flag

WORKDIR /home/overflow-tutorial
RUN chmod 440 /flag
RUN chmod 550 main

RUN chown -R root:overflow-tutorial /home/overflow-tutorial
RUN chown root:overflow-tutorial /flag
USER overflow-tutorial

CMD socat TCP-L:8080,reuseaddr,fork, EXEC:"./main"