FROM ubuntu:20.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt install -y ack antlr3 aria2 asciidoc autoconf automake autopoint binutils bison build-essential \
bzip2 ccache cmake cpio curl device-tree-compiler fastjar flex gawk gettext gcc-multilib g++-multilib \
git gperf haveged help2man intltool libc6-dev-i386 libelf-dev libglib2.0-dev libgmp3-dev libltdl-dev \
libmpc-dev libmpfr-dev libncurses5-dev libncursesw5-dev libreadline-dev libssl-dev libtool lrzsz \
mkisofs msmtp nano ninja-build p7zip p7zip-full patch pkgconf python2.7 python3 python3-pip libpython3-dev qemu-utils \
rsync scons squashfs-tools subversion swig texinfo uglifyjs upx-ucl unzip vim wget xmlto xxd zlib1g-dev
RUN apt-get install -y python3.9 socat
RUN groupadd -r pwn && useradd -r -g pwn pwn

RUN apt-get install git -y
RUN git clone -b dev https://github.com/qilingframework/qiling.git
RUN cd qiling && git submodule update --init --recursive
RUN cd qiling && git checkout a9ae692631ba490b61dc61c4f11bb5ef7979e461

COPY distfiles/apply.diff qiling/apply.diff
RUN cd qiling && patch -p1 < apply.diff
RUN cd qiling && python3.9 -m pip install .

ADD distfiles /home/pwn
WORKDIR /home/pwn
COPY flag flag.txt
RUN chmod 550 app.py
RUN chmod 440 flag.txt

RUN chown -R root:pwn /home/pwn
USER pwn

CMD socat TCP-L:9999,reuseaddr,fork EXEC:"python3.9 app.py ./bin/main"
