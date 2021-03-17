/*******************************************************************************
* File Name: Vol_Increase.c  
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
#include "Vol_Increase.h"

static Vol_Increase_BACKUP_STRUCT  Vol_Increase_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Vol_Increase_Sleep
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
*  \snippet Vol_Increase_SUT.c usage_Vol_Increase_Sleep_Wakeup
*******************************************************************************/
void Vol_Increase_Sleep(void)
{
    #if defined(Vol_Increase__PC)
        Vol_Increase_backup.pcState = Vol_Increase_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Vol_Increase_backup.usbState = Vol_Increase_CR1_REG;
            Vol_Increase_USB_POWER_REG |= Vol_Increase_USBIO_ENTER_SLEEP;
            Vol_Increase_CR1_REG &= Vol_Increase_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Vol_Increase__SIO)
        Vol_Increase_backup.sioState = Vol_Increase_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Vol_Increase_SIO_REG &= (uint32)(~Vol_Increase_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Vol_Increase_Wakeup
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
*  Refer to Vol_Increase_Sleep() for an example usage.
*******************************************************************************/
void Vol_Increase_Wakeup(void)
{
    #if defined(Vol_Increase__PC)
        Vol_Increase_PC = Vol_Increase_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Vol_Increase_USB_POWER_REG &= Vol_Increase_USBIO_EXIT_SLEEP_PH1;
            Vol_Increase_CR1_REG = Vol_Increase_backup.usbState;
            Vol_Increase_USB_POWER_REG &= Vol_Increase_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Vol_Increase__SIO)
        Vol_Increase_SIO_REG = Vol_Increase_backup.sioState;
    #endif
}


/* [] END OF FILE */
