/**
 ******************************************************************************
 * @file    usb.c
 * @brief   This file provides code for the configuration
 *          of the USB instances.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usb.h"

/* USER CODE BEGIN 0 */
#include "cl_log.h"
/* USER CODE END 0 */

PCD_HandleTypeDef hpcd_USB_FS;

/* USB init function */

void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */
  HAL_PCD_Start(&hpcd_USB_FS);

  /* USER CODE END USB_Init 2 */
}

void HAL_PCD_MspInit(PCD_HandleTypeDef *pcdHandle)
{

  if (pcdHandle->Instance == USB)
  {
    /* USER CODE BEGIN USB_MspInit 0 */
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = (GPIO_PIN_11 | GPIO_PIN_12);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* USER CODE END USB_MspInit 0 */
    /* USB clock enable */
    __HAL_RCC_USB_CLK_ENABLE();

    /* USB interrupt Init */
    HAL_NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
    /* USER CODE BEGIN USB_MspInit 1 */
    
    /* USER CODE END USB_MspInit 1 */
  }
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef *pcdHandle)
{

  if (pcdHandle->Instance == USB)
  {
    /* USER CODE BEGIN USB_MspDeInit 0 */

    /* USER CODE END USB_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USB_CLK_DISABLE();

    /* USB interrupt Deinit */
    HAL_NVIC_DisableIRQ(USB_HP_CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
    /* USER CODE BEGIN USB_MspDeInit 1 */

    /* USER CODE END USB_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/*******************************************************************************
                       Driver Callbacks (PCD -> USB Device Library)
*******************************************************************************/

void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)
{
  CL_LOG_LINE("usb setup");
}

void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  CL_LOG_LINE("usb data out");
}

void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  CL_LOG_LINE("usb data in");
}

void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
{
  CL_LOG_LINE("usb sof");
}

void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd)
{
  CL_LOG_LINE("usb reset");
}

void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd)
{
  CL_LOG_LINE("usb suspend");
}

void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd)
{
  CL_LOG_LINE("usb resume");
}

void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  CL_LOG_LINE("usb isoout incom");
}

void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  CL_LOG_LINE("usb isoin incom");
}

void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
{
  CL_LOG_LINE("usb connect");
}

void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
{
  CL_LOG_LINE("usb disconnect");
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
