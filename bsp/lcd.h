/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : lcd.h
* Author             : MCD Application Team
* Date First Issued  : mm/dd/yyyy
* Description        : This file contains all the functions prototypes for the
*                      lcd software driver.
********************************************************************************
* History:
* mm/dd/yyyy
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_H
#define __LCD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* LCD Registers */
#define R000            0x000 /*ID Read*/
#define R001            0x001 /*Driver Output Control */
#define R002            0x002 /*LCD Drive Waveform Control */
#define R003            0x003 /*Entry Mode */
#define R007            0x007 /*Display Control (1) */
#define R008            0x008 /*Display Control (2) */
#define R009            0x009 /*Low Power Control (1) */
#define R00B            0x00B /*Low Power Control (2) */
#define R00C            0x00C /*External Display Controll (1) */
#define R00F            0x00F /*External Display Controll (2) */
#define R010            0x010 /*Panel Interface Control 1 */
#define R011            0x011 /*Panel Interface Control 2 */
#define R012            0x012 /*Panel Interface Control 3 */
#define R020            0x020 /*Panel Interface Control 4 */
#define R021            0x021 /*Panel Interface Control 5 */
#define R022            0x022 /*Panel Interface Control 6 */
#define R090            0x090 /*Frame Marker Control */
#define R100            0x100 /*Power Control(1) */
#define R101            0x101 /*Power Control(2) */
#define R102            0x102 /*Power Control(3) */
#define R103            0x103 /*Power Control(4) */
#define R107            0x107 /*Power Control(5) */
#define R110            0x110 /*Power Control(6) */
#define R200            0x200 /*GRAM address Set Horizontal Address */
#define R201            0x201 /*GRAM address Set Vertical Address */
#define R202            0x202 /*Write Data to GRAM Read Data from GRAM */
#define R210            0x210 /*Window Horzontal RAM Address Start */
#define R211            0x211 /*Window Horzontal RAM Address End */
#define R212            0x212 /*Window Vertical RAM Address Start */
#define R213            0x213 /*Window Vertical RAM Address End */
#define R280            0x280 /*NVM Write/Read */
#define R281            0x281 /*VCom high voltage 1 */
#define R282            0x282 /*VCom high voltage 2 */
#define R300            0x300 /*Control (1) */
#define R301            0x301 /*Control (2) */
#define R302            0x302 /*Control (3) */
#define R303            0x303 /*Control (4) */
#define R304            0x304 /*Control (5) */
#define R305            0x305 /*Control (6) */
#define R306            0x306 /*Control (7) */
#define R307            0x307 /*Control (8) */
#define R308            0x308 /*Control (9) */
#define R309            0x309 /*Control (10) */
#define R30A            0x30A /*Control (11) */
#define R30B            0x30B /*Control (12) */
#define R30C            0x30C /*Control (13) */
#define R30D            0x30D /*Control (14) */
#define R30E            0x30E /*Control (15) */
#define R30F            0x30F /*Control (16) */
#define R400            0x400 /*Size of base image */
#define R401            0x401 /*Base image display control */
#define R404            0x404 /*Vertical Scoll Control */
#define R500            0x500 /*Display Position Partial Display 1*/
#define R501            0x501 /*RAM Address Start Partial Display 1*/
#define R502            0x502 /*RAM Address End Partial Display 1*/
#define R503            0x503 /*Display Position Partial Display 2*/
#define R504            0x504 /*RAM Address Start Partial Display 2*/
#define R505            0x505 /*RAM Address End Partial Display 2*/
#define R600            0x600
#define R606            0x606 /*i80-I/F Endian Control*/
#define R6F0            0x6F0 /*NVM access control*/
#define R7F0            0x7F0
#define R7DE            0x7DE
#define R7F2            0x7F2
#define R7F3            0x7F3
#define R7F4            0x7F4
#define R7F5            0x7F5


/* LCD Control pins */
#define CtrlPin_NCS    GPIO_Pin_12   /* PG.12 */
#define CtrlPin_RS     GPIO_Pin_0   /* PF.00 */
#define CtrlPin_NWR    GPIO_Pin_5  /* PD.05 */

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Blue           0x001F
#define Orange         0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

#define Line0          0
#define Line1          24
#define Line2          48
#define Line3          72
#define Line4          96
#define Line5          120
#define Line6          144
#define Line7          168
#define Line8          192
#define Line9          216

#define Horizontal     0x00
#define Vertical       0x01

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/*----- High layer function -----*/
void LCD_Init(void);
void LCD_SetTextColor(__IO uint16_t Color);
void LCD_SetBackColor(__IO uint16_t Color);
void LCD_ClearLine(uint8_t Line);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t Xpos, uint16_t Ypos);
void LCD_DrawChar(uint8_t Xpos, uint16_t Ypos, const uint16_t *c);
void LCD_DisplayChar(uint8_t Line, uint16_t Column, uint8_t Ascii);
void LCD_DisplayStringLine(uint8_t Line, uint8_t *ptr);
void LCD_DisplayString(uint8_t Line, uint8_t *ptr);
void LCD_ScrollText(uint8_t Line, uint8_t *ptr);
void LCD_SetDisplayWindow(uint8_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width);
void LCD_DrawLine(uint8_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction);
void LCD_DrawRect(uint8_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width);
void LCD_DrawCircle(uint8_t Xpos, uint16_t Ypos, uint16_t Radius);
void LCD_DrawMonoPict(const uint32_t *Pict);
void LCD_DrawBMP(uint32_t BmpAddress);

/*----- Medium layer function -----*/
void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
uint16_t LCD_ReadReg(uint16_t LCD_Reg);
void LCD_WriteRAM(uint16_t RGB_Code);
uint16_t  LCD_ReadRAM(void);
void LCD_PowerOn(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);

/*----- Low layer function -----*/
void LCD_CtrlLinesConfig(void);
void LCD_CtrlLinesWrite(GPIO_TypeDef* GPIOx, uint16_t CtrlPins, BitAction BitVal);
void LCD_SPIConfig(void);
static void LCD_GPIOConfig(void);
static void LCD_FSMCConfig(void);

#endif /* __LCD_H */

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
