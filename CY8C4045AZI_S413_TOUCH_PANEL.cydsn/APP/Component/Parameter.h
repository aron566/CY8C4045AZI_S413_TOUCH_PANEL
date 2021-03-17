/**
 *  @file Parameter.h
 *
 *  @date 2021-03-16
 *
 *  @author aron566
 *
 *  @brief 参数调整接口
 *  
 *  @version V1.0
 */
#ifndef PARAMETER_H
#define PARAMETER_H
/** Includes -----------------------------------------------------------------*/
#include <stdint.h> /**< need definition of uint8_t */
#include <stddef.h> /**< need definition of NULL    */
#include <stdbool.h>/**< need definition of BOOL    */
#include <stdio.h>  /**< if need printf             */
#include <stdlib.h>
#include <string.h>
#include <limits.h> /**< need variable max value    */
/** Private includes ---------------------------------------------------------*/
#include "Protocol.h"
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

/*初始化参数接口*/
void Parameter_Init(void);
/*音量增*/
void Parameter_Increase_Vol(void);
/*音量减*/
void Parameter_Decrease_Vol(void);
/*改变设备算法功能*/
void Parameter_Change_Dev_Function(void);
/*音量更新*/
void Parameter_Update_Vol(uint32_t Vol);
/*更新算法功能参数*/
void Parameter_Update_Dev_Function(uint8_t func_num);

#ifdef __cplusplus ///<end extern c
}
#endif
#endif
/******************************** End of file *********************************/
