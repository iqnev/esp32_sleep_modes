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
  
