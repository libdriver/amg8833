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
 * @file      driver_amg8833.c
 * @brief     driver amg8833 source file
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

#include "driver_amg8833.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Panasonic AMG8833"        /**< chip name */
#define MANUFACTURER_NAME         "Panasonic"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        3.0f                       /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f                       /**< chip max supply voltage */
#define MAX_CURRENT               5.0f                       /**< chip max current */
#define TEMPERATURE_MIN           -20.0f                     /**< chip min operating temperature */
#define TEMPERATURE_MAX           80.0f                      /**< chip max operating temperature */
#define DRIVER_VERSION            1000                       /**< driver version */

/**
 * @brief chip register definition
 */
#define AMG8833_REG_PCTL         0x00        /**< power control register */
#define AMG8833_REG_RST          0x01        /**< software reset register */
#define AMG8833_REG_FPSC         0x02        /**< frame rate register */
#define AMG8833_REG_INTC         0x03        /**< interrupt function register */
#define AMG8833_REG_STAT         0x04        /**< interrupt flag, low voltage flag register */
#define AMG8833_REG_SCLR         0x05        /**< interrupt flag clear register */
#define AMG8833_REG_AVE          0x07        /**< moving average output mode register */
#define AMG8833_REG_INTHL        0x08        /**< interrupt upper value lower level register */
#define AMG8833_REG_INTHH        0x09        /**< interrupt upper value upper level register */
#define AMG8833_REG_INTLL        0x0A        /**< interrupt lower value lower level register */
#define AMG8833_REG_INTLH        0x0B        /**< interrupt lower value upper level register */
#define AMG8833_REG_IHYSL        0x0C        /**< interrupt hysteresis value lower level register */
#define AMG8833_REG_IHYSH        0x0D        /**< interrupt hysteresis value upper level register */
#define AMG8833_REG_TTHL         0x0E        /**< thermistor output value lower level register */
#define AMG8833_REG_TTHH         0x0F        /**< thermistor output value upper level register */
#define AMG8833_REG_INT0         0x10        /**< pixel 1 - 8 interrupt result register */
#define AMG8833_REG_INT1         0x11        /**< pixel 9 - 16 interrupt result register */
#define AMG8833_REG_INT2         0x12        /**< pixel 17 - 24 interrupt result register */
#define AMG8833_REG_INT3         0x13        /**< pixel 25 - 32 interrupt result register */
#define AMG8833_REG_INT4         0x14        /**< pixel 33 - 40 interrupt result register */
#define AMG8833_REG_INT5         0x15        /**< pixel 41 - 48 interrupt result register */
#define AMG8833_REG_INT6         0x16        /**< pixel 49 - 56 interrupt result register */
#define AMG8833_REG_INT7         0x17        /**< pixel 57 - 64 interrupt result register */
#define AMG8833_REG_T01L         0x80        /**< pixel 1 output value lower level register */
#define AMG8833_REG_T01H         0x81        /**< pixel 1 output value upper level register */

/**
 * @brief      read bytes
 * @param[in]  *handle points to an amg8833 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_amg8833_iic_read(amg8833_handle_t *handle, uint8_t reg, uint8_t *data, uint16_t len)
{
    if (handle->iic_read(handle->iic_addr, reg, data, len) != 0)        /* read the register */
    {
        return 1;                                                       /* return error */
    }
    else
    {
        return 0;                                                       /* success return 0 */
    }
}

/**
 * @brief     write bytes
 * @param[in] *handle points to an amg8833 handle structure
 * @param[in] reg is the iic register address
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_amg8833_iic_write(amg8833_handle_t *handle, uint8_t reg, uint8_t *data, uint16_t len)
{
    if (handle->iic_write(handle->iic_addr, reg, data, len) != 0)       /* write the register */
    {
        return 1;                                                       /* return error */
    }
    else
    {
        return 0;                                                       /* success return 0 */
    }
}

/**
 * @brief     set the iic address pin
 * @param[in] *handle points to an amg8833 handle structure
 * @param[in] addr_pin is the address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t amg8833_set_addr_pin(amg8833_handle_t *handle, amg8833_address_t addr_pin)
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    
    handle->iic_addr = (uint8_t)addr_pin;        /* set pin */
    
    return 0;                                    /* success return 0 */
}

/**
 * @brief      get the iic address pin
 * @param[in]  *handle points to an amg8833 handle structure
 * @param[out] *addr_pin points to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t amg8833_get_addr_pin(amg8833_handle_t *handle, amg8833_address_t *addr_pin)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    
    *addr_pin = (amg8833_address_t)(handle->iic_addr);        /* get pin */
    
    return 0;                                                 /* success return 0 */
}

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
uint8_t amg8833_init(amg8833_handle_t *handle)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->debug_print == NULL)                                                /* check debug_print */
    {
        return 3;                                                                   /* return error */
    }
    if (handle->iic_init == NULL)                                                   /* check iic_init */
    {
        handle->debug_print("amg8833: iic_init is null.\n");                        /* iic_init is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->iic_deinit == NULL)                                                 /* check iic_deinit */
    {
        handle->debug_print("amg8833: iic_deinit is null.\n");                      /* iic_deinit is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->iic_read == NULL)                                                   /* check iic_read */
    {
        handle->debug_print("amg8833: iic_read is null.\n");                        /* iic_read is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->iic_write == NULL)                                                  /* check iic_write */
    {
        handle->debug_print("amg8833: iic_write is null.\n");                       /* iic_write is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->delay_ms == NULL)                                                   /* check delay_ms */
    {
        handle->debug_print("amg8833: delay_ms is null.\n");                        /* delay_ms is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->receive_callback == NULL)                                           /* check receive_callback */
    {
        handle->debug_print("amg8833: receive_callback is null.\n");                /* receive_callback is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->iic_init() != 0)                                                    /* iic init */
    {
        handle->debug_print("amg8833: iic init failed.\n");                         /* iic init failed */
       
        return 3;                                                                   /* return error */
    }
    prev = 0x00;                                                                    /* normal mode */
    res = a_amg8833_iic_write(handle, AMG8833_REG_PCTL, (uint8_t *)&prev, 1);       /* write pctl register */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("amg8833: write pctl register failed.\n");              /* write pctl register failed */
        (void)handle->iic_deinit();                                                 /* iic deinit */
       
        return 1;                                                                   /* return error */
    }
    handle->delay_ms(50);                                                           /* wait 50 ms */
    prev = 0x3F;                                                                    /* initial reset */
    res = a_amg8833_iic_write(handle, AMG8833_REG_RST, (uint8_t *)&prev, 1);        /* write rst register */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("amg8833: write rst register failed.\n");               /* write rst register failed */
        (void)handle->iic_deinit();                                                 /* iic deinit */
        
        return 4;                                                                   /* return error */
    }
    handle->delay_ms(2);                                                            /* wait 2 ms */
    prev = 0x30;                                                                    /* flag reset */
    res = a_amg8833_iic_write(handle, AMG8833_REG_RST, (uint8_t *)&prev, 1);        /* write rst register */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("amg8833: write rst register failed.\n");               /* write rst register failed */
        (void)handle->iic_deinit();                                                 /* iic deinit */
        
        return 4;                                                                   /* return error */
    }
    handle->inited = 1;                                                             /* flag finish initialization */
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t amg8833_deinit(amg8833_handle_t *handle)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    prev = 0x10;                                                                    /* sleep mode*/
    res = a_amg8833_iic_write(handle, AMG8833_REG_PCTL, (uint8_t *)&prev, 1);       /* write pctl register */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("amg8833: write pctl register failed.\n");              /* write pctl register failed */
       
        return 4;                                                                   /* return error */
    }
    res = handle->iic_deinit();                                                     /* iic deinit */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("amg8833: iic deinit failed.\n");                       /* iic deinit failed */
       
        return 4;                                                                   /* return error */
    }
    else
    {
        handle->iic_init = 0;                                                       /* flag close */
    
        return 0;                                                                   /* success return 0 */
    }
}

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
uint8_t amg8833_irq_handler(amg8833_handle_t *handle)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_amg8833_iic_read(handle, AMG8833_REG_STAT, (uint8_t *)&prev, 1);       /* read stat register */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("amg8833: read stat register failed.\n");              /* read stat register failed */
        
        return 1;                                                                  /* return error */
    }
    res = a_amg8833_iic_write(handle, AMG8833_REG_SCLR, (uint8_t *)&prev, 1);      /* write sclr register */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("amg8833: write sclr register failed.\n");             /* write sclr register failed */
        
        return 1;                                                                  /* return error */
    }
    if ((prev & (1 << AMG8833_STATUS_INTF)) != 0)                                  /* if interrupt outbreak */
    {
        if (handle->receive_callback != NULL)                                      /* if receive callback */
        {
            handle->receive_callback(AMG8833_STATUS_INTF);                         /* run callback */
        }
    }
    if ((prev & (1 << AMG8833_STATUS_OVF_IRS)) != 0)                               /* if temperature output overflow */
    {
        if (handle->receive_callback != NULL)                                      /* if receive callback */
        {
            handle->receive_callback(AMG8833_STATUS_OVF_IRS);                      /* run callback */
        }
    }
    if ((prev & (1 << AMG8833_STATUS_OVF_THS)) != 0)                               /* if thermistor temperature output overflow */
    {
        if (handle->receive_callback != NULL)                                      /* if receive callback */
        {
            handle->receive_callback(AMG8833_STATUS_OVF_THS);                      /* run callback */
        }
    }
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t amg8833_set_mode(amg8833_handle_t *handle, amg8833_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    prev = mode;                                                                    /* set the prev */
    res = a_amg8833_iic_write(handle, AMG8833_REG_PCTL, (uint8_t *)&prev, 1);       /* write pctl register */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("amg8833: write pctl register failed.\n");              /* write pctl register failed */
       
        return 1;                                                                   /* return error */
    }
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t amg8833_get_mode(amg8833_handle_t *handle, amg8833_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_amg8833_iic_read(handle, AMG8833_REG_PCTL, (uint8_t *)&prev, 1);       /* read pctl register */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("amg8833: read pctl register failed.\n");              /* read pctl register failed */
       
        return 1;                                                                  /* return error */
    }
    *mode = (amg8833_mode_t)(prev);                                                /* set the mode */
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t amg8833_reset(amg8833_handle_t *handle, amg8833_reset_type_t type)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    prev = type;                                                                   /* set the prev */
    res = a_amg8833_iic_write(handle, AMG8833_REG_RST, (uint8_t *)&prev, 1);       /* write rst register */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("amg8833: write rst register failed.\n");              /* write rst register failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t amg8833_set_frame_rate(amg8833_handle_t *handle, amg8833_frame_rate_t rate)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_amg8833_iic_read(handle, AMG8833_REG_FPSC, (uint8_t *)&prev, 1);       /* read fpsc register */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("amg8833: read fpsc register failed.\n");              /* read fpsc register failed */
       
        return 1;                                                                  /* return error */
    }
    prev &= ~(1 << 0);                                                             /* clear the config */
    prev |= rate << 0;                                                             /* set the config */
    res = a_amg8833_iic_write(handle, AMG8833_REG_FPSC, (uint8_t *)&prev, 1);      /* write fpsc register */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("amg8833: write fpsc register failed.\n");             /* write fpsc register failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t amg8833_get_frame_rate(amg8833_handle_t *handle, amg8833_frame_rate_t *rate)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_amg8833_iic_read(handle, AMG8833_REG_FPSC, (uint8_t *)&prev, 1);       /* read fpsc register */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("amg8833: read fpsc register failed.\n");              /* read fpsc register failed */
       
        return 1;                                                                  /* return error */
    }
    *rate = (amg8833_frame_rate_t)((prev >> 0) & 0x01);                            /* set the rate */
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t amg8833_set_interrupt_mode(amg8833_handle_t *handle, amg8833_interrupt_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_amg8833_iic_read(handle, AMG8833_REG_INTC, (uint8_t *)&prev, 1);       /* read intc register */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("amg8833: read intc register failed.\n");              /* read intc register failed */
       
        return 1;                                                                  /* return error */
    }
    prev &= ~(1 << 1);                                                             /* clear the config */
    prev |= mode << 1;                                                             /* set the config */
    res = a_amg8833_iic_write(handle, AMG8833_REG_INTC, (uint8_t *)&prev, 1);      /* write intc register */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("amg8833: write intc register failed.\n");             /* write intc register failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t amg8833_get_interrupt_mode(amg8833_handle_t *handle, amg8833_interrupt_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_amg8833_iic_read(handle, AMG8833_REG_INTC, (uint8_t *)&prev, 1);       /* read intc register */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("amg8833: read intc register failed.\n");              /* read intc register failed */
       
        return 1;                                                                  /* return error */
    }
    *mode = (amg8833_interrupt_mode_t)((prev >> 1) & 0x01);                        /* set the mode */
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t amg8833_set_interrupt(amg8833_handle_t *handle, amg8833_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_amg8833_iic_read(handle, AMG8833_REG_INTC, (uint8_t *)&prev, 1);       /* read intc register */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("amg8833: read intc register failed.\n");              /* read intc register failed */
       
        return 1;                                                                  /* return error */
    }
    prev &= ~(1 << 0);                                                             /* clear the config */
    prev |= enable << 0;                                                           /* set the config */
    res = a_amg8833_iic_write(handle, AMG8833_REG_INTC, (uint8_t *)&prev, 1);      /* write intc register */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("amg8833: write intc register failed.\n");             /* write intc register failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t amg8833_get_interrupt(amg8833_handle_t *handle, amg8833_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_amg8833_iic_read(handle, AMG8833_REG_INTC, (uint8_t *)&prev, 1);       /* read intc register */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("amg8833: read intc register failed.\n");              /* read intc register failed */
       
        return 1;                                                                  /* return error */
    }
    *enable = (amg8833_bool_t)((prev >> 0) & 0x01);                                /* get the bool */
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t amg8833_get_status(amg8833_handle_t *handle, uint8_t *status)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_amg8833_iic_read(handle, AMG8833_REG_STAT, (uint8_t *)&prev, 1);       /* read stat register */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("amg8833: read stat register failed.\n");              /* read stat register failed */
       
        return 1;                                                                  /* return error */
    }
    *status = prev;                                                                /* set the status */
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t amg8833_clear_status(amg8833_handle_t *handle, amg8833_status_t status)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    prev = 1 << status;                                                             /* set the command */
    res = a_amg8833_iic_write(handle, AMG8833_REG_SCLR, (uint8_t *)&prev, 1);       /* write sclr register */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("amg8833: write sclr register failed.\n");              /* write sclr register failed */
       
        return 1;                                                                   /* return error */
    }
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t amg8833_set_average_mode(amg8833_handle_t *handle, amg8833_average_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_amg8833_iic_read(handle, AMG8833_REG_AVE, (uint8_t *)&prev, 1);        /* read ave register */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("amg8833: read ave register failed.\n");               /* read ave register failed */
       
        return 1;                                                                  /* return error */
    }
    prev &= ~(1 << 5);                                                             /* clear the config */
    prev |= mode << 5;                                                             /* set the config */
    res = a_amg8833_iic_write(handle, AMG8833_REG_AVE, (uint8_t *)&prev, 1);       /* write ave register */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("amg8833: write ave register failed.\n");              /* write ave register failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t amg8833_get_average_mode(amg8833_handle_t *handle, amg8833_average_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_amg8833_iic_read(handle, AMG8833_REG_AVE, (uint8_t *)&prev, 1);        /* read ave register */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("amg8833: read ave register failed.\n");               /* read ave register failed */
       
        return 1;                                                                  /* return error */
    }
    *mode = (amg8833_average_mode_t)((prev >> 5) & 0x01);                          /* set the mode */
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t amg8833_set_interrupt_high_level(amg8833_handle_t *handle, int16_t level)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    buf[0] = (level >> 0) & 0xFF;                                                      /* get lower */
    buf[1] = (level >> 8) & 0xF;                                                       /* get upper */
    res = a_amg8833_iic_write(handle, AMG8833_REG_INTHL, (uint8_t *)&buf[0], 1);       /* write inthl register */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("amg8833: write inthl register failed.\n");                /* write inthl register failed */
       
        return 1;                                                                      /* return error */
    }
    res = a_amg8833_iic_write(handle, AMG8833_REG_INTHH, (uint8_t *)&buf[1], 1);       /* write inthh register */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("amg8833: write inthh register failed.\n");                /* write inthh register failed */
       
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t amg8833_get_interrupt_high_level(amg8833_handle_t *handle, int16_t *level)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    res = a_amg8833_iic_read(handle, AMG8833_REG_INTHL, (uint8_t *)&buf[0], 1);       /* read inthl register */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("amg8833: read inthl register failed.\n");                /* read inthl register failed */
       
        return 1;                                                                     /* return error */
    }
    res = a_amg8833_iic_read(handle, AMG8833_REG_INTHH, (uint8_t *)&buf[1], 1);       /* read inthh register */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("amg8833: read inthh register failed.\n");                /* read inthh register failed */
       
        return 1;                                                                     /* return error */
    }
    if ((buf[1] & (1 << 3)) != 0)                                                     /* check negative */
    {
        *level = (int16_t)(((uint16_t)(buf[1]) << 8) | 
                 ((uint16_t)(0xF) << 12) | (buf[0] << 0));                            /* get the level */
    }
    else
    {
        *level = (int16_t)(((uint16_t)(buf[1]) << 8) | (buf[0] << 0));                /* get the level */
    }
    
    return 0;                                                                         /* success return 0 */
}

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
uint8_t amg8833_set_interrupt_low_level(amg8833_handle_t *handle, int16_t level)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    buf[0] = (level >> 0) & 0xFF;                                                      /* get lower */
    buf[1] = (level >> 8) & 0xF;                                                       /* get upper */
    res = a_amg8833_iic_write(handle, AMG8833_REG_INTLL, (uint8_t *)&buf[0], 1);       /* write intll register */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("amg8833: write intll register failed.\n");                /* write intll register failed */
       
        return 1;                                                                      /* return error */
    }
    res = a_amg8833_iic_write(handle, AMG8833_REG_INTLH, (uint8_t *)&buf[1], 1);       /* write intlh register */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("amg8833: write intlh register failed.\n");                /* write intlh register failed */
       
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t amg8833_get_interrupt_low_level(amg8833_handle_t *handle, int16_t *level)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    res = a_amg8833_iic_read(handle, AMG8833_REG_INTLL, (uint8_t *)&buf[0], 1);       /* read intll register */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("amg8833: read intll register failed.\n");                /* read intll register failed */
       
        return 1;                                                                     /* return error */
    }
    res = a_amg8833_iic_read(handle, AMG8833_REG_INTLH, (uint8_t *)&buf[1], 1);       /* read intlh register */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("amg8833: read intlh register failed.\n");                /* read intlh register failed */
       
        return 1;                                                                     /* return error */
    }
    if ((buf[1] & (1 << 3)) != 0)                                                     /* check negative */
    {
        *level = (int16_t)(((uint16_t)(buf[1]) << 8) | 
                 ((uint16_t)(0xF) << 12) | (buf[0] << 0));                            /* get the level */
    }
    else
    {
        *level = (int16_t)(((uint16_t)(buf[1]) << 8) | (buf[0] << 0));                /* get the level */
    }
    
    return 0;                                                                         /* success return 0 */
}

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
uint8_t amg8833_set_interrupt_hysteresis_level(amg8833_handle_t *handle, int16_t level)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    buf[0] = (level >> 0) & 0xFF;                                                      /* get lower */
    buf[1] = (level >> 8) & 0xF;                                                       /* get upper */
    res = a_amg8833_iic_write(handle, AMG8833_REG_IHYSL, (uint8_t *)&buf[0], 1);       /* write ihysl register */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("amg8833: write ihysl register failed.\n");                /* write ihysl register failed */
       
        return 1;                                                                      /* return error */
    }
    res = a_amg8833_iic_write(handle, AMG8833_REG_IHYSH, (uint8_t *)&buf[1], 1);       /* write ihysh register */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("amg8833: write ihysh register failed.\n");                /* write ihysh register failed */
       
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t amg8833_get_interrupt_hysteresis_level(amg8833_handle_t *handle, int16_t *level)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    res = a_amg8833_iic_read(handle, AMG8833_REG_IHYSL, (uint8_t *)&buf[0], 1);       /* read ihysl register */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("amg8833: read ihysl register failed.\n");                /* read ihysl register failed */
       
        return 1;                                                                     /* return error */
    }
    res = a_amg8833_iic_read(handle, AMG8833_REG_IHYSH, (uint8_t *)&buf[1], 1);       /* read ihysh register */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("amg8833: read ihysh register failed.\n");                /* read ihysh register failed */
       
        return 1;                                                                     /* return error */
    }
    if ((buf[1] & (1 << 3)) != 0)                                                     /* check negative */
    {
        *level = (int16_t)(((uint16_t)(buf[1]) << 8) | 
                 ((uint16_t)(0xF) << 12) | (buf[0] << 0));                            /* get the level */
    }
    else
    {
        *level = (int16_t)(((uint16_t)(buf[1]) << 8) | (buf[0] << 0));                /* get the level */
    }
    
    return 0;                                                                         /* success return 0 */
}

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
uint8_t amg8833_interrupt_level_convert_to_register(amg8833_handle_t *handle, float temp, int16_t *reg)
{
    if (handle == NULL)                    /* check handle */
    {
        return 2;                          /* return error */
    }
    if (handle->inited != 1)               /* check handle initialization */
    {
        return 3;                          /* return error */
    }
    
    *reg = (int16_t)(temp / 0.25f);        /* convert real data to register data */
    
    return 0;                              /* success return 0 */
}

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
uint8_t amg8833_interrupt_level_convert_to_data(amg8833_handle_t *handle, int16_t reg, float *temp)
{
    if (handle == NULL)                  /* check handle */
    {
        return 2;                        /* return error */
    }
    if (handle->inited != 1)             /* check handle initialization */
    {
        return 3;                        /* return error */
    }
    
    *temp = (float)(reg) * 0.25f;        /* convert raw data to real data */
    
    return 0;                            /* success return 0 */
}

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
uint8_t amg8833_read_temperature(amg8833_handle_t *handle, int16_t *raw, float *temp)
{
    uint8_t res;
    uint8_t buf[2];
    int16_t data;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_amg8833_iic_read(handle, AMG8833_REG_TTHL, (uint8_t *)&buf[0], 1);       /* read tthl register */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("amg8833: read tthl register failed.\n");                /* read tthl register failed */
       
        return 1;                                                                    /* return error */
    }
    res = a_amg8833_iic_read(handle, AMG8833_REG_TTHH, (uint8_t *)&buf[1], 1);       /* read tthh register */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("amg8833: read tthh register failed.\n");                /* read tthh register failed */
       
        return 1;                                                                    /* return error */
    }
    *raw = (int16_t)(((uint16_t)(buf[1] & 0xF) << 8) | (buf[0] << 0));               /* get the raw */
    data = (int16_t)(((uint16_t)(buf[1] & 0x7) << 8) | (buf[0] << 0));               /* get the raw */
    if ((buf[1] & 0x8) != 0)                                                         /* if negative */
    {
        data = data * (-1);                                                          /* x (-1) */
    }
    *temp = data * 0.0625f;                                                          /* convert the temperature */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t amg8833_read_temperature_array(amg8833_handle_t *handle, int16_t raw[8][8], float temp[8][8])
{
    uint8_t res;
    uint8_t i;
    uint8_t j;
    uint8_t buf[128];
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    res = a_amg8833_iic_read(handle, AMG8833_REG_T01L, (uint8_t *)buf, 128);                /* read t01l register */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("amg8833: read t01l register failed.\n");                       /* read t01l register failed */
       
        return 1;                                                                           /* return error */
    }
    for (i = 0; i < 8; i++)                                                                 /* run 8 times */
    {
        for (j = 0; j < 8; j++)                                                             /* run 8 times */
        {
            raw[7 - i][7 - j] = (int16_t)(((uint16_t)buf[i * 16 + j * 2 + 1] << 8) |        /* get raw data */
                                                     buf[i * 16 + j * 2 + 0]);              /* get raw data */
            temp[7 - i][7 - j] = (float)raw[7 - i][7 - j] * 0.25f;                          /* get converted temperature */
        }
    }

    return 0;                                                                               /* success return 0 */
}

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
uint8_t amg8833_get_interrupt_table(amg8833_handle_t *handle, uint8_t table[8][1])
{
    uint8_t res;
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    res = a_amg8833_iic_read(handle, AMG8833_REG_INT0, (uint8_t *)table[7], 1);       /* read int0 register */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("amg8833: read int0 register failed.\n");                 /* read int0 register failed */
       
        return 1;                                                                     /* return error */
    }
    res = a_amg8833_iic_read(handle, AMG8833_REG_INT1, (uint8_t *)table[6], 1);       /* read int1 register */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("amg8833: read int1 register failed.\n");                 /* read int1 register failed */
       
        return 1;                                                                     /* return error */
    }
    res = a_amg8833_iic_read(handle, AMG8833_REG_INT2, (uint8_t *)table[5], 1);       /* read int2 register */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("amg8833: read int2 register failed.\n");                 /* read int2 register failed */
       
        return 1;                                                                     /* return error */
    }
    res = a_amg8833_iic_read(handle, AMG8833_REG_INT3, (uint8_t *)table[4], 1);       /* read int3 register */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("amg8833: read int3 register failed.\n");                 /* read int3 register failed */
       
        return 1;                                                                     /* return error */
    }
    res = a_amg8833_iic_read(handle, AMG8833_REG_INT4, (uint8_t *)table[3], 1);       /* read int4 register */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("amg8833: read int4 register failed.\n");                 /* read int4 register failed */
       
        return 1;                                                                     /* return error */
    }
    res = a_amg8833_iic_read(handle, AMG8833_REG_INT5, (uint8_t *)table[2], 1);       /* read int5 register */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("amg8833: read int5 register failed.\n");                 /* read int5 register failed */
       
        return 1;                                                                     /* return error */
    }
    res = a_amg8833_iic_read(handle, AMG8833_REG_INT6, (uint8_t *)table[1], 1);       /* read int6 register */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("amg8833: read int6 register failed.\n");                 /* read int6 register failed */
       
        return 1;                                                                     /* return error */
    }
    res = a_amg8833_iic_read(handle, AMG8833_REG_INT7, (uint8_t *)table[0], 1);       /* read int7 register */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("amg8833: read int7 register failed.\n");                 /* read int7 register failed */
       
        return 1;                                                                     /* return error */
    }
    
    return 0;                                                                         /* success return 0 */
}

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
uint8_t amg8833_set_reg(amg8833_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                      /* check handle */
    {
        return 2;                                            /* return error */
    }
    if (handle->inited != 1)                                 /* check handle initialization */
    {
        return 3;                                            /* return error */
    }
    
    return a_amg8833_iic_write(handle, reg, buf, len);       /* write data */
}

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
uint8_t amg8833_get_reg(amg8833_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                     /* check handle */
    {
        return 2;                                           /* return error */
    }
    if (handle->inited != 1)                                /* check handle initialization */
    {
        return 3;                                           /* return error */
    }
    
    return a_amg8833_iic_read(handle, reg, buf, len);       /* read data */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to an amg8833 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t amg8833_info(amg8833_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(amg8833_info_t));                        /* initialize amg8833 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
