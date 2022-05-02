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
 * @file      driver_amg8833_interrupt_test.c
 * @brief     driver amg8833 interrupt test source file
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

#include "driver_amg8833_interrupt_test.h"

static amg8833_handle_t gs_handle;        /**< amg8833 handle */

/**
 * @brief  interrupt test irq callback
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t amg8833_interrupt_test_irq_handler(void)
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
 * @brief     interrupt test
 * @param[in] addr_pin is the iic device address
 * @param[in] mode is the interrupt mode
 * @param[in] high_level is the high level
 * @param[in] low_level is the low level
 * @param[in] hysteresis_level is the hysteresis level
 * @param[in] times is the read times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t amg8833_interrupt_test(amg8833_address_t addr_pin, 
                               amg8833_interrupt_mode_t mode,
                               float high_level, float low_level, 
                               float hysteresis_level, uint32_t times)
{
    uint32_t i;
    uint8_t res;
    int16_t level;
    amg8833_info_t info;
    
    /* link interface function */
    DRIVER_AMG8833_LINK_INIT(&gs_handle, amg8833_handle_t);
    DRIVER_AMG8833_LINK_IIC_INIT(&gs_handle, amg8833_interface_iic_init);
    DRIVER_AMG8833_LINK_IIC_DEINIT(&gs_handle, amg8833_interface_iic_deinit);
    DRIVER_AMG8833_LINK_IIC_READ(&gs_handle, amg8833_interface_iic_read);
    DRIVER_AMG8833_LINK_IIC_WRITE(&gs_handle, amg8833_interface_iic_write);
    DRIVER_AMG8833_LINK_DELAY_MS(&gs_handle, amg8833_interface_delay_ms);
    DRIVER_AMG8833_LINK_DEBUG_PRINT(&gs_handle, amg8833_interface_debug_print);
    DRIVER_AMG8833_LINK_RECEIVE_CALLBACK(&gs_handle, amg8833_interface_receive_callback);
    
    /* get information */
    res = amg8833_info(&info);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        amg8833_interface_debug_print("amg8833: chip is %s.\n", info.chip_name);
        amg8833_interface_debug_print("amg8833: manufacturer is %s.\n", info.manufacturer_name);
        amg8833_interface_debug_print("amg8833: interface is %s.\n", info.interface);
        amg8833_interface_debug_print("amg8833: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000 ) / 100);
        amg8833_interface_debug_print("amg8833: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        amg8833_interface_debug_print("amg8833: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        amg8833_interface_debug_print("amg8833: max current is %0.2fmA.\n", info.max_current_ma);
        amg8833_interface_debug_print("amg8833: max temperature is %0.1fC.\n", info.temperature_max);
        amg8833_interface_debug_print("amg8833: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start interrupt test */
    amg8833_interface_debug_print("amg8833: start interrupt test.\n");
    
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
    
    /* 1 fps */
    res = amg8833_set_frame_rate(&gs_handle, AMG8833_FRAME_RATE_1_FPS);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set frame rate failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    
    /* twice mode */
    res = amg8833_set_average_mode(&gs_handle, AMG8833_AVERAGE_MODE_TWICE);
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
    
    /* delay 1000 ms */
    amg8833_interface_delay_ms(1000);
    
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        float temp;
        
        /* read temperature */
        res = amg8833_read_temperature(&gs_handle, (int16_t *)&raw, (float *)&temp);
        if (res != 0)
        {
            amg8833_interface_debug_print("amg8833: read temperature failed.\n");
            (void)amg8833_deinit(&gs_handle);
           
            return 1;
        }
        else
        {
            amg8833_interface_debug_print("amg8833: temperature is %0.3fC.\n", temp);
        }
        
        /* delay 1000 ms */
        amg8833_interface_delay_ms(1000);
    }
    
    /* finish interrupt test */
    amg8833_interface_debug_print("amg8833: finish interrupt test.\n");
    (void)amg8833_deinit(&gs_handle);
    
    return 0;
}
