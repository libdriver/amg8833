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
 * @file      driver_amg8833_interrupt_test.h
 * @brief     driver amg8833 interrupt test header file
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

#ifndef DRIVER_AMG8833_INTERRUPT_TEST_H
#define DRIVER_AMG8833_INTERRUPT_TEST_H

#include "driver_amg8833_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup amg8833_test_driver
 * @ingroup    amg8833_driver
 * @{
 */

/**
 * @brief  interrupt test irq callback
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t amg8833_interrupt_test_irq_handler(void);

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
                               float hysteresis_level, uint32_t times);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
