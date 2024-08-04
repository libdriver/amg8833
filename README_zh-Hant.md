[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver AMG8833

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/amg8833/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

AMG8833紅外陣列傳感器是一種熱電堆型紅外傳感器。以下條件通常會降低溫度精度。請仔細檢查實際使用條件下的性能和穩定性，並在必要時進行溫度校正。 (1) 當傳感器安裝位置附近存在發熱元件時。 (2) 當傳感器暴露在熱源附近。 (3) 傳感器本體溫度急劇變化時。 (4) 傳感器和檢測對象之間存在難以透過遠紅外線的物質（例如玻璃、丙烯酸樹脂或蒸汽）時。 (5) 當與傳感器貼合的部分存在難以透過遠紅外線物質（例如，異物或水）。 AMG8833可用於高性能家電、辦公節能、數字標牌、自動門等。

LibDriver AMG8833是LibDriver推出的AMG8833全功能驅動，提供溫度讀取和溫度陣列讀取功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example interrupt](#example-interrupt)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver AMG8833的源文件。

/interface目錄包含了LibDriver AMG8833與平台無關的IIC總線模板。

/test目錄包含了LibDriver AMG8833驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver AMG8833編程範例。

/doc目錄包含了LibDriver AMG8833離線文檔。

/datasheet目錄包含了AMG8833數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC總線模板，完成指定平台的IIC總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/amg8833/index.html](https://www.libdriver.com/docs/amg8833/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。
