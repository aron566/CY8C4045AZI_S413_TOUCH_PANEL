/**
 *  @file Protocol.c
 *
 *  @date 2021-03-17
 *
 *  @author aron566
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief 通讯协议接口
 *
 *  @details 1、本机作为主机
 *           2、提供回复报文解析 、设置相应参数
 *
 *  @version V1.0
 */
/** Includes -----------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "Protocol.h"
#include "Uart_Port.h"
#include "utilities_crc.h"
#include "utilities.h"
#include "Parameter.h"
#include "Timer_Port.h"
#include "Response_List.h"
/** Use C compiler -----------------------------------------------------------*/
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/
#define WAIT_RESPONSE_SIZE_MAX            10U /**< 响应队列长度*/
#define FRAME_TIME_OUT                    3U  /**< 3s超时检测*/
#define FRAME_SIZE_MIN                    5U
#define FRAME_SIZE_MAX                    256U
#define IS_LESS_MIN_FRAME(len)            (len < FRAME_SIZE_MIN)?1:0/**< 判断是否小于最小帧长*/
#define LONG_FRAME_PACKAGE_LEN            FRAME_SIZE_MIN/**< 写入回复报文长度*/
#define SHORT_FRAME_PACKAGE_LEN(data_len) (data_len+8)/**< 读取回复报文长度*/
#define LONG_FRAME_TYPE                   0xA5U/**< 长帧类型*/
#define SHORT_FRAME_TYPE                  0x5AU/**< 短帧类型*/
#define WRITE_COMMAND                     0x03U/**< 设置*/
#define READ_COMMAND                      0x04U/**< 请求*/
                                          
/*寄存器地址*/                            
#define REG_SOFT_VER                      0x0000/**< 软件版本*/
#define REG_VOL_PAR                       0x0001/**< 音量参数*/
#define REG_EQ_PAR                        0x0002/**< EQ参数*/
#define REG_WDRC_PAR                      0x0003/**< WDRC参数*/
#define REG_AGC_PAR                       0x0004/**< AGC参数*/
#define REG_RNNSE_PAR                     0x0005/**< RNN降噪参数*/
#define REG_ALGORI_SEL                    0x0006/**< 算法功能选择*/
#define REG_READ_ALL_PAR                  0x0007/**< 读取全部参数*/
#define REG_UPDATE_ENTRY                  0x0008/**< 进入升级模式*/
#define REG_UPDATE_EXIT                   0x0009/**< 退出升级模式*/
#define REG_BATTERY_INFO                  0x000A/**< 电池电量信息*/
#define REG_BATTERY_CHARGE_STATE          0x000B/**< 电池充电状态*/
#define REG_BF_ANGLE_PAR                  0x000C/**< BF入射角参数*/ 
/** Private constants --------------------------------------------------------*/

/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
static uint8_t Send_Buf[256] = {0x7A, 0x55};/**< 发送缓冲区*/
static uint8_t Rec_Buf[256] = {0};/**< 数据接收缓冲区*/
static CQ_handleTypeDef *CQ_Handle = NULL;/**< 缓冲区句柄*/
static volatile uint16_t Last_Opt_Reg_Addr = 0;

/*建立响应队列环形缓冲区*/
static WAIT_RESPONSE_LIST_Typedef_t Wait_Response_List[WAIT_RESPONSE_SIZE_MAX];
/*建立队列句柄*/
static WAIT_RESPONSE_CQ_Typedef_t Wait_Response_List_Handle;
/** Private function prototypes ----------------------------------------------*/
static bool Check_Read_Response_TimeOut(WAIT_RESPONSE_LIST_Typedef_t *pWait);
static void Send_Command_Frame(uint8_t *Data, uint32_t Size);
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
  * @brief   检测帧超时
  * @param   [in]pWait 等待数据.
  * @return  true 已超时.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
static bool Check_Read_Response_TimeOut(WAIT_RESPONSE_LIST_Typedef_t *pWait)
{  
  if((Timer_Port_Get_Current_Time(TIMER_SEC) - pWait->Start_Time) >= FRAME_TIME_OUT)
  {
    return true;
  }
  return false;
}

/**
  ******************************************************************
  * @brief   发送命令帧
  * @param   [in]Data 数据
  * @param   [in]Size 数据长度
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
static inline void Send_Command_Frame(uint8_t *Data, uint32_t Size)
{
  /*检测是否存在等待消息*/
  if(Last_Opt_Reg_Addr != 0)
  {
    return;
  }
  uint16_t crc_val = modbus_crc_return_with_table(Data, Size);
  Data[Size++] = GET_U16_LOW_BYTE(crc_val);
  Data[Size++] = GET_U16_HI_BYTE(crc_val);
  Uart_Port_Send_Data(Data, Size);
}

/**
  ******************************************************************
  * @brief   解析从站数据
  * @param   [in]pWait 等待数据
  * @param   [in]pData 数据
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
static void Decode_Frame_Data(WAIT_RESPONSE_LIST_Typedef_t *pWait, const uint8_t *pData)
{
  switch(pWait->Reg_Addr)
  {
    case REG_VOL_PAR:
      /*更新音量数值*/
      Parameter_Update_Vol(common_get_u32_data(pData, 0));
      break;
    case REG_ALGORI_SEL:
      /*更新算法功能选择*/
      Parameter_Update_Dev_Function(pData[0]);
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
  * @brief   设置设备音量
  * @param   [in]vol
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void Protocol_Set_Vol(uint32_t vol)
{
  uint32_t index = 2;
  Send_Buf[index++] = (uint8_t)LONG_FRAME_TYPE;
  Send_Buf[index++] = (uint8_t)WRITE_COMMAND;
  Send_Buf[index++] = GET_U16_LOW_BYTE(REG_VOL_PAR);
  Send_Buf[index++] = GET_U16_HI_BYTE(REG_VOL_PAR);
  Send_Buf[index++] = 4;
  Send_Buf[index++] = 0;
  Send_Buf[index++] = GET_U16_LOW_BYTE(GET_U32_LOW_HALF_WORD(vol));
  Send_Buf[index++] = GET_U16_HI_BYTE(GET_U32_LOW_HALF_WORD(vol));
  Send_Buf[index++] = GET_U16_LOW_BYTE(GET_U32_HI_HALF_WORD(vol));
  Send_Buf[index++] = GET_U16_HI_BYTE(GET_U32_HI_HALF_WORD(vol));
  Send_Command_Frame(Send_Buf, index);
  /*加入等待队列*/
  WAIT_RESPONSE_LIST_Typedef_t Wait;
  Wait.Command = (uint8_t)WRITE_COMMAND;
  Wait.Reg_Addr = REG_VOL_PAR;
  Wait.Start_Time = Timer_Port_Get_Current_Time(TIMER_SEC);
  Response_List_putData(&Wait_Response_List_Handle, &Wait, 1);
}

/**
  ******************************************************************
  * @brief   设置设备算法功能
  * @param   [in]func
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void Protocol_Set_Dev_Function(ALGORITHM_FUNCTION_Typdef_t func)
{
  uint32_t index = 2;
  Send_Buf[index++] = (uint8_t)LONG_FRAME_TYPE;
  Send_Buf[index++] = (uint8_t)WRITE_COMMAND;
  Send_Buf[index++] = GET_U16_LOW_BYTE(REG_ALGORI_SEL);
  Send_Buf[index++] = GET_U16_HI_BYTE(REG_ALGORI_SEL);
  Send_Buf[index++] = 1;
  Send_Buf[index++] = 0;
  Send_Buf[index++] = (uint8_t)func;
  Send_Command_Frame(Send_Buf, index);
  /*加入等待队列*/
  WAIT_RESPONSE_LIST_Typedef_t Wait;
  Wait.Command = (uint8_t)WRITE_COMMAND;
  Wait.Reg_Addr = REG_ALGORI_SEL;
  Wait.Start_Time = Timer_Port_Get_Current_Time(TIMER_SEC);
  Response_List_putData(&Wait_Response_List_Handle, &Wait, 1);
}

/**
  ******************************************************************
  * @brief   设置设备BF算法角度参数
  * @param   [in]Angle 角度
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-17
  ******************************************************************
  */
void Protocol_Set_Dev_BF_Angle(uint16_t Angle)
{
  uint32_t index = 2;
  Send_Buf[index++] = (uint8_t)LONG_FRAME_TYPE;
  Send_Buf[index++] = (uint8_t)WRITE_COMMAND;
  Send_Buf[index++] = GET_U16_LOW_BYTE(REG_BF_ANGLE_PAR);
  Send_Buf[index++] = GET_U16_HI_BYTE(REG_BF_ANGLE_PAR);
  Send_Buf[index++] = 2;
  Send_Buf[index++] = 0;
  Send_Buf[index++] = GET_U16_LOW_BYTE(Angle);
  Send_Buf[index++] = GET_U16_HI_BYTE(Angle);
  Send_Command_Frame(Send_Buf, index);
  /*加入等待队列*/
  WAIT_RESPONSE_LIST_Typedef_t Wait;
  Wait.Command = (uint8_t)WRITE_COMMAND;
  Wait.Reg_Addr = REG_BF_ANGLE_PAR;
  Wait.Start_Time = Timer_Port_Get_Current_Time(TIMER_SEC);
  Response_List_putData(&Wait_Response_List_Handle, &Wait, 1);
}

/**
  ******************************************************************
  * @brief   读取音量数值
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void Protocol_Read_Vol(void)
{
  uint32_t index = 2;
  Send_Buf[index++] = (uint8_t)SHORT_FRAME_TYPE;
  Send_Buf[index++] = (uint8_t)READ_COMMAND;
  Send_Buf[index++] = GET_U16_LOW_BYTE(REG_VOL_PAR);
  Send_Buf[index++] = GET_U16_HI_BYTE(REG_VOL_PAR);
  Send_Command_Frame(Send_Buf, index);
  /*加入等待队列*/
  WAIT_RESPONSE_LIST_Typedef_t Wait;
  Wait.Command = (uint8_t)READ_COMMAND;
  Wait.Reg_Addr = REG_VOL_PAR;
  Wait.Start_Time = Timer_Port_Get_Current_Time(TIMER_SEC);
  Response_List_putData(&Wait_Response_List_Handle, &Wait, 1);
}

/**
  ******************************************************************
  * @brief   读取设备已启用算法功能
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void Protocol_Read_Dev_Function(void)
{
  uint32_t index = 2;
  Send_Buf[index++] = (uint8_t)SHORT_FRAME_TYPE;
  Send_Buf[index++] = (uint8_t)READ_COMMAND;
  Send_Buf[index++] = GET_U16_LOW_BYTE(REG_ALGORI_SEL);
  Send_Buf[index++] = GET_U16_HI_BYTE(REG_ALGORI_SEL);
  Send_Command_Frame(Send_Buf, index);
  /*加入等待队列*/
  WAIT_RESPONSE_LIST_Typedef_t Wait;
  Wait.Command = (uint8_t)READ_COMMAND;
  Wait.Reg_Addr = REG_ALGORI_SEL;
  Wait.Start_Time = Timer_Port_Get_Current_Time(TIMER_SEC);
  Response_List_putData(&Wait_Response_List_Handle, &Wait, 1);
}

/**
  ******************************************************************
  * @brief   协议栈初启动
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void Protocol_Start(void)
{
  static WAIT_RESPONSE_LIST_Typedef_t Wait;
  
  /*检查响应队列*/
  uint32_t len = Response_List_getLength(&Wait_Response_List_Handle);
  if(len == 0)
  {
    return;
  }
  
  /*获取等待数据信息*/
  Response_List_getData(&Wait_Response_List_Handle, &Wait, 1);
  
  /*检测响应帧超时*/
  if(Check_Read_Response_TimeOut(&Wait) == true)
  {
    /*移除等待队列-清空缓冲区*/
    Response_List_ManualOffsetInc(&Wait_Response_List_Handle, 1);
    CQ_emptyData(CQ_Handle);
    return;
  }
  
  /*判断帧头,跳过无效帧头*/
  if(CQ_ManualGet_Offset_Data(CQ_Handle, 0) != 0x75 || CQ_ManualGet_Offset_Data(CQ_Handle, 1) != 0xAA)
  {
    if(CQ_skipInvaildU8Header(CQ_Handle, 0x75) == 0)
    {
      return;
    }
  }
  
  /*判断缓冲区数据长度*/
  len = CQ_getLength(CQ_Handle);
  if(IS_LESS_MIN_FRAME(len))
  {
    return;
  }
  
  /*判断帧类型*/
  uint8_t frame_type = CQ_ManualGet_Offset_Data(CQ_Handle, 2);
  uint16_t data_len = 0;
  uint32_t package_len = 0;
  switch(frame_type)
  {
    /*长帧-主机写入，从机回复报文*/
    case LONG_FRAME_TYPE:
      package_len = LONG_FRAME_PACKAGE_LEN;
      package_len = (package_len > FRAME_SIZE_MAX)?FRAME_SIZE_MAX:package_len;
      if(len < package_len)
      {
        return;
      }
      /*写入应答完成*/
      CQ_ManualGetData(CQ_Handle, Rec_Buf, package_len);
      /*正常写入-->移除等待队列*/
      Response_List_ManualOffsetInc(&Wait_Response_List_Handle, 1);
      break;
    /*短帧-主机读取，从机回复报文*/
    case SHORT_FRAME_TYPE:
      data_len = CQ_ManualGet_Offset_Data(CQ_Handle, 5) << 8;
      data_len += CQ_ManualGet_Offset_Data(CQ_Handle, 4);
      package_len = SHORT_FRAME_PACKAGE_LEN(data_len);
      package_len = (package_len > FRAME_SIZE_MAX)?FRAME_SIZE_MAX:package_len;
       
      if(len < package_len)
      {
        return;
      }

      CQ_ManualGetData(CQ_Handle, Rec_Buf, package_len);
      if(modbus_get_crc_result(Rec_Buf, package_len-2) == true)
      {
        /*处理数据*/
        Decode_Frame_Data(&Wait, Rec_Buf+6);
        /*正常写入-->移除等待队列*/
        Response_List_ManualOffsetInc(&Wait_Response_List_Handle, 1);
        CQ_ManualOffsetInc(CQ_Handle, package_len);
        return;
      }
      break;
    default:
      /*TODO*/
      CQ_ManualOffsetInc(CQ_Handle, 1);
      return;
  }
  
  CQ_ManualOffsetInc(CQ_Handle, 1);
  return;
}

/**
  ******************************************************************
  * @brief   协议栈初始化
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void Protocol_Init(void)
{
  /*获取缓冲区句柄*/
  CQ_Handle = Uart_Port_Get_CQ_Handle();
  
  /*响应队列初始化*/
  Response_List_Init(&Wait_Response_List_Handle, Wait_Response_List, WAIT_RESPONSE_SIZE_MAX);
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
