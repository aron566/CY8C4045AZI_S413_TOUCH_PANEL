/*******************************************************************************
* File Name: Power_ON_OFF.h  
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

#if !defined(CY_PINS_Power_ON_OFF_H) /* Pins Power_ON_OFF_H */
#define CY_PINS_Power_ON_OFF_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Power_ON_OFF_aliases.h"


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
} Power_ON_OFF_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Power_ON_OFF_Read(void);
void    Power_ON_OFF_Write(uint8 value);
uint8   Power_ON_OFF_ReadDataReg(void);
#if defined(Power_ON_OFF__PC) || (CY_PSOC4_4200L) 
    void    Power_ON_OFF_SetDriveMode(uint8 mode);
#endif
void    Power_ON_OFF_SetInterruptMode(uint16 position, uint16 mode);
uint8   Power_ON_OFF_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Power_ON_OFF_Sleep(void); 
void Power_ON_OFF_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Power_ON_OFF__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Power_ON_OFF_DRIVE_MODE_BITS        (3)
    #define Power_ON_OFF_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Power_ON_OFF_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Power_ON_OFF_SetDriveMode() function.
         *  @{
         */
        #define Power_ON_OFF_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Power_ON_OFF_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Power_ON_OFF_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Power_ON_OFF_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Power_ON_OFF_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Power_ON_OFF_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Power_ON_OFF_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Power_ON_OFF_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Power_ON_OFF_MASK               Power_ON_OFF__MASK
#define Power_ON_OFF_SHIFT              Power_ON_OFF__SHIFT
#define Power_ON_OFF_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Power_ON_OFF_SetInterruptMode() function.
     *  @{
     */
        #define Power_ON_OFF_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Power_ON_OFF_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Power_ON_OFF_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Power_ON_OFF_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Power_ON_OFF__SIO)
    #define Power_ON_OFF_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Power_ON_OFF__PC) && (CY_PSOC4_4200L)
    #define Power_ON_OFF_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Power_ON_OFF_USBIO_DISABLE              ((uint32)(~Power_ON_OFF_USBIO_ENABLE))
    #define Power_ON_OFF_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Power_ON_OFF_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Power_ON_OFF_USBIO_ENTER_SLEEP          ((uint32)((1u << Power_ON_OFF_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Power_ON_OFF_USBIO_SUSPEND_DEL_SHIFT)))
    #define Power_ON_OFF_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Power_ON_OFF_USBIO_SUSPEND_SHIFT)))
    #define Power_ON_OFF_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Power_ON_OFF_USBIO_SUSPEND_DEL_SHIFT)))
    #define Power_ON_OFF_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Power_ON_OFF__PC)
    /* Port Configuration */
    #define Power_ON_OFF_PC                 (* (reg32 *) Power_ON_OFF__PC)
#endif
/* Pin State */
#define Power_ON_OFF_PS                     (* (reg32 *) Power_ON_OFF__PS)
/* Data Register */
#define Power_ON_OFF_DR                     (* (reg32 *) Power_ON_OFF__DR)
/* Input Buffer Disable Override */
#define Power_ON_OFF_INP_DIS                (* (reg32 *) Power_ON_OFF__PC2)

/* Interrupt configuration Registers */
#define Power_ON_OFF_INTCFG                 (* (reg32 *) Power_ON_OFF__INTCFG)
#define Power_ON_OFF_INTSTAT                (* (reg32 *) Power_ON_OFF__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Power_ON_OFF_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Power_ON_OFF__SIO)
    #define Power_ON_OFF_SIO_REG            (* (reg32 *) Power_ON_OFF__SIO)
#endif /* (Power_ON_OFF__SIO_CFG) */

/* USBIO registers */
#if !defined(Power_ON_OFF__PC) && (CY_PSOC4_4200L)
    #define Power_ON_OFF_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Power_ON_OFF_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Power_ON_OFF_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Power_ON_OFF_DRIVE_MODE_SHIFT       (0x00u)
#define Power_ON_OFF_DRIVE_MODE_MASK        (0x07u << Power_ON_OFF_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Power_ON_OFF_H */


/* [] END OF FILE */
