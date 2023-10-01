### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

INT Pin: INT PB0.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. AMG8833

#### 3.1 Command Instruction

1. Show amg8833 chip and driver information.

   ```shell
   amg8833 (-i | --information)
   ```

2. Show amg8833 help.

   ```shell
   amg8833 (-h | --help)
   ```

3. Show amg8833 pin connections of the current board.

   ```shell
   amg8833 (-p | --port)
   ```

4. Run amg8833  register test.

   ```shell
   amg8833 (-t reg | --test=reg) [--addr=<0 | 1>]
   ```

5. Run amg8833 read test, num means test times.

   ```shell
   amg8833 (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]
   ```

6. Run amg8833 interrupt test, num means test times, mode is the interrupt mode and it can be "ABS" and "DIFF", high is the interrupt high level, low is the interrupt low level and hysteresis is the hysteresis level.   

   ```shell
   amg8833 (-t int | --test=int) [--addr=<0 | 1>] [--times=<num>] [--mode=<ABS | DIFF>] [--high-threshold=<high>] [--low-threshold=<low>] [--hysteresis-level=<hysteresis>]
   ```

7. Run amg8833 read function, num means test times.  

   ```shell
   amg8833 (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]
   ```

8. Run amg8833 interrupt function, num means test times, mode is the interrupt mode and it can be "ABS" and "DIFF", high is the interrupt high level, low is the interrupt low level and hysteresis is the hysteresis level.  

   ```shell
   amg8833 (-e int | --example=int) [--addr=<0 | 1>] [--times=<num>] [--mode=<ABS | DIFF>] [--high-threshold=<high>] [--low-threshold=<low>] [--hysteresis-level=<hysteresis>]
   ```

#### 3.2 Command Example

```shell
amg8833 -i

amg8833: chip is Panasonic AMG8833.
amg8833: manufacturer is Panasonic.
amg8833: interface is IIC.
amg8833: driver version is 1.0.
amg8833: min supply voltage is 3.0V.
amg8833: max supply voltage is 3.6V.
amg8833: max current is 5.00mA.
amg8833: max temperature is 80.0C.
amg8833: min temperature is -20.0C.
```

```shell
amg8833 -p

amg8833: SCL connected to GPIOB PIN8.
amg8833: SDA connected to GPIOB PIN9.
amg8833: INT connected to GPIOB PIN0.
```

```shell
amg8833 -t reg --addr=1

amg8833: chip is Panasonic AMG8833.
amg8833: manufacturer is Panasonic.
amg8833: interface is IIC.
amg8833: driver version is 1.0.
amg8833: min supply voltage is 3.0V.
amg8833: max supply voltage is 3.6V.
amg8833: max current is 5.00mA.
amg8833: max temperature is 80.0C.
amg8833: min temperature is -20.0C.
amg8833: start register test.
amg8833: amg8833_set_addr_pin/amg8833_get_addr_pin test.
amg8833: set addr pin address 0.
amg8833: check addr pin ok.
amg8833: set addr pin address 1.
amg8833: check addr pin ok.
amg8833: amg8833_set_mode/amg8833_get_mode test.
amg8833: set sleep mode.
amg8833: check mode ok.
amg8833: set stand by 60s mode.
amg8833: check mode ok.
amg8833: set stand by 10s mode.
amg8833: check mode ok.
amg8833: set normal mode.
amg8833: check mode ok.
amg8833: amg8833_set_frame_rate/amg8833_get_frame_rate test.
amg8833: set 10 fps.
amg8833: check rate ok.
amg8833: set 1 fps.
amg8833: check rate ok.
amg8833: amg8833_set_interrupt_mode/amg8833_get_interrupt_mode test.
amg8833: set difference interrupt mode.
amg8833: check interrupt mode ok.
amg8833: set absolute value interrupt mode.
amg8833: check interrupt mode ok.
amg8833: amg8833_set_interrupt/amg8833_get_interrupt test.
amg8833: disable interrupt mode.
amg8833: check interrupt ok.
amg8833: enable interrupt mode.
amg8833: check interrupt ok.
amg8833: amg8833_set_average_mode/amg8833_get_average_mode test.
amg8833: set once average mode.
amg8833: check average mode ok.
amg8833: set twice average mode.
amg8833: check average mode ok.
amg8833: amg8833_set_interrupt_high_level/amg8833_get_interrupt_high_level test.
amg8833: set interrupt high level 176.
amg8833: check level ok.
amg8833: amg8833_set_interrupt_low_level/amg8833_get_interrupt_low_level test.
amg8833: set interrupt low level -459.
amg8833: check level ok.
amg8833: amg8833_set_interrupt_hysteresis_level/amg8833_get_interrupt_hysteresis_level test.
amg8833: set interrupt hysteresis level -841.
amg8833: check level ok.
amg8833: amg8833_interrupt_level_convert_to_register/amg8833_interrupt_level_convert_to_data test.
amg8833: generated temperature is 31.60.
amg8833: converted temperature is 31.50.
amg8833: amg8833_get_status test.
amg8833: check ok and status is 0x00.
amg8833: amg8833_clear_status test.
amg8833: check thermistor temperature output overflow ok.
amg8833: check temperature output overflow ok.
amg8833: check interrupt outbreak ok.
amg8833: amg8833_get_interrupt_table test.
1  1  1  1  1  1  1  1  
1  1  1  1  1  1  1  1  
1  1  1  1  1  1  1  1  
1  1  1  1  1  1  1  1  
1  1  1  1  1  1  1  1  
1  1  1  1  1  1  1  1  
1  1  1  1  1  1  1  1  
1  1  1  1  1  1  1  1  
amg8833: amg8833_reset test.
amg8833: check reset ok.
amg8833: check reset ok.
amg8833: finish register test.
```

```shell
amg8833 -t read --addr=1 --times=3

amg8833: chip is Panasonic AMG8833.
amg8833: manufacturer is Panasonic.
amg8833: interface is IIC.
amg8833: driver version is 1.0.
amg8833: min supply voltage is 3.0V.
amg8833: max supply voltage is 3.6V.
amg8833: max current is 5.00mA.
amg8833: max temperature is 80.0C.
amg8833: min temperature is -20.0C.
amg8833: start read test.
24.00  22.75  24.50  23.25  22.75  22.00  21.25  22.25  
23.75  22.50  23.00  25.50  26.00  22.75  21.25  24.00  
25.00  23.25  23.50  23.25  25.25  25.00  22.50  24.25  
24.25  23.25  22.75  23.75  23.00  23.25  23.25  23.00  
24.75  24.00  23.50  23.00  23.50  23.50  22.75  24.50  
24.75  23.25  23.50  23.25  23.50  23.25  23.00  25.00  
25.50  24.00  23.75  23.50  23.25  23.75  23.25  25.25  
24.50  23.25  23.75  24.25  24.25  24.00  24.25  25.75  
amg8833: temperature is 30.188C.
21.75  22.50  23.50  22.50  22.25  21.50  20.25  21.00  
22.50  22.00  22.25  25.25  25.50  22.00  20.75  22.25  
23.50  22.50  22.50  23.00  25.00  24.50  22.00  23.75  
23.50  22.50  22.50  22.50  22.75  22.75  22.50  22.75  
24.00  22.75  22.50  22.25  22.75  23.00  23.00  24.00  
23.75  22.25  22.25  22.50  22.50  22.75  23.25  24.50  
23.50  23.00  23.00  22.75  23.00  22.75  23.00  25.75  
23.25  22.75  22.50  23.00  23.25  26.25  25.50  27.50  
amg8833: temperature is 30.188C.
22.75  22.75  23.75  22.75  22.50  21.25  20.25  21.50  
22.25  22.00  22.50  25.00  25.00  22.00  20.75  22.25  
23.50  22.50  22.75  22.50  24.75  24.75  21.75  23.25  
24.00  21.75  22.25  22.50  22.50  22.50  22.50  22.50  
23.75  22.75  22.75  22.75  23.00  22.75  22.25  23.75  
23.75  22.50  22.50  22.50  22.50  22.50  22.50  23.75  
23.50  23.25  22.75  22.75  22.50  22.50  22.00  24.00  
22.50  22.25  22.75  22.50  22.50  22.75  22.75  24.75  
amg8833: temperature is 30.188C.
amg8833: finish read test.
```

```shell
amg8833 -t int --addr=1 --times=3 --mode=ABS --high-threshold=32.0 --low-threshold=25.0 --hysteresis-level=28.0

amg8833: chip is Panasonic AMG8833.
amg8833: manufacturer is Panasonic.
amg8833: interface is IIC.
amg8833: driver version is 1.0.
amg8833: min supply voltage is 3.0V.
amg8833: max supply voltage is 3.6V.
amg8833: max current is 5.00mA.
amg8833: max temperature is 80.0C.
amg8833: min temperature is -20.0C.
amg8833: start interrupt test.
amg8833: temperature is 29.938C.
amg8833: temperature is 30.000C.
amg8833: irq interrupt outbreak.
amg8833: temperature is 30.000C.
amg8833: irq interrupt outbreak.
amg8833: finish interrupt test.
```

```shell
amg8833 -e read --addr=1 --times=3

23.00  23.00  23.75  22.50  22.50  21.75  20.50  21.75  
23.50  22.75  22.75  25.25  25.25  22.50  21.25  23.25  
24.25  22.75  23.00  22.75  25.25  24.50  22.75  23.75  
24.50  23.25  22.50  23.00  23.25  22.50  23.00  23.50  
24.50  23.50  23.25  23.25  23.00  23.00  22.50  24.25  
24.50  23.25  23.00  23.00  22.75  22.75  22.75  24.75  
25.00  23.75  23.50  23.75  22.75  23.25  23.00  24.50  
24.25  23.50  23.25  24.00  24.00  23.50  24.00  25.50  
amg8833: temperature is 29.938C.
21.50  22.50  23.50  22.50  22.00  21.00  20.00  20.75  
21.75  21.75  21.50  25.00  24.75  21.75  20.75  22.50  
23.00  22.00  22.00  22.25  24.75  24.25  21.50  23.75  
23.25  22.00  22.25  22.50  22.50  22.00  22.00  22.25  
23.50  22.00  22.25  22.25  22.00  22.50  21.75  23.50  
23.25  22.50  22.00  22.50  22.25  22.25  22.00  23.75  
23.50  23.00  22.50  22.25  22.75  22.50  22.00  23.75  
22.75  22.50  22.50  22.50  22.25  22.50  22.75  23.75  
amg8833: temperature is 30.000C.
22.00  22.25  23.75  22.25  22.25  21.25  20.00  20.75  
22.75  21.75  22.25  25.25  25.00  22.00  20.50  22.50  
23.00  22.25  22.00  22.50  25.25  24.25  21.75  23.25  
23.25  22.50  22.25  22.75  22.25  22.50  22.25  22.75  
24.00  22.75  22.50  22.50  22.25  22.50  21.75  23.50  
23.75  22.25  22.00  22.25  22.50  22.00  21.75  23.50  
23.50  23.00  22.75  22.75  22.25  22.50  22.00  23.50  
22.75  21.75  22.25  22.25  22.25  22.50  22.75  23.25  
amg8833: temperature is 30.000C.
```

```shell
amg8833 -e int --addr=1 --times=3 --mode=ABS --high-threshold=32.0 --low-threshold=25.0 --hysteresis-level=28.0

amg8833: temperature is 29.875C.
amg8833: temperature is 29.938C.
amg8833: irq interrupt outbreak.
1  1  1  1  1  1  1  1  
1  1  1  0  0  1  1  1  
0  1  1  1  0  0  1  1  
0  1  1  1  1  1  1  1  
0  1  1  1  1  1  1  0  
0  1  1  1  1  1  1  1  
0  1  1  1  1  1  1  1  
0  1  1  1  1  1  1  1  
amg8833: temperature is 29.938C.
amg8833: irq interrupt outbreak.
1  1  1  1  1  1  1  1  
1  1  1  1  1  1  1  1  
0  1  1  1  1  1  1  1  
0  1  1  1  1  1  1  1  
0  1  1  1  1  1  1  1  
0  1  1  1  1  1  1  1  
0  1  1  1  1  1  1  1  
0  1  1  1  1  1  1  1  
```

```shell
amg8833 -h

Usage:
  amg8833 (-i | --information)
  amg8833 (-h | --help)
  amg8833 (-p | --port)
  amg8833 (-t reg | --test=reg) [--addr=<0 | 1>]
  amg8833 (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]
  amg8833 (-t int | --test=int) [--addr=<0 | 1>] [--times=<num>] [--mode=<ABS | DIFF>]
          [--high-threshold=<high>] [--low-threshold=<low>] [--hysteresis-level=<hysteresis>]
  amg8833 (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]
  amg8833 (-e int | --example=int) [--addr=<0 | 1>] [--times=<num>] [--mode=<ABS | DIFF>]
          [--high-threshold=<high>] [--low-threshold=<low>] [--hysteresis-level=<hysteresis>]

Options:
      --addr=<0 | 1>                     Set the chip iic address.([default: 0])
  -e <read | int>, --example=<read | int>
                                         Run the driver example.
  -h, --help                             Show the help.
      --high-threshold=<high>            Set the interrupt high threshold.([default: 30.0f])
      --hysteresis-level=<hysteresis>    Set the hysteresis level.([default: 28.0f])
      --low-threshold=<low>              Set the interrupt low threshold.([default: 25.0f])
  -i, --information                      Show the chip information.
      --mode=<ABS | DIFF>                Set the interrupt mode.([default: ABS])
  -p, --port                             Display the pin connections of the current board.
  -t <reg | read | int>, --test=<reg | read | int>
                                         Run the driver test.
      --times=<num>                      Set the running times.([default: 3])
```

