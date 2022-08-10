#pragma once

#include "cl_common.h"

#define SYSTIME_SECOND(s) (s * 1000UL)

extern uint32_t g_sysTime;

static inline void SysTimeInc(uint32_t inc)
{
    g_sysTime += inc;
}

static inline uint32_t SysTimeSpan(uint32_t base)
{
    if (g_sysTime >= base)
        return g_sysTime - base;
    else
        return UINT32_MAX - base + g_sysTime + 1;
}

static inline uint32_t GetSysTime(void)
{
    return g_sysTime;
}

static inline void DelayOnSysTime(uint32_t time)
{
    uint32_t curTime = GetSysTime();
    while(SysTimeSpan(curTime) < time);
}
