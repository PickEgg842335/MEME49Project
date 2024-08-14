#include    "./include/app.h"
#include    <stdio.h>
#include    <stdlib.h>
#include    <stdbool.h>
#include    <math.h>
#include    <signal.h>
#include    <sys/time.h>
#include    <sys/ipc.h>
#include    <sys/shm.h>
#include    <sys/types.h>
#include    <sys/stat.h>


void sfinitialExitSIGINT(void);
void sfinitialTask(void);
void sfinitialtimer(void);
void timer_handler(int signum);
void handle_sigint(int signum);
void sfClearTaskFlag(unsigned char ubTaskNum);
void sfDebugTask(void);

static unsigned int uwTaskFlag = 0;
static unsigned int uwTaskTimeArray[cTaskQty];
static unsigned int uwOneMsecCounter = 0;
static unsigned int uwMaxCounter = 1;

int main(void)
{
    sfinitialExitSIGINT();
    sfinitialfileWR();
    sfinitialErthItity();
    sfinitialws2812bdispaly();
    sfinitialgarsig();
    sfinitialTask();
    sfinitialtimer();

    while(1)
    {
        sfErthItityTask();
        sfws2812bdisplayTask();
        sfbuzzerTask();
        sffileWRTask();
        sfenvirTask();
        sfgarsigTask();
        sfDebugTask();
    }
    return 0;
}


void sfinitialExitSIGINT(void)
{
    signal(SIGINT, handle_sigint);
}


void sfinitialTask(void)
{
    uwTaskTimeArray[cErthItityTask] = cErthItityTimeCount;
    uwTaskTimeArray[cWs2812bDisplayTask] = cWs2812bDisplayTimeCount;
    uwTaskTimeArray[cFileWRTask] = cFileWRTimeCount;
    uwTaskTimeArray[cBuzzerTask] = cBuzzerTimeCount;
    uwTaskTimeArray[cEnvirTask] = cEnvirTimeCount;
    uwTaskTimeArray[cGarsigTask] = cGarsigTimeCount;
    uwTaskTimeArray[cDebugTask] = cDebugTimeCount;
    uwTaskTimeArray[cIdleTask] = 1;
    for(int i = 0; i < cTaskQty; i++)
    {
        if(uwTaskTimeArray[i] != 0)
        {
            if(uwMaxCounter % uwTaskTimeArray[i] != 0)
            {
                uwMaxCounter = uwMaxCounter * uwTaskTimeArray[i];
            }
        }
    }
}


void sfinitialtimer(void)
{
    struct sigaction sa;
    struct itimerval timer;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timer_handler;
    sigaction(SIGALRM, &sa, NULL);
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 1000;
    setitimer(ITIMER_REAL, &timer, NULL);
}


void timer_handler(int signum) 
{
    if(++uwOneMsecCounter >= uwMaxCounter)
    {
        uwOneMsecCounter = 0;
    }

    for(int i = 0; i < cTaskQty; i++)
    {
        if((uwOneMsecCounter % uwTaskTimeArray[i]) == 0)
        {
            uwTaskFlag |= (1 << i);
        }
    }
}


void handle_sigint(int signum)
{
    sfexitws2812bdisplay();
    sfexitErthItity();
    sfexitbuzzertask();
    sfexitgarsig();
    exit(0);
}


void sfDebugTask(void)
{
    if((uwTaskFlag & (1 << cDebugTask)) == (1 << cDebugTask))
    {
        sfClearTaskFlag(cDebugTask);
        // printf("ID = %x\n", sfubGetadxl485DeviceId());
        // printf("RMSACC = %f\n", sffGetRmsTotalAcccle());
        // printf("Lv = %d\n", sfwGetItityLevel());
    }
}


unsigned char sfGetTaskFlag(void)
{
    return(uwTaskFlag);
}


void sfClearTaskFlag(unsigned char ubTaskNum)
{
    uwTaskFlag &= ~(1 <<  ubTaskNum);
}
