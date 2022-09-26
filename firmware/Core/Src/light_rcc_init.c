
#include "light_rcc_init.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_rcc.h"
#include "stm32f030x6.h"
#include <stdint.h>

#ifdef CONFIG_LIGHT_RCC_INIT


/*
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
*/

#define HSIState              RCC_HSI_ON
#define HSI14State            RCC_HSI14_ON
#define HSICalibrationValue   RCC_HSICALIBRATION_DEFAULT
#define HSI14CalibrationValue 16
#define PLL_PLLState          RCC_PLL_ON
#define PLL_PLLSource         RCC_PLLSOURCE_HSI
#define PLL_PLLMUL            RCC_PLL_MUL12
#define PLL_PREDIV            RCC_PREDIV_DIV1

static inline void Light_HAL_RCC_OscConfig(void)
{
  uint32_t tickstart;
  uint32_t pll_config;
  uint32_t pll_config2;

  /*----------------------------- HSI Configuration --------------------------*/ 

  /* Check if HSI is used as system clock or as PLL source when PLL is selected as system clock */ 
  if(!((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_HSI) || 
  ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_PLLCLK) && 
  (__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_HSI))))
  {
    /* Enable the Internal High Speed oscillator (HSI). */
    __HAL_RCC_HSI_ENABLE();
    
    /* Get Start Tick */
    tickstart = HAL_GetTick();
    
    /* Wait till HSI is ready */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET)
    {
      if((HAL_GetTick() - tickstart ) > HSI_TIMEOUT_VALUE)
      {
        return;
      }
    }
  }

  /* Adjusts the Internal High Speed oscillator (HSI) calibration value.*/
   __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(HSICalibrationValue);

  /*----------------------------- HSI14 Configuration --------------------------*/

  /* Check the HSI14 State */

  /* Disable ADC control of the Internal High Speed oscillator HSI14 */
  __HAL_RCC_HSI14ADC_DISABLE();

  /* Enable the Internal High Speed oscillator (HSI). */
  __HAL_RCC_HSI14_ENABLE();

  /* Get Start Tick */
  tickstart = HAL_GetTick();
  
  /* Wait till HSI is ready */  
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSI14RDY) == RESET)
  {
    if((HAL_GetTick() - tickstart) > HSI14_TIMEOUT_VALUE)
    {
      return;
    }      
  } 

  /* Adjusts the Internal High Speed oscillator 14Mhz (HSI14) calibration value. */
  __HAL_RCC_HSI14_CALIBRATIONVALUE_ADJUST(HSI14CalibrationValue);

  /*-------------------------------- PLL Configuration -----------------------*/

    /* Check if the PLL is used as system clock or not */
    if(__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_PLLCLK)
    { 
      /* Disable the main PLL. */
      __HAL_RCC_PLL_DISABLE();
      
      /* Get Start Tick */
      tickstart = HAL_GetTick();
      
      /* Wait till PLL is disabled */
      while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY)  != RESET)
      {
        if((HAL_GetTick() - tickstart ) > PLL_TIMEOUT_VALUE)
        {
          return;
        }
      }

      /* Configure the main PLL clock source, predivider and multiplication factor. */
      __HAL_RCC_PLL_CONFIG(PLL_PLLSource, PLL_PREDIV, PLL_PLLMUL);
      /* Enable the main PLL. */
      __HAL_RCC_PLL_ENABLE();
      
      /* Get Start Tick */
      tickstart = HAL_GetTick();
      
      /* Wait till PLL is ready */
      while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY)  == RESET)
      {
        if((HAL_GetTick() - tickstart ) > PLL_TIMEOUT_VALUE)
        {
          return;
        }
      }
    }
    else
    {
      /* Do not return HAL_ERROR if request repeats the current configuration */
      pll_config  = RCC->CFGR;
      pll_config2 = RCC->CFGR2;
      if((READ_BIT(pll_config,  RCC_CFGR_PLLSRC)  != PLL_PLLSource) ||
          (READ_BIT(pll_config2, RCC_CFGR2_PREDIV) != PLL_PREDIV) ||
          (READ_BIT(pll_config,  RCC_CFGR_PLLMUL)  != PLL_PLLMUL))
      {
        return;
      }
    }

  return;
}


/*
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
*/

#define FLatency        FLASH_LATENCY_1
#define SYSCLKSource    RCC_SYSCLKSOURCE_PLLCLK
#define AHBCLKDivider   RCC_SYSCLK_DIV1
#define APB1CLKDivider  RCC_HCLK_DIV1

static inline void Light_HAL_RCC_ClockConfig(void)
{
  uint32_t tickstart;

  /* To correctly read data from FLASH memory, the number of wait states (LATENCY) 
  must be correctly programmed according to the frequency of the CPU clock 
    (HCLK) of the device. */

  /* Increasing the number of wait states because of higher CPU frequency */
  if(FLatency > __HAL_FLASH_GET_LATENCY())
  {
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLatency);
    
    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if(__HAL_FLASH_GET_LATENCY() != FLatency)
    {
      return;
    }
  }

  /*-------------------------- HCLK Configuration --------------------------*/

  /* Set the highest APB divider in order to ensure that we do not go through
      a non-spec phase whatever we decrease or increase HCLK. */
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE, RCC_HCLK_DIV16);

  /* Set the new HCLK clock divider */
  MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, AHBCLKDivider);
  

  /*------------------------- SYSCLK Configuration ---------------------------*/ 

  /* PLL is selected as System Clock Source */
  /* Check the PLL ready flag */
  if(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET)
  {
    return;
  }
  
  /* HSI is selected as System Clock Source */
  else
  {
    /* Check the HSI ready flag */  
    if(__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET)
    {
      return;
    }
  }
  __HAL_RCC_SYSCLK_CONFIG(SYSCLKSource);

  /* Get Start Tick */
  tickstart = HAL_GetTick();
  
  while (__HAL_RCC_GET_SYSCLK_SOURCE() != (SYSCLKSource << RCC_CFGR_SWS_Pos))
  {
    if((HAL_GetTick() - tickstart ) > CLOCKSWITCH_TIMEOUT_VALUE)
    {
      return;
    }
  }


  /* Decreasing the number of wait states because of lower CPU frequency */
  if(FLatency < __HAL_FLASH_GET_LATENCY())
  {    
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLatency);
    
    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if(__HAL_FLASH_GET_LATENCY() != FLatency)
    {
      return;
    }
  }    

  /*-------------------------- PCLK1 Configuration ---------------------------*/ 

  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE, APB1CLKDivider);

  /* Update the SystemCoreClock global variable */
  SystemCoreClock = HAL_RCC_GetSysClockFreq() >> AHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE)>> RCC_CFGR_HPRE_BITNUMBER];

  /* Configure the source of time base considering new system clocks settings*/
  HAL_InitTick (TICK_INT_PRIORITY);
  
  return;
}

static inline void Light_HAL_RCCEx_PeriphCLKConfig(void) {

  /* Configure the USART1 clock source PCLK1 */
  __HAL_RCC_USART1_CONFIG(RCC_USART1CLKSOURCE_PCLK1);

  /* Configure the I2C1 clock source */
  __HAL_RCC_I2C1_CONFIG(RCC_I2C1CLKSOURCE_HSI);

}



/* Light System Clock Configuration */

void Light_SystemClock_Config(void)
{

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */

  Light_HAL_RCC_OscConfig();

  Light_HAL_RCC_ClockConfig();

  Light_HAL_RCCEx_PeriphCLKConfig();

}

#endif
