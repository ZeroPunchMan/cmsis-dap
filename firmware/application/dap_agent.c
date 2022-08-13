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

CL_Result_t DapAgent_AddPacket(const uint8_t *buff)
{
    const DapCmd_t* dapCmd = (const DapCmd_t*)buff;
    CL_QueueAdd(&cmdQueue, dapCmd);

    return CL_ResSuccess;
}

void DapAgent_Init(void)
{

}

void DapAgent_Process(void)
{

}

