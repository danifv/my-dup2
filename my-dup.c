#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int myDup2(int fdToCopy, int fdRequested)
{
    if (fdToCopy==fdRequested) {
        return fdToCopy;
    }
    struct stat s;
    if (fstat(fdToCopy, &s) < 0) {
        fprintf(stderr, "EBADF: %s\n", strerror(EBADF));
        return -1;
    }
    int openMax = sysconf(_SC_OPEN_MAX);
    if (fdRequested < 0 || fdRequested > openMax) {
        fprintf(stderr, "EBADF: %s\n", strerror(EBADF));
        return -1;
    }
    if (fstat(fdRequested, &s) == 0) {
        close(fdRequested);
    }

    int fdList[fdRequested];
    int fdListIndex = 0;
    while(fdToCopy!=fdRequested) {
        if((fdToCopy = dup(fdToCopy)) < 0) {
            fprintf(stderr, "EBADF: %s\n", strerror(EBADF));
            return -1;
        };
        fdList[fdListIndex++] = fdToCopy;
    }

    for(int i = 0; i < fdListIndex - 1; i++) {
        close(fdList[i]);
    }
    return fdToCopy;
}

int main(int argc, char *argv[])
{
    
    int myFd = myDup2(2, 5);
    return 0;
    
}


