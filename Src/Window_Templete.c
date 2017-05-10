/* Include user header files -------------------------------------------------*/
#include "Window_Templete.h"

/* Imported variables --------------------------------------------------------*/
/* Private function macro ----------------------------------------------------*/
/* Private enum tag ----------------------------------------------------------*/
static enum BTN_ID_TAG
{
    BTN_ID_Close = BTN_ID_0
};

static enum TXB_ID_TAG
{
    TXB_ID_Test = TXB_ID_0
};

/* Private variables ---------------------------------------------------------*/
/* uGUI */
static UG_OBJECT    obj_this_wnd[TEMPLETE_uGUI_OBJECTS_NUM];
static UG_BUTTON    btn_Test;
static UG_TEXTBOX   txb_Test;

/* thread control */
static bool needFinalize;  // This flag is used in "WindowControlThread" and "window_callback" function

/* "this" pointer */
static TaskHandle_t* pthis_xHandle = &xHandle_MainMenu;
static UG_WINDOW*    pthis_wnd     = &wnd_Templete;

/* Private function prototypes -----------------------------------------------*/
static void WindowControlThread(void const *argument);
static void window_callback( UG_MESSAGE* msg );
static void initialize(void);
static void execute(void);
static void draw(void);
static void finalize(void);

/* Exported functions --------------------------------------------------------*/
/* ---------------------------------------------------------------- */
/* -- Create Window                                              -- */
/* ---------------------------------------------------------------- */
void createMainMenuWindow(void)
{
	UG_U8 id_buf;

    UG_WindowCreate(pthis_wnd, obj_this_wnd, COUNTOF(obj_this_wnd), window_callback);
    UG_WindowSetTitleText(pthis_wnd, MAINMENU_WINDOW_TITLE);
    UG_WindowSetTitleTextFont(pthis_wnd, &FONT_12X20);
	
    // Create buttons
    id_buf = BTN_ID_Close;
    UG_ButtonCreate(pthis_wnd, &btn_Test, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 0 / 2 + MAINMENU_BUTTON_GAP,
            UG_WindowGetInnerHeight(pthis_wnd) * 1 / 4 + MAINMENU_BUTTON_GAP,
            UG_WindowGetInnerWidth(pthis_wnd)  * 1 / 2 - MAINMENU_BUTTON_GAP,
            UG_WindowGetInnerHeight(pthis_wnd) * 2 / 4 - MAINMENU_BUTTON_GAP );
    UG_ButtonSetFont(pthis_wnd, id_buf, &FONT_12X20);
    UG_ButtonSetText(pthis_wnd, id_buf, "Close");
	
    // Create textboxes
    id_buf = TXB_ID_Test;
    UG_TextboxCreate(pthis_wnd, &txb_Test, id_buf,
            UG_WindowGetInnerWidth(pthis_wnd)  * 0 / 2 + 0,
            UG_WindowGetInnerHeight(pthis_wnd) * 0 / 4 + 0,
            UG_WindowGetInnerWidth(pthis_wnd)  * 1 / 2 - 0,
            UG_WindowGetInnerHeight(pthis_wnd) * 1 / 4 - 0 );
    UG_TextboxSetFont(pthis_wnd, id_buf, &FONT_12X20);
    UG_TextboxSetText(pthis_wnd, id_buf, "Hello world!");
    UG_TextboxSetAlignment(pthis_wnd, id_buf, ALIGN_CENTER_LEFT);
    
	//UG_WindowShow(pthis_wnd);
    
    xTaskCreate( (TaskFunction_t)WindowControlThread, "TempleteTask",
    		configMINIMAL_STACK_SIZE+100, NULL, Priority_High, pthis_xHandle);
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
    //portTickType xLastWakeTime = xTaskGetTickCount();

    bool shouldSuspend  = true;     /* Set this flag "false" if this thread state must not be suspended */
    bool needInitialize = false;
    	 needFinalize   = false;    /* only "needFinalize" flag is changed from "window_callback" function */
	
    while (1)
	{
		/* Thread flow */
		//vTaskDelayUntil(&xLastWakeTime, TEMPELTE_UPDATE_MS);
		vTaskDelay(TEMPLETE_UPDATE_MS);
		
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

        execute();
        draw();

        if(needFinalize)
        {
        	finalize();
        	needFinalize  = false;
        	shouldSuspend = true;
        }
    }
}

/* ---------------------------------------------------------------- */
/* -- Callback function ( called from "UG_Update()" )            -- */
/* ---------------------------------------------------------------- */
void window_callback(UG_MESSAGE* msg)
{
    if (msg->type == MSG_TYPE_OBJECT)
    {
        if (msg->id == OBJ_TYPE_BUTTON)
        {
            switch (msg->sub_id)
            {
            case BTN_ID_Close:
#ifdef PRINTF_DEBUG_MDOE
                printf("Pushed Close button\r\n");
#endif
                needFinalize = true;  // <- Finalize and make "WindowControlThread()" state susupend
                //UG_WindowShow(&wnd_Templete2);
                break;
                
            default:
#ifdef PRINTF_DEBUG_MDOE
                printf("%s thread callback error\r\n", pcTaskGetName(*pthis_xHandle) );
#endif
                break;
            }
        }
    }
}


/* ---------------------------------------------------------------- */
/* -- Initialize                                                 -- */
/* ---------------------------------------------------------------- */
static void initialize(void)
{
	/* Variables Initialization */
    
    /* Show this window */
    UG_WindowShow(pthis_wnd);
}

/* ---------------------------------------------------------------- */
/* -- Execute                                                    -- */
/* ---------------------------------------------------------------- */
static void execute(void)
{
    
}

/* ---------------------------------------------------------------- */
/* -- Draw                                                       -- */
/* ---------------------------------------------------------------- */
static void draw(void)
{
    
}

/* ---------------------------------------------------------------- */
/* -- Finalize                                                   -- */
/* ---------------------------------------------------------------- */
static void finalize(void)
{
    /* Variables Finalization */
}

/***************************************************************END OF FILE****/
