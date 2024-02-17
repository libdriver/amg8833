[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver AMG8833

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/amg8833/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

AMG8833赤外線アレイセンサーは、赤外線の量を検出するサーモパイルタイプの赤外線センサーです。以下の条件は一般的に温度精度を低下させます。実際の使用条件で性能と安定性を注意深く確認し、必要に応じて温度補正を行ってください。（1）センサーの取り付け位置の近くに発熱体が存在する場合。（2）センサーが露出している場合冷気または温風。（3）センサー本体の温度が急激に変化する場合。 （4）センサーと検出対象物の間に遠赤外線をほとんど透過しない物質（ガラス、アクリル、蒸気など）が存在する場合（5）異物や水などをほとんど透過しない物質（遠赤外線、センサーのレンズに付着AMG8833は、高機能家電、オフィスでの省エネ、デジタルサイネージ、自動ドアなどに使用できます。

LibDriver AMG8833は、LibDriverによって起動されたAMG8833の全機能ドライバーであり、温度読み取りおよび温度配列読み取り機能を提供します。 LibDriverはMISRAに準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example interrupt](#example-interrupt)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver AMG8833のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver AMG8833用のプラットフォームに依存しないIICバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver AMG8833ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver AMG8833プログラミング例が含まれています。

/ docディレクトリには、LibDriver AMG8833オフラインドキュメントが含まれています。

/ datasheetディレクトリには、AMG8833データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIICバステンプレートを参照して、指定したプラットフォームのIICバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/amg8833/index.html](https://www.libdriver.com/docs/amg8833/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。