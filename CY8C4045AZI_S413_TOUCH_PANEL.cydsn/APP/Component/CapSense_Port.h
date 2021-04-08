/**
 *  @file CapSense_Port.h
 *
 *  @date 2021-03-16
 *
 *  @author aron566
 *
 *  @brief 参数调整接口
 *  
 *  @version V1.0
 */
#ifndef CAPSENSE_H
#define CAPSENSE_H
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

/** Exported constants -------------------------------------------------------*/

/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*初始化触摸检测*/
void CapSense_Port_Init(void);
/*启动触摸检测*/
void CapSense_Port_Start(void);
/*触摸状态检测*/
uint8_t CapSense_Port_Get_Touch_State(void);
/*获取触摸位置信息*/
uint32_t CapSense_Port_Get_Touch_Postion(void);
/*电源触摸状态读取*/
uint8_t CapSense_Port_Power_ON_OFF_Read(void);
/*设置模式触摸状态读取*/
uint8_t CapSense_Port_Set_Mode_Read(void);
/*音量减状态读取*/
uint8_t CapSense_Port_Vol_Decrease_Read(void);
/*音量增状态读取*/
uint8_t CapSense_Port_Vol_Increase_Read(void);

#ifdef __cplusplus ///<end extern c
}
#endif
#endif
/******************************** End of file *********************************/
