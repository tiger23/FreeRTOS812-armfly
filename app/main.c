/*
*********************************************************************************************************
*
*   模块名称 : 主函数
*   文件名称 : main.c
*   版    本 : V0.2
*   说    明 : FreeRTOS基础教程
*   修改记录 :
*       版本号   日期        作者             说明
*       v0.1    2013-09-10  astiger  创建该文件，ST固件库版本为V3.5.0
*       V0.2    2013-02-24  astiger  升级FreeRTOS到V8.1.2，升级F1固件库到3.5.0
*   安富莱F2板
*   Copyright (C), 2013-2014,
*
*********************************************************************************************************
*/
/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "FreeRTOSConfig.h"
/* Library includes. */
#include "stm32f10x_conf.h"
#include "stm32f10x_it.h"

/* Demo app includes. */
#include "lcd.h"
#include "led.h"
#include "LCD_Message.h"
#include "BlockQ.h"
#include "death.h"
#include "integer.h"
#include "blocktim.h"
#include "partest.h"
#include "semtest.h"
#include "PollQ.h"
#include "flash.h"
#include "comtest2.h"

#include "croutine.h"
#include "board.h"
#include "stdio.h"
//#include "MainTask.h"
#include "semphr.h"

/* Task priorities. */
#define mainQUEUE_POLL_PRIORITY             ( tskIDLE_PRIORITY + 2 )
#define mainCHECK_TASK_PRIORITY             ( tskIDLE_PRIORITY + 3 )
#define mainSEM_TEST_PRIORITY               ( tskIDLE_PRIORITY + 1 )
#define mainBLOCK_Q_PRIORITY                ( tskIDLE_PRIORITY + 2 )
#define mainCREATOR_TASK_PRIORITY           ( tskIDLE_PRIORITY + 3 )
#define mainFLASH_TASK_PRIORITY             ( tskIDLE_PRIORITY + 1 )
#define mainCOM_TEST_PRIORITY               ( tskIDLE_PRIORITY + 1 )
#define mainINTEGER_TASK_PRIORITY           ( tskIDLE_PRIORITY )

/* Constants related to the LCD. */
#define mainMAX_LINE                        ( 240 )
#define mainROW_INCREMENT                   ( 24 )
#define mainMAX_COLUMN                      ( 20 )
#define mainCOLUMN_START                    ( 399 )
#define mainCOLUMN_INCREMENT                ( 16 )

/* The maximum number of message that can be waiting for display at any one
time. */
#define mainLCD_QUEUE_SIZE                  ( 3 )

/* The check task uses the sprintf function so requires a little more stack. */
#define mainCHECK_TASK_STACK_SIZE           ( configMINIMAL_STACK_SIZE + 50 )

/* Dimensions the buffer into which the jitter time is written. */
#define mainMAX_MSG_LEN                     25

/* The time between cycles of the 'check' task. */
#define mainCHECK_DELAY                     ( ( TickType_t ) 5000 / portTICK_PERIOD_MS )

/* The number of nano seconds between each processor clock. */
#define mainNS_PER_CLOCK ( ( unsigned long ) ( ( 1.0 / ( double ) configCPU_CLOCK_HZ ) * 1000000000.0 ) )

/* Baud rate used by the comtest tasks. */
#define mainCOM_TEST_BAUD_RATE      ( 115200 )

/* The LED used by the comtest tasks. See the comtest.c file for more
information. */
#define mainCOM_TEST_LED            ( 3 )

/*-----------------------------------------------------------*/

/*
 * Configure the clocks, GPIO and other peripherals as required by the demo.
 */
static void prvSetupHardware(void);

/*
 * Configure the LCD as required by the demo.
 */
static void prvConfigureLCD(void);

/*
 * The LCD is written two by more than one task so is controlled by a
 * 'gatekeeper' task.  This is the only task that is actually permitted to
 * access the LCD directly.  Other tasks wanting to display a message send
 * the message to the gatekeeper.
 */
static void vLCDTask(void *pvParameters);

static void vLEDTask(void *pvParameters);

/*
 * Retargets the C library printf function to the USART.
 */
int fputc(int ch, FILE *f);

/*
 * Checks the status of all the demo tasks then prints a message to the
 * display.  The message will be either PASS - and include in brackets the
 * maximum measured jitter time (as described at the to of the file), or a
 * message that describes which of the standard demo tasks an error has been
 * discovered in.
 *
 * Messages are not written directly to the terminal, but passed to vLCDTask
 * via a queue.
 */
static void vCheckTask(void *pvParameters);

/*
 * Configures the timers and interrupts for the fast interrupt test as
 * described at the top of this file.
 */
extern void vSetupTimerTest(void);

/*-----------------------------------------------------------*/

/* The queue used to send messages to the LCD task. */
QueueHandle_t xLCDQueue;

/*-----------------------------------------------------------*/

int main(void)
{


    prvSetupHardware();

    /* Create the queue used by the LCD task.  Messages for display on the LCD
    are received via this queue. */
    xLCDQueue = xQueueCreate(mainLCD_QUEUE_SIZE, sizeof(xLCDMessage));

    /* Start the standard demo tasks. */
//    vStartBlockingQueueTasks(mainBLOCK_Q_PRIORITY);
//    vCreateBlockTimeTasks();
//    vStartSemaphoreTasks(mainSEM_TEST_PRIORITY);
//    vStartPolledQueueTasks(mainQUEUE_POLL_PRIORITY);
//    vStartIntegerMathTasks(mainINTEGER_TASK_PRIORITY);
//    vStartLEDFlashTasks(mainFLASH_TASK_PRIORITY);
//    vAltStartComTestTasks(mainCOM_TEST_PRIORITY, mainCOM_TEST_BAUD_RATE, mainCOM_TEST_LED);

    /* Start the tasks defined within this file/specific to this demo. */
    //xTaskCreate(vCheckTask, "Check", mainCHECK_TASK_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL);
    //xTaskCreate(vLCDTask, "LCD", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vLEDTask, "LED", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+4, NULL);
    /* The suicide tasks must be created last as they need to know how many
    tasks were running prior to their creation in order to ascertain whether
    or not the correct/expected number of tasks are running at any given time. */
    //vCreateSuicidalTasks(mainCREATOR_TASK_PRIORITY);

    /* Configure the timers used by the fast interrupt timer test. */
    //vSetupTimerTest();

    /* Start the scheduler. */
    vTaskStartScheduler();

    /* Will only get here if there was not enough heap space to create the
    idle task. */
    return 0;
}
/*-----------------------------------------------------------*/

void vLCDTask(void *pvParameters)
{
    xLCDMessage xMessage;

    /* Initialise the LCD and display a startup message. */
    prvConfigureLCD();
    LCD_DrawMonoPict((uint32_t *) pcBitmap);

    for (;;)
    {
        /* Wait for a message to arrive that requires displaying. */
        while (xQueueReceive(xLCDQueue, &xMessage, portMAX_DELAY) != pdPASS);

        /* Display the message.  Print each message to a different position. */
        printf((char const *) xMessage.pcMessage);
    }
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

void vLEDTask(void *pvParameters)
{
    xLCDMessage xMessage;

    /* Initialise the LED and display a startup message. */
    prvConfigureLED();
    

    for (;;)
    {
        hw_led_on(1);
		vTaskDelay(10 / portTICK_PERIOD_MS);   /* Delay 10 ms */
		hw_led_off(1);
    }
}
/*-----------------------------------------------------------*/


static void vCheckTask(void *pvParameters)
{
    TickType_t xLastExecutionTime;
    xLCDMessage xMessage;
    static signed char cPassMessage[ mainMAX_MSG_LEN ];
    extern unsigned short usMaxJitter;

    xLastExecutionTime = xTaskGetTickCount();
    xMessage.pcMessage = cPassMessage;

    for (;;)
    {
        /* Perform this check every mainCHECK_DELAY milliseconds. */
        vTaskDelayUntil(&xLastExecutionTime, mainCHECK_DELAY);

        /* Has an error been found in any task? */

        if (xAreBlockingQueuesStillRunning() != pdTRUE)
        {
            xMessage.pcMessage = "ERROR IN BLOCK Q\n";
        }
        else if (xAreBlockTimeTestTasksStillRunning() != pdTRUE)
        {
            xMessage.pcMessage = "ERROR IN BLOCK TIME\n";
        }
        else if (xAreSemaphoreTasksStillRunning() != pdTRUE)
        {
            xMessage.pcMessage = "ERROR IN SEMAPHORE\n";
        }
        else if (xArePollingQueuesStillRunning() != pdTRUE)
        {
            xMessage.pcMessage = "ERROR IN POLL Q\n";
        }
        else if (xIsCreateTaskStillRunning() != pdTRUE)
        {
            xMessage.pcMessage = "ERROR IN CREATE\n";
        }
        else if (xAreIntegerMathsTaskStillRunning() != pdTRUE)
        {
            xMessage.pcMessage = "ERROR IN MATH\n";
        }
        else if (xAreComTestTasksStillRunning() != pdTRUE)
        {
            xMessage.pcMessage = "ERROR IN COM TEST\n";
        }
        /* XPT2046 */
        else
        {
            sprintf((char *) cPassMessage, "PASS [%uns]\n", ((unsigned long) usMaxJitter) * mainNS_PER_CLOCK);
        }

        /* Send the message to the LCD gatekeeper for display. */
        xQueueSend(xLCDQueue, &xMessage, portMAX_DELAY);
    }
}
/*-----------------------------------------------------------*/

static void prvSetupHardware(void)
{
    /* Start with the clocks in their expected state. */
    RCC_DeInit();

    /* Enable HSE (high speed external clock). */
    RCC_HSEConfig(RCC_HSE_ON);

    /* Wait till HSE is ready. */
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
    {
    }

    /* 2 wait states required on the flash. */
    *((unsigned long *) 0x40022000) = 0x02;

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 9 = 72 MHz. */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL. */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready. */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source. */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source. */
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }

    /* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE and AFIO clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC
                           | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_AFIO, ENABLE);

    /* SPI2 Periph clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);


    /* Set the Vector Table base address at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    /* Configure HCLK clock as SysTick clock source. */
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

    vParTestInitialise();
}
/*-----------------------------------------------------------*/

static void prvConfigureLCD(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure LCD Back Light (PB1) as output push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Set the Backlight Pin */
    GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET);

    /* Initialize the LCD */
    LCD_Init();

    /* Set the Back Color */
    LCD_SetBackColor(White);

    /* Set the Text Color */
    LCD_SetTextColor(0x051F);

    LCD_Clear();
}
/*-----------------------------------------------------------*/

int fputc(int ch, FILE *f)
{
    static unsigned short usColumn = 0, usRefColumn = mainCOLUMN_START;
    static unsigned char ucLine = 0;

    if ((usColumn == 0) && (ucLine == 0))
    {
        LCD_Clear();
    }

    if (ch != '\n')
    {
        /* Display one character on LCD */
        LCD_DisplayChar(ucLine, usRefColumn, (uint8_t) ch);

        /* Decrement the column position by 16 */
        usRefColumn -= mainCOLUMN_INCREMENT;

        /* Increment the character counter */
        usColumn++;
        if (usColumn == mainMAX_COLUMN)
        {
            ucLine += mainROW_INCREMENT;
            usRefColumn = mainCOLUMN_START;
            usColumn = 0;
        }
    }
    else
    {
        /* Move back to the first column of the next line. */
        ucLine += mainROW_INCREMENT;
        usRefColumn = mainCOLUMN_START;
        usColumn = 0;
    }

    /* Wrap back to the top of the display. */
    if (ucLine >= mainMAX_LINE)
    {
        ucLine = 0;
    }

    return ch;
}
/*-----------------------------------------------------------*/

#ifdef USE_FULL_ASSERT
/* Keep the linker happy. */
void assert_failed(uint8_t *pcFile, uint32_t ulLine)
{
    for (;;)
    {
    }
}
#endif
