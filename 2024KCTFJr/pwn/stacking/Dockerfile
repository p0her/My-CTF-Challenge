FROM ubuntu:22.04

RUN apt -y update
RUN apt -y upgrade
RUN apt install -y socat
RUN groupadd -r stacking && useradd -r -g stacking stacking

COPY ./server/main /home/stacking/main
COPY ./server/flag /home/stacking/flag

WORKDIR /home/stacking
RUN chmod 440 flag
RUN chmod 550 main

RUN chown -R root:stacking /home/stacking

USER stacking

CMD socat TCP-L:8080,reuseaddr,fork, EXEC:"./main"
