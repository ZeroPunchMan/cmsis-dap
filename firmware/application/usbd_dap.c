
/* Includes ------------------------------------------------------------------*/
#include "usbd_dap.h"
#include "usbd_ctlreq.h"

static uint8_t USBD_DAP_Init(USBD_HandleTypeDef *pdev,
                             uint8_t cfgidx);

static uint8_t USBD_DAP_DeInit(USBD_HandleTypeDef *pdev,
                               uint8_t cfgidx);

static uint8_t USBD_DAP_Setup(USBD_HandleTypeDef *pdev,
                              USBD_SetupReqTypedef *req);

static uint8_t *USBD_DAP_GetFSCfgDesc(uint16_t *length);

static uint8_t *USBD_DAP_GetDeviceQualifierDesc(uint16_t *length);

static uint8_t USBD_DAP_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
/**
 * @}
 */

/** @defgroup USBD_DAP_Private_Variables
 * @{
 */

USBD_ClassTypeDef USBD_DAP =
    {
        USBD_DAP_Init,
        USBD_DAP_DeInit,
        USBD_DAP_Setup,
        NULL,            /*EP0_TxSent*/
        NULL,            /*EP0_RxReady*/
        USBD_DAP_DataIn, /*DataIn*/
        NULL,            /*DataOut*/
        NULL,            /*SOF */
        NULL,
        NULL,
        NULL,
        USBD_DAP_GetFSCfgDesc,
        NULL,
        USBD_DAP_GetDeviceQualifierDesc,
};

/* USB device FS Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_DAP_CfgFSDesc[USB_DAP_CONFIG_DESC_SIZ] __ALIGN_END =
    {
        0x09,                          /* bLength: Configuration Descriptor size */
        USB_DESC_TYPE_CONFIGURATION,   /* bDescriptorType: Configuration */
        USB_DAP_CONFIG_DESC_SIZ, 0x00, /* wTotalLength: Bytes returned */
        0x01,                          /*bNumInterfaces: 1 interface*/
        0x01,                          /*bConfigurationValue: Configuration value*/
        0x00,                          /*iConfiguration: Index of string descriptor describing
                         the configuration*/
        0x80,                          /*bmAttributes: bus powered and Support Remote Wake-up */
        0x32,                          /*MaxPower 100 mA: this current is used for detecting Vbus*/

        /* 09 */
        0x09,                    /*bLength: Interface Descriptor size*/
        USB_DESC_TYPE_INTERFACE, /*bDescriptorType: Interface descriptor type*/
        0x00,                    /*bInterfaceNumber: Number of Interface*/
        0x00,                    /*bAlternateSetting: Alternate setting*/
        0x03,                    /*bNumEndpoints*/
        0xff,                    /*bInterfaceClass: vendor defined*/
        0x00,                    /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
        0x00,                    /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
        0,                       /*iInterface: Index of string descriptor*/
        /******************** Descriptor of endpoint 1********************/
        /* 18 */
        0x07,                   /*bLength: Endpoint Descriptor size*/
        USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

        DAP_EP1_ADDR, /*bEndpointAddress: Endpoint Address (OUT)*/
        0x02,         /*bmAttributes: bulk endpoint*/
        64,           /*wMaxPacketSize: 64 Byte max */
        0x00,
        0, /*bInterval: Polling Interval */
        /******************** Descriptor of endpoint 2 ********************/
        /* 25 */
        0x07,                   /*bLength: Endpoint Descriptor size*/
        USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

        DAP_EP2_ADDR, /*bEndpointAddress: Endpoint Address (OUT)*/
        0x02,         /*bmAttributes: bulk endpoint*/
        64,           /*wMaxPacketSize: 64 Byte max */
        0x00,
        0, /*bInterval: Polling Interval */
        /******************** Descriptor of endpoint 3 ********************/
        /* 32 */
        0x07,                   /*bLength: Endpoint Descriptor size*/
        USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

        DAP_EP3_ADDR, /*bEndpointAddress: Endpoint Address (OUT)*/
        0x02,         /*bmAttributes: bulk endpoint*/
        64,           /*wMaxPacketSize: 64 Byte max */
        0x00,
        0, /*bInterval: Polling Interval */
};

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_DAP_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
    {
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

/**
 * @}
 */

/** @defgroup USBD_DAP_Private_Functions
 * @{
 */

/**
 * @brief  USBD_DAP_Init
 *         Initialize the interface
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
static uint8_t USBD_DAP_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
    /* Open EP 1 */
    USBD_LL_OpenEP(pdev, DAP_EP1_ADDR, USBD_EP_TYPE_BULK, 64);
    pdev->ep_out[DAP_EP1_ADDR & 0xFU].is_used = 1U;

    /* Open EP 2 */
    USBD_LL_OpenEP(pdev, DAP_EP2_ADDR, USBD_EP_TYPE_BULK, 64);
    pdev->ep_in[DAP_EP2_ADDR & 0xFU].is_used = 1U;
    
    /* Open EP 3 */
    USBD_LL_OpenEP(pdev, DAP_EP3_ADDR, USBD_EP_TYPE_BULK, 64);
    pdev->ep_in[DAP_EP3_ADDR & 0xFU].is_used = 1U;

    pdev->pClassData = USBD_malloc(sizeof(USBD_DAP_HandleTypeDef));

    if (pdev->pClassData == NULL)
    {
        return USBD_FAIL;
    }

    return USBD_OK;
}

/**
 * @brief  USBD_DAP_Init
 *         DeInitialize the DAP layer
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
static uint8_t USBD_DAP_DeInit(USBD_HandleTypeDef *pdev,
                               uint8_t cfgidx)
{
    /* Close EPs */
    USBD_LL_CloseEP(pdev, DAP_EP1_ADDR);
    pdev->ep_out[DAP_EP1_ADDR & 0xFU].is_used = 0U;

    USBD_LL_CloseEP(pdev, DAP_EP2_ADDR);
    pdev->ep_in[DAP_EP2_ADDR & 0xFU].is_used = 0U;

    USBD_LL_CloseEP(pdev, DAP_EP3_ADDR);
    pdev->ep_in[DAP_EP3_ADDR & 0xFU].is_used = 0U;

    /* FRee allocated memory */
    if (pdev->pClassData != NULL)
    {
        USBD_free(pdev->pClassData);
        pdev->pClassData = NULL;
    }

    return USBD_OK;
}

/**
 * @brief  USBD_DAP_Setup
 *         Handle the specific requests
 * @param  pdev: instance
 * @param  req: usb requests
 * @retval status
 */
static uint8_t USBD_DAP_Setup(USBD_HandleTypeDef *pdev,
                              USBD_SetupReqTypedef *req)
{
    USBD_DAP_HandleTypeDef *hdap = (USBD_DAP_HandleTypeDef *)pdev->pClassData;
    // uint16_t len = 0U;
    // uint8_t *pbuf = NULL;
    uint16_t status_info = 0U;
    USBD_StatusTypeDef ret = USBD_OK;

    switch (req->bmRequest & USB_REQ_TYPE_MASK)
    {
    case USB_REQ_TYPE_CLASS:
        USBD_CtlError(pdev, req);
        ret = USBD_FAIL;
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
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
            break;

        case USB_REQ_GET_INTERFACE:
            if (pdev->dev_state == USBD_STATE_CONFIGURED)
            {
                USBD_CtlSendData(pdev, (uint8_t *)(void *)&hdap->AltSetting, 1U);
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
                hdap->AltSetting = (uint8_t)(req->wValue);
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
 * @brief  USBD_DAP_SendData
 *         Send USB data
 * @param  pdev: device instance
 * @param  buff: pointer to report
 * @retval status
 */
uint8_t USBD_DAP_SendData(USBD_HandleTypeDef *pdev,
                          uint8_t epAddr,
                          uint8_t *data,
                          uint16_t len)
{

    if (pdev->dev_state == USBD_STATE_CONFIGURED)
    {
        if(epAddr == DAP_EP2_ADDR || epAddr == DAP_EP3_ADDR)
        USBD_LL_Transmit(pdev, epAddr, data, len);
    }
    return USBD_OK;
}

/**
 * @brief  USBD_DAP_GetCfgFSDesc
 *         return FS configuration descriptor
 * @param  speed : current device speed
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
static uint8_t *USBD_DAP_GetFSCfgDesc(uint16_t *length)
{
    *length = sizeof(USBD_DAP_CfgFSDesc);
    return USBD_DAP_CfgFSDesc;
}

/**
 * @brief  USBD_DAP_DataIn
 *         handle data IN Stage
 * @param  pdev: device instance
 * @param  epnum: endpoint index
 * @retval status
 */
static uint8_t USBD_DAP_DataIn(USBD_HandleTypeDef *pdev,
                               uint8_t epnum)
{

    /* Ensure that the FIFO is empty before a new transfer, this condition could
    be caused by  a new transfer before the end of the previous transfer */
    return USBD_OK;
}

/**
 * @brief  DeviceQualifierDescriptor
 *         return Device Qualifier descriptor
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
static uint8_t *USBD_DAP_GetDeviceQualifierDesc(uint16_t *length)
{
    *length = sizeof(USBD_DAP_DeviceQualifierDesc);
    return USBD_DAP_DeviceQualifierDesc;
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
