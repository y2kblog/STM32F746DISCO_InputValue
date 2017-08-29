/* Include system header files -----------------------------------------------*/
/* Include user header files -------------------------------------------------*/
#include "FreeRTOS_Func.h"

/* Imported variables --------------------------------------------------------*/
/* Private function macro ----------------------------------------------------*/
/* Private enum tag ----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/* ---------------------------------------------------------------- */
/* -- FreeRTOS                                                   -- */
/* ---------------------------------------------------------------- */
#if configCHECK_FOR_STACK_OVERFLOW != 0
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
#ifdef PRINTF_DEBUG_MDOE
    printf("FreeRTOS stack overflow error : %s\r\n", pcTaskName);
#endif
}
#endif

#if configUSE_MALLOC_FAILED_HOOK != 0
void vApplicationMallocFailedHook(void)
{
#ifdef PRINTF_DEBUG_MDOE
    printf("FreeRTOS memory allocation failed error\r\n");
#endif
}
#endif

#if configUSE_TICK_HOOK == 1
void vApplicationTickHook(void)
{
}
#endif

#if configUSE_IDLE_HOOK == 1
void vApplicationIdleHook(void)
{
}
#endif

#ifdef configSUPPORT_STATIC_ALLOCATION
#if configSUPPORT_STATIC_ALLOCATION == 1
static StackType_t  IdleTaskStack[configMINIMAL_STACK_SIZE];
static StaticTask_t IdleTaskTCB;

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer   = &IdleTaskTCB;
    *ppxIdleTaskStackBuffer = &IdleTaskStack[0];
    *pulIdleTaskStackSize   = configMINIMAL_STACK_SIZE;
}
#endif
#endif

/***************************************************************END OF FILE****/
