
#ifndef __GPIO_UART_H
#define __GPIO_UART_H


#ifdef UART_DEBUG_GPIO

/**
 *  Software UART
 *  need to define GPIO_UART_Port and GPIO_UART_Pin in board.h
 * 
 *  e.g.
 *  #define GPIO_UART_Port GPIOA
 *  #define GPIO_UART_Pin GPIO_PIN_2
 */

#include <stdint.h>
#include "board.h"

//#define CONFIG_BAUD_RATE_9600
//#define CONFIG_BAUD_RATE_38400
//#define CONFIG_BAUD_RATE_57600
#define CONFIG_BAUD_RATE_115200

#define CONFIG_START_BIT_LENGTH 1   // start bit : 1
#define CONFIG_DATA_BIT_LENGTH  8   // data bit  : 8
#define CONFIG_STOP_BIT_LENGTH  1   // stop bit  : 1

/**
 *   baud 9600 is mean sending 9600 bit per second,
 *   So bit delay = 1 second / 9600
 *                = 0.000104167 second
 *                = 104.167 microsecond
 */

#ifdef CONFIG_BAUD_RATE_9600
  #define CONFIG_SEND_BIT_DELAY_US 104 // 104 us
#elif defined(CONFIG_BAUD_RATE_38400)
  #define CONFIG_SEND_BIT_DELAY_US 26  // 26 us
#elif defined(CONFIG_BAUD_RATE_57600)
  #define CONFIG_SEND_BIT_DELAY_US 17  // 17 us (17.3)
#elif defined(CONFIG_BAUD_RATE_115200)
  #define CONFIG_SEND_BIT_DELAY_US 9   // 9 us (8.68)
#else
  #define CONFIG_SEND_BIT_DELAY_US 9 // Default Baud Rate is 115200
#endif /* CONFIG_BAUD_RATE_9600 */

void GPIO_UART_send_byte(uint8_t byte_out);
void GPIO_UART_Transmit(uint8_t *ptr, int len);
void GPIO_UART_Init(void);
#endif /* UART_DEBUG_GPIO */

#endif /* __GPIO_UART_H */