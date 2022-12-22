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
 * @file      driver_amg8833_interrupt.c
 * @brief     driver amg8833 interrupt source file
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

#include "driver_amg8833_interrupt.h"

static amg8833_handle_t gs_handle;        /**< amg8833 handle */

/**
 * @brief  interrupt irq callback
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t amg8833_interrupt_irq_handler(void)
{
    if (amg8833_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     interrupt example init
 * @param[in] addr_pin is the iic device address
 * @param[in] mode is the interrupt mode
 * @param[in] high_level is the high level
 * @param[in] low_level is the low level
 * @param[in] hysteresis_level is the hysteresis level
 * @param[in] *callback points to a callback function
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t amg8833_interrupt_init(amg8833_address_t addr_pin,
                               amg8833_interrupt_mode_t mode,
                               float high_level, float low_level, 
                               float hysteresis_level,
                               void (*callback)(uint8_t type) 
                              )
{
    uint8_t res;
    int16_t level;
    
    /* link interface function */
    DRIVER_AMG8833_LINK_INIT(&gs_handle, amg8833_handle_t);
    DRIVER_AMG8833_LINK_IIC_INIT(&gs_handle, amg8833_interface_iic_init);
    DRIVER_AMG8833_LINK_IIC_DEINIT(&gs_handle, amg8833_interface_iic_deinit);
    DRIVER_AMG8833_LINK_IIC_READ(&gs_handle, amg8833_interface_iic_read);
    DRIVER_AMG8833_LINK_IIC_WRITE(&gs_handle, amg8833_interface_iic_write);
    DRIVER_AMG8833_LINK_DELAY_MS(&gs_handle, amg8833_interface_delay_ms);
    DRIVER_AMG8833_LINK_DEBUG_PRINT(&gs_handle, amg8833_interface_debug_print);
    DRIVER_AMG8833_LINK_RECEIVE_CALLBACK(&gs_handle, callback);
    
    /* set the address */
    res = amg8833_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set addr pin failed.\n");
       
        return 1;
    }
    
    /* init */
    res = amg8833_init(&gs_handle);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: init failed.\n");
       
        return 1;
    }
    
    /* set normal mode */
    res = amg8833_set_mode(&gs_handle, AMG8833_MODE_NORMAL);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set frame rate */
    res = amg8833_set_frame_rate(&gs_handle, AMG8833_INTERRUPT_DEFAULT_FRAME_RATE);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set frame rate failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set average mode */
    res = amg8833_set_average_mode(&gs_handle, AMG8833_INTERRUPT_DEFAULT_AVERAGE_MODE);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set average mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    
    
    /* convert */
    res = amg8833_interrupt_level_convert_to_register(&gs_handle, high_level, (int16_t *)&level);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: interrupt level convert to register failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    
    /* high level */
    res = amg8833_set_interrupt_high_level(&gs_handle, level);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set interrupt high level failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    
    /* convert */
    res = amg8833_interrupt_level_convert_to_register(&gs_handle, low_level, (int16_t *)&level);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: interrupt level convert to register failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    
    /* low level */
    res = amg8833_set_interrupt_low_level(&gs_handle, level);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set interrupt low level failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    
    /* convert */
    res = amg8833_interrupt_level_convert_to_register(&gs_handle, hysteresis_level, (int16_t *)&level);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: interrupt level convert to register failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    
    /* hysteresis level */
    res = amg8833_set_interrupt_hysteresis_level(&gs_handle, level);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set interrupt hysteresis level failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set mode */
    res = amg8833_set_interrupt_mode(&gs_handle, mode);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set interrupt mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    
    /* flag reset */
    res = amg8833_reset(&gs_handle, AMG8833_RESET_TYPE_FLAG);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: reset failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    
    /* enable */
    res = amg8833_set_interrupt(&gs_handle, AMG8833_BOOL_TRUE);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set interrupt failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    
    return 0;
}

/**
 * @brief  interrupt example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t amg8833_interrupt_deinit(void)
{
    if (amg8833_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      interrupt example get interrupt table
 * @param[out] **table points to a table buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt table failed
 * @note       none
 */
uint8_t amg8833_interrupt_get_table(uint8_t table[8][1])
{
    /* get interrupt table */
    if (amg8833_get_interrupt_table(&gs_handle, table) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      interrupt example read temperature array
 * @param[out] **temp points to a temperature array
 * @return     status code
 *             - 0 success
 *             - 1 read temperature array failed
 * @note       none
 */
uint8_t amg8833_interrupt_read_temperature_array(float temp[8][8])
{
    int16_t raw[8][8];
    
    /* read temperature array */
    if (amg8833_read_temperature_array(&gs_handle, (int16_t (*)[8])raw, temp) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      interrupt example read temperature
 * @param[out] *temp points to a temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature failed
 * @note       none
 */
uint8_t amg8833_interrupt_read_temperature(float *temp)
{
    int16_t raw;
    
    /* read temperature */
    if (amg8833_read_temperature(&gs_handle, (int16_t *)&raw, (float *)temp) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
