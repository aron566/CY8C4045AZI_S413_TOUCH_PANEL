/**
 *  @file Response_List.c
 *
 *  @date 2021-03-20
 *
 *  @author aron566
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief 响应队列
 *
 *  @details 1、
 *
 *  @version V1.0
 */
/** Includes -----------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "Response_List.h"
/** Use C compiler -----------------------------------------------------------*/
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/
#define GET_MIN(a, b) (((a) < (b)) ? (a) : (b))
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/

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
 * [Response_List_isEmpty 响应队列判断是否为空]
 * @param  List_Handle [响应队列句柄]
 * @return               [true 为空]
 */
bool Response_List_isEmpty(WAIT_RESPONSE_CQ_Typedef_t *List_Handle)
{
  if (List_Handle->Entry_Size == List_Handle->Exit_Size)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * [Response_List_isFull 响应队列判断是否为满]
 * @param  List_Handle [响应队列句柄]
 * @return               [true 为满]
 */
bool Response_List_isFull(WAIT_RESPONSE_CQ_Typedef_t *List_Handle)
{
  if ((List_Handle->Entry_Size - List_Handle->Exit_Size) == List_Handle->Size)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * [Response_List_ManualOffsetInc 响应队列手动增加已取出长度]
 * @param CircularQueue [响应队列句柄]
 * @param len           [偏移长度]
 */
void Response_List_ManualOffsetInc(WAIT_RESPONSE_CQ_Typedef_t *List_Handle, uint32_t len)
{
	List_Handle->Exit_Size += len;
}

/**
 * [Response_List_getLength 响应队列获取剩余空间长度]
 * @param  List_Handle [响应队列句柄]
 * @return               [剩余长度]
 */
uint32_t Response_List_getLength(WAIT_RESPONSE_CQ_Typedef_t *List_Handle)
{
  return (List_Handle->Entry_Size - List_Handle->Exit_Size);
}

/**
 * [Response_List_emptyData 响应队列清空操作]
 * @param  List_Handle [响应队列句柄]
 * @return               [None]
 */
void Response_List_emptyData(WAIT_RESPONSE_CQ_Typedef_t *List_Handle)
{
  List_Handle->Entry_Size = List_Handle->Exit_Size = 0;
  memset(List_Handle->List, 0, (List_Handle->Size*sizeof(WAIT_RESPONSE_LIST_Typedef_t)));
}

/**
 * [Response_List_getData 响应队列读走数据]
 * @param  List_Handle [响应队列句柄]
 * @param  targetBuf     [目标缓冲区]
 * @return               [读取的长度]
 */
uint32_t Response_List_getData(WAIT_RESPONSE_CQ_Typedef_t *List_Handle, WAIT_RESPONSE_LIST_Typedef_t *targetBuf, uint32_t len)
{
    uint32_t size = 0;

    /*此次读取的实际大小，取 可读 和 目标读取数量  两个值小的那个*/
    len = GET_MIN(len, List_Handle->Entry_Size - List_Handle->Exit_Size);
    
    /*原理雷同存入*/
    size = GET_MIN(len, List_Handle->Size - (List_Handle->Exit_Size % List_Handle->Size));
    
    /*Byte Copy*/
    memcpy(targetBuf, List_Handle->List + (List_Handle->Exit_Size % List_Handle->Size), size*sizeof(WAIT_RESPONSE_LIST_Typedef_t));
    memcpy(targetBuf + size, List_Handle->List, (len - size)*sizeof(WAIT_RESPONSE_LIST_Typedef_t));
	
    /*不自动缩减队列长度,需要手动调用Response_List_ManualOffsetInc*/
//    List_Handle->Exit_Size += len;

    return len;
}


/**
 * [Response_List_putData 响应队列加入新数据]
 * @param  List_Handle [响应队列句柄]
 * @param  sourceBuf     [为实际存储区地址]
 * @param  len           [数据存入长度] 
 * @return               [存入的长度]
 */
uint32_t Response_List_putData(WAIT_RESPONSE_CQ_Typedef_t *List_Handle, const WAIT_RESPONSE_LIST_Typedef_t *sourceBuf, uint32_t len)
{
    uint32_t size = 0;
    /*此次存入的实际大小，取 剩余空间 和 目标存入数量  两个值小的那个*/
    len = GET_MIN(len, List_Handle->Size - List_Handle->Entry_Size + List_Handle->Exit_Size);
    
    size = GET_MIN(len, List_Handle->Size - (List_Handle->Entry_Size % List_Handle->Size));
    /*Byte Copy*/
    memcpy(List_Handle->List + (List_Handle->Entry_Size % List_Handle->Size), sourceBuf, size*sizeof(WAIT_RESPONSE_LIST_Typedef_t));
    memcpy(List_Handle->List, sourceBuf + size, (len - size)*sizeof(WAIT_RESPONSE_LIST_Typedef_t));

    /*利用无符号数据的溢出特性*/
    List_Handle->Entry_Size += len;

    return len;
}

/**
  ******************************************************************
  * @brief   初始化环形响应队列
  * @param   [in]List_Handle 句柄
  * @param   [in]List 响应队列
  * @param   [in]Size 响应队列长度
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-20
  ******************************************************************
  */
void Response_List_Init(WAIT_RESPONSE_CQ_Typedef_t *List_Handle, WAIT_RESPONSE_LIST_Typedef_t *List, uint32_t Size)
{
  if(List_Handle == NULL || List == NULL || Size == 0)
  {
    return;
  }
  List_Handle->Entry_Size = 0;
  List_Handle->Exit_Size = 0;
  List_Handle->Size = Size;
  List_Handle->List = List;
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
