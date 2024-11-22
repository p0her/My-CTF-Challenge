FROM ubuntu:22.04@sha256:0e5e4a57c2499249aafc3b40fcd541e9a456aab7296681a3994d631587203f97
RUN apt-get update -y && apt-get -y upgrade
RUN apt-get install cmake pkg-config -y
RUN apt-get install socat -y
RUN apt-get install git -y
RUN git clone https://github.com/unicorn-engine/unicorn.git
RUN cd unicorn && mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && make && make install
RUN cd unicorn/build && ls
RUN cd unicorn/build && cp libunicorn.so.2 /usr/lib/libunicorn.so.2
RUN groupadd -r pwn && useradd -r -g pwn pwn

COPY ./main /home/pwn/main
COPY ./flag /flag

RUN chmod 440 /flag
RUN chown root:pwn /flag
RUN chown -R root:pwn /home/pwn
RUN chmod 1770 /home/pwn
RUN mv /flag /flag-$(md5sum /flag | awk '{print $1}')

WORKDIR /home/pwn
RUN chmod 550 main
USER pwn

CMD socat TCP-L:8080,reuseaddr,fork, EXEC:"./main"