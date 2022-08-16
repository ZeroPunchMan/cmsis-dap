#pragma once

#include "cl_common.h"

void DapAgent_Init(void);
void DapAgent_Process(void);

CL_Result_t DapAgent_AddPacket(const uint8_t *buff, uint16_t len);

void DapAgent_Test(void);
