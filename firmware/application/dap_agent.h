#pragma once

#include "cl_common.h"

void DapAgent_Init(void);
void DapAgent_Process(void);

uint8_t* DapAgent_GetCmdBuff(void);
bool DapAgent_CmdRecvDone(uint32_t len);

bool DapAgent_GetRspBuff(uint8_t** ppbuff, uint32_t* pLen);
void DapAgent_RspSendDone(void);

void DapAgent_Test(void);
