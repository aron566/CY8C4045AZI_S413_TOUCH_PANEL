/*******************************************************************************
* File Name: Set_Mode.h  
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

#if !defined(CY_PINS_Set_Mode_ALIASES_H) /* Pins Set_Mode_ALIASES_H */
#define CY_PINS_Set_Mode_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Set_Mode_0			(Set_Mode__0__PC)
#define Set_Mode_0_PS		(Set_Mode__0__PS)
#define Set_Mode_0_PC		(Set_Mode__0__PC)
#define Set_Mode_0_DR		(Set_Mode__0__DR)
#define Set_Mode_0_SHIFT	(Set_Mode__0__SHIFT)
#define Set_Mode_0_INTR	((uint16)((uint16)0x0003u << (Set_Mode__0__SHIFT*2u)))

#define Set_Mode_INTR_ALL	 ((uint16)(Set_Mode_0_INTR))


#endif /* End Pins Set_Mode_ALIASES_H */


/* [] END OF FILE */
