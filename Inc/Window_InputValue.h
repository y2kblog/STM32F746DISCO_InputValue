/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WINDOW_INPUTVALUE_H
#define __WINDOW_INPUTVALUE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include system header files -----------------------------------------------*/
#include <string.h>
#include <ctype.h>
#include <float.h>

/* Include user header files -------------------------------------------------*/
#include "UserCommon.h"

// FreeRTOS
#include "semphr.h"

/* Private macro -------------------------------------------------------------*/
/* ---------------------------------------------------------------- */
/* -- User setting Configuration                                 -- */
/* ---------------------------------------------------------------- */
#define INPUTVALUE_WINDOW_TITLE             "Input value"
#define uGUI_INPUTVALUE_OBJECTS_NUM         18
#define INPUTVALUE_UPDATE_MS                50
#define INPUTVALUE_BUTTON_GAP_PIXEL         3
#define INPUTVALUE_DIGITS                   15
#define INPUTVALUE_BUTTON_FONT_SIZE         FONT_12X16
#define INPUTVALUE_TXTBOX_FONT_SIZE         FONT_16X26
#define INPUTVALUE_TXTBOX_RANGE_FONT_SIZE   FONT_6X10

/* Exported types ------------------------------------------------------------*/
/* Exported enum tag ---------------------------------------------------------*/
/* Exported struct/union tag -------------------------------------------------*/
typedef union
{
    int32_t InpVal_int32;
    float InpVal_float;
} _InpVal_val_union;

typedef struct
{
    bool StartUpEvent;
    bool *p_isCompleteInFuture;
    bool **pp_isCompleteInFuture;
    _InpVal_val_union InpVal;
} _InpVal_t;

// sample
/* static _InpVal_t sample_t = {false, NULL, &(sample_t.p_isCompleteInFuture), {0} }; */

/* Exported variables --------------------------------------------------------*/

/* FreeRTOS */
TaskHandle_t xHandle_InputValue;


/* Exported function prototypes ----------------------------------------------*/
void createInputValueWindow(void);
bool inputInteger_InBackground(_InpVal_t *, int32_t, int32_t);
bool inputDecimal_InBackground(_InpVal_t *, float, float);

#ifdef __cplusplus
}
#endif

#endif /* __WINDOW_INPUTVALUE_H */
/***************************************************************END OF FILE****/
