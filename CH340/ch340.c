#include "ch340.h"

static USBH_StatusTypeDef CH340_Write(USBH_HandleTypeDef *phost, uint8_t request, uint16_t value, uint16_t index) {
    phost->Control.setup.b.bmRequestType = VENDOR_WRITE_TYPE;
    phost->Control.setup.b.bRequest = request;
    phost->Control.setup.b.wValue.w = value;
    phost->Control.setup.b.wIndex.w = index;
    phost->Control.setup.b.wLength.w = 0;

    return USBH_CtlReq(phost, NULL, 0);
}

static USBH_StatusTypeDef CH340_Read(USBH_HandleTypeDef *phost, uint8_t request, uint16_t value, uint16_t index, uint8_t *buf, uint16_t len) {
    phost->Control.setup.b.bmRequestType = VENDOR_READ_TYPE;
    phost->Control.setup.b.bRequest = request;
    phost->Control.setup.b.wValue.w = value;
    phost->Control.setup.b.wIndex.w = index;
    phost->Control.setup.b.wLength.w = len;

    return USBH_CtlReq(phost, buf, len);
}

static int CH340_CalcBaudRate(uint32_t baud_rate, uint8_t *factor, uint8_t *divisor) {
    uint8_t a;
    uint8_t b;
    unsigned long c;

    switch (baud_rate) {
        case 921600: a = 0xf3; b = 7; break;
        case 307200: a = 0xd9; b = 7; break;
        default:
            if (baud_rate > 6000000/255) { b = 3; c = 6000000; }
            else if (baud_rate > 750000/255) { b = 2; c = 750000; }
            else if (baud_rate > 93750/255) { b = 1; c = 93750; }
            else { b = 0; c = 11719; }

            a = (uint8_t)(c / baud_rate);
            if (a == 0 || a == 0xFF) return -1;
            if ((c / a - baud_rate) > (baud_rate - c / (a + 1))) a++;
            a = 256 - a;
            break;
    }
    *factor = a;
    *divisor = b;
    return 0;
}

static USBH_StatusTypeDef CH340_SetBaudrate(USBH_HandleTypeDef *phost, uint32_t baudrate) {
    uint8_t factor = 0, divisor = 0;

    if (CH340_CalcBaudRate(baudrate, &factor, &divisor) != 0) {
        return USBH_FAIL;
    }

    uint16_t wValue = 0xC09C;
    uint16_t wIndex = 0x0080 | divisor | ((uint16_t)factor << 8);

    return CH340_Write(phost, VENDOR_SERIAL_INIT, wValue, wIndex);
}

USBH_StatusTypeDef CH340_GetVersion(USBH_HandleTypeDef *phost, uint8_t *ver_buf) {
    return CH340_Read(phost, VENDOR_VERSION, 0x0000, 0x0000, ver_buf, 2);
}

USBH_StatusTypeDef CH340_Init(USBH_HandleTypeDef *phost, uint32_t baudrate) {
    USBH_StatusTypeDef status = USBH_OK;

    // init
    status = CH340_Write(phost, VENDOR_SERIAL_INIT, 0x0000, 0x0000);
    if (status != USBH_OK) return status;
    HAL_Delay(10);

    CH340_Write(phost, VENDOR_WRITE, 0x1312, 0xD982);
    CH340_Write(phost, VENDOR_WRITE, 0x0F2C, 0x0004);

    // pins uart
    CH340_Write(phost, VENDOR_WRITE, 0x2727, 0x0000);
    CH340_SetBaudrate(phost, baudrate);

    // modem
    CH340_Write(phost, VENDOR_MODEM_OUT, 0x009F, 0x0000);

    return status;
}
