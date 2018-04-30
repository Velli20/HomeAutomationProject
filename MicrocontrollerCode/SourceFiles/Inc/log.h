#ifndef __LOG_H
#define __LOG_H

// Includes

#include <stdio.h>
#include <string.h>
#include "stm32f7xx_hal.h"

// Macros and defines

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

// Log_Config

void Log_Config(void);

// Log

#define Log(message)(printf("In file=%s line=%d   %s\n", __FILENAME__, __LINE__, message))

#endif
