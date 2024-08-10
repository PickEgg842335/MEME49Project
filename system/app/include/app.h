#include    <stdio.h>
#include    <stdlib.h>
#include    <stdbool.h>
#include    <math.h>
#include    <signal.h>
#include    <sys/time.h>
#include    <sys/ipc.h>
#include    <sys/shm.h>
#include    <sys/types.h>
#include    "erthItity.h"
#include    "ws2812bdisplay.h"
#include    "sharedMemtask.h"

#define cTaskQty                    5
#define cErthItityTask              0
#define cWs2812bDisplayTask         1
#define cSharedMemTask              2
#define cDebugTask                  3
#define cIdleTask                   4

#define cTimeBase                   1
#define cErthItityTimeBase          10
#define cWs2812bDisplayTimeBase     10
#define cSharedMemTimeBase          50
#define cDebugTimeBase              1000

#define cErthItityTimeCount         (cErthItityTimeBase / cTimeBase)
#define cWs2812bDisplayTimeCount    (cWs2812bDisplayTimeBase / cTimeBase)
#define cSharedMemTimeCount         (cSharedMemTimeBase / cTimeBase)
#define cDebugTimeCount             (cDebugTimeBase / cTimeBase)

extern unsigned char sfGetTaskFlag(void);
extern void sfClearTaskFlag(unsigned char ubTaskNum);