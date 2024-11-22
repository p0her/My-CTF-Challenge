FROM ubuntu:20.04

RUN apt-get -y update --fix-missing
RUN apt-get -y upgrade
RUN apt install -y ack antlr3 aria2 asciidoc autoconf automake autopoint binutils bison build-essential \
    bzip2 ccache cmake cpio curl device-tree-compiler fastjar flex gawk gettext gcc-multilib g++-multilib \
    git gperf haveged help2man intltool libc6-dev-i386 libelf-dev libglib2.0-dev libgmp3-dev libltdl-dev \
    libmpc-dev libmpfr-dev libncurses5-dev libncursesw5-dev libreadline-dev libssl-dev libtool lrzsz \
    mkisofs msmtp nano ninja-build p7zip p7zip-full patch pkgconf python2.7 python3 python3-pip libpython3-dev qemu-utils \
    rsync scons squashfs-tools subversion swig texinfo uglifyjs upx-ucl unzip vim wget xmlto xxd zlib1g-dev
RUN apt install software-properties-common
RUN add-apt-repository ppa:deadsnakes/ppa
RUN apt install python3.9
RUN groupadd -r kidding && useradd -r -g kidding kidding
RUN pip3 install qiling

ADD distfiles /home/kidding

WORKDIR /home/kidding
RUN echo "BISC2024{fbe0255aa772fa929be10b6d81e24d794dde4edc5d06634be41a6098ba70e848}" > flag.txt
RUN chmod 440 flag.txt
RUN chmod 550 app.py

RUN chown -R root:kidding /home/kidding

USER kidding
CMD socat TCP-L:9999,reuseaddr,fork EXEC:"./app.py ./bin/main"