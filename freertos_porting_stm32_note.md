FreeRTOS移植到STM32F103笔记
## FreeRTOS特点
## FreeRTOS移植
### 步骤
+ portmacro.h
首先是将一些数据类型定义为port开头的类型，而不是使用C语言自带的类型名。然后是定义一些硬件相关的宏：
~~~
portBYTE_ALIGNMENT //貌似是分配任务堆栈空间用的宏定义
portSTACK_GROWTH   //定义堆栈生长的方向，一般是向下生长的 定义为-1
portTICK_RATE_MS   //这个是在用户程序中能用到的 表示Tick间隔多少ms
portYIELD()        //实现任务切换
portNOP()           //空操作
portENTER_CRITICAL()  //进入临界区
portEXIT_CRITICAL()  //退出临界区
portENABLE_INTERRUPTS() //开中断
portDISABLE_INTERRUPTS() //关中断
~~~
+ port.c
port.c文件里面就是实现了上面的头文件中用的的几个函数：
~~~
pxPortInitialiseStack()
xPortStartScheduler()
vPortEndScheduler()
vPortYield()
vPortTickInterrupt()
~~~
定义了几个全局变量：
~~~
/* The priority used by the kernel is assigned to a variable to make access
from inline assembler easier. */
const unsigned long ulKernelPriority = configKERNEL_INTERRUPT_PRIORITY;
/* Each task maintains its own interrupt status in the critical nesting
variable. */
static unsigned portBASE_TYPE uxCriticalNesting = 0xaaaaaaaa;
~~~
+ FreeRTOSConfig.h
FreeRTOS的全局配置文件
~~~
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H
/* 是否采用抢占式调度器 */
#define configUSE_PREEMPTION            1   
/* 是否使用空闲任务 */
#define configUSE_IDLE_HOOK             0   
/* 是否使用心跳钩子函数 */
#define configUSE_TICK_HOOK             0   
/* 定义MCU内核工作频率 */
#define configCPU_CLOCK_HZ              ( 100000000UL )
/* 时钟Tick的频率 */
#define configTICK_RATE_HZ              ( ( portTickType ) 1000 )
/* 程序中可以使用的最大优先级 */
#define configMAX_PRIORITIES            ( ( unsigned portBASE_TYPE ) 5 )
/* 任务堆栈的最小大小 */
#define configMINIMAL_STACK_SIZE        ( ( unsigned short ) 90 )
/* 设置堆空间的大小，只有当程序中采用FreeRTOS提供的内存分配算法才用到 */
#define configTOTAL_HEAP_SIZE           ( ( size_t ) ( 8 * 1024 ) )
/* 任务名称最大长度，包括末尾的NULL结束字节 */
#define configMAX_TASK_NAME_LEN         ( 10 )
/* 如果要使用TRACE功能 */
#define configUSE_TRACE_FACILITY        0
/* 设为1 portTickType将定义为无符号16整型，否则为无符号32位整型 */
#define configUSE_16_BIT_TICKS          0
/*这个参数控制那些优先级与idle 任务相同的任务的行为，并且只有当内核被配置为抢占式任务调度时才有实际作用。
 * 内核对具有同样优先级的任务会采用时间片轮转调度算法。当任务的优先级高于idle任务时，各个任务分到的时间片是同样大小的。
 * 但当任务的优先级与idle任务相同时情况就有些不同了。当configIDLE_SHOULD_YIELD 被配置为1时，当任何优先级与idle 任务相同的任务处于就绪态时，idle任务会立刻要求调度器进行任务切换。这会使idle任务占用最少的CPU时间，但同时会使得优先级与idle 任务相同的任务获得的时间片不是同样大小的。因为idle任务会占用某个任务的部分时间片  */
#define configIDLE_SHOULD_YIELD         0
/* 程序中是否包含mutex相关代码 */
#define configUSE_MUTEXES               1
/* 队列注册表有两个作用，但是这两个作用都依赖于调试器的支持：
 * 1.        给队列一个名字，方便调试时辨认是哪个队列。
 * 2.        包含调试器需要的特定信息用来定位队列和信号量。
 * 如果你的调试器没有上述功能，哪个这个注册表就毫无用处，还占用的宝贵的RAM空间 */
#define configQUEUE_REGISTRY_SIZE       8
/* 是否检测堆栈溢出 */
#define configCHECK_FOR_STACK_OVERFLOW  2
/* 是否包含recursive mutex代码 */
#define configUSE_RECURSIVE_MUTEXES     1
#define configUSE_MALLOC_FAILED_HOOK    0
#define configUSE_APPLICATION_TASK_TAG  0
#define configUSE_COUNTING_SEMAPHORES   1
/* Co-routine definitions. */
/* 是否使用协程 */
#define configUSE_CO_ROUTINES       0
/* 协程可以使用的优先级数量 */
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )
/* Software timer definitions. */
/* 是否包含软件定时器 */
#define configUSE_TIMERS                1
/* 软件定时器任务优先级 */
#define configTIMER_TASK_PRIORITY       ( 2 )
/* 设置软件定时器任务中用到的命令队列长度 */
#define configTIMER_QUEUE_LENGTH        10
/* 设置软件定时器任务所需任务堆栈大小 */
#define configTIMER_TASK_STACK_DEPTH    ( configMINIMAL_STACK_SIZE * 2 )
/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet        1
#define INCLUDE_uxTaskPriorityGet       1
#define INCLUDE_vTaskDelete             1
#define INCLUDE_vTaskCleanUpResources   1
#define INCLUDE_vTaskSuspend            1
#define INCLUDE_vTaskDelayUntil         1
#define INCLUDE_vTaskDelay              1
/* Run time stats gathering definitions. */
#ifdef __ICCARM__
    /* The #ifdef just prevents this C specific syntax from being included in
    assembly files. */
    void vMainConfigureTimerForRunTimeStats( void );
    unsigned long ulMainGetRunTimeCounterValue( void );
#endif
#define configGENERATE_RUN_TIME_STATS   1
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() vMainConfigureTimerForRunTimeStats()
#define portGET_RUN_TIME_COUNTER_VALUE() ulMainGetRunTimeCounterValue()
/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
    /* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
    #define configPRIO_BITS             __NVIC_PRIO_BITS
#else
    #define configPRIO_BITS             4        /* 15 priority levels */
#endif
/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY         0xf
/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY    5
/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
/* 决定内核使用的优先级 */
#define configKERNEL_INTERRUPT_PRIORITY         ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
/*决定了可以调用API函数的中断的最高优先级。高于这个值的中断处理函数不能调用任何API 函数。 */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); } 
/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler         SVC_Handler
#define xPortPendSVHandler      PendSV_Handler
#define xPortSysTickHandler     SysTick_Handler
~~~