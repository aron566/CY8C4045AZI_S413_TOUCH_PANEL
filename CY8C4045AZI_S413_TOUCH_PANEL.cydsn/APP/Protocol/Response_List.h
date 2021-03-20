/**
 *  @file Response_List.h
 *
 *  @date 2021-03-20
 *
 *  @author aron566
 *
 *  @brief FIFO响应队列
 *  
 *  @version V1.0
 */
#ifndef RESPONSE_LIST_H
#define RESPONSE_LIST_H
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
/*响应队列*/
typedef struct
{
  uint16_t Reg_Addr;      /**< 等待响应的寄存器地址*/
  uint32_t Start_Time;    /**< 等待相应的起始时间*/
  uint8_t Command;        /**< 等待的命令类型 0x03 0x04*/ 
}WAIT_RESPONSE_LIST_Typedef_t;

/*响应队列句柄*/
typedef struct
{
  uint32_t Entry_Size;
  uint32_t Exit_Size;
  uint32_t Size;
  WAIT_RESPONSE_LIST_Typedef_t *List;
}WAIT_RESPONSE_CQ_Typedef_t;

/** Exported constants -------------------------------------------------------*/

/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*初始化环形响应队列*/
void Response_List_Init(WAIT_RESPONSE_CQ_Typedef_t *List_Handle, WAIT_RESPONSE_LIST_Typedef_t *List, uint32_t Size);
/*响应队列加入新数据*/
uint32_t Response_List_putData(WAIT_RESPONSE_CQ_Typedef_t *List_Handle, const WAIT_RESPONSE_LIST_Typedef_t *sourceBuf, uint32_t len);
/*响应队列读走数据,不自动缩减队列长度,需要手动调用Response_List_ManualOffsetInc*/
uint32_t Response_List_getData(WAIT_RESPONSE_CQ_Typedef_t *List_Handle, WAIT_RESPONSE_LIST_Typedef_t *targetBuf, uint32_t len);
/*响应队列清空操作*/
void Response_List_emptyData(WAIT_RESPONSE_CQ_Typedef_t *List_Handle);
/*响应队列获取剩余空间长度*/
uint32_t Response_List_getLength(WAIT_RESPONSE_CQ_Typedef_t *List_Handle);
/*响应队列手动增加已取出长度*/
void Response_List_ManualOffsetInc(WAIT_RESPONSE_CQ_Typedef_t *List_Handle, uint32_t len);
/*响应队列判断是否为满*/
bool Response_List_isFull(WAIT_RESPONSE_CQ_Typedef_t *List_Handle);
/*响应队列判断是否为空*/
bool Response_List_isEmpty(WAIT_RESPONSE_CQ_Typedef_t *List_Handle);

#ifdef __cplusplus ///<end extern c
}
#endif
#endif
/******************************** End of file *********************************/
