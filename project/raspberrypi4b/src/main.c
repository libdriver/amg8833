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
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-02-21
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/02/21  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_amg8833_read_test.h"
#include "driver_amg8833_register_test.h"
#include "driver_amg8833_interrupt_test.h"
#include "driver_amg8833_basic.h"
#include "driver_amg8833_interrupt.h"
#include "gpio.h"
#include "mutex.h"
#include <getopt.h>
#include <stdlib.h>

uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq */

/**
 * @brief     interface receive callback
 * @param[in] type is the interrupt type
 * @note      none
 */
static void a_callback(uint8_t type)
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
            uint8_t res;
            uint8_t i, j;
            uint8_t level;
            uint8_t table[8][1];
            
            amg8833_interface_debug_print("amg8833: irq interrupt outbreak.\n");
            
            /* get table */
            res = amg8833_interrupt_get_table((uint8_t (*)[1])table);
            if (res != 0)
            {
                amg8833_interface_debug_print("amg8833: get table failed.\n");
            }
            else
            {
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
            }
            
            break;
        }
        default :
        {
            amg8833_interface_debug_print("amg8833: unknown code.\n");
            
            break;
        }
    }
}

/**
 * @brief     amg8833 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t amg8833(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"high-threshold", required_argument, NULL, 2},
        {"hysteresis-level", required_argument, NULL, 3},
        {"low-threshold", required_argument, NULL, 4},
        {"mode", required_argument, NULL, 5},
        {"times", required_argument, NULL, 6},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    amg8833_address_t addr = AMG8833_ADDRESS_0;
    amg8833_interrupt_mode_t mode = AMG8833_INTERRUPT_MODE_ABSOLUTE;
    float high_threshold = 30.0f;
    float low_threshold = 25.0f;
    float hysteresis_level = 28.0f;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* addr */
            case 1 :
            {
                /* set the addr pin */
                if (strcmp("0", optarg) == 0)
                {
                    addr = AMG8833_ADDRESS_0;
                }
                else if (strcmp("1", optarg) == 0)
                {
                    addr = AMG8833_ADDRESS_1;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* high threshold */
            case 2 :
            {
                /* set the high threshold */
                high_threshold = atof(optarg);
                
                break;
            }
            
            /* hysteresis level */
            case 3 :
            {
                /* set the hysteresis level */
                hysteresis_level = atof(optarg);
                
                break;
            }
             
            /* low threshold */
            case 4 :
            {
                /* set the low threshold */
                low_threshold = atof(optarg);
                
                break;
            }
            
            /* mode */
            case 5 :
            {
                /* set the mode */
                if (strcmp("ABS", optarg) == 0)
                {
                    mode = AMG8833_INTERRUPT_MODE_ABSOLUTE;
                }
                else if (strcmp("DIFF", optarg) == 0)
                {
                    mode = AMG8833_INTERRUPT_MODE_DIFFERENCE;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* running times */
            case 6 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run reg test */
        if (amg8833_register_test(addr) != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_read", type) == 0)
    {
        /* run read test */
        if (amg8833_read_test(addr, times) != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_int", type) == 0)
    {
        /* set gpio irq */
        g_gpio_irq = amg8833_interrupt_test_irq_handler;
        
        /* gpio init */
        if (gpio_interrupt_init() != 0)
        {
            g_gpio_irq = NULL;
        }
        
        /* run interrupt test */
        if (amg8833_interrupt_test(addr,
                                   mode,
                                   high_threshold,
                                   low_threshold,
                                   hysteresis_level,
                                   times) != 0)
        {
            g_gpio_irq = NULL;
            (void)gpio_interrupt_deinit();
            
            return 1;
        }
        
        /* gpio deinit */
        g_gpio_irq = NULL;
        (void)gpio_interrupt_deinit();
        
        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i, j, k;
        
        /* init */
        res = amg8833_basic_init(addr);
        if (res != 0)
        {
            return 1;
        }
        
        /* delay 1000 ms */
        amg8833_interface_delay_ms(1000);
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            float temp[8][8];
            float tmp;
            
            /* read temperature array */
            res = amg8833_basic_read_temperature_array(temp);
            if (res != 0)
            {
                amg8833_interface_debug_print("amg8833: read temperature array failed.\n");
                (void)amg8833_basic_deinit();
                
                return 1;
            }
            else
            {
                /* output */
                for (j = 0; j < 8; j++)
                {
                    for (k = 0; k < 8; k++)
                    {
                        amg8833_interface_debug_print("%0.2f  ", temp[j][k]);
                    }
                    amg8833_interface_debug_print("\n");
                }
            }
            
            /* read temperature */
            res = amg8833_basic_read_temperature((float *)&tmp);
            if (res != 0)
            {
                (void)amg8833_basic_deinit();
                
                return 1;
            }
            
            /* output */
            amg8833_interface_debug_print("amg8833: temperature is %0.3fC.\n", tmp);
            
            /* delay 1000 ms */
            amg8833_interface_delay_ms(1000);
        }
        
        /* deinit */
        (void)amg8833_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_int", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* set gpio irq */
        g_gpio_irq = amg8833_interrupt_irq_handler;
        
        /* gpio init */
        if (gpio_interrupt_init() != 0)
        {
            g_gpio_irq = NULL;
        }
        
        /* interrupt init */
        if (amg8833_interrupt_init(addr, 
                                   mode,
                                   high_threshold,
                                   low_threshold,
                                   hysteresis_level,
                                   a_callback) != 0)
        {
            g_gpio_irq = NULL;
            (void)gpio_interrupt_deinit();
            
            return 1;
        }
        
        /* delay 1000 ms */
        amg8833_interface_delay_ms(1000);
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            float temp;
            
            /* mutex lock */
            (void)mutex_lock();
            
            /* read data */
            res = amg8833_interrupt_read_temperature((float *)&temp);
            if (res != 0)
            {
                (void)amg8833_interrupt_deinit();
                g_gpio_irq = NULL;
                (void)gpio_interrupt_deinit();
                (void)mutex_unlock();
            }
            
            /* mutex unlock */
            (void)mutex_unlock();
            
            /* output */
            amg8833_interface_debug_print("amg8833: temperature is %0.3fC.\n", temp);
            
            /* delay 1000 ms */
            amg8833_interface_delay_ms(1000);
        }
        
        /* deinit */
        (void)amg8833_interrupt_deinit();
        g_gpio_irq = NULL;
        (void)gpio_interrupt_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        amg8833_interface_debug_print("Usage:\n");
        amg8833_interface_debug_print("  amg8833 (-i | --information)\n");
        amg8833_interface_debug_print("  amg8833 (-h | --help)\n");
        amg8833_interface_debug_print("  amg8833 (-p | --port)\n");
        amg8833_interface_debug_print("  amg8833 (-t reg | --test=reg) [--addr=<0 | 1>]\n");
        amg8833_interface_debug_print("  amg8833 (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]\n");
        amg8833_interface_debug_print("  amg8833 (-t int | --test=int) [--addr=<0 | 1>] [--times=<num>] [--mode=<ABS | DIFF>]\n");
        amg8833_interface_debug_print("          [--high-threshold=<high>] [--low-threshold=<low>] [--hysteresis-level=<hysteresis>]\n");
        amg8833_interface_debug_print("  amg8833 (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]\n");
        amg8833_interface_debug_print("  amg8833 (-e int | --example=int) [--addr=<0 | 1>] [--times=<num>] [--mode=<ABS | DIFF>]\n");
        amg8833_interface_debug_print("          [--high-threshold=<high>] [--low-threshold=<low>] [--hysteresis-level=<hysteresis>]\n");
        amg8833_interface_debug_print("\n");
        amg8833_interface_debug_print("Options:\n");
        amg8833_interface_debug_print("      --addr=<0 | 1>                     Set the chip iic address.([default: 0])\n");
        amg8833_interface_debug_print("  -e <read | int>, --example=<read | int>\n");
        amg8833_interface_debug_print("                                         Run the driver example.\n");
        amg8833_interface_debug_print("  -h, --help                             Show the help.\n");
        amg8833_interface_debug_print("      --high-threshold=<high>            Set the interrupt high threshold.([default: 30.0f])\n");
        amg8833_interface_debug_print("      --hysteresis-level=<hysteresis>    Set the hysteresis level.([default: 28.0f])\n");
        amg8833_interface_debug_print("      --low-threshold=<low>              Set the interrupt low threshold.([default: 25.0f])\n");
        amg8833_interface_debug_print("  -i, --information                      Show the chip information.\n");
        amg8833_interface_debug_print("      --mode=<ABS | DIFF>                Set the interrupt mode.([default: ABS])\n");
        amg8833_interface_debug_print("  -p, --port                             Display the pin connections of the current board.\n");
        amg8833_interface_debug_print("  -t <reg | read | int>, --test=<reg | read | int>\n");
        amg8833_interface_debug_print("                                         Run the driver test.\n");
        amg8833_interface_debug_print("      --times=<num>                      Set the running times.([default: 3])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        amg8833_info_t info;
        
        /* print amg8833 info */
        amg8833_info(&info);
        amg8833_interface_debug_print("amg8833: chip is %s.\n", info.chip_name);
        amg8833_interface_debug_print("amg8833: manufacturer is %s.\n", info.manufacturer_name);
        amg8833_interface_debug_print("amg8833: interface is %s.\n", info.interface);
        amg8833_interface_debug_print("amg8833: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        amg8833_interface_debug_print("amg8833: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        amg8833_interface_debug_print("amg8833: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        amg8833_interface_debug_print("amg8833: max current is %0.2fmA.\n", info.max_current_ma);
        amg8833_interface_debug_print("amg8833: max temperature is %0.1fC.\n", info.temperature_max);
        amg8833_interface_debug_print("amg8833: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        amg8833_interface_debug_print("amg8833: SCL connected to GPIO3(BCM).\n");
        amg8833_interface_debug_print("amg8833: SDA connected to GPIO2(BCM).\n");
        amg8833_interface_debug_print("amg8833: INT connected to GPIO17(BCM).\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = amg8833(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        amg8833_interface_debug_print("amg8833: run failed.\n");
    }
    else if (res == 5)
    {
        amg8833_interface_debug_print("amg8833: param is invalid.\n");
    }
    else
    {
        amg8833_interface_debug_print("amg8833: unknown status code.\n");
    }

    return 0;
}
