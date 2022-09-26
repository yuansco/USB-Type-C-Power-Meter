

#include "board.h"
#include "gpio_uart.h"
#include "ina3221.h"
#include "main.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include <stdio.h>
#include <string.h>

/******************************************************************************/
/* Extern Handle Type */

extern I2C_HandleTypeDef hi2c1;

#if defined(UART_DEBUG) && !defined(UART_DEBUG_GPIO)
extern UART_HandleTypeDef huart1;
#endif

#if !defined(FIXED_SKUID) || (SKUID == SKUID_1)
extern ADC_HandleTypeDef hadc;
#endif


/******************************************************************************/
/* UART Debug  */

#if defined(__GNUC__) && defined(UART_DEBUG) && !defined(UART_DEBUG_GPIO)
int _write(int fd, char * ptr, int len)
{
  HAL_UART_Transmit(&huart1, (uint8_t *) ptr, len, HAL_MAX_DELAY);
  return len;
}
#elif defined(__GNUC__) && defined(UART_DEBUG) && defined(UART_DEBUG_GPIO)
int _write(int fd, char * ptr, int len)
{
  GPIO_UART_Transmit((uint8_t *) ptr, len);
  return len;
}
#elif defined (__ICCARM__) && defined(UART_DEBUG)
#include "LowLevelIOInterface.h"
size_t __write(int handle, const unsigned char * buffer, size_t size)
{
  HAL_UART_Transmit(&huart1, (uint8_t *) buffer, size, HAL_MAX_DELAY);
  return size;
}
#elif defined (__CC_ARM) && defined(UART_DEBUG)
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}
#endif

/******************************************************************************/
/* ADC  */

#if !defined(FIXED_SKUID) || (SKUID == SKUID_1)

uint32_t ADC_Value[ADC_CH_COUNT];

const uint32_t adc_channels[] = {
  [ADC_VCSP_L] = ADC_CHANNEL_0,    // GPIO A0
  [ADC_VCSN_L] = ADC_CHANNEL_1,    // GPIO A1
  [ADC_VBUS]   = ADC_CHANNEL_4,    // GPIO A4
};

/**
 * ADC value correction
 */
/*
const adc_correction correction_table[] = {
  [ADC_VCSP_L] = { 1000, 0},   // ADC channel 0
  [ADC_VCSN_L] = { 1000, 0},   // ADC channel 1
  [ADC_VBUS]   = { 1000, 0},   // ADC channel 4
};

void correction_adc_ch(enum adc_channel ch) {

  // y = a * x + b 
  ADC_Value[ch] = correction_table[ch].a * ADC_Value[ch] + correction_table[ch].b;
  ADC_Value[ch] = ADC_Value[ch] / 1000;

}

void correction_adc_all(void) {

  // correction all ADC_Value[] value
  for(int i = 0; i < ADC_CH_COUNT; i++) {
    correction_adc_ch(i);
  }
}
*/

ADC_ChannelConfTypeDef sConfig = {0};

void ADC_Select_Channel(const uint32_t ch) {

  sConfig.Channel = ch;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
    Error_Handler();
  }
}

void read_adc_ch(enum adc_channel ch) {

  ADC_Select_Channel(adc_channels[ch]);
  HAL_ADC_Start(&hadc);
  HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
  ADC_Value[ch] = HAL_ADC_GetValue(&hadc);
  HAL_ADC_Stop(&hadc);
}
/*
void read_adc_all(void) {

  // read all adc channel and update to ADC_Value[]
  for(int i = 0; i < ADC_CH_COUNT; i++) {
    read_adc_ch(i);
  }
  correction_adc_all();
}
*/

#endif

/******************************************************************************/
/* SKU ID and BOARD ID */

#ifdef FIXED_SKUID
int BOARD_get_sku() {
  return SKUID;
}
#else
int BOARD_get_sku() {

#ifdef BOARD_ID_2BIT
  // support 2 bit board id
  return (HAL_GPIO_ReadPin(BOARD_ID_0_GPIO_Port, BOARD_ID_0_Pin) |
        (HAL_GPIO_ReadPin(BOARD_ID_1_GPIO_Port, BOARD_ID_1_Pin) << 1)) +1;
#else
  return HAL_GPIO_ReadPin(BOARD_ID_0_GPIO_Port, BOARD_ID_0_Pin) +1;
#endif /* BOARD_ID_2BIT */
}
#endif /* FIXED_SKUID */

void sku_Init() {
  PRINTF("sku: %d\r\n", BOARD_get_sku());
}


/******************************************************************************/
/* I2C Scan function */

/**
 * scan 0x00 ~ 0x7f (0~127) 
 * retries: 2 times
 * timeout: 10 mS
 */

#ifdef I2C_SCAN

#define I2C_SCAN_TIMEOUT_MS 10
#define I2C_SCAN_RETRIES 2

void i2c_scan() {
  
  uint8_t i, re;

 	PRINTF("\r\nScanning I2C bus:\r\n");

 	for (i = 1; i < 128; i++) {

 	  re = HAL_I2C_IsDeviceReady(&hi2c1, (i << 1),
            I2C_SCAN_RETRIES, I2C_SCAN_TIMEOUT_MS);

 	  if (re == HAL_OK) {
      // received ACK
 		  PRINTF("\r\n0x%02X\r\n", i); 
      continue;
 	  }

    // No ACK received at that address
 		PRINTF(".");
 	}
 	PRINTF("\r\n");
}

#endif /* I2C_SCAN */


/******************************************************************************/
/* OLED SSD1306 */

#ifdef SSD1306_OLED

#define BUFF_SIZE 10

#define Y_OFFSET_LINE_0 0
#define Y_OFFSET_LINE_1 18
#define Y_OFFSET_LINE_2 36

static char line_1_str[BUFF_SIZE] = "V 20000 mV";
static char line_2_str[BUFF_SIZE] = "I  5000 mA";
static char line_3_str[BUFF_SIZE] = "P 99.9W =>";

void ssd1306_show(int volt, int curr, int watt) {

  if (volt < 0) {
    line_3_str[8] = '<';
    line_3_str[9] = '=';
  } else {
    line_3_str[8] = '=';
    line_3_str[9] = '>';
  }

  // 20001 mV => in line_1_str[2:6] show '20001'
  line_1_str[2] = (char) (MIN(volt / 10000, 2) + 48);
  line_1_str[3] = (char) (volt % 10000 / 1000 + 48);
  line_1_str[4] = (char) (volt % 1000 / 100 + 48);
  line_1_str[5] = (char) (volt % 100 /10 + 48);
  line_1_str[6] = (char) (volt % 10 + 48);
  
  // 2101 mA => in line_2_str[2:6] show ' 2101'
  line_2_str[3] = (char) (MIN(curr / 1000, 5) + 48);
  line_2_str[4] = (char) (curr % 1000 / 100 + 48);
  line_2_str[5] = (char) (curr % 100 / 10 + 48);
  line_2_str[6] = (char) (curr % 10 + 48);
  
  // 14980 mW => in line_2_str[2:5] show 14.98 W
  line_3_str[2] = (char) (watt / 10000 + 48);
  line_3_str[3] = (char) (watt % 10000 / 1000 + 48);
  line_3_str[5] = (char) (watt % 1000 / 100 + 48);
  //line_3_str[6] = (char) (MIN(watt % 100 /10, 9) + 48);


  /* cut 0 in herder on each number*/
  for (int i = 2; i < 6; i++) {
    if (line_1_str[i] == '0')
      line_1_str[i] = ' ';
    else
      break;
  }
  /* cut 0 in herder on each number*/
  for (int i = 3; i < 6; i++) {
    if (line_2_str[i] == '0')
      line_2_str[i] = ' ';
    else
      break;
  }
  /* cut 0 in herder on each number */
  for (int i = 2; i < 3; i++) {
    if (line_3_str[i] == '0')
      line_3_str[i] = ' ';
    else
      break;
  }

  ssd1306_Fill(Black);

  ssd1306_SetCursor(2, Y_OFFSET_LINE_0);
  ssd1306_WriteString(&line_1_str[0], Font_11x18_SUPPORT, White);

  ssd1306_SetCursor(2, Y_OFFSET_LINE_1);
  ssd1306_WriteString(&line_2_str[0], Font_11x18_SUPPORT, White);

  ssd1306_SetCursor(2, Y_OFFSET_LINE_2);
  ssd1306_WriteString(&line_3_str[0], Font_11x18_SUPPORT, White);

  ssd1306_UpdateScreen();
}

#else
void ssd1306_show(int volt, int curr, int watt) {return;}
#endif


/******************************************************************************/
/* board init function */

void BOARD_Init() {

#ifdef UART_DEBUG_GPIO
  GPIO_UART_Init();
#endif

  PRINTF("Ver: %s-%s\r\n", VERSION, BUILD_DATE);

  sku_Init();

#ifdef SSD1306_OLED
  if (ssd1306_Init()) {
    PRINTF("ssd1306 init fail!\r\n");
    i2c_scan();
    //Error_Handler();
  }
#endif

#if defined(FIXED_SKUID) && (SKUID == SKUID_1)
  return;
#else
  if (BOARD_get_sku() == 1) {

    if (!ina3221_init())
      return;
    PRINTF("ina3221 init fail!\r\n");
#ifdef I2C_SCAN
    i2c_scan();
#endif /* I2C_SCAN */
    Error_Handler();
  }
  return;
#endif

}

/******************************************************************************/
/* board loop function */

#ifndef OLED_AUTO_TEST
void BOARD_Loop() {

  int volt = 0, curr = 0, watt = 0;

#ifdef GPIO_SKUID
  // sku 1 is use adc to get power info
  if (BOARD_get_sku() == SKUID_1) {
#endif
#if !(defined(FIXED_SKUID) && (SKUID == SKUID_2)) /* if not FIXED INA sku */
    //read_adc_all();
    read_adc_ch(ADC_VCSP_L);
    read_adc_ch(ADC_VCSN_L);
    read_adc_ch(ADC_VBUS);
    // Vstep = 20 V / 4096 = 0.004882813 V = 4882 uV
    // Vbus (mV) = (ADC_Value * 4882) / 1000
    volt = (ADC_Value[ADC_VBUS] * 4882) / 1000;
    // Istep =  5 A / 4096 = 0.001220703 A = 1220 uA
    if (ADC_Value[ADC_VCSP_L] > ADC_Value[ADC_VCSN_L])
      curr = (ADC_Value[ADC_VCSP_L] * 1220) / 1000; //uA /1000 = mA
    else
      curr = ((ADC_Value[ADC_VCSN_L] * 1220) / 1000) * -1;

#endif
#ifdef GPIO_SKUID
  // sku 2 is use INA3221 to get power info
  } else if (BOARD_get_sku() == SKUID_2) {
#endif
#if !(defined(FIXED_SKUID) && (SKUID == SKUID_1))
    int shunt_volt = 0;
    ina3221_gat_data(1, &shunt_volt, &volt);

    // shunt_volt (uV) / Rsen (mohm) = curr (mA)
    curr = shunt_volt / INA3221_RSEN_MOHM;
#endif
#ifdef GPIO_SKUID
  }
#endif /* GPIO_SKUID */

  watt = (ABS(volt) * curr) / 1000;
  PRINTF("%5dmV , %4dmA, %5dmW\r\n" , volt, curr, watt);
  ssd1306_show(volt, curr, watt);
}
#else 

/* OLED_AUTO_TEST */

typedef struct
{
  int v;
  int i; 
} test_case;

static test_case test[] = {
  { .v = 19900, .i = 4996 },
  { .v = 15005, .i = 2989 },
  { .v = 9000,  .i = 3001 },
  { .v = 4986,  .i = 1499 },
  { .v = -4986,  .i = 1499 },
  { .v = -9000,  .i = 3001 },
  { .v = -15005, .i = 2989 },
  { .v = -19900, .i = 4996 },
};

void BOARD_Loop() {

  for (int i = 0; i<ARRAY_SIZE(test); i++) {
    int watt = (ABS(test[i].v) * test[i].i) / 1000;
    PRINTF("%5dmV , %4dmA, %5dmW\r\n" , test[i].v, test[i].i, watt);
    ssd1306_show(test[i].v, test[i].i, watt);
    HAL_Delay(2000);
  }
}

#endif /* OLED_AUTO_TEST */
