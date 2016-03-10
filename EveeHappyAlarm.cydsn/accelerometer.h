/*****************************************************************************
* File Name: accelerometer.h
* Version 1.0
*
* Description:
*  This file contains the function prototypes and constants used in
*  accelerometer.c
*
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
#if !defined(ACCELEROMETER_H) 
#define ACCELEROMETER_H 

#include "cytypes.h"

/*****************************************************************************
* MACRO Definition
*****************************************************************************/
#define G2_MODE					0x00
#define G4_MODE					0x01
#define G8_MODE					0x02

#define G2_MODE_RESOLUTION		0x400
#define G4_MODE_RESOLUTION		0x200
#define G8_MODE_RESOLUTION		0x100

#define ACCELEROMETER_MODE		G2_MODE

#define ACCELEROMETER_ADDR		0x0F

#define ACC_XOUT_L				0x06
#define ACC_XOUT_H				0x07

#define ACC_YOUT_L				0x08
#define ACC_YOUT_H				0x09

#define ACC_ZOUT_L				0x0A
#define ACC_ZOUT_H				0x0B

#define ACC_DCST_RESP			0x0C

#define ACC_WHO_AM_I			0x0F

#define ACC_INT_SRC_1			0x16
#define ACC_INT_SRC_2			0x17

#define ACC_STATUS_REG			0x18

#define ACC_INT_REL 			0x1A

/* PC1 bit in the ACC_CTRL_REG_1 must be set to '0' before changing the 
 * contents of the following registers (ACC_CTRL_REG_1 to ACC_WAKEUP_TIMER). */
#define ACC_CTRL_REG_1			0x1B
#define ACC_CTRL_REG_2			0x1D
#define ACC_INT_CTRL_REG_1		0x1E
#define ACC_INT_CTRL_REG_2		0x1F
#define ACC_DATA_CTRL_REG		0x21
#define ACC_WAKEUP_TIMER		0x29

#define ACC_SELF_TEST			0x3A
#define ACC_WAKUP_THRESHOLD		0x6A

#define ACC_WRITE				0x00
#define ACC_READ				0x01

#define ACC_ACK					0x00
#define ACC_NACK				0x01

#define ACC_FAIL				0x00
#define ACC_PASS				0x01

#define COMM_FAIL				0x01
#define COMM_PASS				0x00

#define ACC_SELF_TEST_VAL_1		0x55
#define ACC_SELF_TEST_VAL_2		0xAA

#define ACC_PC1_BIT_OR_MASK		0x80
#define ACC_RES_BIT_OR_MASK		0x40
#define ACC_DRDYE_BIT_OR_MASK	0x20
#define ACC_GSEL1_BIT_OR_MASK	0x10
#define ACC_GSEL0_BIT_OR_MASK	0x08
#define ACC_WUFE_BIT_OR_MASK	0x02

#define ACC_PC1_BIT_AND_MASK	0x7F
#define ACC_RES_BIT_AND_MASK	0xBF
#define ACC_DRDYE_BIT_AND_MASK	0xDF
#define ACC_GSEL1_BIT_AND_MASK	0xEF
#define ACC_GSEL0_BIT_AND_MASK	0xF7
#define ACC_WUFE_BIT_AND_MASK	0xFD

#define ACC_SRST_BIT_OR_MASK	0x80
#define ACC_DCST_BIT_OR_MASK	0x10
#define ACC_OWUFA_BIT_OR_MASK	0x04
#define ACC_OWUFB_BIT_OR_MASK	0x02
#define ACC_OWUFC_BIT_OR_MASK	0x01

#define ACC_SRST_BIT_AND_MASK	0x7F
#define ACC_DCST_BIT_AND_MASK	0xEF
#define ACC_OWUFA_BIT_AND_MASK	0xFB
#define ACC_OWUFB_BIT_AND_MASK	0xFD
#define ACC_OWUFC_BIT_AND_MASK	0xFE

#define ACC_IEN_BIT_OR_MASK		0x20
#define ACC_IEA_BIT_OR_MASK		0x10
#define ACC_IEL_BIT_OR_MASK		0x08

#define ACC_IEN_BIT_AND_MASK	0xDF
#define ACC_IEA_BIT_AND_MASK	0xEF
#define ACC_IEL_BIT_AND_MASK	0xF7

#define ACC_XNWUE_BIT_OR_MASK	0x20
#define ACC_XPWUE_BIT_OR_MASK	0x10
#define ACC_YNWUE_BIT_OR_MASK	0x08
#define ACC_YPWUE_BIT_OR_MASK	0x04
#define ACC_ZNWUE_BIT_OR_MASK	0x02
#define ACC_ZPWUE_BIT_OR_MASK	0x01

#define ACC_XNWUE_BIT_AND_MASK	0xDF
#define ACC_XPWUE_BIT_AND_MASK	0xEF
#define ACC_YNWUE_BIT_AND_MASK	0xF7
#define ACC_YPWUE_BIT_AND_MASK	0xFB
#define ACC_ZNWUE_BIT_AND_MASK	0xFD
#define ACC_ZPWUE_BIT_AND_MASK	0xFE

#define ACC_OSAA_BIT_OR_MASK	0x08
#define ACC_OSAB_BIT_OR_MASK	0x04
#define ACC_OSAC_BIT_OR_MASK	0x02
#define ACC_OSAD_BIT_OR_MASK	0x01

#define ACC_OSAA_BIT_AND_MASK	0xF7
#define ACC_OSAB_BIT_AND_MASK	0xFB
#define ACC_OSAC_BIT_AND_MASK	0xFD
#define ACC_OSAD_BIT_AND_MASK	0xFE

#define ACC_WAKEUP_TIMER_VALUE	0x01
#define ACC_WAKUP_THRESHOLD_VAL	0x01

#define ACC_NO_MOVEMENT			0x0000

#define ZERO					0x00	
	
#define TRUE					0x01
#define FALSE					0x00
/*****************************************************************************
* Data Type Definition
*****************************************************************************/


/*****************************************************************************
* Enumerated Data Definition
*****************************************************************************/


/*****************************************************************************
* Data Structure Definition
*****************************************************************************/


/*****************************************************************************
* Global Variable Declaration
*****************************************************************************/
extern uint8 acclerometer_configured;

/*****************************************************************************
* Function Prototypes
*****************************************************************************/
void InitializeAccelerometer(void);
void ReadAccelerometer(int16 *xData, int16 *yData, int16 *zData);
uint8 Accelerometer_RegRead(uint8 reg, uint8 *value);

/*****************************************************************************
* External Function Prototypes
*****************************************************************************/
	
	
#endif /* ACCELEROMETER_H */

/* [] END OF FILE */

