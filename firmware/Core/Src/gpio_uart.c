
#include "board.h"
#include "gpio_uart.h"
#include "main.h"
#include <stdint.h>

#ifdef UART_DEBUG_GPIO

/* CoreClock is run in 48 MHz */
#define SYSTICKCLOCK 48000000ULL
/* if CoreClock is 48 MHz, 48 ticks spend 1 uS */
#define SYSTICKPERUS (SYSTICKCLOCK / 1000000UL)

#define GPIO_LOW GPIO_PIN_RESET
#define GPIO_HIGH GPIO_PIN_SET
#define BIT_DELAY_US CONFIG_SEND_BIT_DELAY_US
#define GPIO_UART_IO_Set(state) HAL_GPIO_WritePin(GPIO_UART_Port, GPIO_UART_Pin, state)

// 9600   104 us  28~32 ok
// 38400   26 us  25~29 ok
// 57600   17 us  .24~26.. ok
// 115200   9 us  16~20 ok

static void delay_1us(void) {

  asm volatile ("MOV R0,R0\n" // 1
                "MOV R0,R0\n"
                "MOV R0,R0\n"
                "MOV R0,R0\n"
                "MOV R0,R0\n"
                "MOV R0,R0\n"
                "MOV R0,R0\n"
                "MOV R0,R0\n"
                "MOV R0,R0\n"
                "MOV R0,R0\n"
                "MOV R0,R0\n"
                "MOV R0,R0\n"
                "MOV R0,R0\n"
                "MOV R0,R0\n"
                "MOV R0,R0\n"
                "MOV R0,R0\n"
                "MOV R0,R0\n"
                "MOV R0,R0\n" // 18
                );

#if defined(CONFIG_BAUD_RATE_57600) || defined(CONFIG_BAUD_RATE_38400) ||\
    defined(CONFIG_BAUD_RATE_9600)
  asm volatile ("MOV R0,R0\n" // 19
                "MOV R0,R0\n" // 20
                "MOV R0,R0\n" // 21
                "MOV R0,R0\n" // 22
                "MOV R0,R0\n" // 23
                "MOV R0,R0\n" // 24
                "MOV R0,R0\n" // 25
                );
#endif

#if defined(CONFIG_BAUD_RATE_38400) ||defined(CONFIG_BAUD_RATE_9600)
  asm volatile ("MOV R0,R0\n" // 26
                "MOV R0,R0\n" // 27
                );
#endif

#if defined(CONFIG_BAUD_RATE_9600)
  asm volatile ("MOV R0,R0\n" // 28
                "MOV R0,R0\n" // 29
                "MOV R0,R0\n" // 30
                );
#endif

}

static void delay_us(unsigned int us) {

  do
  {
    delay_1us();
  }
  while (us--);
}

void GPIO_UART_send_byte(uint8_t byte_out)
{
  int bit;

  /* Start bit */
  GPIO_UART_IO_Set(GPIO_LOW);

  delay_us(BIT_DELAY_US * CONFIG_START_BIT_LENGTH);

  /* Data bit */
  for (bit = BIT(0); bit <= BIT(CONFIG_DATA_BIT_LENGTH); bit <<= 1) {

    /* From Data Bit Set GPIO High/Low */
    GPIO_UART_IO_Set((byte_out & bit) ? GPIO_HIGH : GPIO_LOW);
    
    /* Delay BIT_DELAY_US */
    delay_us(BIT_DELAY_US);
  }

  /* Stop bit */
  GPIO_UART_IO_Set(GPIO_HIGH);
  delay_us(BIT_DELAY_US * CONFIG_STOP_BIT_LENGTH);
}

void GPIO_UART_Transmit(uint8_t *ptr, int len) {

  while(1)
  {
    if(*ptr == '\0' || len-- == 0 )
      break;
    GPIO_UART_send_byte(*ptr++);
  }

}

void GPIO_UART_Init(void) {

  GPIO_UART_IO_Set(GPIO_HIGH);

  HAL_Delay(10);
/*
  uint8_t str1[] = "GPIO_UART_Init Done\r\n";
  GPIO_UART_Transmit(str1, sizeof(str1));
*/
  PRINTF("\r\nGPIO_UART_Init Done\r\n");
}

#endif /* UART_DEBUG_GPIO */
