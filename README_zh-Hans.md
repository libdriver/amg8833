[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver AMG8833

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/amg8833/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

AMG8833红外阵列传感器是一种热电堆型红外传感器。以下条件通常会降低温度精度。请仔细检查实际使用条件下的性能和稳定性，并在必要时进行温度校正。 (1) 当传感器安装位置附近存在发热元件时。 (2) 当传感器暴露在热源附近。(3) 传感器本体温度急剧变化时。 (4) 传感器和检测对象之间存在难以透过远红外线的物质（例如玻璃、丙烯酸树脂或蒸汽）时。 (5) 当与传感器贴合的部分存在难以透过远红外线物质（例如，异物或水）。AMG8833可用于高性能家电、办公节能、数字标牌、自动门等。 

LibDriver AMG8833是LibDriver推出的AMG8833全功能驱动，提供温度读取和温度阵列读取功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example interrupt](#example-interrupt)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver AMG8833的源文件。

/interface目录包含了LibDriver AMG8833与平台无关的IIC总线模板。

/test目录包含了LibDriver AMG8833驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver AMG8833编程范例。

/doc目录包含了LibDriver AMG8833离线文档。

/datasheet目录包含了AMG8833数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的IIC总线模板，完成指定平台的IIC总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

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

### 文档

在线文档: [https://www.libdriver.com/docs/amg8833/index.html](https://www.libdriver.com/docs/amg8833/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。