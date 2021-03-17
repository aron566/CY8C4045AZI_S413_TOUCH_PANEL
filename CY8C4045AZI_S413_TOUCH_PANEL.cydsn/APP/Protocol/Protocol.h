/**
 *  @file Protocol.h
 *
 *  @date 2021-03-16
 *
 *  @author aron566
 *
 *  @brief 通讯协议接口
 *  
 *  @version V1.0
 */
#ifndef PROTOCOL_H
#define PROTOCOL_H
/** Includes -----------------------------------------------------------------*/
#include <stdint.h> /**< need definition of uint8_t */
#include <stddef.h> /**< need definition of NULL    */
#include <stdbool.h>/**< need definition of BOOL    */
#include <stdio.h>  /**< if need printf             */
#include <stdlib.h>
#include <string.h>
#include <limits.h> /**< need variable max value    */
/** Private includes ---------------------------------------------------------*/
/** Use C compiler -----------------------------------------------------------*/
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Private defines ----------------------------------------------------------*/

/** Exported typedefines -----------------------------------------------------*/
/*算法功能分类*/
typedef enum
{
  RNN_NOISE_FUNC     = 0x01,
  MASK_EMD_FUNC      = 0x02,
  MASK_EMD_WDRC_FUNC = 0x03,
  VOL_FUNC           = 0x04,
  AGC_FUNC           = 0x05,
  FUNCTON_MAX        = 0x06,
}ALGORITHM_FUNCTION_Typdef_t;
/** Exported constants -------------------------------------------------------*/

/** Exported macros-----------------------------------------------------------*/

/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*协议栈初始化*/
void Protocol_Init(void);
/*协议栈初启动*/
void Protocol_Start(void);
/*设置设备音量*/
void Protocol_Set_Vol(uint32_t vol);
/*设置设备算法功能*/
void Protocol_Set_Dev_Function(ALGORITHM_FUNCTION_Typdef_t func);
/*设置设备BF算法角度参数*/
void Protocol_Set_Dev_BF_Angle(uint16_t Angle);
/*读取音量数值*/
void Protocol_Read_Vol(void);
/*读取设备已启用算法功能*/
void Protocol_Read_Dev_Function(void);

#ifdef __cplusplus ///<end extern c
}
#endif
#endif
/******************************** End of file *********************************/
