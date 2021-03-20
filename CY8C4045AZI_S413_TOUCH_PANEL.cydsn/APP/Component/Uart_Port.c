/**
 *  @file Uart_Port.c
 *
 *  @date 2021-03-17
 *
 *  @author aron566
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief 串口操作接口
 *
 *  @details 1、
 *
 *  @version V1.0
 */
/** Includes -----------------------------------------------------------------*/
#include <project.h>
/* Private includes ----------------------------------------------------------*/
#include "Uart_Port.h"

/** Use C compiler -----------------------------------------------------------*/
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Private typedef ----------------------------------------------------------*/
/** Private macros -----------------------------------------------------------*/
#define USE_LOOPBACK    0/**< 是否使用回显-转发收到的数据*/
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
/*串口1环形缓冲区*/
static CQ_handleTypeDef Uart_1_CQ_Handle;
static uint8_t UART_1_RX_Buf[CQ_BUF_256] = {0};

/** Private function prototypes ----------------------------------------------*/
static void Uart_Port_ISR_Callback(void);/**< 串口中断服务*/
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
  * @brief   串口中断服务回调
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
static void Uart_Port_ISR_Callback(void)
{
  uint8_t Byte = 0;
  uint32 ISR_Flag = UART_1_GetInterruptCause();
  
  /*清除未决中断*/
  UART_1_ClearPendingInt();
  switch(ISR_Flag)
  {
    case UART_1_INTR_CAUSE_RX:
      if(UART_1_GetRxInterruptSource() == UART_1_INTR_RX_NOT_EMPTY)
      {
        UART_1_ClearRxInterruptSource(UART_1_INTR_RX_NOT_EMPTY);
        
        /*接收数据*/
        if(UART_1_SpiUartGetRxBufferSize() > 0)
        {
          Byte = (uint8_t)(UART_1_UartGetByte()&0xFF);
          CQ_putData(&Uart_1_CQ_Handle, (const uint8_t*)&Byte, 1);
#if USE_LOOPBACK
          Uart_Port_Send_Data((const uint8_t*)&Byte, 1);
#endif
        }
      }
      else
      {
        /*出现错误*/
/*  
*   - UART_1_INTR_RX_FIFO_LEVEL - The number of data elements in the 
      RX FIFO is greater than the value of RX FIFO level.
*   - UART_1_INTR_RX_OVERFLOW - Attempt to write to a full 
*     receiver FIFO.
*   - UART_1_INTR_RX_UNDERFLOW - Attempt to read from an empty 
*     receiver FIFO.
*   - UART_1_INTR_RX_FRAME_ERROR - UART framing error detected.
*   - UART_1_INTR_RX_PARITY_ERROR - UART parity error detected.
*/
        if(UART_1_GetRxInterruptSource() == UART_1_INTR_RX_OVERFLOW)
        {
          UART_1_ClearRxInterruptSource(UART_1_INTR_RX_OVERFLOW);
        }
        else if(UART_1_GetRxInterruptSource() == UART_1_INTR_RX_UNDERFLOW)
        {
          UART_1_ClearRxInterruptSource(UART_1_INTR_RX_UNDERFLOW);
        }
        else if(UART_1_GetRxInterruptSource() == UART_1_INTR_RX_FRAME_ERROR)
        {
          UART_1_ClearRxInterruptSource(UART_1_INTR_RX_FRAME_ERROR);
        }
        else if(UART_1_GetRxInterruptSource() == UART_1_INTR_RX_PARITY_ERROR)
        {
          UART_1_ClearRxInterruptSource(UART_1_INTR_RX_PARITY_ERROR);
        }
        else if(UART_1_GetRxInterruptSource() == UART_1_INTR_RX_FIFO_LEVEL)
        {
          UART_1_ClearRxInterruptSource(UART_1_INTR_RX_FIFO_LEVEL);
        }
      }
      break;
    case UART_1_INTR_CAUSE_TX:
      /*TODO*/
      break;
    case UART_1_INTR_CAUSE_MASTER:
      /*TODO*/
      break;
    case UART_1_INTR_CAUSE_SLAVE:
      /*TODO*/
      break;
    default:
      break;
  }
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
  * @brief   串口发送字符串
  * @param   [in]Str 字符串
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-20
  ******************************************************************
  */
void Uart_Port_Print(const char *Str)
{
  UART_1_UartPutString(Str);
}

/**
  ******************************************************************
  * @brief   串口缓冲区句柄
  * @param   [in]None
  * @return  CQ_handleTypeDef *.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
CQ_handleTypeDef *Uart_Port_Get_CQ_Handle(void)
{
  return &Uart_1_CQ_Handle;
}

/**
  ******************************************************************
  * @brief   串口发送接口
  * @param   [in]data 数据地址
  * @param   [in]Size 数据长度
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void Uart_Port_Send_Data(const uint8_t *data, uint32_t Size)
{
  UART_1_SpiUartPutArray((const uint8 *)data, (uint32)Size);
}

/**
  ******************************************************************
  * @brief   初始化串口接口
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void Uart_Port_Init(void)
{
  /*建立串口1环形缓冲区*/
  CQ_init(&Uart_1_CQ_Handle, UART_1_RX_Buf, CQ_BUF_256);
  
  /* Start SCB (UART mode) operation */
  UART_1_Start();
  
  /*注册串口中断*/
  UART_1_SetCustomInterruptHandler(Uart_Port_ISR_Callback);
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
