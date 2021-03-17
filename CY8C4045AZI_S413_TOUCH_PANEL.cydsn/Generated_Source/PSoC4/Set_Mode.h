/*******************************************************************************
* File Name: Set_Mode.h  
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

#if !defined(CY_PINS_Set_Mode_H) /* Pins Set_Mode_H */
#define CY_PINS_Set_Mode_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Set_Mode_aliases.h"


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
} Set_Mode_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Set_Mode_Read(void);
void    Set_Mode_Write(uint8 value);
uint8   Set_Mode_ReadDataReg(void);
#if defined(Set_Mode__PC) || (CY_PSOC4_4200L) 
    void    Set_Mode_SetDriveMode(uint8 mode);
#endif
void    Set_Mode_SetInterruptMode(uint16 position, uint16 mode);
uint8   Set_Mode_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Set_Mode_Sleep(void); 
void Set_Mode_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Set_Mode__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Set_Mode_DRIVE_MODE_BITS        (3)
    #define Set_Mode_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Set_Mode_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Set_Mode_SetDriveMode() function.
         *  @{
         */
        #define Set_Mode_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Set_Mode_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Set_Mode_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Set_Mode_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Set_Mode_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Set_Mode_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Set_Mode_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Set_Mode_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Set_Mode_MASK               Set_Mode__MASK
#define Set_Mode_SHIFT              Set_Mode__SHIFT
#define Set_Mode_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Set_Mode_SetInterruptMode() function.
     *  @{
     */
        #define Set_Mode_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Set_Mode_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Set_Mode_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Set_Mode_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Set_Mode__SIO)
    #define Set_Mode_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Set_Mode__PC) && (CY_PSOC4_4200L)
    #define Set_Mode_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Set_Mode_USBIO_DISABLE              ((uint32)(~Set_Mode_USBIO_ENABLE))
    #define Set_Mode_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Set_Mode_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Set_Mode_USBIO_ENTER_SLEEP          ((uint32)((1u << Set_Mode_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Set_Mode_USBIO_SUSPEND_DEL_SHIFT)))
    #define Set_Mode_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Set_Mode_USBIO_SUSPEND_SHIFT)))
    #define Set_Mode_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Set_Mode_USBIO_SUSPEND_DEL_SHIFT)))
    #define Set_Mode_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Set_Mode__PC)
    /* Port Configuration */
    #define Set_Mode_PC                 (* (reg32 *) Set_Mode__PC)
#endif
/* Pin State */
#define Set_Mode_PS                     (* (reg32 *) Set_Mode__PS)
/* Data Register */
#define Set_Mode_DR                     (* (reg32 *) Set_Mode__DR)
/* Input Buffer Disable Override */
#define Set_Mode_INP_DIS                (* (reg32 *) Set_Mode__PC2)

/* Interrupt configuration Registers */
#define Set_Mode_INTCFG                 (* (reg32 *) Set_Mode__INTCFG)
#define Set_Mode_INTSTAT                (* (reg32 *) Set_Mode__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Set_Mode_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Set_Mode__SIO)
    #define Set_Mode_SIO_REG            (* (reg32 *) Set_Mode__SIO)
#endif /* (Set_Mode__SIO_CFG) */

/* USBIO registers */
#if !defined(Set_Mode__PC) && (CY_PSOC4_4200L)
    #define Set_Mode_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Set_Mode_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Set_Mode_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Set_Mode_DRIVE_MODE_SHIFT       (0x00u)
#define Set_Mode_DRIVE_MODE_MASK        (0x07u << Set_Mode_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Set_Mode_H */


/* [] END OF FILE */
