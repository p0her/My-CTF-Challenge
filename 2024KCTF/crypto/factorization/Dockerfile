FROM python:3.10-slim-buster

ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && apt-get install -y socat
RUN python3 -m pip install pycryptodome

ADD challenge/secret.py secret.py
ADD challenge/challenge.py challenge.py

CMD socat TCP-L:8080,fork,reuseaddr EXEC:"python3 challenge.py"