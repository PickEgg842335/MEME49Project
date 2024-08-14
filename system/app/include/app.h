#include    "erthItity.h"
#include    "ws2812bdisplay.h"
#include    "fileWRtask.h"
#include    "buzzertask.h"
#include    "envirtask.h"
#include    "garsigtask.h"

#define cTaskQty                    8
#define cErthItityTask              0
#define cWs2812bDisplayTask         1
#define cFileWRTask                 2
#define cBuzzerTask                 3
#define cEnvirTask                  4
#define cGarsigTask                 5
#define cDebugTask                  6
#define cIdleTask                   7

#define cTimeBase                   1
#define cErthItityTimeBase          10
#define cWs2812bDisplayTimeBase     10
#define cFileWRTimeBase             2000
#define cBuzzerTimeBase             20
#define cEnvirTimeBase              5000
#define cGarsigTimeBase             50
#define cDebugTimeBase              1000

#define cErthItityTimeCount         (cErthItityTimeBase / cTimeBase)
#define cWs2812bDisplayTimeCount    (cWs2812bDisplayTimeBase / cTimeBase)
#define cFileWRTimeCount            (cFileWRTimeBase / cTimeBase)
#define cBuzzerTimeCount            (cBuzzerTimeBase / cTimeBase)
#define cEnvirTimeCount             (cEnvirTimeBase / cTimeBase)
#define cGarsigTimeCount            (cGarsigTimeBase / cTimeBase)
#define cDebugTimeCount             (cDebugTimeBase / cTimeBase)

extern unsigned char sfGetTaskFlag(void);
extern void sfClearTaskFlag(unsigned char ubTaskNum);