/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_COMMON_H
#define __USER_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include system header files -----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#include "stm32f7xx_hal.h"
#include "arm_math.h"

/* EVAL includes component */
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"

/* Include user header files -------------------------------------------------*/
// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"

// uGUI
#include "ugui.h"

/* Exported macro ------------------------------------------------------------*/
#define uGUI_MAX_OBJECTS 20

/* ---------------------------------------------------------------- */
/* -- User setting Configuration                                 -- */
/* ---------------------------------------------------------------- */
#define PRINTF_DEBUG_MDOE

/* Exported function macro ---------------------------------------------------*/
// HERE macro for printf debug
#define _STR(x)      #x
#define _STR2(x)     _STR(x)
#define __SLINE__    _STR2(__LINE__)
#define HERE         __FILE__ "(" __SLINE__ ")"

// COUNTOF macro
#define COUNTOF(__BUFFER__) (sizeof(__BUFFER__)/sizeof(*(__BUFFER__)))

/* Exported types ------------------------------------------------------------*/
/* Exported enum tag ---------------------------------------------------------*/
// FreeRTOS
enum FreeRTOSPriority
{
    Priority_Idle = tskIDLE_PRIORITY,
    Priority_Low,
    Priority_BelowNormal,
    Priority_Normal,
    Priority_AboveNormal,
    Priority_High,
    Priority_RealTime
};

/* Exported struct/union tag -------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __USER_COMMON_H */
/***************************************************************END OF FILE****/
