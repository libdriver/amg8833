### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

INT Pin: INT GPIO17.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(amg8833 REQUIRED)
```


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
./amg8833 -i

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
./amg8833 -p

amg8833: SCL connected to GPIO3(BCM).
amg8833: SDA connected to GPIO2(BCM).
amg8833: INT connected to GPIO17(BCM).
```

```shell
./amg8833 -t reg --addr=1

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
amg8833: set interrupt high level 1383.
amg8833: check level ok.
amg8833: amg8833_set_interrupt_low_level/amg8833_get_interrupt_low_level test.
amg8833: set interrupt low level -966.
amg8833: check level ok.
amg8833: amg8833_set_interrupt_hysteresis_level/amg8833_get_interrupt_hysteresis_level test.
amg8833: set interrupt hysteresis level -105.
amg8833: check level ok.
amg8833: amg8833_interrupt_level_convert_to_register/amg8833_interrupt_level_convert_to_data test.
amg8833: generated temperature is 19.15.
amg8833: converted temperature is 19.00.
amg8833: amg8833_get_status test.
amg8833: check ok and status is 0x00.
amg8833: amg8833_clear_status test.
amg8833: check thermistor temperature output overflow ok.
amg8833: check temperature output overflow ok.
amg8833: check interrupt outbreak ok.
amg8833: amg8833_get_interrupt_table test.
0  0  0  0  0  0  0  0  
0  0  0  0  0  0  0  0  
0  0  0  0  0  0  0  0  
0  0  0  0  0  0  0  0  
0  0  0  0  0  0  0  0  
0  0  0  0  0  0  0  0  
0  0  0  0  0  0  0  0  
0  0  0  0  0  0  0  0  
amg8833: amg8833_reset test.
amg8833: check reset ok.
amg8833: check reset ok.
amg8833: finish register test.
```

```shell
./amg8833 -t read --addr=1 --times=3

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
23.75  23.50  23.00  22.75  22.50  22.50  23.50  23.50  
24.00  24.00  23.75  23.00  24.25  24.50  23.50  23.25  
24.75  24.75  24.25  25.25  25.75  25.25  24.50  23.50  
25.75  25.50  27.25  29.75  28.00  26.00  24.50  24.00  
26.25  27.50  31.00  32.25  30.75  27.00  24.75  25.75  
25.75  26.75  31.50  32.50  32.25  30.25  26.75  26.00  
29.25  29.25  31.50  33.25  30.75  28.25  29.75  28.75  
32.00  31.25  32.75  32.75  30.25  28.75  29.25  30.75  
amg8833: temperature is 32.062C.
23.25  22.25  22.25  22.25  22.50  21.75  23.00  22.75  
23.75  22.75  22.50  22.75  24.25  23.75  23.25  23.00  
24.50  24.25  24.00  25.25  26.25  24.50  24.25  22.75  
25.25  24.50  26.50  29.25  27.50  25.25  24.00  23.25  
26.00  26.50  30.25  31.50  30.75  26.25  23.75  24.50  
24.00  26.75  31.00  32.50  31.50  30.25  26.50  25.50  
27.75  28.50  31.00  32.75  30.25  28.00  29.25  28.50  
30.50  30.00  32.00  32.25  29.00  28.00  28.00  29.50  
amg8833: temperature is 32.125C.
23.00  22.25  22.75  21.75  22.00  21.75  22.75  22.75  
23.50  22.75  23.00  23.00  24.00  23.75  23.75  22.75  
24.50  24.25  23.50  25.00  25.50  24.50  24.25  22.75  
24.75  24.25  26.50  29.25  27.00  25.00  24.00  23.75  
25.25  26.75  30.75  31.50  30.00  26.00  24.00  24.75  
24.50  26.25  31.00  32.25  31.25  29.50  26.50  25.00  
28.25  28.50  31.00  32.00  30.00  28.00  29.25  27.75  
30.50  30.25  31.75  31.75  29.25  27.75  27.50  29.50  
amg8833: temperature is 32.062C.
amg8833: finish read test.
```

```shell
./amg8833 -t int --addr=1 --times=3 --mode=ABS --high-threshold=32.0 --low-threshold=25.0 --hysteresis-level=28.0

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
amg8833: temperature is 31.438C.
amg8833: temperature is 31.500C.
amg8833: irq interrupt outbreak.
amg8833: temperature is 31.500C.
amg8833: irq interrupt outbreak.
amg8833: finish interrupt test.
```

```shell
./amg8833 -e read --addr=1 --times=3

23.25  23.00  22.75  22.50  23.00  22.50  23.50  23.25  
24.25  23.75  23.50  23.00  24.25  24.50  24.25  23.75  
25.00  24.75  24.50  25.25  26.25  25.25  24.75  23.25  
25.50  25.75  27.50  28.75  27.00  25.75  25.25  24.75  
26.50  27.75  31.75  32.00  29.25  26.50  24.50  25.75  
26.00  28.25  32.50  33.00  29.00  26.25  26.00  26.50  
29.00  29.50  32.75  32.75  28.25  27.25  27.75  30.50  
32.00  32.50  33.25  32.50  30.50  30.50  31.50  31.75  
amg8833: temperature is 30.688C.
22.75  22.50  22.75  22.50  22.00  22.00  22.75  22.75  
23.50  22.50  23.50  23.50  24.50  24.25  23.50  22.50  
24.75  24.50  24.00  25.25  25.75  24.75  24.25  22.50  
25.00  25.00  27.00  28.25  26.25  25.75  24.25  24.00  
25.50  27.00  31.25  31.75  28.50  26.25  24.50  25.00  
25.00  28.00  32.00  32.00  29.00  25.75  24.75  25.25  
27.50  28.75  32.25  32.00  27.25  26.75  27.00  29.00  
30.50  30.75  32.50  31.50  29.00  29.75  30.25  30.00  
amg8833: temperature is 30.688C.
22.75  22.75  22.50  22.00  22.50  22.00  23.00  22.50  
23.50  23.25  22.75  22.75  23.50  24.00  23.75  23.50  
24.50  24.25  23.75  24.75  25.50  24.50  24.00  23.00  
24.75  25.00  26.25  27.25  26.25  25.25  23.75  23.75  
25.00  27.00  31.00  31.50  28.00  25.75  23.50  24.00  
24.50  27.25  31.75  32.25  28.50  25.50  25.00  24.75  
27.25  28.25  32.00  31.50  27.50  26.50  26.25  28.25  
30.25  30.75  31.50  30.75  28.75  29.25  30.25  30.00  
amg8833: temperature is 30.750C.
```

```shell
./amg8833 -e int --addr=1 --times=3 --mode=ABS --high-threshold=32.0 --low-threshold=25.0 --hysteresis-level=28.0

amg8833: temperature is 30.375C.
amg8833: temperature is 30.438C.
amg8833: irq interrupt outbreak.
1  1  1  1  1  1  1  1  
1  1  1  1  1  1  1  1  
1  1  1  1  0  0  0  1  
1  1  1  1  0  0  1  1  
1  1  1  1  1  0  1  0  
0  0  1  1  0  0  0  1  
0  0  1  1  1  1  1  0  
1  1  1  1  1  1  0  0  
amg8833: temperature is 30.438C.
amg8833: irq interrupt outbreak.
1  1  1  1  1  1  1  1  
1  1  1  1  1  1  1  1  
1  1  1  1  1  0  0  1  
1  1  1  1  1  0  1  1  
1  1  1  1  1  0  1  1  
1  1  1  1  1  1  1  1  
1  1  1  1  1  1  1  0  
1  1  1  1  1  1  0  0  
```

```shell
./amg8833 -h

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
