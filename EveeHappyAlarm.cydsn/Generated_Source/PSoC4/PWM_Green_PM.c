/*******************************************************************************
* File Name: PWM_Green_PM.c
* Version 2.0
*
* Description:
*  This file contains the setup, control, and status commands to support
*  the component operations in the low power mode.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PWM_Green.h"

static PWM_Green_BACKUP_STRUCT PWM_Green_backup;


/*******************************************************************************
* Function Name: PWM_Green_SaveConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to save here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM_Green_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: PWM_Green_Sleep
********************************************************************************
*
* Summary:
*  Stops the component operation and saves the user configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM_Green_Sleep(void)
{
    if(0u != (PWM_Green_BLOCK_CONTROL_REG & PWM_Green_MASK))
    {
        PWM_Green_backup.enableState = 1u;
    }
    else
    {
        PWM_Green_backup.enableState = 0u;
    }

    PWM_Green_Stop();
    PWM_Green_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_Green_RestoreConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to restore here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM_Green_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: PWM_Green_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and restores the enable state.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM_Green_Wakeup(void)
{
    PWM_Green_RestoreConfig();

    if(0u != PWM_Green_backup.enableState)
    {
        PWM_Green_Enable();
    }
}


/* [] END OF FILE */
