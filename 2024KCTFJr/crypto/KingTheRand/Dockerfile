FROM ubuntu:22.04

RUN apt -y update
RUN apt -y upgrade
RUN apt install -y socat python3 python3-pip
RUN python3 -m pip install pycryptodome
RUN python3 -m pip install ecdsa
RUN groupadd -r king_the_rand && useradd -r -g king_the_rand king_the_rand

COPY ./server/main.py /home/king_the_rand/main.py 
COPY ./server/flag /home/king_the_rand/flag

WORKDIR /home/king_the_rand
RUN chmod 440 flag
RUN chmod 550 main.py

RUN chown -R root:king_the_rand /home/king_the_rand

USER king_the_rand

CMD socat TCP-L:8080,reuseaddr,fork, EXEC:"python3 main.py"
