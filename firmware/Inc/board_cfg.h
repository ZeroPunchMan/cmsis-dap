#pragma once

#include "stm32f1xx_ll_gpio.h"

// swd-clk
#define DAP_SWK_TCK_PORT (GPIOA)
#define DAP_SWK_TCK_PIN (LL_GPIO_PIN_5)

// swd--dio
#define DAP_SWDIO_TMS_PORT (GPIOA)
#define DAP_SWDIO_TMS_PIN (LL_GPIO_PIN_6)

// tdi
#define DAP_TDI_PORT (GPIOA)
#define DAP_TDI_PIN (LL_GPIO_PIN_7)

//tdo
#define DAP_TDO_PORT (GPIOA)
#define DAP_TDO_PIN (LL_GPIO_PIN_3)

// test reset
#define DAP_TRST_PORT (GPIOB)
#define DAP_TRST_PIN (LL_GPIO_PIN_0)

// device reset
#define DAP_NRESET_PORT (GPIOA)
#define DAP_NRESET_PIN (LL_GPIO_PIN_2)

// led1 connect
#define DAP_LED1_PORT (GPIOB)
#define DAP_LED1_PIN (LL_GPIO_PIN_1)

// led2  running
#define DAP_LED2_PORT (GPIOB)
#define DAP_LED2_PIN (LL_GPIO_PIN_2)

//txs0108e OE
#define TXS_OE_PORT (GPIOA)
#define TXS_OE_PIN (LL_GPIO_PIN_0)

#define USB_RESET_MCU //no remote wakeup, no self power
