/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2012  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.16 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIDRV_IST3088.h
Purpose     : Interface definition for GUIDRV_IST3088 driver
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUIDRV_IST3088_H
#define GUIDRV_IST3088_H

/*********************************************************************
*
*       Display drivers
*/
//
// Addresses
//
extern const GUI_DEVICE_API GUIDRV_IST3088_4_API;
extern const GUI_DEVICE_API GUIDRV_IST3088_OY_4_API;
extern const GUI_DEVICE_API GUIDRV_IST3088_OX_4_API;
extern const GUI_DEVICE_API GUIDRV_IST3088_OXY_4_API;
extern const GUI_DEVICE_API GUIDRV_IST3088_OS_4_API;
extern const GUI_DEVICE_API GUIDRV_IST3088_OSY_4_API;
extern const GUI_DEVICE_API GUIDRV_IST3088_OSX_4_API;
extern const GUI_DEVICE_API GUIDRV_IST3088_OSXY_4_API;

//
// Macros to be used in configuration files
//
#if defined(WIN32) && !defined(LCD_SIMCONTROLLER)

  #define GUIDRV_IST3088_4       &GUIDRV_Win_API
  #define GUIDRV_IST3088_OY_4    &GUIDRV_Win_API
  #define GUIDRV_IST3088_OX_4    &GUIDRV_Win_API
  #define GUIDRV_IST3088_OXY_4   &GUIDRV_Win_API
  #define GUIDRV_IST3088_OS_4    &GUIDRV_Win_API
  #define GUIDRV_IST3088_OSY_4   &GUIDRV_Win_API
  #define GUIDRV_IST3088_OSX_4   &GUIDRV_Win_API
  #define GUIDRV_IST3088_OSXY_4  &GUIDRV_Win_API

#else

  #define GUIDRV_IST3088_4       &GUIDRV_IST3088_4_API
  #define GUIDRV_IST3088_OY_4    &GUIDRV_IST3088_OY_4_API
  #define GUIDRV_IST3088_OX_4    &GUIDRV_IST3088_OX_4_API
  #define GUIDRV_IST3088_OXY_4   &GUIDRV_IST3088_OXY_4_API
  #define GUIDRV_IST3088_OS_4    &GUIDRV_IST3088_OS_4_API
  #define GUIDRV_IST3088_OSY_4   &GUIDRV_IST3088_OSY_4_API
  #define GUIDRV_IST3088_OSX_4   &GUIDRV_IST3088_OSX_4_API
  #define GUIDRV_IST3088_OSXY_4  &GUIDRV_IST3088_OSXY_4_API

#endif

/*********************************************************************
*
*       Public routines
*/
void GUIDRV_IST3088_SetBus16(GUI_DEVICE * pDevice, GUI_PORT_API * pHW_API);

#endif

/*************************** End of file ****************************/
