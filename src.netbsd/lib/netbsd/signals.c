#include <stdlib.h>
#include <signal.h>

#include "compat.h"

struct sigtab signals[] = {
    {"HUP", SIGHUP},
    {"INT", SIGINT},
    {"QUIT", SIGQUIT},
    {"ILL", SIGILL},
    {"ABRT", SIGABRT},
    {"FPE", SIGFPE},
    {"KILL", SIGKILL},
    {"SEGV", SIGSEGV},
    {"PIPE", SIGPIPE},
    {"ALRM", SIGALRM},
    {"TERM", SIGTERM},
    {"USR1", SIGUSR1},
    {"USR2", SIGUSR2},
    {NULL, 0}
};

int signalnumber(const char *name) {
    for(int i = 0; signals[i].name != NULL; i++) {
        if(strcmp(signals[i].name, name) == 0)
            return signals[i].number;
    }
    return -1;
}

int signalnext(int sig) {
    for(int i = 0; signals[i].name != NULL; i++) {
        if(signals[i].number == sig) {
            if(signals[i+1].name != NULL)
                return signals[i+1].number;
            else
                return 0;
        }
    }
    return 0;
}

