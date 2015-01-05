/*
*********************************************************************************************************
*	                                  
*	模块名称 : 主函数
*	文件名称 : main.c
*	版    本 : V0.2
*	说    明 : FreeRTOS基础教程
*	修改记录 :
*		版本号   日期        作者             说明
*		v0.1    2013-09-10  astiger  创建该文件，ST固件库版本为V3.5.0
*		V0.2    2013-02-24  astiger  升级FreeRTOS到V8.1.2，升级F1固件库到3.5.0 
*   安富莱
*	Copyright (C), 2013-2014, 
*
*********************************************************************************************************
*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "bsp.h"
#include "stdio.h"
#include "MainTask.h"
#include "semphr.h"


/*
**********************************************************************************************************
											函数声明
**********************************************************************************************************
*/
static void AppTaskCreate (void);
static void AppTaskObject (void);
extern void vSetupTimerTest( void );
/*
**********************************************************************************************************
											变量声明
**********************************************************************************************************
*/
xTaskHandle xHandleTask4;
xQueueHandle xQueue;

xSemaphoreHandle xSemaphore;
/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
int main(void)
{
    /* 硬件初始化 */
    bsp_Init();
	  
	/* 创建任务 */
	AppTaskCreate();

	/* 创建*/
	AppTaskObject();
	
    /* 启动调度，开始执行任务 */
    vTaskStartScheduler();

    /* 
		   If all is well we will never reach here as the scheduler will now be
	   running.  If we do reach here then it is likely that there was insufficient
	   heap available for the idle task to be created. 
		*/
    while (1)
    {
    } 
}

/*
*********************************************************************************************************
*	函 数 名: vTask1
*	功能说明: LED闪烁		
*	形    参：pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*********************************************************************************************************
*/
void vTask1( void *pvParameters )
{
	
    while(1)
    {
		MainTask();			
	}
}

/*
*********************************************************************************************************
*	函 数 名: vTask2
*	功能说明: LED闪烁		
*	形    参：pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*********************************************************************************************************
*/
extern __IO uint8_t s_ucRA8875BusyNow;
void vTask2( void *pvParameters )
{
	portTickType xLastWakeTime;
    const portTickType xFrequency = 10;

	 // Initialise the xLastWakeTime variable with the current time.
     xLastWakeTime = xTaskGetTickCount();
	
	while (1) 
	{
		/* RA8875 触摸*/
		if (g_ChipID == IC_8875)
		{
		    /* 资源共享标志 */
			if(s_ucRA8875BusyNow == 0)
			{
				GUI_TOUCH_Exec();	
			}
		}
		/* XPT2046 */
		else
		{
			GUI_TOUCH_Exec();	
		}
    
		// Wait for the next cycle.
        vTaskDelayUntil( &xLastWakeTime, xFrequency );					 
	}
}

/*
*********************************************************************************************************
*	函 数 名: vTask3
*	功能说明: LED闪烁		
*	形    参：pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*********************************************************************************************************
*/
void vTask3( void *pvParameters )
{   
    uint8_t uKeyCode;
    
    while(1)
    {
		bsp_KeyScan();
	    
		uKeyCode = bsp_GetKey();
		if(uKeyCode != KEY_NONE)
		{
			if(uKeyCode == KEY_1_DOWN)
			{
                 xSemaphoreGive( xSemaphore );
			}
		}
        
        bsp_LedToggle(2);
        vTaskDelay( 20 );
    }
}

/*
*********************************************************************************************************
*	函 数 名: vTask4
*	功能说明: LED闪烁		
*	形    参：pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*********************************************************************************************************
*/
void vTask4( void *pvParameters )
{
    uint8_t		Pic_Name = 0;
	char buf[20];
    
    while(1)
    {
		/* 二值信号量，第一次是可用的 */
        xSemaphoreTake( xSemaphore, portMAX_DELAY);
        sprintf(buf,"0:/Picture/%d.bmp",Pic_Name);
        GUI_SaveBMP(0, 0, LCD_GetXSize(), LCD_GetYSize(),buf);
        Pic_Name++; 		
    }
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
	/* Create one task. */
    xTaskCreate(    vTask1,     /* Pointer to the function that implements the task.              */
                    "Task 1",   /* Text name for the task.  This is to facilitate debugging only. */
                    500,        /* Stack depth in words.                                          */
                    NULL,       /* We are not using the task parameter.                           */
                    1,          /* This task will run at priority 1.                              */
                    NULL );     /* We are not using the task handle.                              */

    /* Create one task. */
    xTaskCreate(    vTask2,     /* Pointer to the function that implements the task. */
                    "Task 2",   /* Text name for the task.  This is to facilitate debugging only. */
                    500,        /* Stack depth in words.                                          */
                    NULL,       /* We are not using the task parameter.                           */
                    2,          /* This task will run at priority 2.                              */
                    NULL );     /* We are not using the task handle.                              */
	
	 /* Create one task. */
    xTaskCreate(    vTask3,     /* Pointer to the function that implements the task. */
                    "Task 3",   /* Text name for the task.  This is to facilitate debugging only. */
                    500,        /* Stack depth in words.                                          */
                    NULL,       /* We are not using the task parameter.                           */
                    3,          /* This task will run at priority 2.                              */
                    NULL);    
	
	/* Create one task. */
    xTaskCreate(    vTask4,     /* Pointer to the function that implements the task. */
                    "Task 4",   /* Text name for the task.  This is to facilitate debugging only. */
                    500,        /* Stack depth in words.                                          */
                    NULL,       /* We are not using the task parameter.                           */
                    4,          /* This task will run at priority 2.                              */
                    &xHandleTask4 );     /* We are not using the task handle.                              */				
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskObject
*	功能说明: 创建应用任务
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskObject (void)
{ 
   vSemaphoreCreateBinary( xSemaphore );
    
   /* 二值信号量，第一次是可用的 */
   xSemaphoreTake( xSemaphore, portMAX_DELAY);
}

