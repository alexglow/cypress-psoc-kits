/*****************************************************************************
* File Name: accelerometer.c
*
* Version 1.0
*
* Description:
*  This file provides functions to configure the onboard accelerometer of the 
*  PSoC 4 M-Series Pioneer Kit. This file also defines function to read the 
*  acceleration data from the accelerometer.
******************************************************************************
* Copyright (2015), Cypress Semiconductor Corporation.
******************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and
* foreign), United States copyright laws and international treaty provisions.
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the
* Cypress Source Code and derivative works for the sole purpose of creating
* custom software in support of licensee product to be used only in conjunction
* with a Cypress integrated circuit as specified in the applicable agreement.
* Any reproduction, modification, translation, compilation, or representation of
* this software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the
* materials described herein. Cypress does not assume any liability arising out
* of the application or use of any product or circuit described herein. Cypress
* does not authorize its products for use as critical components in life-support
* systems where a malfunction or failure may reasonably be expected to result in
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of
* such use and in doing so indemnifies Cypress against all charges. Use may be
* limited by and subject to the applicable Cypress software license agreement.
*****************************************************************************/
#include "accelerometer.h"
#include "project.h"

/*****************************************************************************
* Local Function Prototypes
*****************************************************************************/
static uint8 Accelerometer_RegWrite(uint8 reg, uint8 value);


/*****************************************************************************
* Global Variable Declarations
*****************************************************************************/


/*****************************************************************************
* Local Global Variable Declarations
*****************************************************************************/


/*******************************************************************************
* Function Name: Accelerometer_RegWrite
********************************************************************************
* Summary:
* 	Write a single byte to a specified register location.
*
* Parameters:
*  uint8 reg: Register to which data is to be written.
*  uint8 *value: Variable which holds the data to be written.
*
* Return:
*  uint8: Status of write operation
*
*******************************************************************************/
static uint8 Accelerometer_RegWrite(uint8 reg, uint8 value)
{
	uint8 ackStatus = COMM_FAIL;
	ackStatus = I2C_I2CMasterSendStart(ACCELEROMETER_ADDR, ACC_WRITE);
	if(ackStatus == I2C_I2C_MSTR_NO_ERROR)
	{
		ackStatus = I2C_I2CMasterWriteByte(reg);
		ackStatus = I2C_I2CMasterWriteByte(value);
	}
	ackStatus = I2C_I2CMasterSendStop();
	return(ackStatus);
}

/*******************************************************************************
* Function Name: Accelerometer_RegRead
********************************************************************************
* Summary:
* 	Read a single byte from a specified register location.
*
* Parameters:
*  uint8 reg: Register from where data is to be read.
*  uint8 *value: Variable to which the data from register is stored.
*
* Return:
*  uint8: Status of read operation
*
*******************************************************************************/
uint8 Accelerometer_RegRead(uint8 reg, uint8 *value)
{
	uint8 ackStatus = COMM_FAIL;
	ackStatus = I2C_I2CMasterSendStart(ACCELEROMETER_ADDR, ACC_WRITE);
	if(ackStatus == I2C_I2C_MSTR_NO_ERROR)
	{
		ackStatus = I2C_I2CMasterWriteByte(reg);
		ackStatus = I2C_I2CMasterSendRestart(ACCELEROMETER_ADDR, ACC_READ);
		*value = I2C_I2CMasterReadByte(I2C_I2C_NAK_DATA);
	}
	ackStatus = I2C_I2CMasterSendStop();
	return(ackStatus);
}

/*******************************************************************************
* Function Name: Accelerometer_SelfTest
********************************************************************************
* Summary:
* 	Runs a test routine to make sure that the accelerometer communication is 
*   functional.
*
* Parameters:
*  void
*
* Return:
*  uint8: Communication functional status.
*
*******************************************************************************/
uint8 Accelerometer_SelfTest(void)
{
	uint8 accStatus = ACC_FAIL;
	uint8 accRegValue = ZERO;
	uint8 commStatus = COMM_FAIL;
	
	/* Read the initial value of the DCST register. This must be 0x55 at reset. */
	commStatus = Accelerometer_RegRead(ACC_DCST_RESP, &accRegValue);
	if(commStatus == COMM_PASS && accRegValue == ACC_SELF_TEST_VAL_1)
	{
		/* Set the PC1 bit of the CTRL_1 register of accelerometer to 0. 
		 * This puts the accelerometer in standby mode. */
		commStatus = Accelerometer_RegRead(ACC_CTRL_REG_1, &accRegValue);
		accRegValue = accRegValue & ACC_PC1_BIT_AND_MASK;
		commStatus = Accelerometer_RegWrite(ACC_CTRL_REG_1, accRegValue);
		
		/* Set the DCST bit of CTRL_2 register of the accelerometer to 
		 * 1 for self-test. */
		commStatus = Accelerometer_RegRead(ACC_CTRL_REG_2, &accRegValue);
		accRegValue = accRegValue | ACC_DCST_BIT_OR_MASK;
		commStatus = Accelerometer_RegWrite(ACC_CTRL_REG_2, accRegValue);
		
		/* Read the DCST value again. This must be 0xAA after the DCST bit
		 * is set to 1 in the CTRL_2 register. */
		commStatus = Accelerometer_RegRead(ACC_DCST_RESP, &accRegValue);
		if(commStatus == COMM_PASS && accRegValue == ACC_SELF_TEST_VAL_2)
		{
			/* Read the DCST value again. This must be 0x55 after the 
			 * register is read once. */
			commStatus = Accelerometer_RegRead(ACC_DCST_RESP, &accRegValue);
			if(commStatus == COMM_PASS && accRegValue == ACC_SELF_TEST_VAL_1)
			{
				accStatus = ACC_PASS;
			}
		}
	}
	return(accStatus);
}

/*******************************************************************************
* Function Name: InitializeAccelerometer
********************************************************************************
* Summary:
* 	Initializes the accelerometer control registers for proper operation.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void InitializeAccelerometer(void)
{
	uint8 accWorkStatus = ACC_FAIL;
	uint8 accRegValue = ZERO;
	uint8 commStatus = COMM_FAIL;
	
	/* Ensure proper functioning of hardware */
	accWorkStatus = Accelerometer_SelfTest();
	
	
	if(accWorkStatus == ACC_PASS)
	{
		/* Initialize the CTRL_REG1 after putting the accelerometer in standby mode. */
		accRegValue = accRegValue & ACC_PC1_BIT_AND_MASK;
		commStatus = Accelerometer_RegWrite(ACC_CTRL_REG_1, accRegValue);
		
		if(commStatus == COMM_PASS)
		{
			/* Modify the current value for the following settings:
			 * RES (Resolution) 								= 12-bit mode
			 * DRDYE (Report acceleration via Interrupt) 		= DISABLED
			 * GSEL1, GSEL0 (Acceleration range of the output) 	= +/-2g
			 * WUFE (Wake Up on Motion Detect) 					= ENABLED
			 */
			accRegValue = accRegValue | ACC_RES_BIT_OR_MASK | ACC_WUFE_BIT_OR_MASK;
			accRegValue = accRegValue & ACC_GSEL0_BIT_AND_MASK & ACC_GSEL1_BIT_AND_MASK & ACC_DRDYE_BIT_AND_MASK;
			
			/* Write back the modified value. */
			commStatus = Accelerometer_RegWrite(ACC_CTRL_REG_1, accRegValue);
		}
		else
		{
			commStatus = COMM_FAIL;
		}
		
		if(commStatus == COMM_PASS)
		{
			/* Initialize the CTRL_REG2 */
			/* Read the current value of the CTRL_REG2 register */
			commStatus = Accelerometer_RegRead(ACC_CTRL_REG_2, &accRegValue);
		}
		else
		{
			commStatus = COMM_FAIL;
		}
		
		if(commStatus == COMM_PASS)
		{
			/* Modify the current value for the following settings:
			 * SRST (Software Reset)							= FALSE
			 * DCST (Digital Communication Self Test)	 		= DISABLED
			 * OWUFA,OWUFB,OWUFC(Output Data Rate for Wakeup)	= 12.5 Hz
			 */
			accRegValue = accRegValue | ACC_OWUFA_BIT_OR_MASK;
			accRegValue = accRegValue & ACC_SRST_BIT_AND_MASK & ACC_DCST_BIT_AND_MASK & ACC_OWUFB_BIT_AND_MASK & ACC_OWUFC_BIT_AND_MASK;

			/* Write back the modified value. */
			commStatus = Accelerometer_RegWrite(ACC_CTRL_REG_2, accRegValue);
		}
		else
		{
			commStatus = COMM_FAIL;
		}
		
		if(commStatus == COMM_PASS)
		{
			/* Read the current value of the INT_CTRL_REG1 register */
			commStatus = Accelerometer_RegRead(ACC_INT_CTRL_REG_1, &accRegValue);
		}
		else
		{
			commStatus = COMM_FAIL;
		}
		
		if(commStatus == COMM_PASS)
		{
			/* Modify the current value for the following settings:
			 * IEN (Physical Interrupt Pin)						= ENABLED
			 * IEA (Interrupt Polarity)					 		= ACTIVE HIGH
			 * IEL (Interrupt Latch/Pulse Enable)				= LATCH INTERRUPT
			 */
			accRegValue = accRegValue | ACC_IEN_BIT_OR_MASK | ACC_IEA_BIT_OR_MASK;
			accRegValue = accRegValue & ACC_IEL_BIT_AND_MASK;
			
			/* Write back the modified value. */
			commStatus = Accelerometer_RegWrite(ACC_INT_CTRL_REG_1, accRegValue);
		}
		else
		{
			commStatus = COMM_FAIL;
		}
		
		if(commStatus == COMM_PASS)
		{
			/* Read the current value of the INT_CTRL_REG2 register */
			commStatus = Accelerometer_RegRead(ACC_INT_CTRL_REG_2, &accRegValue);
		}
		else
		{
			commStatus = COMM_FAIL;
		}
		
		if(commStatus == COMM_PASS)
		{
			/* Modify the current value for the following settings:
			 * XNWU, XPWU, YNWU, YPWU, ZNWU, ZPWU (Direction)	= SENSING_ENABLED
			 */
			accRegValue = accRegValue | ACC_XNWUE_BIT_OR_MASK | ACC_XPWUE_BIT_OR_MASK | \
			ACC_YNWUE_BIT_OR_MASK | ACC_YPWUE_BIT_OR_MASK | ACC_ZNWUE_BIT_OR_MASK | ACC_ZPWUE_BIT_OR_MASK;
			
			/* Write back the modified value. */
			commStatus = Accelerometer_RegWrite(ACC_INT_CTRL_REG_2, accRegValue);
		}
		else
		{
			commStatus = COMM_FAIL;
		}
		
		if(commStatus == COMM_PASS)
		{
			/* Read the current value of the DATA_CTRL register */
			commStatus = Accelerometer_RegRead(ACC_DATA_CTRL_REG, &accRegValue);
		}
		else
		{
			commStatus = COMM_FAIL;
		}
		
		if(commStatus == COMM_PASS)
		{
			/* Modify the current value for the following settings:
			 * OSAA, OSAB, OSAC, OSAD (Output Data Rate)		= 50 Hz
			 */
			accRegValue = accRegValue | ACC_OSAC_BIT_OR_MASK;
			accRegValue = accRegValue & ACC_OSAA_BIT_AND_MASK & ACC_OSAB_BIT_AND_MASK & ACC_OSAD_BIT_AND_MASK;
			
			/* Write back the modified value. */
			commStatus = Accelerometer_RegWrite(ACC_DATA_CTRL_REG, accRegValue);
		}
		else
		{
			commStatus = COMM_FAIL;
		}
		
		if(commStatus == COMM_PASS)
		{
			/* Read the current value of the WAKEUP_TIMER register */
			commStatus = Accelerometer_RegRead(ACC_WAKEUP_TIMER, &accRegValue);
		}
		else
		{
			commStatus = COMM_FAIL;
		}
		
		if(commStatus == COMM_PASS)
		{
			/* Modify the current value for the following settings:
			 * Wakeup Timer value	 = 0.02s * 50 = 1 count
			 * WAKEUP_TIMER (counts) = Desired Delay Time (sec) x OWUF (Hz) 
			 */
			accRegValue = ACC_WAKEUP_TIMER_VALUE;
			
			/* Write back the modified value. */
			commStatus = Accelerometer_RegWrite(ACC_WAKEUP_TIMER, accRegValue);
		}
		else
		{
			commStatus = COMM_FAIL;
		}
		
		if(commStatus == COMM_PASS)
		{
			/* Read the current value of the ACC_WAKUP_THRESHOLD register */
			commStatus = Accelerometer_RegRead(ACC_WAKUP_THRESHOLD, &accRegValue);
		}
		else
		{
			commStatus = COMM_FAIL;
		}
		
		if(commStatus == COMM_PASS)
		{
			/* Modify the current value for the following settings:
			 * Wakeup Timer value								= (1/16)g * 16 = 1 count
			 * WAKEUP_THRESHOLD (counts) 						= Desired Threshold (g) x 16 (counts/g) 
			 */
			accRegValue = ACC_WAKUP_THRESHOLD_VAL;
			
			/* Write back the modified value. */
			commStatus = Accelerometer_RegWrite(ACC_WAKUP_THRESHOLD, accRegValue);
		}
		else
		{
			commStatus = COMM_FAIL;
		}
		
		if(commStatus == COMM_PASS)
		{
			/* Read the current value of the CTRL_REG1 register */
			commStatus = Accelerometer_RegRead(ACC_CTRL_REG_1, &accRegValue);
		}
		else
		{
			commStatus = COMM_FAIL;
		}
		
		if(commStatus == COMM_PASS)
		{
			/* Put the Accelerometer in the Operating Mode */
			accRegValue = accRegValue | ACC_PC1_BIT_OR_MASK;
					
			/* Write back the modified value. */
			commStatus = Accelerometer_RegWrite(ACC_CTRL_REG_1, accRegValue);
		}
		else
		{
			commStatus = COMM_FAIL;
		}
		
		if(commStatus == COMM_PASS)
		{
			/* Set the flag to indicate the accelerometer is configured. */
			acclerometer_configured = TRUE;
			
			/* Clear all pending interrupts from accelerometer. */
			Accelerometer_RegRead(ACC_INT_REL, &accRegValue);
		}
	}	
}

/*******************************************************************************
* Function Name: ReadAccelerometer
********************************************************************************
* Summary:
* 	Reads the accelerometer acceleration values in x, y, and z directions.
*
* Parameters:
*  int16 *xData: Pointer to the variable in which the acceleration data 
*                for x-direction is to be stored. 
*
*  int16 *yData: Pointer to the variable in which the acceleration data 
*                for y-direction is to be stored. 
*
*  int16 *zData: Pointer to the variable in which the acceleration data 
*                for z-direction is to be stored. 
*
* Return:
*  void
*
*******************************************************************************/
void ReadAccelerometer(int16 *xData, int16 *yData, int16 *zData)
{
	/* Temporary variable for extracting only the 12 most significant
	 * bits of accelerometer data registers. */
	uint8 tempVal = ZERO;
	
	Accelerometer_RegRead(ACC_XOUT_H, &tempVal);
	*xData = tempVal;
	Accelerometer_RegRead(ACC_XOUT_L, &tempVal);
	*xData = (*xData << 8) | tempVal;
	if((*xData & 0x8000) == 0x8000)
	{
		*xData = *xData >> 4;
		*xData |= 0xF000;
	}
	else
	{
		*xData = *xData >> 4;
	}
	
	Accelerometer_RegRead(ACC_YOUT_H, &tempVal);
	*yData = tempVal;
	Accelerometer_RegRead(ACC_YOUT_L, &tempVal);
	*yData = (*yData << 8) | tempVal;
	
	if((*yData & 0x8000) == 0x8000)
	{
		*yData = *yData >> 4;
		*yData |= 0xF000;
	}
	else
	{
		*yData = *yData >> 4;
	}
	
	Accelerometer_RegRead(ACC_ZOUT_H, &tempVal);
	*zData = tempVal;
	Accelerometer_RegRead(ACC_ZOUT_L, &tempVal);
	*zData = (*zData << 8) | tempVal;
	
	if((*zData & 0x8000) == 0x8000)
	{
		*zData = *zData >> 4;
		*zData |= 0xF000;
	}
	else
	{
		*zData = *zData >> 4;
	}
}

/* [] END OF FILE */

