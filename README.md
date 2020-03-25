# ESP32 Sleep modes

One of the biggest challenges within smart devices is maximizing the life of small, on-board batteries. In order to achieve longer battery life, engineers must make accurate battery drain measurements and understand the power consumption patterns. ESP32 is an excellent low-power SoC but it can be a relatively power-hungry device. I want to get an ESP32 to run for over 15 weeks on a single 1000mAh battery. To achieve this goal I am going try to explain one of the many possibilities of this SoC.

## What is ESP32 sleep mode?
ESP32 Sleep mode is a power-saving state that ESP32 can enter when not in use. With the use of advanced power-management technologies, ESP32 can switch between different power modes.

• Power modes
  > **Active mode:** The chip radio is powered on. The chip can receive, transmit, or listen.
  
  > **Modem-sleep mode:** The CPU is operational and the clock is configurable. The Wi-Fi/Bluetooth baseband and radio are disabled.
  
  > **Light-sleep mode:** The CPU is paused. The RTC memory and RTC peripherals, as well as the ULP co-processor are running.  Any wake-up events (MAC, host, RTC timer, or external interrupts) will wake up the chip.
  
  > **Deep-sleep mode:** Only the RTC memory and RTC peripherals are powered on. Wi-Fi and Bluetooth connection data are stored in the RTC memory. The ULP co-processor is functional.
  
  > **Hibernation mode:** The internal 8-MHz oscillator and ULP co-processor are disabled. The RTC recovery memory is powered down. Only one RTC timer on the slow clock and certain RTC GPIOs are active.

### Power Consumption by Power Modes
 | Power mode       | Description           | Power consumption  |
| ------------- |:-------------:| -----:|
| Active (RF working) | Wi-Fi/BT Rx and listening | 160 ~ 260mA |
|Modem-sleep |The CPU is powered on | 20 ~ 68mA |
| Light-sleep  | ---- |0.8 mA |
| Deep-sleep  |RTC timer + RTC memory |10µA |
| Hibernation  | RTC timer only |5µA |
| Power off  | CHIP_PU is set to low level, the chip is powered off |0.1µA |

Wake up from deep or lights leep modes can be done using several sources:
- Timer

- Touch pad

- External wakeup(ext0 & ext1)

### Timer Wake Up
RTC controller has a built in timer which can be used to wake up the chip after a predefined amount of time. The following function can be used to enable deep sleep wakeup using a timer ```esp_err_tesp_sleep_enable_timer_wakeup(uint64_t time_in_us)```

### Touch pad
You can wake up the ESP32 from deep sleep using the touch pins. You need to configure the touch pad interrupt before the chip starts deep sleep.   The ```touch_pad_tesp_sleep_get_touchpad_wakeup_status(void)``` function can be used to enable this wake-up source.

### External wakeup (ext0) and External wakeup (ext1)
The ext0 allows you to use a pin to wake up the chip ```esp_err_tesp_sleep_enable_ext0_wakeup(gpio_num_t gpio_num, int level)```

The ext1 allows you to use multiple RTC GPIOs ```esp_err_tesp_sleep_enable_ext1_wakeup(uint64_t mask, esp_sleep_ext1_wakeup_mode_tmode)```

### Save Data on RTC Memories
You can save data on the RTC memories. The ESP32 has 8kB SRAM on the RTC part, called RTC fast memory. The data saved here is not erased during deep sleep or light-sleep mode. It is erased when you press the reset button. It is not possible to do dynamic allocation in RTC memory, but you can do static (compile-time) allocation. If you define a global variable with ```RTC_DATA_ATTR``` attribute, the variable will be placed into ```RTC_SLOW_MEM memory```. Also you should be able to use ```RTC_DATA_ATTR``` with struct instances.
For example : ```RTC_DATA_ATTR int counter = 0;```

## ESP32 Deep Sleep
In deep sleep mode, CPUs, most of the RAM, and all the digital peripherals which are clocked from APB_CLK are powered off. The only parts of the chip which can still be powered on are: RTC controller, RTC peripherals (including ULP coprocessor), and RTC memories (slow and fast). The chip consumes around 0.15 mA to 10µA. Along with the CPU, the main memory of the chip is also disabled. So, everything stored in that memory is wiped out and cannot be accessed. If you want to use the data over reboot, store it into the RTC memory by defining a global variable with ```RTC_DATA_ATTR``` attribute. ```esp_deep_sleep_start()``` function can be used to immediately enter deep sleep once wake-up sources are configured. By default, ESP32 will automatically power down the peripherals not needed by the wake-up source. Before entering deep sleep mode, applications must disable WiFi and BT using appropriate calls (```esp_bluedroid_disable()```, ```esp_bt_controller_disable()```, ```esp_wifi_stop()```). 
You have to remember something very important!!! When the ESP32 enters deep sleep, it turns off the processor that's running your code. The contents of memory and the current state of the processor are lost. So when it restarts out of deep sleep it's as if it just powered up.
If you use Arduino as framework your ```setup()``` function will run again and will need to do any initialization again. 
Espressif IoT Development Framework is official development framework for ESP32. In this case your program will run from ```void app_main(void)```. But you have to know that ESP-IDF provides additional opportunities- ESP32 supports running a “deep sleep wake stub” when coming out of deep sleep. This function runs immediately as soon as the chip wakes up - before any normal initialisation, bootloader, or ESP-IDF code has run. 

## How to use
This example should be able to run on any commonly available ESP32 development board without any extra hardware.
1. Firstly, i declare my ```counter``` which counts each program restart.
2. I check if my program runs after sleep mode or not, i use the ```esp_sleep_get_wakeup_cause()``` function.
3. I define the wake up source by using the following function ```esp_sleep_enable_timer_wakeup(wakeup_time_sec * 1000000);```
4. The esp SoC goes to sleep by calling the following function: ``` esp_deep_sleep_start()```
You can look the whole my code within the following [file...](https://github.com/iqnev/esp32_sleep_modes/blob/master/deep_sleep_demo_main.c)

Every time the ESP wakes up the ```counter``` variable increases. It also prints the wake up reason as shown in the figure below:
```
rst:0x1 (POWERON_��UQ�,boot:0x13 (SPI_FAST_F*M!}        OOT)
configsip: 0, SPR␕���5
clk_drv:0x00,q_drv'��0,d_drv:0x00,cs0_drv:0x�bB�}drv:0x00,wp_drv:0x00
m���DIO, clock div:2
load:��fff0018,len:4
load:0x3ff␖��␚�len:6284
load:0x40078��bb�n:10112
load:0x400804�bb��:6492
entry 0x4008075j
␛[0;32mI (28) boot: ESP-IDF 3.30300.190916 2nd stage bootloader␛[0m
␛[0;32mI (28) boot: compile time 20:04:59␛[0m
␛[0;32mI (29) boot: Enabling RNG early entropy source...␛[0m
␛[0;32mI (34) boot: SPI Speed      : 40MHz␛[0m
␛[0;32mI (38) boot: SPI Mode       : DIO␛[0m
␛[0;32mI (42) boot: SPI Flash Size : 4MB␛[0m
␛[0;32mI (46) boot: Partition Table:␛[0m
␛[0;32mI (49) boot: ## Label            Usage          Type ST Offset   Length␛[0m
␛[0;32mI (57) boot:  0 nvs              WiFi data        01 02 00009000 00006000␛[0m
␛[0;32mI (64) boot:  1 phy_init         RF data          01 01 0000f000 00001000␛[0m
␛[0;32mI (72) boot:  2 factory          factory app      00 00 00010000 00100000␛[0m
␛[0;32mI (79) boot: End of partition table␛[0m
␛[0;32mI (83) esp_image: segment 0: paddr=0x00010020 vaddr=0x3f400020 size=0x07d3c ( 32060) map␛[0m
␛[0;32mI (103) esp_image: segment 1: paddr=0x00017d64 vaddr=0x3ffbdb60 size=0x01ef4 (  7924) load␛[0m
␛[0;32mI (106) esp_image: segment 2: paddr=0x00019c60 vaddr=0x40080000 size=0x00400 (  1024) load␛[0m
␛[0;32mI (110) esp_image: segment 3: paddr=0x0001a068 vaddr=0x40080400 size=0x05fa8 ( 24488) load␛[0m
␛[0;32mI (128) esp_image: segment 4: paddr=0x00020018 vaddr=0x400d0018 size=0x14450 ( 83024) map␛[0m
␛[0;32mI (157) esp_image: segment 5: paddr=0x00034470 vaddr=0x400863a8 size=0x01f18 (  7960) load␛[0m
␛[0;32mI (160) esp_image: segment 6: paddr=0x00036390 vaddr=0x400c0000 size=0x00064 (   100) load␛[0m
␛[0;32mI (163) esp_image: segment 7: paddr=0x000363fc vaddr=0x50000000 size=0x00004 (     4) load␛[0m
␛[0;32mI (177) boot: Loaded app from partition at offset 0x10000␛[0m
␛[0;32mI (178) boot: Disabling RNG early entropy source...␛[0m
␛[0;32mI (184) cpu_start: Pro cpu up.␛[0m
␛[0;32mI (188) cpu_start: Application information:␛[0m
␛[0;32mI (192) cpu_start: Project name:     DeepSleep␛[0m
␛[0;32mI (198) cpu_start: App version:      1.0.0␛[0m
␛[0;32mI (202) cpu_start: Compile time:     Mar 25 2020 20:05:10␛[0m
␛[0;32mI (208) cpu_start: ELF file SHA256:  0000000000000000...␛[0m
␛[0;32mI (214) cpu_start: ESP-IDF:          3.30300.190916␛[0m
␛[0;32mI (220) cpu_start: Starting app cpu, entry point is 0x40080fd4␛[0m
␛[0;32mI (0) cpu_start: App cpu up.␛[0m
␛[0;32mI (230) heap_init: Initializing. RAM available for dynamic allocation:␛[0m
␛[0;32mI (237) heap_init: At 3FFAE6E0 len 0000F480 (61 KiB): DRAM␛[0m
␛[0;32mI (243) heap_init: At 3FFC0AA8 len 0001F558 (125 KiB): DRAM␛[0m
␛[0;32mI (250) heap_init: At 3FFE0440 len 00003AE0 (14 KiB): D/IRAM␛[0m
␛[0;32mI (256) heap_init: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM␛[0m
␛[0;32mI (262) heap_init: At 400882C0 len 00017D40 (95 KiB): IRAM␛[0m
␛[0;32mI (269) cpu_start: Pro cpu start user code␛[0m
␛[0;32mI (287) cpu_start: Chip Revision: 1␛[0m
␛[0;33mW (287) cpu_start: Chip revision is higher than the one configured in menuconfig. Suggest to upgrade it.␛[0m
␛[0;32mI (291) cpu_start: Starting scheduler on PRO CPU.␛[0m
␛[0;32mI (0) cpu_start: Starting scheduler on APP CPU.␛[0m
Not a deep sleep reset
Enabling timer wakeup, 20s
Entering deep sleep
ets Jun  8 2016 00:22:57

rst:0x5 (DEEPSLEEP_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:2
load:0x3fff0018,len:4
load:0x3fff001c,len:6284
load:0x40078000,len:10112
load:0x40080400,len:6492
entry 0x40080750
␛[0;32mI (29) boot: ESP-IDF 3.30300.190916 2nd stage bootloader␛[0m
␛[0;32mI (29) boot: compile time 20:04:59␛[0m
␛[0;32mI (29) boot: Enabling RNG early entropy source...␛[0m
␛[0;32mI (35) boot: SPI Speed      : 40MHz␛[0m
␛[0;32mI (39) boot: SPI Mode       : DIO␛[0m
␛[0;32mI (43) boot: SPI Flash Size : 4MB␛[0m
␛[0;32mI (47) boot: Partition Table:␛[0m
␛[0;32mI (50) boot: ## Label            Usage          Type ST Offset   Length␛[0m
␛[0;32mI (58) boot:  0 nvs              WiFi data        01 02 00009000 00006000␛[0m
␛[0;32mI (65) boot:  1 phy_init         RF data          01 01 0000f000 00001000␛[0m
␛[0;32mI (73) boot:  2 factory          factory app      00 00 00010000 00100000␛[0m
␛[0;32mI (80) boot: End of partition table␛[0m
␛[0;32mI (84) esp_image: segment 0: paddr=0x00010020 vaddr=0x3f400020 size=0x07d3c ( 32060) map␛[0m
␛[0;32mI (104) esp_image: segment 1: paddr=0x00017d64 vaddr=0x3ffbdb60 size=0x01ef4 (  7924) load␛[0m
␛[0;32mI (107) esp_image: segment 2: paddr=0x00019c60 vaddr=0x40080000 size=0x00400 (  1024) load␛[0m
␛[0;32mI (111) esp_image: segment 3: paddr=0x0001a068 vaddr=0x40080400 size=0x05fa8 ( 24488) load␛[0m
␛[0;32mI (129) esp_image: segment 4: paddr=0x00020018 vaddr=0x400d0018 size=0x14450 ( 83024) map␛[0m
␛[0;32mI (158) esp_image: segment 5: paddr=0x00034470 vaddr=0x400863a8 size=0x01f18 (  7960) load␛[0m
␛[0;32mI (161) esp_image: segment 6: paddr=0x00036390 vaddr=0x400c0000 size=0x00064 (   100) ␛[0m
␛[0;32mI (164) esp_image: segment 7: paddr=0x000363fc vaddr=0x50000000 size=0x00004 (     4) ␛[0m
␛[0;32mI (178) boot: Loaded app from partition at offset 0x10000␛[0m
␛[0;32mI (179) boot: Disabling RNG early entropy source...␛[0m
␛[0;32mI (184) cpu_start: Pro cpu up.␛[0m
␛[0;32mI (188) cpu_start: Application information:␛[0m
␛[0;32mI (193) cpu_start: Project name:     DeepSleep␛[0m
␛[0;32mI (198) cpu_start: App version:      1.0.0␛[0m
␛[0;32mI (203) cpu_start: Compile time:     Mar 25 2020 20:05:10␛[0m
␛[0;32mI (209) cpu_start: ELF file SHA256:  0000000000000000...␛[0m
␛[0;32mI (215) cpu_start: ESP-IDF:          3.30300.190916␛[0m
␛[0;32mI (220) cpu_start: Starting app cpu, entry point is 0x40080fd4␛[0m
␛[0;32mI (0) cpu_start: App cpu up.␛[0m
␛[0;32mI (231) heap_init: Initializing. RAM available for dynamic allocation:␛[0m
␛[0;32mI (238) heap_init: At 3FFAE6E0 len 0000F480 (61 KiB): DRAM␛[0m
␛[0;32mI (244) heap_init: At 3FFC0AA8 len 0001F558 (125 KiB): DRAM␛[0m
␛[0;32mI (250) heap_init: At 3FFE0440 len 00003AE0 (14 KiB): D/IRAM␛[0m
␛[0;32mI (256) heap_init: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM␛[0m
␛[0;32mI (263) heap_init: At 400882C0 len 00017D40 (95 KiB): IRAM␛[0m
␛[0;32mI (269) cpu_start: Pro cpu start user code␛[0m
␛[0;32mI (287) cpu_start: Chip Revision: 1␛[0m
␛[0;33mW (288) cpu_start: Chip revision is higher than the one configured in menuconfig. Suggest to upgrade it.␛[0m
␛[0;32mI (291) cpu_start: Starting scheduler on PRO CPU.␛[0m
␛[0;32mI (0) cpu_start: Starting scheduler on APP CPU.␛[0m
Wake up from timer. Time spent in deep sleep: 1500
Wake up from timer. The value of counter is: 1
Enabling timer wakeup, 20s
Entering deep sleep
ets Jun  8 2016 00:22:57
```

