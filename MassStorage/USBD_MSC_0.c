/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Device:MSC
 * Copyright (c) 2004-2024 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    USBD_MSC_0.c
 * Purpose: Functions for media ownership control between USB and File System
 * Rev.:    V6.3.7
 *----------------------------------------------------------------------------*/
/*
 * USBD_MSC_0.c is a code template for the application specific functionality of
 * the USB Device MSC class 0 instance. It implements the ownership control over
 * media and changing the owner of media between USB and File System.
 *
 * USBD_MSC_0.h is the related header file.
 *
 * To select USB as owner of media you can call function:
 *   USBD_MSC0_SetMediaOwnerUSB ()
 *
 * To select File System as owner of media you can call function:
 *   USBD_MSC0_SetMediaOwnerFS  ()
 *
 */
 
#include "rl_usb.h"
 
#include "USBD_MSC_0.h"                 // Media ownership control for USB Device
#include "USBD_Config_MSC_0.h"
 
#ifndef  RTE_CMSIS_RTOS2
#error   This user template requires CMSIS-RTOS2!
#else
 
extern volatile uint8_t usbd_msc0_media_own;    // USB MSC0 media ownership
 
 
//! [usbd_msc_setmediaownerusb]
/// \brief Set USB as media owner
/// \return     execution status
///               - USBD_MSC_OK           = Media ownership changed successfully
///               - USBD_MSC_ERROR        = Media ownership change has failed (due timeout)
int32_t USBD_MSC0_SetMediaOwnerUSB (void) {
  uint32_t timeout_cnt;
 
  timeout_cnt = 300U;                   // 3 second timeout (300 * 10 ms)
  usbd_msc0_media_own = USBD_MSC0_MEDIA_OWN_CHG | USBD_MSC0_MEDIA_OWN_USB;
  while (usbd_msc0_media_own & USBD_MSC0_MEDIA_OWN_CHG) {
    (void)osDelay(10);
    if ((--timeout_cnt) == 0) { return USBD_MSC0_ERROR; }
  }
 
  return USBD_MSC0_OK;
}
//! [usbd_msc_setmediaownerusb]
 
 
//! [usbd_msc_setmediaownerfs]
/// \brief Set File System as media owner
/// \return     execution status
///               - USBD_MSC_OK           = Media ownership changed successfully
///               - USBD_MSC_ERROR        = Media ownership change has failed (due timeout)
int32_t USBD_MSC0_SetMediaOwnerFS (void) {
  uint32_t timeout_cnt;
 
  timeout_cnt = 300U;                   // 3 second timeout (300 * 10 ms)
  usbd_msc0_media_own = USBD_MSC0_MEDIA_OWN_CHG;
  while (usbd_msc0_media_own & USBD_MSC0_MEDIA_OWN_CHG) {
    if(!USBD_Configured(USBD_MSC0_DEV)) {
      /* USB device not configured, so call CheckMedia to do ownership handling */
      (void)USBD_MSC0_CheckMedia();
    }
    (void)osDelay(10);
    if ((--timeout_cnt) == 0) { return USBD_MSC0_ERROR; }
  }
 
  return USBD_MSC0_OK;
}
//! [usbd_msc_setmediaownerfs]

#endif // RTE_CMSIS_RTOS2
