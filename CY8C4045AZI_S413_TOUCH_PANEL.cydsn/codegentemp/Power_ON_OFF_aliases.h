/*******************************************************************************
* File Name: Power_ON_OFF.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Power_ON_OFF_ALIASES_H) /* Pins Power_ON_OFF_ALIASES_H */
#define CY_PINS_Power_ON_OFF_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Power_ON_OFF_0			(Power_ON_OFF__0__PC)
#define Power_ON_OFF_0_PS		(Power_ON_OFF__0__PS)
#define Power_ON_OFF_0_PC		(Power_ON_OFF__0__PC)
#define Power_ON_OFF_0_DR		(Power_ON_OFF__0__DR)
#define Power_ON_OFF_0_SHIFT	(Power_ON_OFF__0__SHIFT)
#define Power_ON_OFF_0_INTR	((uint16)((uint16)0x0003u << (Power_ON_OFF__0__SHIFT*2u)))

#define Power_ON_OFF_INTR_ALL	 ((uint16)(Power_ON_OFF_0_INTR))


#endif /* End Pins Power_ON_OFF_ALIASES_H */


/* [] END OF FILE */
