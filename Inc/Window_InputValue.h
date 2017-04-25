/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WINDOW_INPUTVALUE_H
#define __WINDOW_INPUTVALUE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include system header files -----------------------------------------------*/
#include <string.h>
#include <ctype.h>

/* Include user header files -------------------------------------------------*/
#include "UserCommon.h"

// FreeRTOS
#include "semphr.h"

/* Private macro -------------------------------------------------------------*/
/* ---------------------------------------------------------------- */
/* -- User setting Configuration                                 -- */
/* ---------------------------------------------------------------- */
#define INPUTVALUE_WINDOW_TITLE             "Input value"
#define uGUI_INPUTVALUE_OBJECTS_NUM         20
#define INPUTVALUE_UPDATE_MS                50
#define INPUTVALUE_BUTTON_GAP_PIXEL         3
#define INPUTVALUE_DIGITS                   15
#define INPUTVALUE_BUTTON_FONT_SIZE         FONT_12X16
#define INPUTVALUE_TXTBOX_FONT_SIZE         FONT_16X26
#define INPUTVALUE_TXTBOX_RANGE_FONT_SIZE   FONT_6X10

/* Exported types ------------------------------------------------------------*/
/* Exported enum tag ---------------------------------------------------------*/
/* Exported struct/union tag -------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
void createInputValueWindow(void);
bool inputInteger_InBackground(bool *, bool **, int32_t, int32_t, int32_t *);
bool inputDecimal_InBackground(bool *, bool **, float, float, float *);

#ifdef __cplusplus
}
#endif

#endif /* __WINDOW_INPUTVALUE_H */
/***************************************************************END OF FILE****/
