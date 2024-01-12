#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <sysexits.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

typedef struct {
    const char *name;
    int fd;
    struct stat stat;
    void *addr;
} IO;

// from FreeBSD's dd.h
#define BISZERO(p, s) ((s) > 0 && *((const char *)p) == 0 && !memcmp( \
                       (const void *)(p), (const void *) \
                       ((const char *)p + 1), (s) - 1))

int main(int argc, char *argv[]) {
    IO in, out;

    if(argc != 3)
        errx(EX_USAGE, "src dst");

    in.name = argv[1];
    out.name = argv[2];

    if((in.fd = open(in.name, O_RDONLY)) == -1)
        err(EX_NOINPUT, "%s", in.name);

    if (fstat(in.fd, &in.stat) == -1)
        err(EX_IOERR, "%s", in.name);

    if (in.stat.st_size == 0)
        errx(EX_NOINPUT, "%s: zero-length input", in.name);

    if((in.addr = mmap(0, in.stat.st_size, PROT_READ, MAP_SHARED, in.fd, 0)) == (void *) -1)
        err(EX_OSERR, "%s", in.name);

    if((out.fd = open(out.name, O_RDWR|O_CREAT|O_TRUNC, 0666)) == -1)
        err(EX_IOERR, "%s", out.name);

    if (lseek(out.fd, in.stat.st_size - 1, SEEK_SET) == -1)
        err(EX_IOERR, "%s", out.name);

    if (write(out.fd, (char[]){0}, 1) == -1)
        err(EX_IOERR, "%s", out.name);

    if (fstat(out.fd, &out.stat) == -1)
        err(EX_IOERR, "%s", out.name);

    if((out.addr = mmap(0, out.stat.st_size, PROT_WRITE, MAP_SHARED, out.fd, 0)) == (void *) -1)
        err(EX_OSERR, "%s", out.name);

    printf("input stat:  st_size=%lld st_blocks=%lld st_blksize=%d\n", in.stat.st_size, in.stat.st_blocks, in.stat.st_blksize);
    printf("output stat: st_size=%lld st_blocks=%lld st_blksize=%d\n", out.stat.st_size, out.stat.st_blocks, out.stat.st_blksize);

    void *page_from = in.addr;
    void *page_to = out.addr;
    size_t remaining = in.stat.st_size;
    size_t blocksize = out.stat.st_blksize;

    while (remaining) {
        if (remaining < blocksize)
            blocksize = remaining;

        if (!BISZERO(page_from, blocksize)) {
            memcpy(page_to, page_from, blocksize);
#ifdef NOISY_PROGRESS
            printf("+");
#endif
        }

        page_from += blocksize;
        page_to += blocksize;
        remaining -= blocksize;
    }

    munmap(in.addr, in.stat.st_size);
    munmap(out.addr, out.stat.st_size);

    close(in.fd);
    close(out.fd);

    return 0;
}
