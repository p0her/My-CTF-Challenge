FROM ubuntu:22.04
RUN apt-get update
RUN apt-get install socat -y
RUN useradd -d /home/kinga kinga -s /bin/bash
RUN mkdir /home/kinga

COPY ./main /home/kinga/main
COPY ./flag /flag

COPY ./start.sh /start.sh

RUN chown root:kinga /flag
RUN chmod 440 /flag
RUN chmod +x /start.sh
