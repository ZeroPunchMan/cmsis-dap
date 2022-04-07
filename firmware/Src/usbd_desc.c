/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : usbd_desc.c
 * @version        : v2.0_Cube
 * @brief          : This file implements the USB device descriptors.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_conf.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
 * @{
 */

/** @addtogroup USBD_DESC
 * @{
 */

/** @defgroup USBD_DESC_Private_TypesDefinitions USBD_DESC_Private_TypesDefinitions
 * @brief Private types.
 * @{
 */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
 * @}
 */

/** @defgroup USBD_DESC_Private_Defines USBD_DESC_Private_Defines
 * @brief Private defines.
 * @{
 */

#define USBD_LANGID_STRING 1033
#define USBD_MANUFACTURER_STRING "Microsoft"
#define USBD_PRODUCT_STRING_FS "Xbox360 Controller for Windows"
#define USBD_SERIAL_NUMBER "00000009"

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
 * @}
 */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/** @defgroup USBD_DESC_Private_Macros USBD_DESC_Private_Macros
 * @brief Private macros.
 * @{
 */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
 * @}
 */

/** @defgroup USBD_DESC_Private_FunctionPrototypes USBD_DESC_Private_FunctionPrototypes
 * @brief Private functions declaration.
 * @{
 */

/**
 * @}
 */

/** @defgroup USBD_DESC_Private_FunctionPrototypes USBD_DESC_Private_FunctionPrototypes
 * @brief Private functions declaration for FS.
 * @{
 */

uint8_t *USBD_FS_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_FS_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_FS_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_FS_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_FS_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);

/**
 * @}
 */

/** @defgroup USBD_DESC_Private_Variables USBD_DESC_Private_Variables
 * @brief Private variables.
 * @{
 */

USBD_DescriptorsTypeDef FS_Desc = {
    .GetDeviceDescriptor = USBD_FS_DeviceDescriptor,
    .GetLangIDStrDescriptor = USBD_FS_LangIDStrDescriptor,
    .GetManufacturerStrDescriptor = NULL,
    .GetProductStrDescriptor = USBD_FS_ProductStrDescriptor,
    .GetSerialStrDescriptor = USBD_FS_SerialStrDescriptor,
    .GetConfigurationStrDescriptor = NULL,
    .GetInterfaceStrDescriptor = NULL};

#if defined(__ICCARM__) /* IAR Compiler */
#pragma data_alignment = 4
#endif /* defined ( __ICCARM__ ) */
/** USB standard device descriptor. */
__ALIGN_BEGIN uint8_t USBD_FS_DeviceDesc[USB_LEN_DEV_DESC] __ALIGN_END = {
    0x12, //length 
    0x01, //type device
    0x00, 0x02, //usb version
    0xFF, //device class
    0xFF, //sub class
    0xFF, //protocl
    0x08, //max pack size
    0x5E, 0x04, //verdor
    0x8E, 0x02, //product
    0x0a, 0x07, //device
    0x00, //iManu
    0x02, //iProduct
    0x03, //iSerial
    0x01  //bNumCfg
    };

/* USB_DeviceDescriptor */

/**
 * @}
 */

/** @defgroup USBD_DESC_Private_Variables USBD_DESC_Private_Variables
 * @brief Private variables.
 * @{
 */

#if defined(__ICCARM__) /* IAR Compiler */
#pragma data_alignment = 4
#endif /* defined ( __ICCARM__ ) */

/** USB lang indentifier descriptor. */
__ALIGN_BEGIN uint8_t USBD_LangIDDesc[USB_LEN_LANGID_STR_DESC] __ALIGN_END =
    {
        USB_LEN_LANGID_STR_DESC,
        USB_DESC_TYPE_STRING,
        LOBYTE(USBD_LANGID_STRING),
        HIBYTE(USBD_LANGID_STRING)};

#if defined(__ICCARM__) /* IAR Compiler */
#pragma data_alignment = 4
#endif /* defined ( __ICCARM__ ) */
/* Internal string descriptor. */
__ALIGN_BEGIN uint8_t USBD_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

#if defined(__ICCARM__) /*!< IAR Compiler */
#pragma data_alignment = 4
#endif

/**
 * @}
 */

/** @defgroup USBD_DESC_Private_Functions USBD_DESC_Private_Functions
 * @brief Private functions.
 * @{
 */

/**
 * @brief  Return the device descriptor
 * @param  speed : Current device speed
 * @param  length : Pointer to data length variable
 * @retval Pointer to descriptor buffer
 */
uint8_t *USBD_FS_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);
  uint16_t maxLen = sizeof(USBD_FS_DeviceDesc);
  if (length[0] > maxLen)
    length[0] = maxLen;
  return USBD_FS_DeviceDesc;
}

/**
 * @brief  Return the LangID string descriptor
 * @param  speed : Current device speed
 * @param  length : Pointer to data length variable
 * @retval Pointer to descriptor buffer
 */
uint8_t *USBD_FS_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);
  *length = sizeof(USBD_LangIDDesc);
  return USBD_LangIDDesc;
}

/**
 * @brief  Return the product string descriptor
 * @param  speed : Current device speed
 * @param  length : Pointer to data length variable
 * @retval Pointer to descriptor buffer
 */
uint8_t *USBD_FS_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  if (speed == 0)
  {
    USBD_GetString((uint8_t *)USBD_PRODUCT_STRING_FS, USBD_StrDesc, length);
  }
  else
  {
    USBD_GetString((uint8_t *)USBD_PRODUCT_STRING_FS, USBD_StrDesc, length);
  }
  return USBD_StrDesc;
}

/**
 * @brief  Return the manufacturer string descriptor
 * @param  speed : Current device speed
 * @param  length : Pointer to data length variable
 * @retval Pointer to descriptor buffer
 */
uint8_t *USBD_FS_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);
  USBD_GetString((uint8_t *)USBD_MANUFACTURER_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}

/**
 * @brief  Return the serial number string descriptor
 * @param  speed : Current device speed
 * @param  length : Pointer to data length variable
 * @retval Pointer to descriptor buffer
 */
uint8_t *USBD_FS_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);
  uint16_t maxLen;
  USBD_GetString((uint8_t *)USBD_SERIAL_NUMBER, USBD_StrDesc, &maxLen);
  if (length[0] > maxLen)
    length[0] = maxLen;
  return USBD_StrDesc;
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
