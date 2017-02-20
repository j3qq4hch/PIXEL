###PIXEL###
Pixel(yet another) is a dirt cheap 10W LED constant current driver with bells and whistles. 

Two common use scenarios are:

* LED LIGHTING with digital interface(smarthome applications)
* HUGE low-res LED screens with incredible brightness
 
 The device can operate in 2 modes:
 
* As modbus slave device over RS485 non-isolated inteface. This is relative slow mode which can be used for lighting automation systems or stage light control systems.  In this mode device's clock source is external quartz oscillator and it uses UART1 for communication. 
* As shift register. In this mode device operates like three daisy-chained 595 shift registers. In this mode device's clock source is internal RC(as frequency precision is not needed) and pins PF0 PF1 are used as GPIO(for LATCH and RESET signals). More, data clock is connected to PA4, data in is PA3, data out is PA9. So, data signal appears to be buffered by MCU. Clock, latch and reset signals are buffered by external ICs, so many PIXELs can be daisy-chained without concersn about bus integrity.  

Distinguishing between these modes is mostly a matter of proper connector selection, I think.  Mode selection may be done by adding solder joint to PA3 or PA9. These pins should be sampled right after MCU reset and one of the modes should be chosen then.