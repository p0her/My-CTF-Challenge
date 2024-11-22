#include <stdio.h>
#include <unistd.h>
#include <seccomp.h>
#include <linux/seccomp.h>
#include <syscall.h>
#include <sys/prctl.h>
#include <fcntl.h>
#include <seccomp.h>

void init_buf() {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}

void sandbox() {
    scmp_filter_ctx ctx;
	ctx = seccomp_init(SCMP_ACT_KILL);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(read), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mmap), 0);
	seccomp_load(ctx);
}

int main() {
    init_buf();
    sandbox();
    void (*shellcode_addr)();
    shellcode_addr = mmap(0x41410000, 0x1000, 7, 34, -1, 0);
    read(0, shellcode_addr, 0x1000);
    shellcode_addr();
}