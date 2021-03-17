/**
 *  @file PressKey_Port.c
 *
 *  @date 2021-03-17
 *
 *  @author aron566
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief 按键接口
 *
 *  @details 1、
 *
 *  @version V1.0
 */
/** Includes -----------------------------------------------------------------*/
#include <project.h>
/* Private includes ----------------------------------------------------------*/
#include "PressKey_Port.h"
#include "Utilities_Multi_Button.h"
#include "Parameter.h"
#include "utilities.h"
#include "CapSense_Port.h"
/** Use C compiler -----------------------------------------------------------*/
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
/*按键句柄*/
struct Button Power_key_Handle;
struct Button Set_Mode_Key_Handle;
struct Button Vol_Decrease_Key_Handle;
struct Button Vol_Increase_Key_Handle;
struct Button Touch_Key_Handle;

/** Private function prototypes ----------------------------------------------*/

/** Private user code --------------------------------------------------------*/

/** Private application code -------------------------------------------------*/
/*******************************************************************************
*
*       Static code
*
********************************************************************************
*/

/**
  ******************************************************************
  * @brief   设置算法功能按键触发
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-17
  ******************************************************************
  */
static void Set_Mode_Key_SINGLE_Click_Handler(void* btn)
{
  UNUSED(btn);
  Parameter_Change_Dev_Function();
}

/**
  ******************************************************************
  * @brief   设置音量减按键触发
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-17
  ******************************************************************
  */
static void Vol_Decrease_Key_SINGLE_Click_Handler(void* btn)
{
  UNUSED(btn);
  Parameter_Decrease_Vol();
}

/**
  ******************************************************************
  * @brief   设置音量增按键触发
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-17
  ******************************************************************
  */
static void Vol_Increase_Key_SINGLE_Click_Handler(void* btn)
{
  UNUSED(btn);
  Parameter_Increase_Vol();
}

/**
  ******************************************************************
  * @brief   设置禁触按键触发
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-17
  ******************************************************************
  */
static void Power_key_SINGLE_Click_Handler(void* btn)
{
  UNUSED(btn);
  /*禁止触发按键功能-关机按键除外*/
  button_stop(&Set_Mode_Key_Handle);
  button_stop(&Vol_Decrease_Key_Handle);
  button_stop(&Vol_Increase_Key_Handle);
  button_stop(&Touch_Key_Handle);
}

/**
  ******************************************************************
  * @brief   设置允许按键触发
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-17
  ******************************************************************
  */
static void Power_key_DOUBLE_Click_Handler(void* btn)
{
  UNUSED(btn);
  /*允许触发按键功能*/
  button_start(&Set_Mode_Key_Handle);
  button_start(&Vol_Decrease_Key_Handle);
  button_start(&Vol_Increase_Key_Handle);
  button_start(&Touch_Key_Handle);
}

/**
  ******************************************************************
  * @brief   设置开关电源按键触发
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-17
  ******************************************************************
  */
static void Power_key_LONG_PRESS_START_Handler(void* btn)
{
  UNUSED(btn);
  /*开关设备电源*/
  static uint8 State_Store = 0;
  State_Store = ~State_Store;
  if(State_Store == 0)
  {
    /*开机*/
    
  }
  else
  {
    /*关机*/
    
  }
}

/**
  ******************************************************************
  * @brief   触摸按键触发
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-17
  ******************************************************************
  */
static void Touch_Key_SINGLE_Click_Handler(void* btn)
{
  UNUSED(btn);
  /*读取位置信息*/
  uint32_t Postion = CapSense_Port_Get_Touch_Postion();
  float Postion_f = (float)(Postion*359)/100;
  Protocol_Set_Dev_BF_Angle((uint16)Postion_f);
}

/**
  ******************************************************************
  * @brief   触摸按键状态读取
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-17
  ******************************************************************
  */
static uint8_t Touch_Key_Read(void)
{
  return CapSense_Port_Get_Touch_State();
}

/** Public application code --------------------------------------------------*/
/*******************************************************************************
*
*       Public code
*
********************************************************************************
*/

/**
  ******************************************************************
  * @brief   按键检测初始化
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-10
  ******************************************************************
  */
void PressKey_Port_Init(void)
{
  /*初始化按键句柄,设定激活状态值0*/
  button_init(&Power_key_Handle, Power_ON_OFF_Read, 0);
  button_init(&Set_Mode_Key_Handle, Set_Mode_Read, 0);
  button_init(&Vol_Decrease_Key_Handle, Vol_Decrease_Read, 0);
  button_init(&Vol_Increase_Key_Handle, Vol_Increase_Read, 0);
  button_init(&Touch_Key_Handle, Touch_Key_Read, 1);
  
  /*注册触发事件*/
  button_attach(&Set_Mode_Key_Handle, SINGLE_CLICK, Set_Mode_Key_SINGLE_Click_Handler);
  button_attach(&Vol_Decrease_Key_Handle, SINGLE_CLICK, Vol_Decrease_Key_SINGLE_Click_Handler);
  button_attach(&Vol_Increase_Key_Handle, SINGLE_CLICK, Vol_Increase_Key_SINGLE_Click_Handler);
  button_attach(&Power_key_Handle, SINGLE_CLICK, Power_key_SINGLE_Click_Handler);
  button_attach(&Power_key_Handle, DOUBLE_CLICK, Power_key_DOUBLE_Click_Handler);
  button_attach(&Power_key_Handle, LONG_PRESS_START, Power_key_LONG_PRESS_START_Handler);
  button_attach(&Touch_Key_Handle, SINGLE_CLICK, Touch_Key_SINGLE_Click_Handler);
  
  /*使能*/
  button_start(&Set_Mode_Key_Handle);
  button_start(&Vol_Decrease_Key_Handle);
  button_start(&Vol_Increase_Key_Handle);
  button_start(&Power_key_Handle);
  button_start(&Touch_Key_Handle);
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
