/*******************************************************************************
* File Name: Power_ON_OFF.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "Power_ON_OFF.h"

static Power_ON_OFF_BACKUP_STRUCT  Power_ON_OFF_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Power_ON_OFF_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet Power_ON_OFF_SUT.c usage_Power_ON_OFF_Sleep_Wakeup
*******************************************************************************/
void Power_ON_OFF_Sleep(void)
{
    #if defined(Power_ON_OFF__PC)
        Power_ON_OFF_backup.pcState = Power_ON_OFF_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Power_ON_OFF_backup.usbState = Power_ON_OFF_CR1_REG;
            Power_ON_OFF_USB_POWER_REG |= Power_ON_OFF_USBIO_ENTER_SLEEP;
            Power_ON_OFF_CR1_REG &= Power_ON_OFF_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Power_ON_OFF__SIO)
        Power_ON_OFF_backup.sioState = Power_ON_OFF_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Power_ON_OFF_SIO_REG &= (uint32)(~Power_ON_OFF_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Power_ON_OFF_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to Power_ON_OFF_Sleep() for an example usage.
*******************************************************************************/
void Power_ON_OFF_Wakeup(void)
{
    #if defined(Power_ON_OFF__PC)
        Power_ON_OFF_PC = Power_ON_OFF_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Power_ON_OFF_USB_POWER_REG &= Power_ON_OFF_USBIO_EXIT_SLEEP_PH1;
            Power_ON_OFF_CR1_REG = Power_ON_OFF_backup.usbState;
            Power_ON_OFF_USB_POWER_REG &= Power_ON_OFF_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Power_ON_OFF__SIO)
        Power_ON_OFF_SIO_REG = Power_ON_OFF_backup.sioState;
    #endif
}


/* [] END OF FILE */
