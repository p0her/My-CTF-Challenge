FROM python:3.10-slim-buster

ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && apt-get install -y socat
RUN python3 -m pip install --upgrade pip
RUN python3 -m pip install torch==2.3.1+cpu -f https://download.pytorch.org/whl/torch_stable.html
ADD challenge/flag.txt flag.txt
ADD challenge/challenge.py challenge.py

CMD socat TCP-L:8080,fork,reuseaddr EXEC:"python3 challenge.py"