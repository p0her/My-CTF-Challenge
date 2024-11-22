#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <seccomp.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>

void init()
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}

void init_seccomp()
{
    scmp_filter_ctx ctx;

    ctx = seccomp_init(SCMP_ACT_ALLOW);
    if (ctx == NULL)
    {
        perror("seccomp_init");
        exit(1);
    }

    if (seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execve), 0) < 0 ||
        seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(open), 0) < 0 ||
        seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(read), 0) < 0 ||
        seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(write), 0) < 0 ||
        seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(openat), 0) < 0 ||
        seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(openat2), 0) < 0 ||
        seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(clone), 0) < 0 ||
        seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(fork), 0) < 0 ||
        seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(vfork), 0) < 0 ||
        seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(clone), 0) < 0 ||
        seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(mmap), 0) < 0)
    {
        perror("seccomp_rule_add");
        seccomp_release(ctx);
        exit(1);
    }

    if (seccomp_load(ctx) < 0)
    {
        perror("seccomp_load");
        seccomp_release(ctx);
        exit(1);
    }
    seccomp_release(ctx);
}

int main()
{
    init();
    int fd = open("/flag", O_RDONLY);
    void *mem = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }
    printf("input opcode > ");
    read(0, mem, 0x100);
    init_seccomp();
    ((void (*)())mem)(); // sendfile syscall
    munmap(mem, 0x1000);
    return 0;
}