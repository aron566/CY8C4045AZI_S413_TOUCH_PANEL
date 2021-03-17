/*******************************************************************************
* File Name: Vol_Decrease.h  
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

#if !defined(CY_PINS_Vol_Decrease_ALIASES_H) /* Pins Vol_Decrease_ALIASES_H */
#define CY_PINS_Vol_Decrease_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Vol_Decrease_0			(Vol_Decrease__0__PC)
#define Vol_Decrease_0_PS		(Vol_Decrease__0__PS)
#define Vol_Decrease_0_PC		(Vol_Decrease__0__PC)
#define Vol_Decrease_0_DR		(Vol_Decrease__0__DR)
#define Vol_Decrease_0_SHIFT	(Vol_Decrease__0__SHIFT)
#define Vol_Decrease_0_INTR	((uint16)((uint16)0x0003u << (Vol_Decrease__0__SHIFT*2u)))

#define Vol_Decrease_INTR_ALL	 ((uint16)(Vol_Decrease_0_INTR))


#endif /* End Pins Vol_Decrease_ALIASES_H */


/* [] END OF FILE */
