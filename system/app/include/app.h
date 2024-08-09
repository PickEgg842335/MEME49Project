#include    <stdio.h>
#include    <math.h>
#include    <signal.h>
#include    <sys/time.h>
#include    "erthItity.h"
#include    "ws2812bdisplay.h"

#define cTaskQty                    4
#define cErthItityTask              0
#define cWs2812bDisplayTask         1
#define cDebugTask                  2
#define cIdleTask                   3

#define cTimeBase                   1
#define cErthItityTimeBase          10
#define cWs2812bDisplayTimeBase     10
#define cDebugTimeBase              1000

#define cErthItityTimeCount         (cErthItityTimeBase / cTimeBase)
#define cWs2812bDisplayTimeCount    (cWs2812bDisplayTimeBase / cTimeBase)
#define cDebugTimeCount             (cDebugTimeBase / cTimeBase)

extern unsigned char sfGetTaskFlag(void);
extern void sfClearTaskFlag(unsigned char ubTaskNum);