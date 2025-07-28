/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Device
 * Copyright (c) 2004-2025 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    MassStorage.c
 * Purpose: USB Device - Mass Storage example
 *----------------------------------------------------------------------------*/

#include <stdio.h>

#include "main.h"

#include "cmsis_os2.h"

#include "rl_usb.h"
#include "rl_fs.h"
#include "cmsis_vio.h" 
#include "USBD_MSC_0.h"

// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (4096U)
static uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
static const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

/*-----------------------------------------------------------------------------
 * Application main thread
 *----------------------------------------------------------------------------*/
void app_main_thread (void *argument) {
  uint32_t state;
  uint32_t last = 0U;
  FILE *f ;
  int32_t i, n ;
  uint8_t buf[8U];
  
  (void)argument;

  printf("USB Device Mass Storage example\n");

  finit          ("M0:");               // Initialize SD Card 0
  fmount         ("M0:");               // Mount SD Card 0
  USBD_Initialize(0U);                  // USB Device 0 Initialization
  USBD_Connect   (0U);                  // USB Device 0 Connect

  USBD_MSC0_SetMediaOwnerUSB( );
 
  vioInit();
  for (;;) {
    state = (vioGetSignal (vioBUTTON0)); // Get pressed button state
    if (state != last){
      if (state == vioBUTTON0){
        // do something
        USBD_MSC0_SetMediaOwnerFS( ) ;
        f = fopen( "Test.txt", "r" ) ;
        if( NULL != f ) {
          while( 0 < ( n = fread( buf, 1, sizeof( buf ) - 1, f ) ) ) {
            buf[ n ] = 0 ; 
            printf( "%s", buf ) ;
          }
          printf( "\n" ) ;
          fclose( f ) ;
          USBD_MSC0_SetMediaOwnerUSB( );
        }
      }
    }
    else {
     osDelay(100U);                      // Delay to avoid busy waiting
    }
    last = state;
  }

  osThreadExit();
}

/*-----------------------------------------------------------------------------
 *        Application main function
 *----------------------------------------------------------------------------*/
int app_main (void) {
  osKernelInitialize();
  osThreadNew(app_main_thread, NULL, &app_main_attr);
  osKernelStart();
  return 0;
}
