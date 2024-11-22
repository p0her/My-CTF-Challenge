FROM ubuntu:22.04@sha256:0e5e4a57c2499249aafc3b40fcd541e9a456aab7296681a3994d631587203f97
RUN apt-get update -y && apt-get -y upgrade
RUN apt-get install socat -y
RUN groupadd -r pwn && useradd -r -g pwn pwn

COPY ./main /home/pwn/main
COPY ./flag /flag

RUN chmod 440 /flag
RUN chown root:pwn /flag
RUN chown -R root:pwn /home/pwn
RUN chmod 1770 /home/pwn

WORKDIR /home/pwn
RUN chmod 550 main
USER pwn

CMD socat TCP-L:8080,reuseaddr,fork, EXEC:"./main"
