/**
 ******************************************************************************
 * @file    usbd_hid.c
 * @author  MCD Application Team
 * @brief   This file provides the HID core functions.
 *
 * @verbatim
 *
 *          ===================================================================
 *                                HID Class  Description
 *          ===================================================================
 *           This module manages the HID class V1.11 following the "Device Class Definition
 *           for Human Interface Devices (HID) Version 1.11 Jun 27, 2001".
 *           This driver implements the following aspects of the specification:
 *             - The Boot Interface Subclass
 *             - The Mouse protocol
 *             - Usage Page : Generic Desktop
 *             - Usage : Joystick
 *             - Collection : Application
 *
 * @note     In HS mode and when the DMA is used, all variables and data structures
 *           dealing with the DMA during the transaction process should be 32-bit aligned.
 *
 *
 *  @endverbatim
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                      www.st.com/SLA0044
 *
 ******************************************************************************
 */

/* BSPDependencies
- "stm32xxxxx_{eval}{discovery}{nucleo_144}.c"
- "stm32xxxxx_{eval}{discovery}_io.c"
EndBSPDependencies */

/* Includes ------------------------------------------------------------------*/
#include "usbd_hid.h"
#include "usbd_ctlreq.h"
#include "cl_log.h"
#include "systime.h"
/** @addtogroup STM32_USB_DEVICE_LIBRARY
 * @{
 */

/** @defgroup USBD_HID
 * @brief usbd core module
 * @{
 */

/** @defgroup USBD_HID_Private_TypesDefinitions
 * @{
 */
/**
 * @}
 */

/** @defgroup USBD_HID_Private_Defines
 * @{
 */

/**
 * @}
 */

/** @defgroup USBD_HID_Private_Macros
 * @{
 */
/**
 * @}
 */

/** @defgroup USBD_HID_Private_FunctionPrototypes
 * @{
 */

static uint8_t USBD_HID_Init(USBD_HandleTypeDef *pdev,
                             uint8_t cfgidx);

static uint8_t USBD_HID_DeInit(USBD_HandleTypeDef *pdev,
                               uint8_t cfgidx);

static uint8_t USBD_HID_Setup(USBD_HandleTypeDef *pdev,
                              USBD_SetupReqTypedef *req);

static uint8_t *USBD_HID_GetFSCfgDesc(uint16_t *length);

static uint8_t *USBD_HID_GetDeviceQualifierDesc(uint16_t *length);

static uint8_t USBD_HID_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_HID_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
/**
 * @}
 */

/** @defgroup USBD_HID_Private_Variables
 * @{
 */

USBD_ClassTypeDef USBD_HID = {
    .Init = USBD_HID_Init,
    .DeInit = USBD_HID_DeInit,
    .Setup = USBD_HID_Setup,
    .EP0_TxSent = NULL,          /*EP0_TxSent*/
    .EP0_RxReady = NULL,         /*EP0_RxReady*/
    .DataIn = USBD_HID_DataIn,   /*DataIn*/
    .DataOut = USBD_HID_DataOut, /*DataOut*/
    .SOF = NULL,                 /*SOF */
    .IsoINIncomplete = NULL,
    .IsoOUTIncomplete = NULL,
    .GetHSConfigDescriptor = NULL,
    .GetFSConfigDescriptor = USBD_HID_GetFSCfgDesc,
    .GetOtherSpeedConfigDescriptor = NULL,
    .GetDeviceQualifierDescriptor = USBD_HID_GetDeviceQualifierDesc,
};

/* USB HID device FS Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_HID_CfgFSDesc[USB_HID_CONFIG_DESC_SIZ] __ALIGN_END = {
    0x09,                        /* bLength: Configuration Descriptor size */
    USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
    0x60,
    /* wTotalLength: Bytes returned */
    0x00,
    0x03, /*bNumInterfaces: 3 interface*/
    0x01, /*bConfigurationValue: Configuration value*/
    0x00, /*iConfiguration: Index of string descriptor describing
the configuration*/
    0xa0, /*bmAttributes: bus powered and Support Remote Wake-up */
    0xfa, /*MaxPower 100 mA: this current is used for detecting Vbus*/

    /*--------------- interface 0 ****************/
    /* 09 */
    0x09,                    /*bLength: Interface Descriptor size*/
    USB_DESC_TYPE_INTERFACE, /*bDescriptorType: Interface descriptor type*/
    0x00,                    /*bInterfaceNumber: Number of Interface*/
    0x00,                    /*bAlternateSetting: Alternate setting*/
    0x02,                    /*bNumEndpoints*/
    0xff,                    /*bInterfaceClass*/
    0x47,                    /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0xd0,                    /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,                       /*iInterface: Index of string descriptor*/
    /*~~~ endpoint 02h ********************/
    /* 27 */
    0x07,                   /*bLength: Endpoint Descriptor size*/
    USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/
    0x02,                   /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,                   /*bmAttributes: Interrupt endpoint*/
    0x40,                   /*wMaxPacketSize */
    0x00,
    4, /*bInterval: Polling Interval */
    /*~~~ endpoint 82h ********************/
    /* 27 */
    0x07,                   /*bLength: Endpoint Descriptor size*/
    USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/
    0x82,                   /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,                   /*bmAttributes: Interrupt endpoint*/
    0x40,                   /*wMaxPacketSize */
    0x00,
    4, /*bInterval: Polling Interval */

    /*---------------- interface 1 ****************/
    /* 09 */
    0x09,                    /*bLength: Interface Descriptor size*/
    USB_DESC_TYPE_INTERFACE, /*bDescriptorType: Interface descriptor type*/
    0x01,                    /*bInterfaceNumber: Number of Interface*/
    0x00,                    /*bAlternateSetting: Alternate setting*/
    0x00,                    /*bNumEndpoints*/
    0xff,                    /*bInterfaceClass*/
    0x47,                    /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0xd0,                    /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,                       /*iInterface: Index of string descriptor*/
    /*++++ interface 1.1 ****************/
    /* 09 */
    0x09,                    /*bLength: Interface Descriptor size*/
    USB_DESC_TYPE_INTERFACE, /*bDescriptorType: Interface descriptor type*/
    0x01,                    /*bInterfaceNumber: Number of Interface*/
    0x01,                    /*bAlternateSetting: Alternate setting*/
    0x02,                    /*bNumEndpoints*/
    0xff,                    /*bInterfaceClass*/
    0x47,                    /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0xd0,                    /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,                       /*iInterface: Index of string descriptor*/
    /*~~~ endpoint 03h ********************/
    /* 27 */
    0x07,                   /*bLength: Endpoint Descriptor size*/
    USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/
    0x03,                   /*bEndpointAddress: Endpoint Address (IN)*/
    0x01,                   /*bmAttributes: iso*/
    0xe4,                   /*wMaxPacketSize */
    0x00,
    1, /*bInterval: Polling Interval */
    /*~~~ endpoint 83h ********************/
    /* 27 */
    0x07,                   /*bLength: Endpoint Descriptor size*/
    USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/
    0x83,                   /*bEndpointAddress: Endpoint Address (IN)*/
    0x01,                   /*bmAttributes: iso*/
    0x40,                   /*wMaxPacketSize */
    0x00,
    1, /*bInterval: Polling Interval */

    /*----------------- interface 2 ****************/
    /* 09 */
    0x09,                    /*bLength: Interface Descriptor size*/
    USB_DESC_TYPE_INTERFACE, /*bDescriptorType: Interface descriptor type*/
    0x02,                    /*bInterfaceNumber: Number of Interface*/
    0x00,                    /*bAlternateSetting: Alternate setting*/
    0x00,                    /*bNumEndpoints*/
    0xff,                    /*bInterfaceClass*/
    0x47,                    /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0xd0,                    /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,                       /*iInterface: Index of string descriptor*/
    /*+++ interface 2.1 ****************/
    /* 09 */
    0x09,                    /*bLength: Interface Descriptor size*/
    USB_DESC_TYPE_INTERFACE, /*bDescriptorType: Interface descriptor type*/
    0x02,                    /*bInterfaceNumber: Number of Interface*/
    0x01,                    /*bAlternateSetting: Alternate setting*/
    0x02,                    /*bNumEndpoints*/
    0xff,                    /*bInterfaceClass*/
    0x47,                    /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0xd0,                    /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,                       /*iInterface: Index of string descriptor*/
    /******************** endpoint 04h ********************/
    /* 27 */
    0x07,                   /*bLength: Endpoint Descriptor size*/
    USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/
    0x04,                   /*bEndpointAddress: Endpoint Address (IN)*/
    0x02,                   /*bmAttributes: iso*/
    0x40,                   /*wMaxPacketSize */
    0x00,
    0, /*bInterval: Polling Interval */
    /******************** endpoint 84h ********************/
    /* 27 */
    0x07,                   /*bLength: Endpoint Descriptor size*/
    USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/
    0x84,                   /*bEndpointAddress: Endpoint Address (IN)*/
    0x02,                   /*bmAttributes: iso*/
    0x40,                   /*wMaxPacketSize */
    0x00,
    0, /*bInterval: Polling Interval */
};

/* USB HID device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_HID_Desc[USB_HID_DESC_SIZ] __ALIGN_END = {
    0x09,                        /* bLength: Configuration Descriptor size */
    USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
    0x60,
    /* wTotalLength: Bytes returned */
    0x00,
    0x03, /*bNumInterfaces: 1 interface*/
    0x01, /*bConfigurationValue: Configuration value*/
    0x00, /*iConfiguration: Index of string descriptor describing
the configuration*/
    0xa0, /*bmAttributes: bus powered and Support Remote Wake-up */
    0xfa, /*MaxPower 100 mA: this current is used for detecting Vbus*/
};

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_HID_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END = {
    USB_LEN_DEV_QUALIFIER_DESC,
    USB_DESC_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x01,
    0x00,
};

__ALIGN_BEGIN static uint8_t USBD_Custom_0x90[40] = {
    0x28,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x04,
    0x00,
    0x01,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x58,
    0x47,
    0x49,
    0x50,
    0x31,
    0x30,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};

__ALIGN_BEGIN static uint8_t HID_MOUSE_ReportDesc[HID_MOUSE_REPORT_DESC_SIZE] __ALIGN_END = {
    0x05, 0x01,
    0x09, 0x02,
    0xA1, 0x01,
    0x09, 0x01,

    0xA1, 0x00,
    0x05, 0x09,
    0x19, 0x01,
    0x29, 0x03,

    0x15, 0x00,
    0x25, 0x01,
    0x95, 0x03,
    0x75, 0x01,

    0x81, 0x02,
    0x95, 0x01,
    0x75, 0x05,
    0x81, 0x01,

    0x05, 0x01,
    0x09, 0x30,
    0x09, 0x31,
    0x09, 0x38,

    0x15, 0x81,
    0x25, 0x7F,
    0x75, 0x08,
    0x95, 0x03,

    0x81, 0x06,
    0xC0, 0x09,
    0x3c, 0x05,
    0xff, 0x09,

    0x01, 0x15,
    0x00, 0x25,
    0x01, 0x75,
    0x01, 0x95,

    0x02, 0xb1,
    0x22, 0x75,
    0x06, 0x95,
    0x01, 0xb1,

    0x01, 0xc0};

/**
 * @}
 */

/** @defgroup USBD_HID_Private_Functions
 * @{
 */

/**
 * @brief  USBD_HID_Init
 *         Initialize the HID interface
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
static uint8_t USBD_HID_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
    /* Open EP IN */
    USBD_LL_OpenEP(pdev, CON_INTR_EP_OUT, USBD_EP_TYPE_INTR, 0x40);
    pdev->ep_in[CON_INTR_EP_OUT & 0xFU].is_used = 1U;

    USBD_LL_OpenEP(pdev, CON_INTR_EP_IN, USBD_EP_TYPE_INTR, 0x40);
    pdev->ep_in[CON_INTR_EP_IN & 0xFU].is_used = 1U;

    pdev->pClassData = USBD_malloc(sizeof(USBD_HID_HandleTypeDef));

    if (pdev->pClassData == NULL)
    {
        return USBD_FAIL;
    }

    ((USBD_HID_HandleTypeDef *)pdev->pClassData)->state = HID_IDLE;

    return USBD_OK;
}

/**
 * @brief  USBD_HID_Init
 *         DeInitialize the HID layer
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
static uint8_t USBD_HID_DeInit(USBD_HandleTypeDef *pdev,
                               uint8_t cfgidx)
{
    /* Close HID EPs */
    USBD_LL_CloseEP(pdev, CON_INTR_EP_OUT);
    pdev->ep_in[CON_INTR_EP_OUT & 0xFU].is_used = 0U;

    USBD_LL_CloseEP(pdev, CON_INTR_EP_IN);
    pdev->ep_in[CON_INTR_EP_IN & 0xFU].is_used = 0U;

    /* FRee allocated memory */
    if (pdev->pClassData != NULL)
    {
        USBD_free(pdev->pClassData);
        pdev->pClassData = NULL;
    }

    return USBD_OK;
}

/**
 * @brief  USBD_HID_Setup
 *         Handle the HID specific requests
 * @param  pdev: instance
 * @param  req: usb requests
 * @retval status
 */
static uint8_t USBD_HID_Setup(USBD_HandleTypeDef *pdev,
                              USBD_SetupReqTypedef *req)
{
    USBD_HID_HandleTypeDef *hhid = (USBD_HID_HandleTypeDef *)pdev->pClassData;
    uint16_t len = 0U;
    uint8_t *pbuf = NULL;
    uint16_t status_info = 0U;
    USBD_StatusTypeDef ret = USBD_OK;

    switch (req->bmRequest & USB_REQ_TYPE_MASK)
    {
    case USB_REQ_TYPE_VENDOR:
        if (req->bRequest == 0x90)
        {
            pbuf = USBD_Custom_0x90;
            len = req->wLength;
            if (len > sizeof(USBD_Custom_0x90))
                len = sizeof(USBD_Custom_0x90);

            USBD_CtlSendData(pdev, USBD_Custom_0x90, len);
        }
        break;
    case USB_REQ_TYPE_CLASS:
        switch (req->bRequest)
        {
        case HID_REQ_SET_PROTOCOL:
            hhid->Protocol = (uint8_t)(req->wValue);
            break;

        case HID_REQ_GET_PROTOCOL:
            USBD_CtlSendData(pdev, (uint8_t *)(void *)&hhid->Protocol, 1U);
            break;

        case HID_REQ_SET_IDLE:
            hhid->IdleState = (uint8_t)(req->wValue >> 8);
            break;

        case HID_REQ_GET_IDLE:
            USBD_CtlSendData(pdev, (uint8_t *)(void *)&hhid->IdleState, 1U);
            break;

        default:
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
            break;
        }
        break;
    case USB_REQ_TYPE_STANDARD:
        switch (req->bRequest)
        {
        case USB_REQ_GET_STATUS:
            if (pdev->dev_state == USBD_STATE_CONFIGURED)
            {
                USBD_CtlSendData(pdev, (uint8_t *)(void *)&status_info, 2U);
            }
            else
            {
                USBD_CtlError(pdev, req);
                ret = USBD_FAIL;
            }
            break;

        case USB_REQ_GET_DESCRIPTOR:
            if (req->wValue >> 8 == HID_REPORT_DESC)
            {
                len = MIN(HID_MOUSE_REPORT_DESC_SIZE, req->wLength);
                pbuf = HID_MOUSE_ReportDesc;
            }
            else if (req->wValue >> 8 == HID_DESCRIPTOR_TYPE)
            {
                pbuf = USBD_HID_Desc;
                len = MIN(USB_HID_DESC_SIZ, req->wLength);
            }
            else
            {
                USBD_CtlError(pdev, req);
                ret = USBD_FAIL;
                break;
            }
            USBD_CtlSendData(pdev, pbuf, len);
            break;

        case USB_REQ_GET_INTERFACE:
            if (pdev->dev_state == USBD_STATE_CONFIGURED)
            {
                USBD_CtlSendData(pdev, (uint8_t *)(void *)&hhid->AltSetting, 1U);
            }
            else
            {
                USBD_CtlError(pdev, req);
                ret = USBD_FAIL;
            }
            break;

        case USB_REQ_SET_INTERFACE:
            if (pdev->dev_state == USBD_STATE_CONFIGURED)
            {
                hhid->AltSetting = (uint8_t)(req->wValue);
                CL_LOG_LINE("set itf: %d, %d", req->wIndex, req->wValue);
                USBD_CtlSendStatus(pdev);
            }
            else
            {
                USBD_CtlError(pdev, req);
                ret = USBD_FAIL;
            }
            break;

        default:
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
            break;
        }
        break;

    default:
        USBD_CtlError(pdev, req);
        ret = USBD_FAIL;
        break;
    }

    return ret;
}

/**
 * @brief  USBD_HID_GetPollingInterval
 *         return polling interval from endpoint descriptor
 * @param  pdev: device instance
 * @retval polling interval
 */
uint32_t USBD_HID_GetPollingInterval(USBD_HandleTypeDef *pdev)
{
    uint32_t polling_interval = 0U;

    /* HIGH-speed endpoints */
    if (pdev->dev_speed == USBD_SPEED_HIGH)
    {
        /* Sets the data transfer polling interval for high speed transfers.
         Values between 1..16 are allowed. Values correspond to interval
         of 2 ^ (bInterval-1). This option (8 ms, corresponds to HID_HS_BINTERVAL */
        polling_interval = (((1U << (HID_HS_BINTERVAL - 1U))) / 8U);
    }
    else /* LOW and FULL-speed endpoints */
    {
        /* Sets the data transfer polling interval for low and full
        speed transfers */
        polling_interval = HID_FS_BINTERVAL;
    }

    return ((uint32_t)(polling_interval));
}

/**
 * @brief  USBD_HID_GetCfgFSDesc
 *         return FS configuration descriptor
 * @param  speed : current device speed
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
static uint8_t *USBD_HID_GetFSCfgDesc(uint16_t *length)
{
    uint16_t maxLen = sizeof(USBD_HID_CfgFSDesc);
    if (length[0] > maxLen)
        length[0] = maxLen;
    return USBD_HID_CfgFSDesc;
}

/**
 * @brief  USBD_HID_DataIn
 *         handle data IN Stage
 * @param  pdev: device instance
 * @param  epnum: endpoint index
 * @retval status
 */
static uint8_t USBD_HID_DataIn(USBD_HandleTypeDef *pdev,
                               uint8_t epnum)
{
    uint8_t testData[] = {
        0x20,
        0x00,
        0x00, // count
        0x0e,
        0x10, // bit-4:A
        0x00, 0x00, 0x00, 0x00, 0x00, 0x5c, 0x03, 0x73, 0xf4, 0xf9, 0xfe, 0x9d, 0x01};

    testData[2]++;
    uint32_t sec = GetSysTime() / 1000;
    if (sec & 0x01)
        testData[4] = 0x10;
    else
        testData[4] = 0x00;

    /* Ensure that the FIFO is empty before a new transfer, this condition could
    be caused by  a new transfer before the end of the previous transfer */
    ((USBD_HID_HandleTypeDef *)pdev->pClassData)->state = HID_IDLE;
    CL_LOG_LINE("data in: %x", epnum);
    if (epnum == CON_INTR_EP_IN)
    {
        USBD_LL_Transmit(pdev, CON_INTR_EP_IN, testData, sizeof(testData));
    }
    return USBD_OK;
}

static uint8_t USBD_HID_DataOut(struct _USBD_HandleTypeDef *pdev, uint8_t epnum)
{
    CL_LOG_LINE("data out");
    ((USBD_HID_HandleTypeDef *)pdev->pClassData)->state = HID_IDLE;
    return USBD_OK;
}

/**
 * @brief  DeviceQualifierDescriptor
 *         return Device Qualifier descriptor
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
static uint8_t *USBD_HID_GetDeviceQualifierDesc(uint16_t *length)
{
    *length = sizeof(USBD_HID_DeviceQualifierDesc);
    *length = 0;
    return USBD_HID_DeviceQualifierDesc;
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
