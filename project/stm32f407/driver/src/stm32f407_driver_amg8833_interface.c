/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      stm32f407_driver_amg8833_interface.c
 * @brief     stm32f407 driver amg8833 interface source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-01-01
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/01/01  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_amg8833_interface.h"
#include "delay.h"
#include "iic.h"
#include "uart.h"
#include <stdarg.h>

/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t amg8833_interface_iic_init(void)
{
    return iic_init();
}

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t amg8833_interface_iic_deinit(void)
{
    return iic_deinit();
}

/**
 * @brief      interface iic bus read
 * @param[in]  addr is iic device write address
 * @param[in]  reg is iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t amg8833_interface_iic_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    uint8_t res;
    
    __set_BASEPRI(1);
    res = iic_read(addr, reg, buf, len);
    __set_BASEPRI(0);
    
    return res;
}

/**
 * @brief     interface iic bus write
 * @param[in] addr is iic device write address
 * @param[in] reg is iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t amg8833_interface_iic_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    uint8_t res;
    
    __set_BASEPRI(1);
    res = iic_write(addr, reg, buf, len);
    __set_BASEPRI(0);
    
    return res;
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void amg8833_interface_delay_ms(uint32_t ms)
{
    delay_ms(ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void amg8833_interface_debug_print(const char *const fmt, ...)
{
    char str[256];
    uint16_t len;
    va_list args;
    
    memset((char *)str, 0, sizeof(char) * 256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);
    
    len = strlen((char *)str);
    (void)uart_write((uint8_t *)str, len);
}

/**
 * @brief     interface receive callback
 * @param[in] type is the interrupt type
 * @note      none
 */
void amg8833_interface_receive_callback(uint8_t type)
{
    switch (type)
    {
        case AMG8833_STATUS_OVF_THS :
        {
            amg8833_interface_debug_print("amg8833: irq thermistor temperature output overflow.\n");
            
            break;
        }
        case AMG8833_STATUS_OVF_IRS :
        {
            amg8833_interface_debug_print("amg8833: irq temperature output overflow.\n");
            
            break;
        }
        case AMG8833_STATUS_INTF :
        {
            amg8833_interface_debug_print("amg8833: irq interrupt outbreak.\n");
            
            break;
        }
        default :
        {
            amg8833_interface_debug_print("amg8833: unknown code.\n");
            
            break;
        }
    }
}
