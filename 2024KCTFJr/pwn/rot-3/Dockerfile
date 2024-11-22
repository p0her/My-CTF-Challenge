FROM ubuntu:22.04@sha256:77906da86b60585ce12215807090eb327e7386c8fafb5402369e421f44eff17e

RUN apt -y update
RUN apt -y upgrade
RUN apt install -y socat
RUN groupadd -r rot3 && useradd -r -g rot3 rot3

COPY ./server/main /home/rot3/main
COPY ./server/flag /flag

WORKDIR /home/rot3
RUN chmod 440 /flag
RUN chmod 550 main

RUN chown -R root:rot3 /home/rot3
RUN chown root:rot3 /flag
USER rot3

CMD socat TCP-L:8080,reuseaddr,fork, EXEC:"./main"