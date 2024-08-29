#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, const char *argv[])
{
    int fd = open("file.txt",
                  O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRWXU);
    assert(fd > -1);
    int rc = write(fd, "Hello, world!", 13);
    assert(rc == 13);
    close(fd);

    return 0;
}
