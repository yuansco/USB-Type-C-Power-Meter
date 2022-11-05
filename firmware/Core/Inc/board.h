
#ifndef __BOARD_H__
#define __BOARD_H__


#include <stdint.h>
#include <stdio.h>

/**
 * Firmware version and auto build date
 * 
 * BUILD_DATE will update by build.sh, Don't modify date directly.
 */
#define VERSION "V1.0"
#define BUILD_DATE "2022.11.05"

/**
 *  UART Debug log
 * 
 *  free about 4000 bytes FLASH and 64 bytes SRAM if undef it.
 *  Note: Should use PRINTF() replace printf().
 */

#define UART_DEBUG
//#undef UART_DEBUG

#define UART_DEBUG_GPIO
//#undef UART_DEBUG_GPIO

#ifdef UART_DEBUG_GPIO
#define GPIO_UART_Port GPIOA
#define GPIO_UART_Pin GPIO_PIN_2
#endif

#ifdef UART_DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(args...)
#endif


/**
 *  I2C Scan
 * 
 *  Support I2C Scan function.
 */
#define I2C_SCAN
//#undef I2C_SCAN


/**
 *  Shunt Resistor
 * 
 *  Shunt resistor value, unit is mohm.
 */
//#define INA3221_RSEN_MOHM 10
//#define INA3221_RSEN_MOHM 20
#define INA3221_RSEN_MOHM 30

#define INA3221_I2C_PORT        hi2c1
#define INA3221_I2C_ADDR        INA3221_ADDR_0



/**
 *  SKU ID and BOARD ID
 * 
 *  Board ID is the resistors which is used to indicate SKU ID corresponding.
 *
 *  SKU ID : BOARD ID : Description
 *       1 :     0b00 : Use ADC and error amplifier to get voltage and current.
 *       2 :     0b01 : Use I2C and INA3221 to get voltage and current.
 *       3 :     0b10 : Reserved.
 *       4 :     0b11 : Reserved.
 */

// SKU ID list:
#define SKUID_1 0x01    /* 1: ADC sku */
#define SKUID_2 0x02    /* 2: INA3221 sku */
#define SKUID_3 0x03    /* 3: Reserved */
#define SKUID_4 0x04    /* 4: Reserved */

/* fixed SKU ID */
//#define SKUID SKUID_1 /* Force ADC sku */
#define SKUID SKUID_2 /* Force INA3221 sku */


/**
 * BOARD ID list:
 * 
 * BOARD_ID_0_Pin is bit 0
 * BOARD_ID_1_Pin is bit 1
 * 
 */

#ifdef SKUID
#define FIXED_SKUID
#else
#define GPIO_SKUID
#define BOARD_ID_2BIT
//#undef BOARD_ID_2BIT
#endif


/**
 * LED Behavior
 * 
 * Slowly blinking LED in Normal mode,
 * Fastly blinking LED before reset mcu if any error happened.
 * 
 * Normal: LED 1 second on, 1 second off
 * Error: LED 0.1 second on, 0.1 second off
 */

#define  CONFIG_LED_NORMAL
#define  CONFIG_LED_NORMAL_BLINK_MS 1000
//#undef CONFIG_LED_NORMAL

#define  CONFIG_LED_ERROR
#define  CONFIG_LED_ERROR_SEC 3
#define  CONFIG_LED_ERROR_TIME_MS (CONFIG_LED_ERROR_SEC * 1000)
#define  CONFIG_LED_ERROR_BLINK_MS 100
//#undef CONFIG_LED_ERROR



/**
 * Board loop delay
 * main function loop delay as well as oled update delay.
 */
#define CONFIG_BOARD_LOOP_TIME_MS 300



/**
 *  ADC
 */

enum adc_channel {
  ADC_VCSP_L = 0,
  ADC_VCSN_L,
  ADC_VBUS,
  ADC_CH_COUNT
};

extern uint32_t ADC_Value[ADC_CH_COUNT];

/**
 * ADC correction
 * 
 *   y = a * x + b
 */

typedef struct {
  uint16_t a;      // 0.001
  uint16_t b;      // 0.001
} adc_correction;

/* Disable adc config test */

//#define ADC_INIT_CONFIG_TEST
#undef ADC_INIT_CONFIG_TEST

/**
 * Light rcc init function
 * 
 * light rcc init function can save about
 * 1500 kbyte sram than SystemClock_Config
 */

#define CONFIG_LIGHT_RCC_INIT


/**
 * OLED SSD1306
 */

#define SSD1306_OLED
//#undef SSD1306_OLED

//#define OLED_AUTO_TEST

// I2C Configuration
#define SSD1306_I2C_PORT        hi2c1
#define SSD1306_I2C_ADDR        (0x3C << 1)

// Mirror the screen if needed
// #define SSD1306_MIRROR_VERT
// #define SSD1306_MIRROR_HORIZ

// Set inverse color if needed
// # define SSD1306_INVERSE_COLOR

// Include only needed fonts
//#define SSD1306_INCLUDE_FONT_6x8
//#define SSD1306_INCLUDE_FONT_7x10
//#define SSD1306_INCLUDE_FONT_11x18
//#define SSD1306_INCLUDE_FONT_16x26

// Some OLEDs don't display anything in first two columns.
// In this case change the following macro to 130.
// The default value is 128.
// #define SSD1306_WIDTH           130

// The height can be changed as well if necessary.
// It can be 32, 64 or 128. The default value is 64.
// #define SSD1306_HEIGHT          32


/* util */

#define ARRAY_SIZE(x)  (sizeof(x) / sizeof((x)[0]))

#define BIT(nr) (1U << (nr))

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define ABS(x) ((x) >= 0 ? (x) : -(x))



/* main function API */
void BOARD_Init();
void BOARD_Loop();
int BOARD_get_sku();


#endif /* __BOARD_H__ */


