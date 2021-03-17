/*******************************************************************************
* File Name: Vol_Decrease.c  
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
#include "Vol_Decrease.h"

static Vol_Decrease_BACKUP_STRUCT  Vol_Decrease_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Vol_Decrease_Sleep
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
*  \snippet Vol_Decrease_SUT.c usage_Vol_Decrease_Sleep_Wakeup
*******************************************************************************/
void Vol_Decrease_Sleep(void)
{
    #if defined(Vol_Decrease__PC)
        Vol_Decrease_backup.pcState = Vol_Decrease_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Vol_Decrease_backup.usbState = Vol_Decrease_CR1_REG;
            Vol_Decrease_USB_POWER_REG |= Vol_Decrease_USBIO_ENTER_SLEEP;
            Vol_Decrease_CR1_REG &= Vol_Decrease_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Vol_Decrease__SIO)
        Vol_Decrease_backup.sioState = Vol_Decrease_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Vol_Decrease_SIO_REG &= (uint32)(~Vol_Decrease_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Vol_Decrease_Wakeup
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
*  Refer to Vol_Decrease_Sleep() for an example usage.
*******************************************************************************/
void Vol_Decrease_Wakeup(void)
{
    #if defined(Vol_Decrease__PC)
        Vol_Decrease_PC = Vol_Decrease_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Vol_Decrease_USB_POWER_REG &= Vol_Decrease_USBIO_EXIT_SLEEP_PH1;
            Vol_Decrease_CR1_REG = Vol_Decrease_backup.usbState;
            Vol_Decrease_USB_POWER_REG &= Vol_Decrease_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Vol_Decrease__SIO)
        Vol_Decrease_SIO_REG = Vol_Decrease_backup.sioState;
    #endif
}


/* [] END OF FILE */
