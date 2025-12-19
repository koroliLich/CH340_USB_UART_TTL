# CH340_USB_UART_TTL

This project implements an additional UART interface using the CH340 module with custom driver. An arbitrary array is transmitted from the STM32 via USB Host (CDC) and received via UART on interrupt. The received array is sent back via UART in reverse order. 

* **Development Board:** WeAct STM32F411 "Black Pill"
* **USB-UART:** CH340 module

## Software
* STM32CubeIDE 
