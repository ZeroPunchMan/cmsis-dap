#include "dap_agent.h"
#include "DAP_config.h"
#include "my_log.h"
#include "cl_queue.h"
#include "DAP_config.h"
#include "DAP.h"
#include "multi_buffer.h"

MULTIBUFFER_STATIC_DEF(cmdMulitBuff, DAP_PACKET_SIZE, DAP_PACKET_COUNT * 2, static);
MULTIBUFFER_STATIC_DEF(rspMultiBuff, DAP_PACKET_SIZE, DAP_PACKET_COUNT * 2, static);

uint8_t *DapAgent_GetCmdBuff(void)
{
    uint8_t *pBuff = NULL;
    int res = MultiBufferGetBack(&cmdMulitBuff, &pBuff);
    if (res != 0)
    { // error, shouldn't be
    }
    return pBuff;
}

bool DapAgent_CmdRecvDone(uint32_t len)
{
    uint8_t *pBuff = NULL;
    MultiBufferGetBack(&cmdMulitBuff, &pBuff);
    // USB_LOG("cmd:%d,len:%ld", pBuff[0], len);
    return MultiBufferPush(&cmdMulitBuff, len) == 0;
}

void DapAgent_Init(void)
{
    DAP_Setup();
}

static volatile bool clearCmd = false;
void DapAgent_ResetFromIsr(void)
{  //USB reset
#if defined (USB_RESET_MCU)
    NVIC_SystemReset();
#endif

    DAP_Setup();

    uint8_t* pBuff;
    uint32_t len;
    while(DapAgent_GetRspBuff(&pBuff, &len))
        DapAgent_RspSendDone();

    clearCmd = true;
}

static void DapAgent_RecvProc(void)
{
    if (MultiBufferGetCount(&cmdMulitBuff) == 0)
        return;

    uint8_t *pRecvBuff;
    uint32_t bufLen;
    MultiBufferPeek(&cmdMulitBuff, 0, &pRecvBuff, &bufLen);

    uint8_t *pRspBuff;
    uint16_t rspLen = 0;
    if (pRecvBuff[0] == ID_DAP_QueueCommands)
    {
        uint8_t *pEnd;
        uint16_t endIdx = UINT16_MAX;
        for (uint16_t i = 1; i < MultiBufferGetCount(&cmdMulitBuff); i++)
        {
            MultiBufferPeek(&cmdMulitBuff, i, &pEnd, &bufLen);
            if (pEnd[0] != ID_DAP_QueueCommands)
            {
                endIdx = i;
                break;
            }
        }

        if (endIdx != UINT16_MAX)
        {
            for (uint16_t i = 0; i < endIdx; i++)
            {
                MultiBufferPeek(&cmdMulitBuff, 0, &pRecvBuff, &bufLen);

                pRecvBuff[0] = ID_DAP_ExecuteCommands;
                MultiBufferGetBack(&rspMultiBuff, &pRspBuff);
                rspLen = DAP_ExecuteCommand(pRecvBuff, pRspBuff);
                MultiBufferPush(&rspMultiBuff, rspLen);

                MultiBufferPop(&cmdMulitBuff);
            }
        }
    }
    else if (pRecvBuff[0] == ID_DAP_ExecuteCommands || pRecvBuff[0] < ID_DAP_QueueCommands)
    {
        MultiBufferGetBack(&rspMultiBuff, &pRspBuff);
        rspLen = DAP_ExecuteCommand(pRecvBuff, pRspBuff);
        MultiBufferPush(&rspMultiBuff, rspLen);

        MAIN_LOG("cmd:%x,%x,%x\r\n", pRecvBuff[0], pRecvBuff[1], pRecvBuff[2]);
        // MAIN_LOG(" rsp:%d--%x,%x,%x,%x,%x\r\n", rspLen, pRspBuff[0], pRspBuff[1], pRspBuff[2], pRspBuff[3], pRspBuff[4]);
        // MAIN_LOG(" rsp:%d-%x\r\n", rspLen, pRspBuff[0]);

        MultiBufferPop(&cmdMulitBuff);
    }
}

void DapAgent_Process(void)
{
    if(clearCmd)
    {
        while(MultiBufferGetCount(&cmdMulitBuff) > 0)
            MultiBufferPop(&cmdMulitBuff);

        clearCmd = false;
    }

    DapAgent_RecvProc();
}

bool DapAgent_GetRspBuff(uint8_t **ppbuff, uint32_t *pLen)
{
    if (MultiBufferGetCount(&rspMultiBuff) > 0)
    {
        MultiBufferPeek(&rspMultiBuff, 0, ppbuff, pLen);
        return true;
    }

    return false;
}

void DapAgent_RspSendDone(void)
{
    MultiBufferPop(&rspMultiBuff);
}


