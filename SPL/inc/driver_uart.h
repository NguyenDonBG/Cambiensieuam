/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRIVER_UART_H
#define __DRIVER_UART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_conf.h"
#define SERIAL_RX_BUFFER_SIZE 256
void UART1_Init_A9A10(void);
int _write(int file, char *ptr, int len);
void USART1_IRQHandler();
int HardwareSerial_available();
int HardwareSerial_read(void);

#ifdef __cplusplus
}
#endif

#endif /* __DRIVER_UART_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2020 Giaosu*****END OF FILE****/
