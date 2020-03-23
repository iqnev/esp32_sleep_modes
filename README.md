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

### Save Data on RTC Memories
You can save data on the RTC memories. The ESP32 has 8kB SRAM on the RTC part, called RTC fast memory. The data saved here is not erased during deep sleep or light-sleep mode. It is erased when you press the reset button. It is not possible to do dynamic allocation in RTC memory, but you can do static (compile-time) allocation. If you define a global variable with ```RTC_DATA_ATTR``` attribute, the variable will be placed into ```RTC_SLOW_MEM memory```. Also you should be able to use ```RTC_DATA_ATTR``` with struct instances.
For example : ```RTC_DATA_ATTR int counter = 0;```

## ESP32 Deep Sleep
In deep sleep mode, CPUs, most of the RAM, and all the digital peripherals which are clocked from APB_CLK are powered off. The only parts of the chip which can still be powered on are: RTC controller, RTC peripherals (including ULP coprocessor), and RTC memories (slow and fast). The chip consumes around 0.15 mA to 10µA. Along with the CPU, the main memory of the chip is also disabled. So, everything stored in that memory is wiped out and cannot be accessed. If you want to use the data over reboot, store it into the RTC memory by defining a global variable with ```RTC_DATA_ATTR``` attribute. ```esp_deep_sleep_start()``` function can be used to immediately enter deep sleep once wake-up sources are configured. By default, ESP32 will automatically power down the peripherals not needed by the wake-up source. Before entering deep sleep mode, applications must disable WiFi and BT using appropriate calls (```esp_bluedroid_disable()```, ```esp_bt_controller_disable()```, ```esp_wifi_stop()```). 

Wake up from deep sleep and  mode can be done using several sources
