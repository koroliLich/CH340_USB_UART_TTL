#ifndef CH340_H_
#define CH340_H_

#include "usbh_core.h"
#include "usbh_cdc.h"

#define CH34x_VENDOR_ID			0x1A86
#define CH340_PRODUCT_ID        0x7523
#define CH341_PRODUCT_ID		0x5523

#define VENDOR_WRITE_TYPE       0x40  // stm -> ch340
#define VENDOR_READ_TYPE        0xC0  // ch340 -> stm

#define VENDOR_READ             0x95  // read reg
#define VENDOR_WRITE            0x9A  // write reg
#define VENDOR_SERIAL_INIT      0xA1  // init
#define VENDOR_MODEM_OUT        0xA4  //
#define VENDOR_VERSION          0x5F  // read version


USBH_StatusTypeDef CH340_Init(USBH_HandleTypeDef *phost, uint32_t baudrate);
USBH_StatusTypeDef CH340_GetVersion(USBH_HandleTypeDef *phost, uint8_t *ver_buf);

#endif /* CH340_H_ */
