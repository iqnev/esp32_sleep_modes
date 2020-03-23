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

## ESP32 Deep Sleep
In deep sleep mode, CPUs, most of the RAM, and all the digital peripherals which are clocked from APB_CLK are powered off. The only parts of the chip which can still be powered on are: RTC controller, RTC peripherals (including ULP coprocessor), and RTC memories (slow and fast). The chip consumes around 0.15 mA to 10µA. Along with the CPU, the main memory of the chip is also disabled. So, everything stored in that memory is wiped out and cannot be accessed. If you want to use the data over reboot, store it into the RTC memory by defining a global variable with ```RTC_DATA_ATTR``` attribute. ```esp_deep_sleep_start()``` function can be used to immediately enter deep sleep once wake-up sources are configured. By default, ESP32 will automatically power down the peripherals not needed by the wake-up source. Before entering deep sleep mode, applications must disable WiFi and BT using appropriate calls (```esp_bluedroid_disable()```, ```esp_bt_controller_disable()```, ```esp_wifi_stop()```). 


