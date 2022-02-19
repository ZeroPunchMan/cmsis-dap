#pragma once

#include "board_cfg.h"
#include "string.h"

//----------------------swclk tck----------------------------------
//swk tck 1
static inline void SwkTckHigh(void)
{
    LL_GPIO_SetOutputPin(DAP_SWK_TCK_PORT, DAP_SWK_TCK_PIN);
}

//swk tck 0
static inline void SwkTckLow(void)
{
    LL_GPIO_ResetOutputPin(DAP_SWK_TCK_PORT, DAP_SWK_TCK_PIN);
}

static inline uint32_t TckReadInput(void)
{
    return LL_GPIO_IsInputPinSet(DAP_SWK_TCK_PORT, DAP_SWK_TCK_PIN);
}

//---------------------swdio tms---------------------------------
//swdio tms 1
static inline void SwdioTmsHigh(void)
{
    LL_GPIO_SetOutputPin(DAP_SWDIO_TMS_PORT, DAP_SWDIO_TMS_PIN);
}

//swdio tms 0
static inline void SwdioTmsLow(void)
{
    LL_GPIO_ResetOutputPin(DAP_SWDIO_TMS_PORT, DAP_SWDIO_TMS_PIN);
}

//swdio tms read
static inline uint32_t SwdioTmsReadInput(void)
{
    return LL_GPIO_IsInputPinSet(DAP_SWDIO_TMS_PORT, DAP_SWDIO_TMS_PIN);
}

//dio output
static inline void SwdioOutput(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DAP_SWDIO_TMS_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(DAP_SWDIO_TMS_PORT, &GPIO_InitStruct);
}

//dio input
static inline void SwdioInput(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DAP_SWDIO_TMS_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    LL_GPIO_Init(DAP_SWDIO_TMS_PORT, &GPIO_InitStruct);
}

//------------------------tdi-------------------------------------
static inline void TdiHigh(void)
{
    LL_GPIO_SetOutputPin(DAP_TDI_PORT, DAP_TDI_PIN);
}

static inline void TdiLow(void)
{
    LL_GPIO_ResetOutputPin(DAP_TDI_PORT, DAP_TDI_PIN);
}

static inline uint32_t TidReadInput(void)
{
    return LL_GPIO_IsInputPinSet(DAP_TDI_PORT, DAP_TDI_PIN);
}

//----------------------tdo--------------------------------
// static inline void TdoHigh(void)
// {
//     LL_GPIO_SetOutputPin(DAP_TDO_PORT, DAP_TDO_PIN);

// }

// static inline void TdoLow(void)
// {
//     LL_GPIO_ResetOutputPin(DAP_TDO_PORT, DAP_TDO_PIN);

// }

static inline uint32_t TdoReadInput(void)
{
    return LL_GPIO_IsInputPinSet(DAP_TDO_PORT, DAP_TDO_PIN);
}

//---------------------nTRST------------------------------
static inline void NtrstHigh(void)
{
    LL_GPIO_SetOutputPin(DAP_NTRST_PORT, DAP_NTRST_PIN);
}

static inline void NtrstLow(void)
{
    LL_GPIO_ResetOutputPin(DAP_NTRST_PORT, DAP_NTRST_PIN);
}

static inline uint32_t NtrstReadInput(void)
{
    return LL_GPIO_IsInputPinSet(DAP_NTRST_PORT, DAP_NTRST_PIN);
}

//----------------------nRESET------------------------
static inline void NresetHigh(void)
{
    LL_GPIO_SetOutputPin(DAP_NRESET_PORT, DAP_NRESET_PIN);
}

static inline void NresetLow(void)
{
    LL_GPIO_ResetOutputPin(DAP_NRESET_PORT, DAP_NRESET_PIN);
}

static inline uint32_t NresetReadInput(void)
{
    return LL_GPIO_IsInputPinSet(DAP_NRESET_PORT, DAP_NRESET_PIN);
}

//jtag port init
static inline void JtagPortInit(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = DAP_SWK_TCK_PIN | DAP_SWDIO_TMS_PIN | DAP_TDI_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = DAP_NTRST_PIN | DAP_NRESET_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    LL_GPIO_SetOutputPin(GPIOA, DAP_SWK_TCK_PIN);
    LL_GPIO_SetOutputPin(GPIOA, DAP_SWDIO_TMS_PIN);
    LL_GPIO_SetOutputPin(GPIOA, DAP_TDI_PIN);
    LL_GPIO_SetOutputPin(GPIOA, DAP_NTRST_PIN);
    LL_GPIO_SetOutputPin(GPIOA, DAP_NRESET_PIN);

    GPIO_InitStruct.Pin = DAP_TDO_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static inline void SwdPortInit(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = DAP_SWK_TCK_PIN | DAP_SWDIO_TMS_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = DAP_NRESET_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    LL_GPIO_SetOutputPin(GPIOA, DAP_SWK_TCK_PIN);
    LL_GPIO_SetOutputPin(GPIOA, DAP_SWDIO_TMS_PIN);
    LL_GPIO_SetOutputPin(GPIOA, DAP_NRESET_PIN);

    GPIO_InitStruct.Pin = DAP_TDO_PIN | DAP_TDI_PIN | DAP_NTRST_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

//port exit
static inline void SwjPortExit(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DAP_SWK_TCK_PIN | DAP_SWDIO_TMS_PIN | DAP_TDI_PIN | DAP_TDO_PIN | DAP_NTRST_PIN | DAP_NRESET_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

//--------------------------led----------------------------
static inline void SetDapConnLed(uint32_t bit)
{
    if (bit)
        LL_GPIO_SetOutputPin(DAP_LED1_PORT, DAP_LED1_PIN);
    else
        LL_GPIO_ResetOutputPin(DAP_LED1_PORT, DAP_LED1_PIN);
}

static inline void SetDapRunningLed(uint32_t bit)
{
    if (bit)
        LL_GPIO_SetOutputPin(DAP_LED2_PORT, DAP_LED2_PIN);
    else
        LL_GPIO_ResetOutputPin(DAP_LED2_PORT, DAP_LED2_PIN);
}

//-------------------------setup------------------------------------
static inline void DapSetup(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = DAP_SWK_TCK_PIN | DAP_SWDIO_TMS_PIN | DAP_TDI_PIN | DAP_TDO_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = DAP_NTRST_PIN | DAP_NRESET_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    SetDapConnLed(0);
    SetDapRunningLed(0);
}
