#include "dap_agent.h"
#include "DAP_config.h"
#include "cl_log.h"
#include "cl_queue.h"
#include "DAP_config.h"
#include "DAP.h"


typedef struct
{
    uint8_t cmdBuff[DAP_PACKET_SIZE];
} DapCmd_t;


uint8_t rspTotalBuff[DAP_PACKET_SIZE * DAP_PACKET_COUNT];

CL_QUEUE_DEF_INIT(cmdQueue, DAP_PACKET_COUNT * 2, DapCmd_t, static);

CL_Result_t DapAgent_AddPacket(const uint8_t *buff, uint16_t len)
{
    const DapCmd_t* dapCmd = (const DapCmd_t*)buff;
    CL_QueueAdd(&cmdQueue, dapCmd);
    CL_LOG_LINE("cmd:%x,%x", buff[0], len);

    return CL_ResSuccess;
}

void DapAgent_Init(void)
{
    DAP_Setup();
}

void DapAgent_Process(void)
{
    DapCmd_t* pFirstCmd;
    if(CL_QueuePeek(&cmdQueue, &pFirstCmd) == CL_ResSuccess)
    {
        if(pFirstCmd->cmdBuff[0] < ID_DAP_QueueCommands) //command
        {   
            DAP_ProcessCommand(pFirstCmd->cmdBuff, )
            CL_QueuePoll(&cmdQueue, CL_NULL);
        }
    }
}

