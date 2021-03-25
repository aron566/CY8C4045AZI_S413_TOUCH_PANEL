/**
 *  @file CapSense_Port.c
 *
 *  @date 2021-03-17
 *
 *  @author aron566
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief 触摸接口
 *
 *  @details 1、
 *
 *  @version V1.0
 */
/** Includes -----------------------------------------------------------------*/
#include <project.h>
/* Private includes ----------------------------------------------------------*/
#include "CapSense_Port.h"

/** Use C compiler -----------------------------------------------------------*/
#ifdef __cplusplus ///< use C compiler
extern "C" {
#endif
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/

/* Boolean constants */
#define LED_ON						          (0u)
#define LED_OFF						          (1u)

/*Set the macro value to '1' to use tuner for debugging and tuning CapSense sensors
  Set the macro value to '0' to send the CapSense Linear slider and buttons data to EZ-BLE PRoC module via I2C*/
#define ENABLE_TUNER                (1u)

/*I2C Buffer size = 4 bytes
  BYTE0 = CapSense linear slider touch position
  BYTE1 = No of buttons on CY8CKIT-145-40XX PSoC 4000S Prototyping Kit
  BYTE2 = bit0= BTN1 status, bit1 = BTN2 status, bit2 = BTN3 status
  BYTE3 = dummy for this project, for future expansion */
#define BUFFER_SIZE                 (4u)
#define READ_ONLY_OFFSET            (0u)
#define TOTAL_CAPSENSE_BUTTONS      (3u)

/*I2C buffer index*/
#define SLIDER_CENTROID_INDEX       (0u)
#define BUTTON_COUNT_INDEX          (1u)
#define BUTTON_STATUS_INDEX1        (2u)
#define BUTTON_STATUS_INDEX2        (3u)

#define SET_BIT(data, bitPosition)   ((data) |= (1 << (bitPosition)))
#define CLEAR_BIT(data, bitPosition) ((data) &= (~(1 << (bitPosition))))

/* Finite state machine states for device operating states 
    SENSOR_SCAN - Sensors are scanned in this state
    WAIT_FOR_SCAN_COMPLETE - CPU is put to sleep in this state
    PROCESS_DATA - Sensor data is processed, LEDs are controlled,
                   and I2C buffer is updated in this state */
typedef enum
{
    SENSOR_SCAN = 0x01u,                
    WAIT_FOR_SCAN_COMPLETE = 0x02u,     
    PROCESS_DATA = 0x03u,               
} DEVICE_STATE;

/*Defining step size for LED control based on centroid position of slider*/
#define STEP_SIZE (CapSense_SLD_X_RESOLUTION/CapSense_SLD_NUM_SENSORS) 

/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
#if !ENABLE_TUNER 
    /*In this project CapSense linear slider touch position and buttons status 
        is sent to EZ-BLE PRoC Module via I2C interface. EZ-BLE PRoC Module transmits 
        the CapSense linear slider touch position and button ON/OFF status data via BLE and 
        the CapSense linear slider touch position can be viewed on mobile phones using CySmart app.
        I2C buffer data format is decided based on the Cypress CapSense custom service
        See this link for more details: http://www.cypress.com/file/175556/download*/
    
    
    /*I2C Buffer used to send CapSense linear touch position and buttons status detected 
        by PSoC 4000S device to EZ-BLE PRoC Module
      BYTE0 = slider centroid position
      BYTE1 = No of buttons on CY8CKIT-145-40XX PSoC 4000S Prototyping Kit
      BYTE2 = bit0= BTN1 status, bit1 = BTN2 status, bit2 = BTN3 status
      BYTE3 = dummy for this project, for future expansion */
    uint8 i2cBuffer[] = {0,TOTAL_CAPSENSE_BUTTONS,0,0};
#endif

/* Variable to hold the current device state 
*  State machine starts with sensor scan state after power-up
*/
static DEVICE_STATE currentState = SENSOR_SCAN;
static uint32_t Postion_On_Touch_Panel = 0;/**< 触摸位置信息*/
/** Private function prototypes ----------------------------------------------*/

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
  * @brief   LED控制
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
static void LED_Control(void)
{
  uint32 centroid;
  /*Get the touch position(centroid) of CapSense Linear Slider*/
  centroid = CapSense_GetCentroidPos(CapSense_SLD_WDGT_ID);

  /*Turn ON/OFF LEDs based on the status of the corresponding CapSense buttons*/
//  LED9_Write(CapSense_IsWidgetActive(CapSense_BTN0_WDGT_ID) ? LED_ON : LED_OFF );/**< 开发板LED9 3.4脚给increase按键用*/
  LED10_Write(CapSense_IsWidgetActive(CapSense_BTN1_WDGT_ID) ? LED_ON : LED_OFF );
  LED11_Write(CapSense_IsWidgetActive(CapSense_BTN2_WDGT_ID) ? LED_ON : LED_OFF );

  /*Turn ON/OFF LEDs based on the finger position (centroid) on the CapSense Linear slider*/
  if(CapSense_IsWidgetActive(CapSense_SLD_WDGT_ID))
  {        
    LED4_Write( ( (centroid > 0) || (centroid == 0)) ? LED_ON : LED_OFF );
    LED5_Write( ( centroid > ( 1 * STEP_SIZE ) ) ? LED_ON : LED_OFF);
    LED6_Write( ( centroid > ( 2 * STEP_SIZE ) ) ? LED_ON : LED_OFF);
    LED7_Write( ( centroid > ( 3 * STEP_SIZE ) ) ? LED_ON : LED_OFF);
    LED8_Write( ( centroid > ( 4 * STEP_SIZE ) ) ? LED_ON : LED_OFF);
  }
  else
  {
    LED4_Write(LED_OFF);
    LED5_Write(LED_OFF);
    LED6_Write(LED_OFF);
    LED7_Write(LED_OFF);
    LED8_Write(LED_OFF);
  }
}

/**
  ******************************************************************
  * @brief   触摸更新位置信息
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-17
  ******************************************************************
  */
static void Touch_Key_Process(void)
{
  uint32 centroid = 0;
  
  /*Get the touch position(centroid) of CapSense Linear Slider*/
  centroid = CapSense_GetCentroidPos(CapSense_SLD_WDGT_ID);
 
  /*based on the finger position (centroid) on the CapSense Linear or Circular slider*/
  if(CapSense_IsWidgetActive(CapSense_SLD_WDGT_ID))
  {
    Postion_On_Touch_Panel = centroid;
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
  * @brief   触摸位置检测
  * @param   [in]None
  * @return  位置信息.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-17
  ******************************************************************
  */
uint32_t CapSense_Port_Get_Touch_Postion(void)
{
  return Postion_On_Touch_Panel;
}

/**
  ******************************************************************
  * @brief   触摸状态检测
  * @param   [in]None
  * @return  返回 1触摸中.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
uint8_t CapSense_Port_Get_Touch_State(void)
{
  if(CapSense_IsWidgetActive(CapSense_SLD_WDGT_ID))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
  ******************************************************************
  * @brief   启动触摸检测
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void CapSense_Port_Start(void)
{
#if !ENABLE_TUNER
    /*Used as loop counter and widget ID*/
    uint8 widgetID = 0;
    
    /*Contains the buttons status, one bit per button
      bit0= BTN1 status, bit1 = BTN2 status, bit2 = BTN3 status*/
    uint8 buttonStatus = 0;    
#endif
  /* Switch between SENSOR_SCAN->WAIT_FOR_SCAN_COMPLETE->PROCESS_DATA states */
  switch(currentState)
  {
    case SENSOR_SCAN:
      /* Initiate new scan only if the CapSense block is idle */
        if(CapSense_NOT_BUSY == CapSense_IsBusy())
        {
            #if ENABLE_TUNER
                /* Update CapSense parameters set via CapSense tuner before the 
                   beginning of CapSense scan 
                */
                CapSense_RunTuner();
            #endif
            
            /* Scan widget configured by CSDSetupWidget API */
            CapSense_ScanAllWidgets();
                                
            /* Set next state to WAIT_FOR_SCAN_COMPLETE  */
            currentState = WAIT_FOR_SCAN_COMPLETE;
        }
        break;

    case WAIT_FOR_SCAN_COMPLETE:

        /* Put the device to CPU Sleep until CapSense scanning is complete*/
        if(CapSense_NOT_BUSY != CapSense_IsBusy())
        {
          CySysPmSleep();
        }
        /* If CapSense scanning is complete, process the CapSense data */
        else
        {
          currentState = PROCESS_DATA;
        }
        break;

    case PROCESS_DATA:
        
        /* Process data on all the enabled widgets */
        CapSense_ProcessAllWidgets();
        
        /* Controls LEDs Status based on the result of Widget processing. */
        LED_Control();
        
        /*触摸按键控制*/
        Touch_Key_Process();
        
        #if !ENABLE_TUNER
            
            /*If tuner is not enabled send the CapSense slider centroid position and button status to 
                EZ-BLE PRoC Module on CY8CKIT-145-40XX PSoC 4000S Prototyping Kit  via I2C interface*/
            
            /*Update the I2C buffer with slider centroid position*/
            i2cBuffer[SLIDER_CENTROID_INDEX] = (uint8) CapSense_GetCentroidPos(CapSense_SLD_WDGT_ID);
            
            /*Calculate the button staus mask and update the I2C buffer
                bit0= BTN1 status, bit1 = BTN2 status, bit2 = BTN3 status*/
            for(widgetID = 0; widgetID < TOTAL_CAPSENSE_BUTTONS ; widgetID++)
            {
                if(CapSense_IsWidgetActive(widgetID))
                {
                    SET_BIT(buttonStatus, widgetID);
                }
                else
                {
                    CLEAR_BIT(buttonStatus, widgetID);
                }
            }                     
            i2cBuffer[BUTTON_STATUS_INDEX1] = buttonStatus;                   
        #endif                
        
        /* Set the device state to SENSOR_SCAN */
        currentState = SENSOR_SCAN;  
        break;  
     
    /*******************************************************************
     * Unknown power mode state. Unexpected situation.
     ******************************************************************/    
    default:
        break;
  }
}

/**
  ******************************************************************
  * @brief   初始化触摸检测
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-16
  ******************************************************************
  */
void CapSense_Port_Init(void)
{
  /* Start EZI2C block */
  EZI2C_Start();
        
#if ENABLE_TUNER
  /* Set up I2C communication data buffer with CapSense data structure 
  to be exposed to I2C master on a primary slave address request
  */
  EZI2C_EzI2CSetBuffer1(sizeof(CapSense_dsRam),\
  sizeof(CapSense_dsRam),(uint8 *)&CapSense_dsRam);
#else
  /*Set up communication data buffer to send CapSense slider centroid 
      position and button status to EZ-BLE on CY8CKIT-145-40XX PSoC 4000S Prototyping Kit*/
  EZI2C_EzI2CSetBuffer1(sizeof(i2cBuffer), sizeof(i2cBuffer),i2cBuffer);
#endif
        
  /* Start CapSense block */
  CapSense_Start();
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
