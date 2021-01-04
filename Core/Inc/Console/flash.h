#ifndef _FLASH_H_
#define _FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
void flash_memory_map(OSPI_HandleTypeDef *hospi1);

#ifdef __cplusplus
}
#endif

#endif
