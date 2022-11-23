#include "cl_queue.h"
#include "multi_buffer.h"
#include "stdlib.h"

#define TEST_BUFF_SIZE (256)
#define TEST_BUFF_NUM (12)

typedef struct
{
    uint8_t start;
    uint32_t length;
} TestBuffInfo_t;

CL_QUEUE_DEF_INIT(testQueue, TEST_BUFF_NUM, TestBuffInfo_t, static);

MULTIBUFFER_STATIC_DEF(testBuffer, TEST_BUFF_SIZE, TEST_BUFF_NUM, static);

void MultiBufferTestInit(void)
{
}

static bool AddPage(void)
{
    TestBuffInfo_t buffInfo;
    buffInfo.start = rand() % 256;
    buffInfo.length = (uint32_t)rand() % TEST_BUFF_SIZE;
    if (CL_QueueAdd(&testQueue, &buffInfo) != CL_ResSuccess)
        return false;

    uint8_t *pBuff;
    int res = MultiBufferGetBack(&testBuffer, &pBuff);
    if (!res == 0)
        return false;

    for (uint32_t i = 0; i < buffInfo.length; i++)
    {
        pBuff[i] = buffInfo.start + i;
    }

    res = MultiBufferPush(&testBuffer, buffInfo.length);
    if (!res == 0)
        return false;

    return true;
}

static bool PollAndCheckPage(void)
{
    TestBuffInfo_t buffInfo;
    if (CL_QueuePoll(&testQueue, &buffInfo) != CL_ResSuccess)
        return false;

    uint8_t *pBuff;
    uint32_t buffLen;
    int res = MultiBufferPeek(&testBuffer, 0, &pBuff, &buffLen);
    if (!res == 0)
        return false;

    if (buffLen != buffInfo.length)
        return false;
    for (uint32_t i = 0; i < buffLen; i++)
    {
        if (pBuff[i] != buffInfo.start + i)
            return false;
    }

    res = MultiBufferPop(&testBuffer);
    if (!res == 0)
        return false;

    return true;
}

void MultiBufferTestMain(void)
{
}

void MultiBufferTestIsr(void)
{
}
