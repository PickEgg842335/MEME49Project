#include    "erthItity.h"
#include    "ws2812bdisplay.h"
#include    "fileWRtask.h"
#include    "buzzertask.h"
#include    "envirtask.h"

#define cTaskQty                    7
#define cErthItityTask              0
#define cWs2812bDisplayTask         1
#define cFileWRTask                 2
#define cBuzzerTask                 3
#define cEnvirTask                  4
#define cDebugTask                  5
#define cIdleTask                   6

#define cTimeBase                   1
#define cErthItityTimeBase          10
#define cWs2812bDisplayTimeBase     10
#define cFileWRTimeBase             2000
#define cBuzzerTimeBase             5
#define cEnvirTimeBase              5000
#define cDebugTimeBase              1000

#define cErthItityTimeCount         (cErthItityTimeBase / cTimeBase)
#define cWs2812bDisplayTimeCount    (cWs2812bDisplayTimeBase / cTimeBase)
#define cFileWRTimeCount            (cFileWRTimeBase / cTimeBase)
#define cBuzzerTimeCount            (cBuzzerTimeBase / cTimeBase)
#define cEnvirTimeCount             (cEnvirTimeBase / cTimeBase)
#define cDebugTimeCount             (cDebugTimeBase / cTimeBase)

#define FourPcsLED_BLUE             0
#define FourPcsLED_LED_GREEN        1
#define FourPcsLED_LED_YELLOW       2
#define FourPcsLED_LED_RED          3

extern unsigned char sfGetTaskFlag(void);
extern void sfClearTaskFlag(unsigned char ubTaskNum);