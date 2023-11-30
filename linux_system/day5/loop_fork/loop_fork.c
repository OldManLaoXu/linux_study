#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int main(int argc, char *agrv[])
{
    pid_t pid;
    int i;
    for(i = 0; i<5; i++)
    {
        pid = fork();
        if(pid == 0)
        {
            printf("I am %dth child\n", i+1);
            break;
        }
        else
        {
            sleep(1);
            printf("I am parent\n");
        }
    }
    return 0;
}

