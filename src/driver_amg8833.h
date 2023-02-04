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
 * @file      driver_amg8833.h
 * @brief     driver amg8833 header file
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

#ifndef DRIVER_AMG8833_H
#define DRIVER_AMG8833_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup amg8833_driver amg8833 driver function
 * @brief    amg8833 driver modules
 * @{
 */

/**
 * @addtogroup amg8833_basic_driver
 * @{
 */

/**
 * @brief amg8833 address enumeration definition
 */
typedef enum
{
    AMG8833_ADDRESS_0 = 0xD0,        /**< ad select pin connected to the GND */
    AMG8833_ADDRESS_1 = 0xD2,        /**< ad select pin connected to the VCC */
} amg8833_address_t;

/**
 * @brief amg8833 bool enumeration definition
 */
typedef enum
{
    AMG8833_BOOL_FALSE = 0x00,        /**< false */
    AMG8833_BOOL_TRUE  = 0x01,        /**< true */
} amg8833_bool_t;

/**
 * @brief amg8833 mode enumeration definition
 */
typedef enum
{
    AMG8833_MODE_NORMAL       = 0x00,        /**< normal mode */
    AMG8833_MODE_SLEEP        = 0x10,        /**< sleep mode */
    AMG8833_MODE_STAND_BY_60S = 0x20,        /**< stand-by 60s mode */
    AMG8833_MODE_STAND_BY_10S = 0x21,        /**< stand-by 10s mode */
} amg8833_mode_t;

/**
 * @brief amg8833 reset type enumeration definition
 */
typedef enum
{
    AMG8833_RESET_TYPE_FLAG = 0x30,        /**< flag reset type */
    AMG8833_RESET_TYPE_INIT = 0x3F,        /**< initial reset type */
} amg8833_reset_type_t;

/**
 * @brief amg8833 frame rate enumeration definition
 */
typedef enum
{
    AMG8833_FRAME_RATE_10_FPS = 0x00,        /**< 10 fps */
    AMG8833_FRAME_RATE_1_FPS  = 0x01,        /**< 1 fps */
} amg8833_frame_rate_t;

/**
 * @brief amg8833 interrupt mode enumeration definition
 */
typedef enum
{
    AMG8833_INTERRUPT_MODE_DIFFERENCE = 0x00,        /**< difference interrupt mode */
    AMG8833_INTERRUPT_MODE_ABSOLUTE   = 0x01,        /**< absolute value interrupt mode */
} amg8833_interrupt_mode_t;

/**
 * @brief amg8833 status enumeration definition
 */
typedef enum
{
    AMG8833_STATUS_OVF_THS = 0x03,        /**< thermistor temperature output overflow */
    AMG8833_STATUS_OVF_IRS = 0x02,        /**< temperature output overflow */
    AMG8833_STATUS_INTF    = 0x01,        /**< interrupt outbreak */
} amg8833_status_t;

/**
 * @brief amg8833 average mode enumeration definition
 */
typedef enum
{
    AMG8833_AVERAGE_MODE_ONCE  = 0x00,        /**< once moving average output mode  */
    AMG8833_AVERAGE_MODE_TWICE = 0x01,        /**< twice moving average output mode */
} amg8833_average_mode_t;

/**
 * @brief amg8833 handle structure definition
 */
typedef struct amg8833_handle_s
{
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to an iic_read function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    void (*receive_callback)(uint8_t type);                                             /**< point to a receive_callback function address */
    uint8_t inited;                                                                     /**< inited flag */
    uint8_t iic_addr;                                                                   /**< iic address */
} amg8833_handle_t;

/**
 * @brief amg8833 information structure definition
 */
typedef struct amg8833_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} amg8833_info_t;

/**
 * @}
 */

/**
 * @defgroup amg8833_link_driver amg8833 link driver function
 * @brief    amg8833 link driver modules
 * @ingroup  amg8833_driver
 * @{
 */

/**
 * @brief     initialize amg8833_handle_t structure
 * @param[in] HANDLE points to an amg8833 handle structure
 * @param[in] STRUCTURE is amg8833_handle_t
 * @note      none
 */
#define DRIVER_AMG8833_LINK_INIT(HANDLE, STRUCTURE)         memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE points to an amg8833 handle structure
 * @param[in] FUC points to an iic_init function address
 * @note      none
 */
#define DRIVER_AMG8833_LINK_IIC_INIT(HANDLE, FUC)          (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to an amg8833 handle structure
 * @param[in] FUC points to an iic_deinit function address
 * @note      none
 */
#define DRIVER_AMG8833_LINK_IIC_DEINIT(HANDLE, FUC)        (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE points to an amg8833 handle structure
 * @param[in] FUC points to an iic_read function address
 * @note      none
 */
#define DRIVER_AMG8833_LINK_IIC_READ(HANDLE, FUC)          (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE points to an amg8833 handle structure
 * @param[in] FUC points to an iic_write function address
 * @note      none
 */
#define DRIVER_AMG8833_LINK_IIC_WRITE(HANDLE, FUC)         (HANDLE)->iic_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an amg8833 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_AMG8833_LINK_DELAY_MS(HANDLE, FUC)          (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an amg8833 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_AMG8833_LINK_DEBUG_PRINT(HANDLE, FUC)       (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE points to an amg8833 handle structure
 * @param[in] FUC points to a receive_callback function address
 * @note      none
 */
#define DRIVER_AMG8833_LINK_RECEIVE_CALLBACK(HANDLE, FUC)  (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup amg8833_basic_driver amg8833 basic driver function
 * @brief    amg8833 basic driver modules
 * @ingroup  amg8833_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to an amg8833 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t amg8833_info(amg8833_info_t *info);

/**
 * @brief     irq handler
 * @param[in] *handle points to an amg8833 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t amg8833_irq_handler(amg8833_handle_t *handle);

/**
 * @brief     set the iic address pin
 * @param[in] *handle points to an amg8833 handle structure
 * @param[in] addr_pin is the address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t amg8833_set_addr_pin(amg8833_handle_t *handle, amg8833_address_t addr_pin);

/**
 * @brief      get the iic address pin
 * @param[in]  *handle points to an amg8833 handle structure
 * @param[out] *addr_pin points to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t amg8833_get_addr_pin(amg8833_handle_t *handle, amg8833_address_t *addr_pin);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an amg8833 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 reset failed
 * @note      none
 */
uint8_t amg8833_init(amg8833_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to an amg8833 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 * @note      none
 */
uint8_t amg8833_deinit(amg8833_handle_t *handle);

/**
 * @brief      read the temperature
 * @param[in]  *handle points to an amg8833 handle structure
 * @param[out] *raw points to a raw temperature buffer
 * @param[out] *temp points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t amg8833_read_temperature(amg8833_handle_t *handle, int16_t *raw, float *temp);

/**
 * @brief      read the temperature array
 * @param[in]  *handle points to an amg8833 handle structure
 * @param[out] **raw points to a raw temperature buffer
 * @param[out] **temp points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature array failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       pixel_64 pixel_63 ... pixel_57
 *             .                            .
 *             .                            .
 *             .                            .
 *             .                            .
 *             .                            .
 * @note       pixel_08 pixel_07 ... pixel_01
 */
uint8_t amg8833_read_temperature_array(amg8833_handle_t *handle, int16_t raw[8][8], float temp[8][8]);

/**
 * @brief      get the interrupt table
 * @param[in]  *handle points to an amg8833 handle structure
 * @param[out] **table points to an interrupt table buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt table failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       pixel_64 pixel_63 ... pixel_57 (bit)
 *             .                            .
 *             .                            .
 *             .                            .
 *             .                            .
 *             .                            .
 * @note       pixel_08 pixel_07 ... pixel_01
 */
uint8_t amg8833_get_interrupt_table(amg8833_handle_t *handle, uint8_t table[8][1]);

/**
 * @brief     set the mode
 * @param[in] *handle points to an amg8833 handle structure
 * @param[in] mode is the chip mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t amg8833_set_mode(amg8833_handle_t *handle, amg8833_mode_t mode);

/**
 * @brief      get the mode
 * @param[in]  *handle points to an amg8833 handle structure
 * @param[out] *mode points to a chip mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t amg8833_get_mode(amg8833_handle_t *handle, amg8833_mode_t *mode);

/**
 * @brief     reset the chip
 * @param[in] *handle points to an amg8833 handle structure
 * @param[in] type is the reset type
 * @return    status code
 *            - 0 success
 *            - 1 reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t amg8833_reset(amg8833_handle_t *handle, amg8833_reset_type_t type);

/**
 * @brief     set the frame rate
 * @param[in] *handle points to an amg8833 handle structure
 * @param[in] rate is the frame rate
 * @return    status code
 *            - 0 success
 *            - 1 set frame rate failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t amg8833_set_frame_rate(amg8833_handle_t *handle, amg8833_frame_rate_t rate);

/**
 * @brief      get the frame rate
 * @param[in]  *handle points to an amg8833 handle structure
 * @param[out] *rate points to a frame rate buffer
 * @return     status code
 *             - 0 success
 *             - 1 get frame rate failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t amg8833_get_frame_rate(amg8833_handle_t *handle, amg8833_frame_rate_t *rate);

/**
 * @brief     set the interrupt mode
 * @param[in] *handle points to an amg8833 handle structure
 * @param[in] mode is the interrupt mode
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t amg8833_set_interrupt_mode(amg8833_handle_t *handle, amg8833_interrupt_mode_t mode);

/**
 * @brief      get the interrupt mode
 * @param[in]  *handle points to an amg8833 handle structure
 * @param[out] *mode points to an interrupt mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t amg8833_get_interrupt_mode(amg8833_handle_t *handle, amg8833_interrupt_mode_t *mode);

/**
 * @brief     enable or disable the interrupt
 * @param[in] *handle points to an amg8833 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t amg8833_set_interrupt(amg8833_handle_t *handle, amg8833_bool_t enable);

/**
 * @brief      get the interrupt status
 * @param[in]  *handle points to an amg8833 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t amg8833_get_interrupt(amg8833_handle_t *handle, amg8833_bool_t *enable);

/**
 * @brief      get the status
 * @param[in]  *handle points to an amg8833 handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t amg8833_get_status(amg8833_handle_t *handle, uint8_t *status);

/**
 * @brief     clear the interrupt status
 * @param[in] *handle points to an amg8833 handle structure
 * @param[in] status is the cleared interrupt status
 * @return    status code
 *            - 0 success
 *            - 1 clear status failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t amg8833_clear_status(amg8833_handle_t *handle, amg8833_status_t status);

/**
 * @brief     set the average_mode
 * @param[in] *handle points to an amg8833 handle structure
 * @param[in] mode is the average mode
 * @return    status code
 *            - 0 success
 *            - 1 set average mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t amg8833_set_average_mode(amg8833_handle_t *handle, amg8833_average_mode_t mode);

/**
 * @brief      get the average_mode
 * @param[in]  *handle points to an amg8833 handle structure
 * @param[out] *mode points to an average mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get average mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t amg8833_get_average_mode(amg8833_handle_t *handle, amg8833_average_mode_t *mode);

/**
 * @brief     set the interrupt high level
 * @param[in] *handle points to an amg8833 handle structure
 * @param[in] level is the interrupt high level
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt high level failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t amg8833_set_interrupt_high_level(amg8833_handle_t *handle, int16_t level);

/**
 * @brief      get the interrupt high level
 * @param[in]  *handle points to an amg8833 handle structure
 * @param[out] *level points to an interrupt high level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt high level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t amg8833_get_interrupt_high_level(amg8833_handle_t *handle, int16_t *level);

/**
 * @brief     set the interrupt low level
 * @param[in] *handle points to an amg8833 handle structure
 * @param[in] level is the interrupt low level
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt low level failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t amg8833_set_interrupt_low_level(amg8833_handle_t *handle, int16_t level);

/**
 * @brief      get the interrupt low level
 * @param[in]  *handle points to an amg8833 handle structure
 * @param[out] *level points to an interrupt low level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt low level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t amg8833_get_interrupt_low_level(amg8833_handle_t *handle, int16_t *level);

/**
 * @brief     set the interrupt hysteresis level
 * @param[in] *handle points to an amg8833 handle structure
 * @param[in] level is the interrupt hysteresis level
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt hysteresis level failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t amg8833_set_interrupt_hysteresis_level(amg8833_handle_t *handle, int16_t level);

/**
 * @brief      get the interrupt hysteresis level
 * @param[in]  *handle points to an amg8833 handle structure
 * @param[out] *level points to an interrupt hysteresis level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt hysteresis level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t amg8833_get_interrupt_hysteresis_level(amg8833_handle_t *handle, int16_t *level);

/**
 * @brief      convert the interrupt level to the register raw data
 * @param[in]  *handle points to an amg8833 handle structure
 * @param[in]  temp is the temperature
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t amg8833_interrupt_level_convert_to_register(amg8833_handle_t *handle, float temp, int16_t *reg);

/**
 * @brief      convert the register raw data to the interrupt level
 * @param[in]  *handle points to an amg8833 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *temp points to a temperature buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t amg8833_interrupt_level_convert_to_data(amg8833_handle_t *handle, int16_t reg, float *temp);

/**
 * @}
 */

/**
 * @defgroup amg8833_extern_driver amg8833 extern driver function
 * @brief    amg8833 extern driver modules
 * @ingroup  amg8833_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle points to an amg8833 handle structure
 * @param[in] reg is the register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t amg8833_set_reg(amg8833_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle points to an amg8833 handle structure
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t amg8833_get_reg(amg8833_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
