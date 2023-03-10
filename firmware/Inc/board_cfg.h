#pragma once

#include "stm32f1xx_ll_gpio.h"

//swd-clk
#define DAP_SWK_TCK_PORT GPIOA
#define DAP_SWK_TCK_PIN LL_GPIO_PIN_6

//swd--dio
#define DAP_SWDIO_TMS_PORT GPIOA
#define DAP_SWDIO_TMS_PIN LL_GPIO_PIN_5

#define DAP_TDI_PORT GPIOA
#define DAP_TDI_PIN LL_GPIO_PIN_4

#define DAP_TDO_PORT GPIOA
#define DAP_TDO_PIN LL_GPIO_PIN_3

//test reset
#define DAP_NTRST_PORT GPIOA
#define DAP_NTRST_PIN LL_GPIO_PIN_2

//device reset
#define DAP_NRESET_PORT GPIOA
#define DAP_NRESET_PIN LL_GPIO_PIN_1

#define DAP_LED1_PORT GPIOA
#define DAP_LED1_PIN LL_GPIO_PIN_7

#define DAP_LED2_PORT GPIOA
#define DAP_LED2_PIN LL_GPIO_PIN_8
