FROM ubuntu:22.04

RUN apt -y update
RUN apt -y upgrade
RUN apt install -y socat python3 python3-pip
RUN apt install -y libgmp-dev
RUN python3 -m pip install pycryptodome
RUN python3 -m pip install fastecdsa
RUN groupadd -r encryptor_service && useradd -r -g encryptor_service encryptor_service

COPY ./server/main.py /home/encryptor_service/main.py 
COPY ./server/flag /home/encryptor_service/flag

WORKDIR /home/encryptor_service
RUN chmod 440 flag
RUN chmod 550 main.py

RUN chown -R root:encryptor_service /home/encryptor_service

USER encryptor_service

CMD socat TCP-L:8080,reuseaddr,fork, EXEC:"python3 main.py"
