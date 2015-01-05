/*
*********************************************************************************************************
*	                                  
*	ģ������ : ������
*	�ļ����� : main.c
*	��    �� : V2.0
*	˵    �� : FreeRTOS�����̳�
*	�޸ļ�¼ :
*		�汾��   ����        ����             ˵��
*		v1.0    2013-09-10  Eric2013  �������ļ���ST�̼���汾ΪV1.0.2
*		V2.0    2013-02-24  Eric2013  ����FreeRTOS��V8.0.0������F4�̼��⵽1.3.0 
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
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
											��������
**********************************************************************************************************
*/
static void AppTaskCreate (void);
static void AppTaskObject (void);
extern void vSetupTimerTest( void );
/*
**********************************************************************************************************
											��������
**********************************************************************************************************
*/
xTaskHandle xHandleTask4;
xQueueHandle xQueue;

xSemaphoreHandle xSemaphore;
/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: ��׼c������ڡ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int main(void)
{
    /* Ӳ����ʼ�� */
    bsp_Init();
	  
	/* �������� */
	AppTaskCreate();

	/* ����*/
	AppTaskObject();
	
    /* �������ȣ���ʼִ������ */
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
*	�� �� ��: vTask1
*	����˵��: LED��˸		
*	��    �Σ�pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
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
*	�� �� ��: vTask2
*	����˵��: LED��˸		
*	��    �Σ�pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
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
		/* RA8875 ����*/
		if (g_ChipID == IC_8875)
		{
		    /* ��Դ�����־ */
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
*	�� �� ��: vTask3
*	����˵��: LED��˸		
*	��    �Σ�pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
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
*	�� �� ��: vTask4
*	����˵��: LED��˸		
*	��    �Σ�pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void vTask4( void *pvParameters )
{
    uint8_t		Pic_Name = 0;
	char buf[20];
    
    while(1)
    {
		/* ��ֵ�ź�������һ���ǿ��õ� */
        xSemaphoreTake( xSemaphore, portMAX_DELAY);
        sprintf(buf,"0:/Picture/%d.bmp",Pic_Name);
        GUI_SaveBMP(0, 0, LCD_GetXSize(), LCD_GetYSize(),buf);
        Pic_Name++; 		
    }
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskCreate
*	����˵��: ����Ӧ������
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: AppTaskObject
*	����˵��: ����Ӧ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AppTaskObject (void)
{ 
   vSemaphoreCreateBinary( xSemaphore );
    
   /* ��ֵ�ź�������һ���ǿ��õ� */
   xSemaphoreTake( xSemaphore, portMAX_DELAY);
}

