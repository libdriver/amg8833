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
 * @file      driver_amg8833_interrupt.h
 * @brief     driver amg8833 interrupt header file
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

#ifndef DRIVER_AMG8833_INTERRUPT_H
#define DRIVER_AMG8833_INTERRUPT_H

#include "driver_amg8833_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup amg8833_example_driver
 * @ingroup    amg8833_driver
 * @{
 */

/**
 * @brief amg8833 interrupt example default definition
 */
#define AMG8833_INTERRUPT_DEFAULT_FRAME_RATE          AMG8833_FRAME_RATE_1_FPS         /**< 1 fps */
#define AMG8833_INTERRUPT_DEFAULT_AVERAGE_MODE        AMG8833_AVERAGE_MODE_TWICE       /**< twice */

/**
 * @brief  interrupt irq callback
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t amg8833_interrupt_irq_handler(void);

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
                              );

/**
 * @brief  interrupt example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t amg8833_interrupt_deinit(void);

/**
 * @brief      interrupt example get interrupt table
 * @param[out] **table points to a table buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt table failed
 * @note       none
 */
uint8_t amg8833_interrupt_get_table(uint8_t table[8][1]);

/**
 * @brief      interrupt example read temperature array
 * @param[out] **temp points to a temperature array
 * @return     status code
 *             - 0 success
 *             - 1 read temperature array failed
 * @note       none
 */
uint8_t amg8833_interrupt_read_temperature_array(float temp[8][8]);

/**
 * @brief      interrupt example read temperature
 * @param[out] *temp points to a temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature failed
 * @note       none
 */
uint8_t amg8833_interrupt_read_temperature(float *temp);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
