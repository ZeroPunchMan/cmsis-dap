#pragma once

#include "cl_log.h"

#define USB_LOG(format, ...) //CL_LOG_INFO(format, ##__VA_ARGS__)

#define MAIN_LOG(format, ...) CL_LOG_INFO(format, ##__VA_ARGS__)
