#include    "erthItity.h"
#include    "ws2812bdisplay.h"
#include    "fileWRtask.h"
#include    "buzzertask.h"

#define cTaskQty                    6
#define cErthItityTask              0
#define cWs2812bDisplayTask         1
#define cFileWRTask                 2
#define cBuzzerTask                 3
#define cDebugTask                  4
#define cIdleTask                   5

#define cTimeBase                   1
#define cErthItityTimeBase          10
#define cWs2812bDisplayTimeBase     10
#define cFileWRTimeBase             2000
#define cBuzzerTimeBase             5
#define cDebugTimeBase              1000

#define cErthItityTimeCount         (cErthItityTimeBase / cTimeBase)
#define cWs2812bDisplayTimeCount    (cWs2812bDisplayTimeBase / cTimeBase)
#define cFileWRTimeCount            (cFileWRTimeBase / cTimeBase)
#define cBuzzerTimeCount            (cBuzzerTimeBase / cTimeBase)
#define cDebugTimeCount             (cDebugTimeBase / cTimeBase)

extern unsigned char sfGetTaskFlag(void);
extern void sfClearTaskFlag(unsigned char ubTaskNum);