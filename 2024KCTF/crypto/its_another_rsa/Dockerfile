FROM python:3.10-slim-buster

ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && apt-get install -y socat
RUN apt-get install -y sagemath

ADD challenge/secret.py secret.py
ADD challenge/challenge.sage challenge.sage

CMD socat TCP-L:8080,fork,reuseaddr EXEC:"sage challenge.sage"