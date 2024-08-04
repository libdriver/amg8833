[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver AMG8833

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/amg8833/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

AMG8833 Infrared Array Sensor is a thermopile type infrared sensor which detects the amount of infrared rays. Below conditions generally degrade the temperature accuracy.Carefully check the performance and stability under actual use conditions, and perform temperature corrections when necessary.(1) When heating elements exist near the mounting position of the sensor.(2) When the sensor is exposed to cold or hot air.(3) When the temperature of the sensor body rapidly changes. (4) When substances (e.g., glasses, acrylics or steams), which hardly transmit a far infrared ray, exist between the sensor and the detected object.(5) When substances (e.g., foreign substances or water), which hardly transmit a far infrared ray,adhere to the lense of the sensor.AMG8833 can be used in high function home appliances, energy saving at office, digital signage, automatic doors and so on.

LibDriver AMG8833 is a full function driver of AMG8833 launched by LibDriver.It provides temperature reading and temperature array reading functions. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example interrupt](#example-interrupt)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver AMG8833 source files.

/interface includes LibDriver AMG8833 IIC platform independent template.

/test includes LibDriver AMG8833 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver AMG8833 sample code.

/doc includes LibDriver AMG8833 offline document.

/datasheet includes AMG8833 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface IIC platform independent template and finish your platform IIC driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_amg8833_basic.h"

uint32_t i, j, k, times;
uint8_t res;

/* init */
res = amg8833_basic_init(AMG8833_ADDRESS_1);
if (res != 0)
{
    return 1;
}

/* delay 1000 ms */
amg8833_interface_delay_ms(1000);

...

times = 3;
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
    else
    {
        amg8833_interface_debug_print("amg8833: temperature is %0.3fC.\n", tmp);
    }

    /* delay 1000 ms */
    amg8833_interface_delay_ms(1000);
    
    ....
}

...

(void)amg8833_basic_deinit();

return 0;
```
#### example interrupt

```C
#include "driver_amg8833_interrupt.h"

uint32_t i, times;
uint8_t res;
uint8_t (*g_gpio_irq)(void) = NULL;

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
            volatile uint8_t res;
            volatile uint8_t i, j;
            volatile uint8_t level;
            volatile uint8_t table[8][1];
            
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
            break;
        }
    }
    
    return 0;
}

/* run interrupt test */
g_gpio_irq = amg8833_interrupt_irq_handler;
if (gpio_interrupt_init() != 0)
{
    g_gpio_irq = NULL;
}
times = 3;

...

if (interrupt_interrupt_init(addr, 
                             mode,
                             32.0f,
                             25.0f,
                             28.0f,
                             a_callback) != 0)
{
    g_gpio_irq = NULL;
    (void)gpio_interrupt_deinit();

    return 1;
}

...

/* delay 1000 ms */
amg8833_interface_delay_ms(1000);
for (i = 0; i < times; i++)
{
    float temp;

    res = amg8833_interrupt_read_temperature((float *)&temp);
    if (res != 0)
    {
        (void)amg8833_interrupt_deinit();
        g_gpio_irq = NULL;
        (void)gpio_interrupt_deinit();
    }
    else
    {
        amg8833_interface_debug_print("amg8833: temperature is %0.3fC.\n", temp);
    }

    /* delay 1000 ms */
    amg8833_interface_delay_ms(1000);
    
    ...
}

...

/* deinit */
(void)amg8833_interrupt_deinit();
g_gpio_irq = NULL;
(void)gpio_interrupt_deinit();

return 0;
```
### Document

Online documents: [https://www.libdriver.com/docs/amg8833/index.html](https://www.libdriver.com/docs/amg8833/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.