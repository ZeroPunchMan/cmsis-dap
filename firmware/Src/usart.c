/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
CL_QUEUE_DEF_INIT(usart1Send_q, 256, uint8_t, );
CL_QUEUE_DEF_INIT(usart1Recv_q, 256, uint8_t, );
/* USER CODE END 0 */

/* USART1 init function */

void MX_USART1_UART_Init(void)
{
  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
  
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration  
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART1_IRQn);

  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);

}

/* USER CODE BEGIN 1 */
uint16_t UsartSendData(USART_TypeDef *usart, const uint8_t* data, uint16_t length)
{
  if(CL_QueueFreeSpace(&usart1Send_q) < length)
    return 0;

  for(uint16_t i = 0; i < length; i++)
  {
    uint8_t b = data[i];
    CL_QueueAdd(&usart1Send_q, &b);
  }
  
  LL_USART_EnableIT_TXE(USART1);
  return length;
}


int _write(int fd, char *str, int len)
{
    if (CL_QueueFreeSpace(&usart1Send_q) < len)
        return 0;

    for (int i = 0; i < len; i++)
    {
        uint8_t b = str[i];
        CL_QueueAdd(&usart1Send_q, &b);
    }

    LL_USART_EnableIT_TXE(USART1);
    return len;
}

// int fputc(int ch, FILE *f)
// {
//     uint8_t data = ch;
//     int res;

//     res = CL_QueueAdd(&usart1Send_q, &data);

//     LL_USART_EnableIT_TXE(USART1);
//     if(res == CL_ResSuccess)
//     {
//         return ch;
//     }
//     else
//     {
//         return EOF; 
//     }
// }
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
