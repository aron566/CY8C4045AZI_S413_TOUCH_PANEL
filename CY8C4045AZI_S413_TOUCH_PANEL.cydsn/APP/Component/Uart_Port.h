/**
 *  @file Uart_Port.h
 *
 *  @date 2021-03-16
 *
 *  @author aron566
 *
 *  @brief 串口操作接口
 *  
 *  @version V1.0
 */
#ifndef UART_PORT_H
#define UART_PORT_H
/** Includes -----------------------------------------------------------------*/
#include <stdint.h> /**< need definition of uint8_t */
#include <stddef.h> /**< need definition of NULL    */
#include <stdbool.h>/**< need definition of BOOL    */
#include <stdio.h>  /**< if need printf             */
#include <stdlib.h>
#include <string.h>
#include <limits.h> /**< need variable max value    */
/** Private includes ---------------------------------------------------------*/
#include "CircularQueue.h"
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

/*初始化串口接口*/
void Uart_Port_Init(void);
/*串口发送接口*/
void Uart_Port_Send_Data(const uint8_t *data, uint32_t Size);
/*获取串口环形缓冲区句柄*/
CQ_handleTypeDef *Uart_Port_Get_CQ_Handle(void);

#ifdef __cplusplus ///<end extern c
}
#endif
#endif
/******************************** End of file *********************************/
