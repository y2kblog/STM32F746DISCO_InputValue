/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WINDOW_TEMPLETE_H
#define __WINDOW_TEMPLETE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include system header files -----------------------------------------------*/
// Include user header files
#include "UserCommon.h"

// Callee of this window
//#include "Window_Templete2.h"

/* Exported macro ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define TEMPLETE_WINDOW_TITLE	        "Templete window"
#define TEMPLETE_uGUI_OBJECTS_NUM       10
#define TEMPLETE_UPDATE_MS              50
#define TEMPLETE_BUTTON_GAP             5

/* Exported function macro ---------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported enum tag ---------------------------------------------------------*/
/* Exported struct/union tag -------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
// FreeRTOS
TaskHandle_t xHandle_Templete;

// uGUI
UG_WINDOW wnd_Templete;

/* Exported function prototypes ----------------------------------------------*/
void createTempleteWindow(void);

#ifdef __cplusplus
}
#endif

#endif /* __WINDOW_TEMPLETE_H */

/***************************************************************END OF FILE****/
