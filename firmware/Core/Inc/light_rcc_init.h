

/**
 * light rcc init function
 * it can save about 1500 kbyte sram than SystemClock_Config
 */

#include "board.h"

//#define CONFIG_LIGHT_RCC_INIT
//#undef CONFIG_LIGHT_RCC_INIT


#ifdef CONFIG_LIGHT_RCC_INIT
void Light_SystemClock_Config(void);
#endif /* CONFIG_LIGHT_RCC_INIT */
