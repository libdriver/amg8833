### 1. chip

#### 1.1 chip info

chip name : Raspberry Pi 4B

iic pin: SCL/SDA GPIO3/GPIO2

int pin: INT GPIO17

### 2. install

#### 2.1 install info

```shell
sudo apt-get install libgpiod-dev

make
```


### 3. amg8833

#### 3.1 command Instruction

​          amg8833 is a basic command which can test all amg8833 driver function:

​          -i         show amg8833 chip and driver information.

​          -h        show amg8833 help.

​          -p        show amg8833 pin connections of the current board.

​          -t (reg (0 | 1) | read (0 | 1) <times> | int (0 | 1) <times> <mode> <high> <low> <hysteresis>) 

​          -t reg (0 | 1)         run amg8833  register test.

​          -t read  (0 | 1)  <times>        run amg8833 read test. times means test times.         

​          -t int  (0 | 1)  <times> <mode> <high> <low> <hysteresis>        run amg8833 interrupt test. times means test times. mode is the interrupt mode and it can be "abs" and "diff". high is the interrupt high level.   low is the interrupt low level. hysteresis is the hysteresis level.   

​           -c (read (0 | 1) <times> | int (0 | 1) <times> <mode> <high> <low> <hysteresis>) 

​           -c read  (0 | 1)  <times>        run amg8833 read function. times means test times.  

​           -c int  (0 | 1)  <times> <mode> <high> <low> <hysteresis>        run amg8833 interrupt function. times means test times. mode is the interrupt mode and it can be "abs" and "diff". high is the interrupt high level.   low is the interrupt low level. hysteresis is the hysteresis level.  

#### 3.2 command example

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
./amg8833 -t reg 1

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
./amg8833 -t read 1 3

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
24.25  24.25  24.25  24.00  24.00  24.00  24.50  26.50  
23.75  23.75  24.25  24.75  24.25  24.50  24.00  25.75  
24.50  24.25  25.75  25.00  25.00  24.50  24.75  24.75  
25.50  25.50  24.75  25.50  25.00  25.25  25.50  29.25  
25.00  25.50  25.00  25.25  25.75  25.75  27.00  30.00  
25.25  25.50  24.75  25.50  25.50  25.25  26.75  29.50  
26.00  25.25  25.75  25.25  25.50  25.75  28.25  29.50  
28.25  27.25  27.00  26.75  27.00  27.00  28.75  29.25  
amg8833: temperature is 28.188C.
23.25  23.50  23.75  23.50  24.25  23.50  23.75  25.00  
22.50  23.50  23.50  24.25  24.00  23.75  23.00  24.75  
24.00  24.00  24.25  24.25  24.00  24.00  23.75  24.50  
24.25  24.50  23.75  24.25  24.50  24.75  24.75  27.75  
23.75  24.25  24.25  24.50  24.75  24.50  25.75  28.75  
23.75  23.75  24.25  24.50  24.75  24.50  26.00  28.50  
24.25  24.25  24.75  24.25  24.25  24.25  26.75  28.75  
26.25  25.25  25.75  25.75  25.50  24.25  27.25  27.00  
amg8833: temperature is 28.250C.
23.25  23.75  23.75  23.75  24.00  24.00  23.75  25.25  
23.25  23.00  23.50  23.75  24.00  23.75  23.75  24.50  
23.75  23.75  24.75  24.50  23.75  24.25  23.50  24.25  
24.00  24.50  23.75  24.25  24.25  24.50  24.25  27.25  
23.25  24.25  24.25  24.25  24.50  24.75  25.00  28.50  
23.25  23.50  24.00  24.25  24.25  24.00  25.75  28.50  
24.00  23.75  24.25  24.25  24.25  24.00  26.00  28.50  
26.00  25.50  25.25  25.50  25.25  24.75  26.50  26.75  
amg8833: temperature is 28.312C.
amg8833: finish read test.
```

```shell
./amg8833 -t int 1 3 abs 32.0 25.0 28.0

amg8833: absolute mode.
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
amg8833: temperature is 27.812C.
amg8833: temperature is 27.938C.
amg8833: irq interrupt outbreak.
amg8833: temperature is 27.938C.
amg8833: irq interrupt outbreak.
amg8833: finish interrupt test.
```

```shell
./amg8833 -c read 1 3

30.25  32.00  29.75  26.75  28.00  26.75  26.75  26.75  
28.00  29.75  29.75  30.00  28.25  28.25  27.50  26.75  
27.00  29.00  30.25  30.75  29.75  28.25  27.75  28.00  
27.00  27.75  29.25  30.50  30.25  28.50  28.00  28.00  
27.50  26.50  27.25  29.25  28.75  28.00  27.25  28.00  
27.75  27.25  26.75  27.25  27.50  27.00  27.25  28.75  
27.75  27.00  27.50  26.75  27.00  26.75  26.75  28.50  
27.75  26.75  27.25  27.00  27.25  27.00  27.25  27.75  
amg8833: temperature is 29.125C.
29.25  31.75  29.00  26.75  27.00  26.75  26.25  25.75  
26.50  29.50  28.75  29.00  27.50  27.50  26.00  25.50  
25.50  27.75  29.25  29.50  29.00  27.00  27.00  26.75  
26.00  26.75  28.50  29.00  29.25  27.25  26.75  26.75  
26.75  25.25  26.25  28.50  27.75  27.00  26.25  27.00  
26.25  26.50  25.50  26.25  26.50  26.25  26.25  27.50  
26.00  26.00  25.75  25.25  25.75  25.75  24.75  26.25  
25.00  25.50  25.25  26.00  25.75  25.25  25.75  25.75  
amg8833: temperature is 29.125C.
29.50  31.50  28.75  26.50  26.75  26.50  26.25  25.50  
27.00  29.25  28.50  28.75  27.75  27.50  26.25  25.75  
25.75  27.50  29.25  29.75  28.75  27.00  26.50  27.00  
25.50  26.75  28.25  29.25  29.50  27.00  27.00  26.75  
26.25  25.25  26.00  28.50  27.75  26.50  26.25  27.00  
26.00  26.00  25.50  25.75  26.00  26.00  25.50  27.75  
25.75  25.25  25.50  25.25  25.50  25.50  25.00  27.00  
24.75  25.00  25.50  25.25  25.25  24.75  25.50  26.00  
amg8833: temperature is 29.188C.
```

```shell
./amg8833 -c int 1 3 abs 32.0 25.0 28.0

amg8833: absolute mode.
amg8833: temperature is 28.562C.
amg8833: temperature is 28.625C.
amg8833: irq interrupt outbreak.
0  0  0  0  0  1  0  0  
0  0  0  0  0  0  0  0  
0  0  0  0  0  0  0  0  
0  0  0  0  0  0  0  0  
1  0  1  0  0  0  0  0  
0  0  0  0  0  0  0  0  
0  0  0  0  0  0  0  0  
0  0  0  0  0  0  0  0  
amg8833: temperature is 28.688C.
amg8833: irq interrupt outbreak.
0  0  0  0  0  1  0  1  
0  0  0  0  0  0  0  1  
0  0  0  0  0  0  0  0  
0  0  0  0  0  0  0  0  
1  0  1  0  0  0  0  0  
0  0  0  0  0  0  0  0  
0  0  0  1  0  0  1  0  
0  0  0  0  0  0  0  0
```

```shell
./amg8833 -h

amg8833 -i
	show amg8833 chip and driver information.
amg8833 -h
	show amg8833 help.
amg8833 -p
	show amg8833 pin connections of the current board.
amg8833 -t reg (0 | 1)
	run amg8833 register test.
amg8833 -t read (0 | 1) <times>
	run amg8833 read test.times means test times.
amg8833 -t int (0 | 1) <times> <mode> <high> <low> <hysteresis>
	run amg8833 interrupt test.times means test times.mode is the interrupt mode and it can be "abs" and "diff".high is the interrupt high level.low is the interrupt low level.hysteresis is the hysteresis level.
amg8833 -c read (0 | 1) <times>
	run amg8833 read function.times means test times.
amg8833 -c int (0 | 1) <times> <mode> <high> <low> <hysteresis>
	run amg8833 interrupt function.times means test times.mode is the interrupt mode and it can be "abs" and "diff".high is the interrupt high level.low is the interrupt low level.hysteresis is the hysteresis level.
```
