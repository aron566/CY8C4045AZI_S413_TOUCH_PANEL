/**
 *  @file Parameter.c
 *
 *  @date 2021-03-17
 *
 *  @author aron566
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief 参数调整接口
 *
 *  @details 1、
 *
 *  @version V1.0
 */
/** Includes -----------------------------------------------------------------*/
#include <project.h>
/* Private includes ----------------------------------------------------------*/
#include "Parameter.h"

/** Use C compiler -----------------------------------------------------------*/
#ifdef __cplusplus ///< use C compiler
extern "C" {
#endif
/** Private typedef ----------------------------------------------------------*/
/*设备参数*/
typedef struct
{
  uint32_t Vol;
  ALGORITHM_FUNCTION_Typdef_t Algorithm_Sel;
}DEVICE_PARAMETER_Typedef_t;
/** Private macros -----------------------------------------------------------*/
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
static DEVICE_PARAMETER_Typedef_t Device_Par;/**< 设备参数*/
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
  * @brief   音量更新
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void Parameter_Update_Vol(uint32_t Vol)
{
  Device_Par.Vol = Vol;
}

/**
  ******************************************************************
  * @brief   音量增
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void Parameter_Increase_Vol(void)
{
  uint32_t Vol = Device_Par.Vol;
  Vol = (Vol >= 100)?100:Vol+1;
  if(Device_Par.Vol == Vol)
  {
    return;
  }
  Device_Par.Vol = Vol;
  /*更新*/
  Protocol_Set_Vol(Vol);
}

/**
  ******************************************************************
  * @brief   音量减
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void Parameter_Decrease_Vol(void)
{
  uint32_t Vol = Device_Par.Vol;
  Vol = (Vol == 0)?0:Vol-1;
  if(Device_Par.Vol == Vol)
  {
    return;
  }
  Device_Par.Vol = Vol;
  /*更新*/
  Protocol_Set_Vol(Vol);
}

/**
  ******************************************************************
  * @brief   改变设备算法功能
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void Parameter_Change_Dev_Function(void)
{
  uint32_t func_num = (uint32_t)Device_Par.Algorithm_Sel;
  func_num = (func_num >= (uint32_t)FUNCTON_MAX)?(uint32_t)RNN_NOISE_FUNC:func_num+1;
  if(func_num == (uint32_t)Device_Par.Algorithm_Sel)
  {
    return;
  }
  Device_Par.Algorithm_Sel = (ALGORITHM_FUNCTION_Typdef_t)func_num;
  /*更新*/
  Protocol_Set_Dev_Function(Device_Par.Algorithm_Sel);
}

/**
  ******************************************************************
  * @brief   更新算法功能参数
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void Parameter_Update_Dev_Function(uint8_t func_num)
{
  if(func_num >= (uint8_t)FUNCTON_MAX)
  {
    return;
  }
  Device_Par.Algorithm_Sel = (ALGORITHM_FUNCTION_Typdef_t)func_num;
}

/**
  ******************************************************************
  * @brief   参数检测启动
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void Parameter_Start(void)
{
  /*TODO*/
}

/**
  ******************************************************************
  * @brief   初始化参数接口
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void Parameter_Init(void)
{
  memset(&Device_Par, 0, sizeof(DEVICE_PARAMETER_Typedef_t));
  
  /*读取设备音量参数*/
  Protocol_Read_Vol();
  
  /*读取设备功能选择参数*/
  Protocol_Read_Dev_Function();
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
