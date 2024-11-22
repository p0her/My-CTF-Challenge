FROM ubuntu:22.04@sha256:77906da86b60585ce12215807090eb327e7386c8fafb5402369e421f44eff17e

RUN apt -y update
RUN apt -y upgrade
RUN apt install -y socat openssl libssl-dev
RUN groupadd -r kapomarket && useradd -r -g kapomarket kapomarket

COPY ./server/main /home/kapomarket/main
COPY ./server/flag /flag

WORKDIR /home/kapomarket
RUN chmod 440 /flag
RUN chmod 550 main

RUN chown -R root:kapomarket /home/kapomarket
RUN chown root:kapomarket /flag
USER kapomarket

CMD socat TCP-L:8080,reuseaddr,fork, EXEC:"./main"