/* Include system header files -----------------------------------------------*/
/* Include user header files -------------------------------------------------*/
#include "Window_InputValue.h"

/* Imported variables --------------------------------------------------------*/
/* Private function macro ----------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
const char str_invalid_range[] = "Invalid range";

/* Private enum tag ----------------------------------------------------------*/
static enum BTN_ID_TAG
{
	BTN_ID_Numeral_0 = BTN_ID_0,
    BTN_ID_Numeral_1,
    BTN_ID_Numeral_2,
    BTN_ID_Numeral_3,
    BTN_ID_Numeral_4,
    BTN_ID_Numeral_5,
    BTN_ID_Numeral_6,
    BTN_ID_Numeral_7,
    BTN_ID_Numeral_8,
    BTN_ID_Numeral_9,
    BTN_ID_Decimal_Point,
    BTN_ID_Minus_Sign,
    BTN_ID_OK,
    BTN_ID_Cancel,
    BTN_ID_Clear,
    BTN_ID_BackSpace,
};

static enum TXB_ID_TAG
{
    TXB_ID_DisplayValue = TXB_ID_0,
    TXB_ID_ValueRange
};

/* Private variables ---------------------------------------------------------*/
/* uGUI */
static UG_WINDOW wnd_InputValue;

/* "this" pointer */
static TaskHandle_t* pthis_xHandle = &xHandle_InputValue;
static UG_WINDOW* pthis_wnd = &wnd_InputValue;

/* thread control */
static bool needFinalize;  // This flag is used in "WindowControlThread" and "window_callback" function
static bool isCompletedInputValue = false;
static SemaphoreHandle_t xSemaphore_InputValue;

/* Objects of uGUI */
static UG_OBJECT obj_this_wnd[uGUI_INPUTVALUE_OBJECTS_NUM];
static UG_BUTTON btn_Numeral_0;
static UG_BUTTON btn_Numeral_1;
static UG_BUTTON btn_Numeral_2;
static UG_BUTTON btn_Numeral_3;
static UG_BUTTON btn_Numeral_4;
static UG_BUTTON btn_Numeral_5;
static UG_BUTTON btn_Numeral_6;
static UG_BUTTON btn_Numeral_7;
static UG_BUTTON btn_Numeral_8;
static UG_BUTTON btn_Numeral_9;
static UG_BUTTON btn_Minus_Sign;
static UG_BUTTON btn_Decimal_Point;
static UG_BUTTON btn_OK;
static UG_BUTTON btn_Cancel;
static UG_BUTTON btn_Clear;
static UG_BUTTON btn_BackSpace;
static UG_TEXTBOX txb_DisplayValue;
static UG_TEXTBOX txb_ValueRange;

/* Value */
static char     strValue[INPUTVALUE_DIGITS+2];
static char     strValue_prev[INPUTVALUE_DIGITS+2];
static char     strValueRange[30];
static int32_t  *pInt32ResultValue;
static int32_t  Int32Value;
static int32_t  Int32MaxValue;
static int32_t  Int32MinValue;
static float    *pDecimalResultValue;
static float    DecimalValue;
static float    DecimalMaxValue;
static float    DecimalMinValue;

static bool isIntegerMode;
static bool needToShowMinusSign;
static bool hasUpdated;

/* Private function prototypes -----------------------------------------------*/
static void WindowControlThread(void const *argument);
static void window_callback( UG_MESSAGE* msg );
static void initialize(void);
static void finalize(void);

static void init_strValue(void);
static void addChar_strValue(char);
static void addFirstChar_strValue(char);
static void deleteChar_strValue(void);
static void deleteFirstChar_strValue(void);

static bool* requestInputIntegerInRange(int32_t, int32_t, int32_t *);
static bool* requestInputDecimalInRange(float, float, float *);
static bool  hasUpdatedInputValue(void);

/* Exported functions --------------------------------------------------------*/

/* ---------------------------------------------------------------- */
/* -- Create Window                                              -- */
/* ---------------------------------------------------------------- */
void createInputValueWindow(void)
{
    UG_U8 id_buf;

    UG_WindowCreate(pthis_wnd, obj_this_wnd, COUNTOF(obj_this_wnd), window_callback);
    UG_WindowSetTitleText(pthis_wnd, INPUTVALUE_WINDOW_TITLE);
    UG_WindowSetTitleTextFont(pthis_wnd, &FONT_10X16);
    UG_WindowResize(pthis_wnd,
            BSP_LCD_GetXSize() * 0.2f,
            BSP_LCD_GetYSize() * 0.05f,
            BSP_LCD_GetXSize() * (1.0f - 0.2f),
            BSP_LCD_GetYSize() * (1.0f - 0.05f) );
    
    // Create buttons
    id_buf = BTN_ID_Numeral_0;
    UG_ButtonCreate(pthis_wnd, &btn_Numeral_0, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 0 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 4 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 1 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 5 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL );
    UG_ButtonSetFont(pthis_wnd, id_buf, &INPUTVALUE_BUTTON_FONT_SIZE);
    UG_ButtonSetText(pthis_wnd, id_buf, "0");
    
    id_buf = BTN_ID_Numeral_1;
    UG_ButtonCreate(pthis_wnd, &btn_Numeral_1, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 0 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 3 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 1 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 4 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL );
    UG_ButtonSetFont(pthis_wnd, id_buf, &INPUTVALUE_BUTTON_FONT_SIZE);
    UG_ButtonSetText(pthis_wnd, id_buf, "1");
    
    id_buf = BTN_ID_Numeral_2;
    UG_ButtonCreate(pthis_wnd, &btn_Numeral_2, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 1 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 3 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 2 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 4 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL );
    UG_ButtonSetFont(pthis_wnd, id_buf, &INPUTVALUE_BUTTON_FONT_SIZE);
    UG_ButtonSetText(pthis_wnd, id_buf, "2");
    
    id_buf = BTN_ID_Numeral_3;
    UG_ButtonCreate(pthis_wnd, &btn_Numeral_3, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 2 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 3 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 3 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 4 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL );
    UG_ButtonSetFont(pthis_wnd, id_buf, &INPUTVALUE_BUTTON_FONT_SIZE);
    UG_ButtonSetText(pthis_wnd, id_buf, "3");
    
    id_buf = BTN_ID_Numeral_4;
    UG_ButtonCreate(pthis_wnd, &btn_Numeral_4, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 0 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 2 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 1 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 3 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL );
    UG_ButtonSetFont(pthis_wnd, id_buf, &INPUTVALUE_BUTTON_FONT_SIZE);
    UG_ButtonSetText(pthis_wnd, id_buf, "4");
    
    id_buf = BTN_ID_Numeral_5;
    UG_ButtonCreate(pthis_wnd, &btn_Numeral_5, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 1 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 2 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 2 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 3 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL );
    UG_ButtonSetFont(pthis_wnd, id_buf, &INPUTVALUE_BUTTON_FONT_SIZE);
    UG_ButtonSetText(pthis_wnd, id_buf, "5");
    
    id_buf = BTN_ID_Numeral_6;
    UG_ButtonCreate(pthis_wnd, &btn_Numeral_6, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 2 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 2 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 3 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 3 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL );
    UG_ButtonSetFont(pthis_wnd, id_buf, &INPUTVALUE_BUTTON_FONT_SIZE);
    UG_ButtonSetText(pthis_wnd, id_buf, "6");
    
    id_buf = BTN_ID_Numeral_7;
    UG_ButtonCreate(pthis_wnd, &btn_Numeral_7, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 0 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 1 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 1 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 2 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL );
    UG_ButtonSetFont(pthis_wnd, id_buf, &INPUTVALUE_BUTTON_FONT_SIZE);
    UG_ButtonSetText(pthis_wnd, id_buf, "7");
    
    id_buf = BTN_ID_Numeral_8;
    UG_ButtonCreate(pthis_wnd, &btn_Numeral_8, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 1 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 1 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 2 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 2 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL );
    UG_ButtonSetFont(pthis_wnd, id_buf, &INPUTVALUE_BUTTON_FONT_SIZE);
    UG_ButtonSetText(pthis_wnd, id_buf, "8");
    
    id_buf = BTN_ID_Numeral_9;
    UG_ButtonCreate(pthis_wnd, &btn_Numeral_9, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 2 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 1 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 3 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 2 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL );
    UG_ButtonSetFont(pthis_wnd, id_buf, &INPUTVALUE_BUTTON_FONT_SIZE);
    UG_ButtonSetText(pthis_wnd, id_buf, "9");
    
    id_buf = BTN_ID_OK;
    UG_ButtonCreate(pthis_wnd, &btn_OK, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 3 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 4 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 5 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 5 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL );
    UG_ButtonSetFont(pthis_wnd, id_buf, &INPUTVALUE_BUTTON_FONT_SIZE);
    UG_ButtonSetText(pthis_wnd, id_buf, "OK");
    
    id_buf = BTN_ID_Cancel;
    UG_ButtonCreate(pthis_wnd, &btn_Cancel, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 3 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 3 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 5 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 4 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL );
    UG_ButtonSetFont(pthis_wnd, id_buf, &INPUTVALUE_BUTTON_FONT_SIZE);
    UG_ButtonSetText(pthis_wnd, id_buf, "Cancel");
    
    id_buf = BTN_ID_Clear;
    UG_ButtonCreate(pthis_wnd, &btn_Clear, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 3 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 2 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 5 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 3 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL );
    UG_ButtonSetFont(pthis_wnd, id_buf, &INPUTVALUE_BUTTON_FONT_SIZE);
    UG_ButtonSetText(pthis_wnd, id_buf, "Clear");
    
    id_buf = BTN_ID_BackSpace;
    UG_ButtonCreate(pthis_wnd, &btn_BackSpace, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 3 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 1 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 5 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 2 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL );
    UG_ButtonSetFont(pthis_wnd, id_buf, &INPUTVALUE_BUTTON_FONT_SIZE);
    UG_ButtonSetText(pthis_wnd, id_buf, "Back");
    
    id_buf = BTN_ID_Minus_Sign;
    UG_ButtonCreate(pthis_wnd, &btn_Minus_Sign, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 1 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 4 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 2 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 5 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL );
    UG_ButtonSetFont(pthis_wnd, id_buf, &INPUTVALUE_BUTTON_FONT_SIZE);
    UG_ButtonSetText(pthis_wnd, id_buf, "+/-");
    
    id_buf = BTN_ID_Decimal_Point;
    UG_ButtonCreate(pthis_wnd, &btn_Decimal_Point, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 2 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 4 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 3 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 5 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL );
    UG_ButtonSetFont(pthis_wnd, id_buf, &INPUTVALUE_BUTTON_FONT_SIZE);
    UG_ButtonSetText(pthis_wnd, id_buf, ".");
    
    /* Configure textbox */
    init_strValue();
    id_buf = TXB_ID_DisplayValue;
    UG_TextboxCreate(pthis_wnd, &txb_DisplayValue, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 0 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 0 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL + 10 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 5 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 1 / 5 - 0 );
    UG_TextboxSetFont(pthis_wnd, id_buf, &INPUTVALUE_TXTBOX_FONT_SIZE);
    UG_TextboxSetText(pthis_wnd, id_buf, strValue);
    UG_TextboxSetBackColor(pthis_wnd, id_buf, C_WHITE);
    UG_TextboxSetForeColor(pthis_wnd, id_buf, C_BLACK);
    UG_TextboxSetAlignment(pthis_wnd, id_buf, ALIGN_CENTER_RIGHT);
    
    id_buf = TXB_ID_ValueRange;
    UG_TextboxCreate(pthis_wnd, &txb_ValueRange, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 0 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 0 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerWidth(pthis_wnd)  * 5 / 5 - INPUTVALUE_BUTTON_GAP_PIXEL,
            UG_WindowGetInnerHeight(pthis_wnd) * 0 / 5 + INPUTVALUE_BUTTON_GAP_PIXEL + 10 );
    UG_TextboxSetFont(pthis_wnd, id_buf, &INPUTVALUE_TXTBOX_RANGE_FONT_SIZE);
    UG_TextboxSetText(pthis_wnd, id_buf, "Range [0, 0]");
    UG_TextboxSetAlignment(pthis_wnd, id_buf, ALIGN_CENTER_LEFT);

    xTaskCreate( (TaskFunction_t)WindowControlThread, "InputValueTask", 
            configMINIMAL_STACK_SIZE + 256, NULL, Priority_High, pthis_xHandle);
}

// return value -> true : update
// Arrange of "Future pattern" : Future instance is "*p_isCompleteInFuture"
bool inputInteger_InBackground(_InpVal_t *p, int32_t min, int32_t max)
{
    // "p_StartUpEvent" is normally set "true" in callback function
    if(p->StartUpEvent)
    {
        p->StartUpEvent = false;
        *(p->pp_isCompleteInFuture) = requestInputIntegerInRange(min, max, &(p->InpVal.InpVal_int32) );
    }
    
    if(*(p->pp_isCompleteInFuture) != NULL)
    {
        if(**(p->pp_isCompleteInFuture))
        {
            **(p->pp_isCompleteInFuture) = false;
            *(p->pp_isCompleteInFuture) = NULL;
            if(hasUpdatedInputValue())
                return true;
        }
    }
    return false;
}

bool inputDecimal_InBackground(_InpVal_t *p, float min, float max)
{
    // "p_StartUpEvent" is normally set "true" in callback function
    if(p->StartUpEvent)
    {
        p->StartUpEvent = false;
        *(p->pp_isCompleteInFuture) = requestInputDecimalInRange(min, max, &(p->InpVal.InpVal_float) );
    }
    
    if(*(p->pp_isCompleteInFuture) != NULL)
    {
        if(**(p->pp_isCompleteInFuture))
        {
            **(p->pp_isCompleteInFuture) = false;
            *(p->pp_isCompleteInFuture) = NULL;
            if(hasUpdatedInputValue())
                return true;
        }
    }
    return false;
}


/* Private functions ---------------------------------------------------------*/

/* ---------------------------------------------------------------- */
/* -- Thread                                                     -- */
/* ---------------------------------------------------------------- */
static void WindowControlThread(void const *argument)
{
#ifdef PRINTF_DEBUG_MDOE
    printf("%s thread start\r\n", pcTaskGetName(*pthis_xHandle) );
#endif
    
	/* Variables initialization ------------------------------------*/
    xSemaphore_InputValue = xSemaphoreCreateMutex();
	
	bool shouldSuspend  = true ;
    bool needInitialize = false;
    	 needFinalize   = false;    /* only "needFinalize" flag is changed from "window_callback" function */
	
    while (1)
    {
    	/* Thread flow */
    	vTaskDelay(INPUTVALUE_UPDATE_MS);
		
    	if(shouldSuspend)
    	{
#ifdef PRINTF_DEBUG_MDOE
            printf("%s thread suspend\r\n", pcTaskGetName(*pthis_xHandle) );
#endif
            vTaskSuspend(NULL);     // <- start here when resume
#ifdef PRINTF_DEBUG_MDOE
            printf("%s thread resume\r\n", pcTaskGetName(*pthis_xHandle) );
#endif
    		shouldSuspend  = false;
    		needInitialize = true;
    	}

        if(needInitialize)
        {
        	initialize();
        	needInitialize = false;
        }

        if(needFinalize)
        {
        	finalize();
        	needFinalize   = false;
        	shouldSuspend  = true;
        }
    }
}

/* ---------------------------------------------------------------- */
/* -- Callback function                                          -- */
/* ---------------------------------------------------------------- */
static void window_callback(UG_MESSAGE* msg)
{
    if (msg->type == MSG_TYPE_OBJECT)
    {
        if (msg->id == OBJ_TYPE_BUTTON)
        {
            bool isInvalid = false;
            if(strcmp(strValue, str_invalid_range) == 0)
            {
                isInvalid = true;
                init_strValue();
                UG_TextboxSetAlignment(pthis_wnd, TXB_ID_DisplayValue, ALIGN_CENTER_RIGHT);
                UG_TextboxSetForeColor(pthis_wnd, TXB_ID_DisplayValue, C_BLACK);
            }
            
            switch (msg->sub_id)
            {
            case BTN_ID_Numeral_0:
                addChar_strValue('0');
                break;
                
            case BTN_ID_Numeral_1:
                addChar_strValue('1');
                break;
                
            case BTN_ID_Numeral_2:
                addChar_strValue('2');
                break;
                
            case BTN_ID_Numeral_3:
                addChar_strValue('3');
                break;
                
            case BTN_ID_Numeral_4:
                addChar_strValue('4');
                break;
                
            case BTN_ID_Numeral_5:
                addChar_strValue('5');
                break;
                
            case BTN_ID_Numeral_6:
                addChar_strValue('6');
                break;
                
            case BTN_ID_Numeral_7:
                addChar_strValue('7');
                break;
                
            case BTN_ID_Numeral_8:
                addChar_strValue('8');
                break;
                
            case BTN_ID_Numeral_9:
                addChar_strValue('9');
                break;
                
            case BTN_ID_Minus_Sign:
                if(needToShowMinusSign)
                {
                    if (strValue[0] == '-')
                        deleteFirstChar_strValue();
                    else
                        addFirstChar_strValue('-');
                }
                break;

            case BTN_ID_Decimal_Point:
                if (!isIntegerMode)
                {
                    // Prevent to input '.' more than once
                    if (strchr(strValue, '.') == NULL)
                        addChar_strValue('.');
                }
                break;
                
            case BTN_ID_OK:
                /* Complete WITH value reflection */
                
                if(isInvalid)
                    break;
                
                // add '0' if last character is '.'
                if (strlen(strValue) > 0)
                {
                    if (strValue[strlen(strValue) - 1] == '.')
                        strncat(strValue, (char *)'0', 1);
                }
                
                // String -> Value
                if(isIntegerMode)
                {
                    Int32Value = atol(strValue);
                    if( (Int32Value < Int32MinValue) || (Int32Value > Int32MaxValue) )
                    {
                        // Value range invalid
                        sprintf(strValue, str_invalid_range);
                        UG_TextboxSetAlignment(pthis_wnd, TXB_ID_DisplayValue, ALIGN_CENTER);
                        UG_TextboxSetForeColor(pthis_wnd, TXB_ID_DisplayValue, C_RED);
                        break;
                    }
                    *pInt32ResultValue = Int32Value;
                }
                else
                {
                    DecimalValue = atof(strValue);
                    if( (DecimalValue < DecimalMinValue) || (DecimalValue > DecimalMaxValue) )
                    {
                        // Value range invalid
                        sprintf(strValue, str_invalid_range);
                        UG_TextboxSetAlignment(pthis_wnd, TXB_ID_DisplayValue, ALIGN_CENTER);
                        UG_TextboxSetForeColor(pthis_wnd, TXB_ID_DisplayValue, C_RED);
                        break;
                    }
                    *pDecimalResultValue = DecimalValue;
                }
                
                hasUpdated   = true;
                needFinalize = true;
                break;
                
            case BTN_ID_Cancel:
                /* Complete WITHOUT value reflection */
                hasUpdated   = false;
                needFinalize = true;
                break;
                
            case BTN_ID_Clear:
                init_strValue();
                break;
                
            case BTN_ID_BackSpace:
                deleteChar_strValue();
                break;

            default:
#ifdef PRINTF_DEBUG_MDOE
                printf("Error: " HERE "\r\n");
#endif
            break;
            }
            
            // add '0' to display when numeral is not input
            if( (strcmp(strValue, "") == 0) || (strcmp(strValue, "-") == 0) )
                addChar_strValue('0');

            // To avoid flickering, update displayed value if string is changed
            if (strcmp(strValue_prev, strValue) != 0)
                UG_TextboxSetText(pthis_wnd, TXB_ID_DisplayValue, strValue);
            strcpy(strValue_prev, strValue);
        }
    }
}

/* ---------------------------------------------------------------- */
/* -- Initialize                                                 -- */
/* ---------------------------------------------------------------- */
static void initialize(void)
{
	/* Variables Initialization */
    hasUpdated = false;
	
	/* Show this Window */
    UG_WindowShow(pthis_wnd);
}

/* ---------------------------------------------------------------- */
/* -- Finalize                                                   -- */
/* ---------------------------------------------------------------- */
static void finalize(void)
{
    /* Variables Finalization */
    init_strValue();
    
	/* Hide this Window */
	UG_WindowHide(pthis_wnd);
	
	/* Control flow */
    isCompletedInputValue = true;
    xSemaphoreGive(xSemaphore_InputValue);
}


static void init_strValue(void)
{
    for (uint16_t i = 0; i < COUNTOF(strValue); i++)
        strValue[i] = '\0';
    strValue[0] = '0';
}

static void addChar_strValue(char c)
{
    // Delete '0' at the head of integer part
    if(isdigit(c))
    {
        if (strchr(strValue, '.') == NULL)
        {
            if (strValue[0] == '0')
                deleteChar_strValue();
            else if ((strValue[0] == '-') && (strValue[1] == '0'))
                deleteChar_strValue();
        }
    }
    
    if(strlen(strValue) < INPUTVALUE_DIGITS)
        strncat(strValue, &c, 1);
}

static void addFirstChar_strValue(char c)
{
    size_t tmp = strlen(strValue);
    if (tmp < INPUTVALUE_DIGITS)
    {
        for (uint16_t i = tmp; i > 0; i--)
            strValue[i] = strValue[i - 1];
        strValue[0] = c;
    }
}

static void deleteChar_strValue(void)
{
    size_t tmp = strlen(strValue);
    if(tmp > 0)
        strValue[tmp-1] = '\0';
}

static void deleteFirstChar_strValue(void)
{
    size_t tmp = strlen(strValue);
    for (uint16_t i = 0; i < tmp; i++)
        strValue[i] = strValue[i + 1];
}


static bool* requestInputIntegerInRange(int32_t min, int32_t max, int32_t *InputValue)
{
    // Take Mutex
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if(xSemaphoreTakeFromISR(xSemaphore_InputValue, &xHigherPriorityTaskWoken) == pdFALSE)
    {
#ifdef PRINTF_DEBUG_MDOE
        printf("xSemaphoreTakeFromISR error\r\n");
#endif
        return NULL;
    }
    if (xHigherPriorityTaskWoken != pdFALSE)
        taskYIELD();
    
    isIntegerMode = true;
    pInt32ResultValue = InputValue;
    if (min < max)
    {
        Int32MinValue = min;
        Int32MaxValue = max;
    }
    else
    {
        Int32MaxValue = min;
        Int32MinValue = max;
    }
    
    /* config button Show/Hide */
    //UG_ButtonHide(pthis_wnd, BTN_ID_Decimal_Point);
    UG_ButtonSetText(pthis_wnd, BTN_ID_Decimal_Point, "");
    UG_ButtonSetStyle(pthis_wnd, BTN_ID_Decimal_Point, BTN_STYLE_2D);
    UG_ButtonSetAlternateForeColor(pthis_wnd, BTN_ID_Decimal_Point, C_WHITE_SMOKE);
    
    if(Int32MinValue < 0)
    {
        needToShowMinusSign = true;
        //UG_ButtonShow(pthis_wnd, BTN_ID_Minus_Sign);
        UG_ButtonSetText(pthis_wnd, BTN_ID_Minus_Sign, "+/-");
        UG_ButtonSetStyle(pthis_wnd, BTN_ID_Minus_Sign, BTN_STYLE_3D);
        UG_ButtonSetAlternateForeColor(pthis_wnd, BTN_ID_Minus_Sign, C_BLACK);
    }
    else
    {
        needToShowMinusSign = false;
        //UG_ButtonHide(pthis_wnd, BTN_ID_Minus_Sign);
        UG_ButtonSetText(pthis_wnd, BTN_ID_Minus_Sign, "");
        UG_ButtonSetStyle(pthis_wnd, BTN_ID_Minus_Sign, BTN_STYLE_2D);
        UG_ButtonSetAlternateForeColor(pthis_wnd, BTN_ID_Minus_Sign, C_WHITE_SMOKE);
    }
    
    if(Int32MinValue == INT32_MIN)
    {
        if(Int32MaxValue == INT32_MAX)
            sprintf(strValueRange, "Range [-inf, +inf]");
        else
            sprintf(strValueRange, "Range [-inf, %d]", (int)Int32MaxValue);
    }
    else
    {
        if(Int32MaxValue == INT32_MAX)
            sprintf(strValueRange, "Range [%d, +inf]", (int)Int32MinValue);
        else
            sprintf(strValueRange, "Range [%d, %d]", (int)Int32MinValue, (int)Int32MaxValue);
    }
    //sprintf(strValueRange, "Range [%d, %d]", (int)Int32MinValue, (int)Int32MaxValue);
    UG_TextboxSetText(pthis_wnd, TXB_ID_ValueRange, strValueRange);
    
    vTaskResume(*pthis_xHandle);
    return &isCompletedInputValue;
}

static bool* requestInputDecimalInRange(float min, float max, float *InputValue)
{
    // Take Mutex
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if(xSemaphoreTakeFromISR(xSemaphore_InputValue, &xHigherPriorityTaskWoken) == pdFALSE)
    {
#ifdef PRINTF_DEBUG_MDOE
        printf("xSemaphoreTakeFromISR error\r\n");
#endif
        return NULL;
    }
    if (xHigherPriorityTaskWoken != pdFALSE)
        taskYIELD();
    
    isIntegerMode = false;
    pDecimalResultValue = InputValue;
    if (min < max)
    {
        DecimalMinValue = min;
        DecimalMaxValue = max;
    }
    else
    {
        DecimalMaxValue = min;
        DecimalMinValue = max;
    }
    
    /* config button Show/Hide */
    //UG_ButtonShow(pthis_wnd, BTN_ID_Decimal_Point);
    UG_ButtonSetText(pthis_wnd, BTN_ID_Decimal_Point, ".");
    UG_ButtonSetStyle(pthis_wnd, BTN_ID_Decimal_Point, BTN_STYLE_3D);
    UG_ButtonSetAlternateForeColor(pthis_wnd, BTN_ID_Decimal_Point, C_BLACK);
    
    if(DecimalMinValue < 0.0f)
    {
        needToShowMinusSign = true;
        //UG_ButtonShow(pthis_wnd, BTN_ID_Minus_Sign);
        UG_ButtonSetText(pthis_wnd, BTN_ID_Minus_Sign, "+/-");
        UG_ButtonSetStyle(pthis_wnd, BTN_ID_Minus_Sign, BTN_STYLE_3D);
        UG_ButtonSetAlternateForeColor(pthis_wnd, BTN_ID_Minus_Sign, C_BLACK);
    }
    else
    {
        needToShowMinusSign = false;
        //UG_ButtonHide(pthis_wnd, BTN_ID_Minus_Sign);
        UG_ButtonSetText(pthis_wnd, BTN_ID_Minus_Sign, "");
        UG_ButtonSetStyle(pthis_wnd, BTN_ID_Minus_Sign, BTN_STYLE_2D);
        UG_ButtonSetAlternateForeColor(pthis_wnd, BTN_ID_Minus_Sign, C_WHITE_SMOKE);
    }
    
    if (DecimalMinValue == FLT_MIN)
    {
        if (DecimalMaxValue == FLT_MAX)
            sprintf(strValueRange, "Range [-inf, +inf]");
        else
            sprintf(strValueRange, "Range [-inf, %g]", DecimalMaxValue);
    }
    else
    {
        if (DecimalMaxValue == FLT_MAX)
            sprintf(strValueRange, "Range [%g, +inf]", DecimalMinValue);
        else
            sprintf(strValueRange, "Range [%g, %g]", DecimalMinValue, DecimalMaxValue);
    }
    //sprintf(strValueRange, "Range [%g, %g]", DecimalMinValue, DecimalMaxValue);
    UG_TextboxSetText(pthis_wnd, TXB_ID_ValueRange, strValueRange);
    
    vTaskResume(*pthis_xHandle);
    return &isCompletedInputValue;
}

static bool hasUpdatedInputValue(void)
{
    return hasUpdated;
}

/***************************************************************END OF FILE****/
