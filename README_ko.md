[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver AMG8833

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/amg8833/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

AMG8833 적외선 어레이 센서는 적외선의 양을 감지하는 열전퇴형 적외선 센서입니다. 이하의 조건에서는 일반적으로 온도 정확도가 저하됩니다. 실제 사용 조건에서 성능 및 안정성을 잘 확인하고 필요한 경우 온도 보정을 수행하십시오. (1) 센서 장착 위치 근처에 발열체가 있는 경우 (2) 센서가 노출된 경우 찬 공기 또는 뜨거운 공기.(3) 센서 본체의 온도가 급격히 변할 때. (4) 원적외선을 거의 투과하지 않는 물질(예: 유리, 아크릴 또는 증기)이 센서와 감지 대상 사이에 존재할 때. (5) 적외선을 거의 투과하지 않는 물질(예: 이물질 또는 물)이 원적외선은 센서의 렌즈에 부착하십시오. AMG8833은 고성능 가전 제품, 사무실의 에너지 절약, 디지털 간판, 자동 도어 등에 사용할 수 있습니다.

LibDriver AMG8833은 LibDriver에서 출시한 AMG8833의 전체 기능 드라이버입니다. 온도 판독 및 온도 배열 판독 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
    - [example interrupt](#example-interrupt)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver AMG8833의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver AMG8833용 플랫폼 독립적인 IIC버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver AMG8833드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver AMG8833프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver AMG8833오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 AMG8833데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인IIC 버스 템플릿을 참조하여 지정된 플랫폼에 대한 IIC버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

```c
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

### 문서

온라인 문서: [https://www.libdriver.com/docs/amg8833/index.html](https://www.libdriver.com/docs/amg8833/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.