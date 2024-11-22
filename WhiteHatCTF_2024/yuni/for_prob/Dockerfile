FROM ubuntu:22.04
RUN apt update
RUN apt install xinetd -y
RUN useradd -mU kinga

COPY ./kinga /etc/xinetd.d/kinga
COPY ./main /home/kinga/main
COPY ./flag /flag

COPY ./start.sh /start.sh

RUN chown root:kinga /flag
RUN chomd 440 /flag
