/*******************************************************************************
* File Name: Vol_Increase.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Vol_Increase_H) /* Pins Vol_Increase_H */
#define CY_PINS_Vol_Increase_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Vol_Increase_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} Vol_Increase_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Vol_Increase_Read(void);
void    Vol_Increase_Write(uint8 value);
uint8   Vol_Increase_ReadDataReg(void);
#if defined(Vol_Increase__PC) || (CY_PSOC4_4200L) 
    void    Vol_Increase_SetDriveMode(uint8 mode);
#endif
void    Vol_Increase_SetInterruptMode(uint16 position, uint16 mode);
uint8   Vol_Increase_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Vol_Increase_Sleep(void); 
void Vol_Increase_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Vol_Increase__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Vol_Increase_DRIVE_MODE_BITS        (3)
    #define Vol_Increase_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Vol_Increase_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Vol_Increase_SetDriveMode() function.
         *  @{
         */
        #define Vol_Increase_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Vol_Increase_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Vol_Increase_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Vol_Increase_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Vol_Increase_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Vol_Increase_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Vol_Increase_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Vol_Increase_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Vol_Increase_MASK               Vol_Increase__MASK
#define Vol_Increase_SHIFT              Vol_Increase__SHIFT
#define Vol_Increase_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Vol_Increase_SetInterruptMode() function.
     *  @{
     */
        #define Vol_Increase_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Vol_Increase_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Vol_Increase_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Vol_Increase_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Vol_Increase__SIO)
    #define Vol_Increase_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Vol_Increase__PC) && (CY_PSOC4_4200L)
    #define Vol_Increase_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Vol_Increase_USBIO_DISABLE              ((uint32)(~Vol_Increase_USBIO_ENABLE))
    #define Vol_Increase_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Vol_Increase_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Vol_Increase_USBIO_ENTER_SLEEP          ((uint32)((1u << Vol_Increase_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Vol_Increase_USBIO_SUSPEND_DEL_SHIFT)))
    #define Vol_Increase_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Vol_Increase_USBIO_SUSPEND_SHIFT)))
    #define Vol_Increase_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Vol_Increase_USBIO_SUSPEND_DEL_SHIFT)))
    #define Vol_Increase_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Vol_Increase__PC)
    /* Port Configuration */
    #define Vol_Increase_PC                 (* (reg32 *) Vol_Increase__PC)
#endif
/* Pin State */
#define Vol_Increase_PS                     (* (reg32 *) Vol_Increase__PS)
/* Data Register */
#define Vol_Increase_DR                     (* (reg32 *) Vol_Increase__DR)
/* Input Buffer Disable Override */
#define Vol_Increase_INP_DIS                (* (reg32 *) Vol_Increase__PC2)

/* Interrupt configuration Registers */
#define Vol_Increase_INTCFG                 (* (reg32 *) Vol_Increase__INTCFG)
#define Vol_Increase_INTSTAT                (* (reg32 *) Vol_Increase__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Vol_Increase_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Vol_Increase__SIO)
    #define Vol_Increase_SIO_REG            (* (reg32 *) Vol_Increase__SIO)
#endif /* (Vol_Increase__SIO_CFG) */

/* USBIO registers */
#if !defined(Vol_Increase__PC) && (CY_PSOC4_4200L)
    #define Vol_Increase_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Vol_Increase_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Vol_Increase_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Vol_Increase_DRIVE_MODE_SHIFT       (0x00u)
#define Vol_Increase_DRIVE_MODE_MASK        (0x07u << Vol_Increase_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Vol_Increase_H */


/* [] END OF FILE */