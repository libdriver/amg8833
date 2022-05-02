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
 * @file      driver_amg8833_register_test.c
 * @brief     driver amg8833 register test source file
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

#include "driver_amg8833_register_test.h"
#include <stdlib.h>

static amg8833_handle_t gs_handle;        /**< amg8833 handle */

/**
 * @brief     register test
 * @param[in] addr_pin is the iic device address
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t amg8833_register_test(amg8833_address_t addr_pin)
{
    uint8_t res;
    int16_t level, level_check;
    float tmp, tmp_check;
    uint8_t status;
    uint8_t i, j;
    uint8_t table[8][1];
    amg8833_info_t info;
    amg8833_address_t addr;
    amg8833_mode_t mode;
    amg8833_interrupt_mode_t interrupt_mode;
    amg8833_frame_rate_t rate;
    amg8833_bool_t enable;
    amg8833_average_mode_t avg_mode;
    
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
    
    /* start register test */
    amg8833_interface_debug_print("amg8833: start register test.\n");
    
    /* amg8833_set_addr_pin/amg8833_get_addr_pin test */
    amg8833_interface_debug_print("amg8833: amg8833_set_addr_pin/amg8833_get_addr_pin test.\n");
    
    /* address_0 */
    res = amg8833_set_addr_pin(&gs_handle, AMG8833_ADDRESS_0);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set addr pin failed.\n");
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: set addr pin address 0.\n");
    res = amg8833_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get addr pin failed.\n");
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check addr pin %s.\n", (addr == AMG8833_ADDRESS_0) ? "ok" : "error");
    
    /* address_1 */
    res = amg8833_set_addr_pin(&gs_handle, AMG8833_ADDRESS_1);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set addr pin failed.\n");
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: set addr pin address 1.\n");
    res = amg8833_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get addr pin failed.\n");
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check addr pin %s.\n", (addr == AMG8833_ADDRESS_1) ? "ok" : "error");
    
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
    
    /* amg8833_set_mode/amg8833_get_mode test */
    amg8833_interface_debug_print("amg8833: amg8833_set_mode/amg8833_get_mode test.\n");

    /* sleep mode */
    res = amg8833_set_mode(&gs_handle, AMG8833_MODE_SLEEP);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: set sleep mode.\n");
    amg8833_interface_delay_ms(50);
    res = amg8833_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check mode %s.\n", (res == 0) ? "ok" : "error");
    
    /* set normal mode */
    res = amg8833_set_mode(&gs_handle, AMG8833_MODE_NORMAL);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_delay_ms(50);
    
    /* stand by 60s mode */
    res = amg8833_set_mode(&gs_handle, AMG8833_MODE_STAND_BY_60S);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: set stand by 60s mode.\n");
    res = amg8833_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check mode %s.\n", (mode == AMG8833_MODE_STAND_BY_60S) ? "ok" : "error");
    
    /* stand by 10s mode */
    res = amg8833_set_mode(&gs_handle, AMG8833_MODE_STAND_BY_10S);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: set stand by 10s mode.\n");
    res = amg8833_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check mode %s.\n", (mode == AMG8833_MODE_STAND_BY_10S) ? "ok" : "error");
    
    /* normal mode */
    res = amg8833_set_mode(&gs_handle, AMG8833_MODE_NORMAL);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: set normal mode.\n");
    res = amg8833_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check mode %s.\n", (mode == AMG8833_MODE_NORMAL) ? "ok" : "error");
    
    /* amg8833_set_frame_rate/amg8833_get_frame_rate test */
    amg8833_interface_debug_print("amg8833: amg8833_set_frame_rate/amg8833_get_frame_rate test.\n");
    
    /* 10 fps */
    res = amg8833_set_frame_rate(&gs_handle, AMG8833_FRAME_RATE_10_FPS);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set frame rate failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: set 10 fps.\n");
    res = amg8833_get_frame_rate(&gs_handle, &rate);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get frame rate failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check rate %s.\n", (rate == AMG8833_FRAME_RATE_10_FPS) ? "ok" : "error");
    
    /* 1 fps */
    res = amg8833_set_frame_rate(&gs_handle, AMG8833_FRAME_RATE_1_FPS);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set frame rate failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: set 1 fps.\n");
    res = amg8833_get_frame_rate(&gs_handle, &rate);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get frame rate failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check rate %s.\n", (rate == AMG8833_FRAME_RATE_1_FPS) ? "ok" : "error");
    
    /* amg8833_set_interrupt_mode/amg8833_get_interrupt_mode test */
    amg8833_interface_debug_print("amg8833: amg8833_set_interrupt_mode/amg8833_get_interrupt_mode test.\n");
    
    /* diff */
    res = amg8833_set_interrupt_mode(&gs_handle, AMG8833_INTERRUPT_MODE_DIFFERENCE);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set interrupt mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: set difference interrupt mode.\n");
    res = amg8833_get_interrupt_mode(&gs_handle, &interrupt_mode);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get interrupt mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check interrupt mode %s.\n", (interrupt_mode == AMG8833_INTERRUPT_MODE_DIFFERENCE) ? "ok" : "error");
    
    /* abs */
    res = amg8833_set_interrupt_mode(&gs_handle, AMG8833_INTERRUPT_MODE_ABSOLUTE);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set interrupt mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: set absolute value interrupt mode.\n");
    res = amg8833_get_interrupt_mode(&gs_handle, &interrupt_mode);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get interrupt mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check interrupt mode %s.\n", (interrupt_mode == AMG8833_INTERRUPT_MODE_ABSOLUTE) ? "ok" : "error");
    
    /* amg8833_set_interrupt/amg8833_get_interrupt test */
    amg8833_interface_debug_print("amg8833: amg8833_set_interrupt/amg8833_get_interrupt test.\n");
    
    /* disable */
    res = amg8833_set_interrupt(&gs_handle, AMG8833_BOOL_FALSE);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set interrupt failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: disable interrupt mode.\n");
    res = amg8833_get_interrupt(&gs_handle, &enable);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get interrupt failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check interrupt %s.\n", (enable == AMG8833_BOOL_FALSE) ? "ok" : "error");
    
    /* enable */
    res = amg8833_set_interrupt(&gs_handle, AMG8833_BOOL_TRUE);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set interrupt failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: enable interrupt mode.\n");
    res = amg8833_get_interrupt(&gs_handle, &enable);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get interrupt failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check interrupt %s.\n", (enable == AMG8833_BOOL_TRUE) ? "ok" : "error");
    
    /* amg8833_set_average_mode/amg8833_get_average_mode test */
    amg8833_interface_debug_print("amg8833: amg8833_set_average_mode/amg8833_get_average_mode test.\n");
    
    /* once mode */
    res = amg8833_set_average_mode(&gs_handle, AMG8833_AVERAGE_MODE_ONCE);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set average mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: set once average mode.\n");
    res = amg8833_get_average_mode(&gs_handle, &avg_mode);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get average mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check average mode %s.\n", (avg_mode == AMG8833_AVERAGE_MODE_ONCE) ? "ok" : "error");
    
    /* twice mode */
    res = amg8833_set_average_mode(&gs_handle, AMG8833_AVERAGE_MODE_TWICE);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set average mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: set twice average mode.\n");
    res = amg8833_get_average_mode(&gs_handle, &avg_mode);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get average mode failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check average mode %s.\n", (res == 0) ? "ok" : "error");
    
    /* amg8833_set_interrupt_high_level/amg8833_get_interrupt_high_level test */
    amg8833_interface_debug_print("amg8833: amg8833_set_interrupt_high_level/amg8833_get_interrupt_high_level test.\n");
    
    level = rand() % 2048;
    res = amg8833_set_interrupt_high_level(&gs_handle, level);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set interrupt high level failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: set interrupt high level %d.\n", level);
    res = amg8833_get_interrupt_high_level(&gs_handle, (int16_t *)&level_check);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get interrupt high level failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check level %s.\n", (level_check == level) ? "ok" : "error");
    
    /* amg8833_set_interrupt_low_level/amg8833_get_interrupt_low_level test */
    amg8833_interface_debug_print("amg8833: amg8833_set_interrupt_low_level/amg8833_get_interrupt_low_level test.\n");
    
    level = -(rand() % 2048);
    res = amg8833_set_interrupt_low_level(&gs_handle, level);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set interrupt low level failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: set interrupt low level %d.\n", level);
    res = amg8833_get_interrupt_low_level(&gs_handle, (int16_t *)&level_check);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get interrupt low level failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check level %s.\n", (level_check == level) ? "ok" : "error");
    
    /* amg8833_set_interrupt_hysteresis_level/amg8833_get_interrupt_hysteresis_level test */
    amg8833_interface_debug_print("amg8833: amg8833_set_interrupt_hysteresis_level/amg8833_get_interrupt_hysteresis_level test.\n");
    
    level = -(rand() % 2048);
    res = amg8833_set_interrupt_hysteresis_level(&gs_handle, level);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: set interrupt hysteresis level failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: set interrupt hysteresis level %d.\n", level);
    res = amg8833_get_interrupt_hysteresis_level(&gs_handle, (int16_t *)&level_check);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get interrupt hysteresis level failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check level %s.\n", (level_check == level) ? "ok" : "error");
    
    /* amg8833_interrupt_level_convert_to_register/amg8833_interrupt_level_convert_to_data test */
    amg8833_interface_debug_print("amg8833: amg8833_interrupt_level_convert_to_register/amg8833_interrupt_level_convert_to_data test.\n");
    
    tmp = (float)(rand() % 5000) / 100.0f;
    res = amg8833_interrupt_level_convert_to_register(&gs_handle, tmp, (int16_t *)&level);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: interrupt level convert to register failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: generated temperature is %0.2f.\n", tmp);
    res = amg8833_interrupt_level_convert_to_data(&gs_handle, level, (float *)&tmp_check);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: interrupt level convert to data failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: converted temperature is %0.2f.\n", tmp_check);
    
    /* amg8833_get_status test */
    amg8833_interface_debug_print("amg8833: amg8833_get_status test.\n");
    
    res = amg8833_get_status(&gs_handle, (uint8_t *)&status);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get status failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check %s and status is 0x%02X.\n", (res == 0) ? "ok" : "error", status);
    
    /* amg8833_clear_status test */
    amg8833_interface_debug_print("amg8833: amg8833_clear_status test.\n");
    
    /* thermistor temperature output overflow */
    res = amg8833_clear_status(&gs_handle, AMG8833_STATUS_OVF_THS);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: clear status failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check thermistor temperature output overflow %s.\n", (res == 0) ? "ok" : "error");
    
    /* temperature output overflow */
    res = amg8833_clear_status(&gs_handle, AMG8833_STATUS_OVF_IRS);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: clear status failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check temperature output overflow %s.\n", (res == 0) ? "ok" : "error");
    
    /* interrupt outbreak */
    res = amg8833_clear_status(&gs_handle, AMG8833_STATUS_INTF);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: clear status failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check interrupt outbreak %s.\n", (res == 0) ? "ok" : "error");
    
    /* amg8833_get_interrupt_table test */
    amg8833_interface_debug_print("amg8833: amg8833_get_interrupt_table test.\n");
    
    res = amg8833_get_interrupt_table(&gs_handle, (uint8_t (*)[1])table);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: get interrupt table failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    for (i = 0; i < 8; i++)
    {
        level = table[i][0];
        for (j = 0; j < 8; j++)
        {
            if (((level >> (7 - j)) & 0x01) != 0)
            {
                amg8833_interface_debug_print("%d  ", 1);
            }
            else
            {
                amg8833_interface_debug_print("%d  ", 0);
            }
        }
        amg8833_interface_debug_print("\n");
    }
    
    /* amg8833_reset test */
    amg8833_interface_debug_print("amg8833: amg8833_reset test.\n");
    
    /* init reset */
    res = amg8833_reset(&gs_handle, AMG8833_RESET_TYPE_INIT);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: reset failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check reset %s.\n", (res == 0) ? "ok" : "error");
    
    /* flag reset */
    res = amg8833_reset(&gs_handle, AMG8833_RESET_TYPE_FLAG);
    if (res != 0)
    {
        amg8833_interface_debug_print("amg8833: reset failed.\n");
        (void)amg8833_deinit(&gs_handle);
       
        return 1;
    }
    amg8833_interface_debug_print("amg8833: check reset %s.\n", (res == 0) ? "ok" : "error");
    amg8833_interface_delay_ms(50);
    
    /* finish register test */
    amg8833_interface_debug_print("amg8833: finish register test.\n");
    (void)amg8833_deinit(&gs_handle);
    
    return 0;
}
