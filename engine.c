#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "monitor_ioctl.h"

#define STACK_SIZE 1024 * 1024

char child_stack[STACK_SIZE];

// 🔥 Container function
int container_main(void *arg) {
    char **args = (char **)arg;

    printf("🔥 Inside container!\n");

    // Set hostname (namespace isolation demo)
    sethostname("container", 9);

    // Execute command
    execvp(args[0], args);

    perror("exec failed");
    return 1;
}

int main(int argc, char *argv[]) {

    if (argc < 4) {
        printf("Usage:\n");
        printf("./engine run <id> <command>\n");
        return 1;
    }

    // Example: ./engine run 1 /bin/ls
    char *cmd = argv[3];
    char *args[] = {cmd, NULL};

    int id = atoi(argv[2]);

    pid_t pid = clone(container_main,
                      child_stack + STACK_SIZE,
                      CLONE_NEWPID | CLONE_NEWUTS | SIGCHLD,
                      args);

    if (pid < 0) {
        perror("clone failed");
        return 1;
    }

    printf("Container %d running (PID %d)\n", id, pid);

    // 🔥 Connect to kernel module
    int fd = open("/dev/container_monitor", O_RDWR);

    if (fd >= 0) {
        int limit = 50000; // 50MB

        ioctl(fd, SET_LIMIT, &limit);
        ioctl(fd, ADD_PID, &pid);

        close(fd);
    } else {
        perror("open /dev/container_monitor failed");
    }

    // Wait for container
    waitpid(pid, NULL, 0);

    printf("Container exited\n");

    return 0;
}
