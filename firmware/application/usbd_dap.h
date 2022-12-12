

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DAP_H
#define __USB_DAP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_DAP
  * @brief This file is the Header file for usbd_dap.c
  * @{
  */


/** @defgroup USBD_DAP_Exported_Defines
  * @{
  */
#define DAP_EP1_ADDR  (0x01) //Bulk Out – used for commands received from host PC.
#define DAP_EP2_ADDR  (0x81) //Bulk In – used for responses send to host PC.
#define DAP_EP3_ADDR  (0x82) //Bulk In (optional) – used for streaming SWO trace (if enabled with SWO_STREAM).)

#define USB_DAP_CONFIG_DESC_SIZ       39U

/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */
typedef enum
{
  DAP_IDLE = 0,
  DAP_BUSY,
}
DAP_StateTypeDef;


typedef struct
{
  uint32_t             AltSetting;
}
USBD_DAP_HandleTypeDef;
/**
  * @}
  */



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

extern USBD_ClassTypeDef  USBD_DAP;
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
uint8_t USBD_DAP_SendData(USBD_HandleTypeDef *pdev,
                            uint8_t endpoint,
                            uint8_t *data,
                            uint16_t len);


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* __USB_DAP_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
