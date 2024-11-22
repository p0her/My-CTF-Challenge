FROM ubuntu:22.04@sha256:77906da86b60585ce12215807090eb327e7386c8fafb5402369e421f44eff17e

RUN apt -y update
RUN apt -y upgrade
RUN apt install -y socat
RUN groupadd -r heap_tutorial && useradd -r -g heap_tutorial heap_tutorial

COPY ./deploy/main /home/heap_tutorial/main
COPY ./deploy/flag /flag

WORKDIR /home/heap_tutorial
RUN chmod 440 /flag
RUN chmod 550 main

RUN chown -R root:heap_tutorial /home/heap_tutorial
RUN chown root:heap_tutorial /flag
USER heap_tutorial

CMD socat TCP-L:8080,reuseaddr,fork, EXEC:"./main"