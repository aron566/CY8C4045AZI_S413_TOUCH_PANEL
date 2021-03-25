/**
 *  @file User_Main.c
 *
 *  @date 2021-01-11
 *
 *  @author aron566
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief 主线任务
 *
 *  @details 1、
 *
 *  @version V1.0
 */
/** Includes -----------------------------------------------------------------*/
#include <project.h>
/* Private includes ----------------------------------------------------------*/
#include "User_Main.h"
#include "Protocol.h"
#include "Uart_Port.h"
#include "Timer_Port.h"
#include "CapSense_Port.h"
#include "PressKey_Port.h"
/* Use C compiler ----------------------------------------------------------*/
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Private typedef ----------------------------------------------------------*/
/** Private macros -----------------------------------------------------------*/                                                          
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/

/** Private function prototypes ----------------------------------------------*/

/** Private user code --------------------------------------------------------*/

/** Private application code -------------------------------------------------*/
/*******************************************************************************
*
*       Static code
*
********************************************************************************
*/

/** Public application code --------------------------------------------------*/
/*******************************************************************************
*
*       Public code
*
********************************************************************************
*/

/**
  ******************************************************************
  * @brief   任务处理循环
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void User_Main_Task_Process_Loop(void)
{
  for(;;)
  {
    /*启动协议栈*/
    Protocol_Start();
    
    /*定时器模块启动*/
    Timer_Port_Start();
    
    /*触碰按键检测*/
    CapSense_Port_Start();
    
#if USE_DEBUG_PRESSKEY
/*按键检测事件调试*/
    PressKey_Port_Start();
#endif
  }
  /*never return*/
}

/**
  ******************************************************************
  * @brief   初始化各模块功能
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void User_Main_Task_Init(void)
{
  /*触摸检测初始化*/
  CapSense_Port_Init();
  
  /*串口操作初始化*/
  Uart_Port_Init();
  
  /*定时器初始化*/
  Timer_Port_Init();
  
  /*协议栈初始化*/
  Protocol_Init();
  
  /*按钮按键初始化*/
  PressKey_Port_Init();
  
  /*other initialization task code*/
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
