/**
 *  @file User_Mian.h
 *
 *  @date 2021-01-11
 *
 *  @author aron566
 *
 *  @brief 主线任务
 *  
 *  @version V1.0
 */
#ifndef USER_MAIN_H
#define USER_MAIN_H
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
#include <stdint.h> /**< need definition of uint8_t */
#include <stddef.h> /**< need definition of NULL    */
#include <stdbool.h>/**< need definition of BOOL    */
#include <stdio.h>  /**< if need printf             */
#include <stdlib.h>
#include <string.h>
#include <limits.h> /**< need variable max value    */
/** Private includes ---------------------------------------------------------*/

/** Private defines ----------------------------------------------------------*/
/** Exported constants -------------------------------------------------------*/

/** Exported macros-----------------------------------------------------------*/

/** Exported typedefines -----------------------------------------------------*/

/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*初始化各模块功能*/
void User_Main_Task_Init(void);
/*任务处理循环*/
void User_Main_Task_Process_Loop(void);

#ifdef __cplusplus ///<end extern c
}
#endif
#endif
/******************************** End of file *********************************/
