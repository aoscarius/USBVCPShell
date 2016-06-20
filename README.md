# USBVCPShell

### Introduction
This is a simple C project for the STM32F4-Discovery board from STMicroelectronics. It implement 
a very simple shell command that run on a copy of FreeRTOS (with CMSIS-RTOS) and interact with user
through an emulated UART on USB-OTG.

### Requirements
It's written with AC6 Workshop tool (an STM32 operated custom version of Eclipse) but it work on 
any other tool that you wont with the opportune adaptions.

### Installation
Import the two project into AC6 Workshop. Remeber that the USBVCPShell project depend from halperiph_lib project.

### Use
Attach the USB-OTG on your machine and after a reset of the board, launch your favourite terminal (putty, gtkterm, screen, cu, ...). If you don't display anything press ENTER and the shell PROMPT appear.

### Todos
- Implement the concept of APP on FreeRTOS

### Contact
**Author**: A[O]scar(ius)

**Co-Author**: Iorio Raffaele
  
License
----
USBVCPShell is licensed under a [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-nc-sa/4.0/)

![Creative Commons License](https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png)
