# example-gpio
Demonstrates how to config GPIO and register a GPIO ISR.

In this example, we use sifive-hifive1-revb as our target.

Program ESP32-SOLO1 as gpio output source and sifive fe310 as gpio input destination.

1. Please follow the env setup in
   https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html

2. Copy gpio_example_main.c into esp-idf/examples/peripherals/gpio/main

The ESP32-SOLO1 will toggle gpio 13 which is gpio 3 fe310 on hifive1-revb.

The expected output:
```
irq:11                    <== PLIC interrupt number
gpio 3: 1
in GPIO RISING CONFIG
gpio 3: 1
in GPIO RISING CONFIG
gpio 3: 0
in GPIO RISING CONFIG
gpio 3: 0
in GPIO RISING CONFIG
gpio 3: 0
in GPIO RISING CONFIG
in GPIO RISING ISR        <== GPIO RISING ISR looks ok
gpio 3: 1
in GPIO FALLING CONFIG
gpio 3: 1
in GPIO FALLING CONFIG
gpio 3: 1
in GPIO FALLING CONFIG
in GPIO FALLING ISR       <== GPIO FALLING ISR looks ok
gpio 3: 0
in GPIO HIGH CONFIG
gpio 3: 0
in GPIO HIGH CONFIG
gpio 3: 0
in GPIO HIGH CONFIG
in GPIO HIGH ISR          <== GPIO LEVEL HIGH ISR looks ok
gpio 3: 1
in GPIO LOW CONFIG
gpio 3: 1
in GPIO LOW CONFIG
gpio 3: 1
in GPIO LOW CONFIG
in GPIO LOW ISR           <== GPIO LEVEL LOW ISR looks ok
gpio 3: 0
Finish all GPIO ISR tests
```
