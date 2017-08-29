/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PORT_UGUI_H
#define __PORT_UGUI_H

/* Include system header files -----------------------------------------------*/
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* -------------------------------------------------------------------------------- */
/* -- CONFIG SECTION                                                             -- */
/* -------------------------------------------------------------------------------- */
/* Enable needed fonts here */
//#define  USE_FONT_4X6
#define  USE_FONT_5X8
//#define  USE_FONT_5X12
#define  USE_FONT_6X8
#define  USE_FONT_6X10
#define  USE_FONT_7X12
#define  USE_FONT_8X8
#define  USE_FONT_8X12
#define  USE_FONT_8X14
#define  USE_FONT_10X16
#define  USE_FONT_12X16
#define  USE_FONT_12X20
#define  USE_FONT_16X26
#define  USE_FONT_22X36
//#define  USE_FONT_24X40
//#define  USE_FONT_32X53

/* Specify platform-dependent integer types here */
//#define uGUI_DISPLAY_VERTICAL_MODE

#define __UG_CONST   const
typedef uint8_t      UG_U8;
typedef int8_t       UG_S8;
typedef uint16_t     UG_U16;
typedef int16_t      UG_S16;
typedef uint32_t     UG_U32;
typedef int32_t      UG_S32;
/* -------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------------- */
/* -- TYPEDEFS                                                                   -- */
/* -------------------------------------------------------------------------------- */
typedef UG_S8                                         UG_RESULT;
typedef UG_U32                                        UG_COLOR;

/* -------------------------------------------------------------------------------- */
/* -- FUNCTION RESULTS                                                           -- */
/* -------------------------------------------------------------------------------- */
#define UG_RESULT_FAIL                               -1
#define UG_RESULT_OK                                  0

// uGUI porting function
void pset(UG_S16, UG_S16, UG_COLOR);
UG_RESULT _HW_DrawLine(UG_S16, UG_S16, UG_S16, UG_S16, UG_COLOR);
UG_RESULT _HW_FillFrame(UG_S16, UG_S16, UG_S16, UG_S16, UG_COLOR);

#endif /* __PORT_UGUI_H */
/***************************************************************END OF FILE****/
