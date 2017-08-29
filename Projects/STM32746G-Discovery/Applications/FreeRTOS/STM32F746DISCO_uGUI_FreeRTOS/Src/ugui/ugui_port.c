/* Include user header files -------------------------------------------------*/
#include "ugui_port.h"


/* Include system header files -----------------------------------------------*/
#include "stm32746g_discovery_lcd.h"

/* -------------------------------------------------------------------------------- */
/* -- Porting function for uGUI                                                  -- */
/* -------------------------------------------------------------------------------- */

#ifdef uGUI_DISPLAY_VERTICAL_MODE
static inline void convertVertical(UG_S16 *px, UG_S16 *py)
{
    UG_S16 tmp;
    tmp = *px;
    *px = BSP_LCD_GetXSize() - *py -1;
    *py = tmp;
}
#endif

inline void pset(UG_S16 x, UG_S16 y, UG_COLOR col)
{
#ifdef uGUI_DISPLAY_VERTICAL_MODE
    convertVertical(&x, &y);
#endif
    //if( (x < BSP_LCD_GetXSize()) && (y < BSP_LCD_GetYSize()) )
        BSP_LCD_DrawPixel(x, y, (0xFF000000 | col) );
}

/* Hardware accelerator */
UG_RESULT _HW_DrawLine(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c)
{
#ifdef uGUI_DISPLAY_VERTICAL_MODE
    convertVertical(&x1, &y1);
    convertVertical(&x2, &y2);
#endif
    if( (x1 < BSP_LCD_GetXSize()) && (x2 < BSP_LCD_GetXSize()) && (y1 < BSP_LCD_GetYSize()) && (y2 < BSP_LCD_GetYSize()) )
    {
        BSP_LCD_SetTextColor( 0xFF000000 | c );
        BSP_LCD_DrawLine(x1, y1, x2, y2);
        return UG_RESULT_OK;
    }
    else
        return UG_RESULT_FAIL;
}

UG_RESULT _HW_FillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c)
{
#ifdef uGUI_DISPLAY_VERTICAL_MODE
    convertVertical(&x1, &y1);
    convertVertical(&x2, &y2);
#endif
    if( (x1 < BSP_LCD_GetXSize()) && (x2 < BSP_LCD_GetXSize()) && (y1 < BSP_LCD_GetYSize()) && (y2 < BSP_LCD_GetYSize()) )
    {
        BSP_LCD_SetTextColor( 0xFF000000 | c );
        BSP_LCD_FillRect((x1<x2)?x1:x2, (y1<y2)?y1:y2, (x1>x2)?(x1-x2+1):(x2-x1+1), (y1>y2)?(y1-y2+1):(y2-y1+1));
        return UG_RESULT_OK;
    }
    else
        return UG_RESULT_FAIL;
}

/***************************************************************END OF FILE****/
