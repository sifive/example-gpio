/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <unistd.h>
#include <metal/cpu.h>
#include <metal/gpio.h>

#define GPIO_INPUT_RISING_TEST          1
#define GPIO_INPUT_FALLING_TEST         2
#define GPIO_INPUT_LEVEL_HIGH_TEST      3
#define GPIO_INPUT_LEVEL_LOW_TEST       4
#define GPIO_INPUT_END_TEST             5

struct metal_interrupt *gpio3_ic;
int gpio3_irq;
int gpio_test_status = GPIO_INPUT_RISING_TEST;

void gpio3_isr_rising(int id, void *data) {

    struct metal_gpio *gpio0;
    //Get gpio device handle
    gpio0 = metal_gpio_get_device(0);

    metal_gpio_config_interrupt(gpio0, 3, METAL_GPIO_INT_DISABLE);
    metal_gpio_clear_interrupt(gpio0, 3, METAL_GPIO_INT_RISING);
    gpio_test_status = GPIO_INPUT_FALLING_TEST;
    printf("in GPIO RISING ISR\n");
};

void gpio3_isr_falling(int id, void *data) {

    struct metal_gpio *gpio0;
    //Get gpio device handle
    gpio0 = metal_gpio_get_device(0);

    metal_gpio_config_interrupt(gpio0, 3, METAL_GPIO_INT_DISABLE);
    metal_gpio_clear_interrupt(gpio0, 3, METAL_GPIO_INT_FALLING);
    gpio_test_status = GPIO_INPUT_LEVEL_HIGH_TEST;
    printf("in GPIO FALLING ISR\n");
};

void gpio3_isr_high(int id, void *data) {

    struct metal_gpio *gpio0;
    //Get gpio device handle
    gpio0 = metal_gpio_get_device(0);

    metal_gpio_config_interrupt(gpio0, 3, METAL_GPIO_INT_DISABLE);
    metal_gpio_clear_interrupt(gpio0, 3, METAL_GPIO_INT_HIGH);
    gpio_test_status = GPIO_INPUT_LEVEL_LOW_TEST;
    printf("in GPIO HIGH ISR\n");
};

void gpio3_isr_low(int id, void *data) {

    struct metal_gpio *gpio0;
    //Get gpio device handle
    gpio0 = metal_gpio_get_device(0);

    metal_gpio_config_interrupt(gpio0, 3, METAL_GPIO_INT_DISABLE);
    metal_gpio_clear_interrupt(gpio0, 3, METAL_GPIO_INT_LOW);
    printf("in GPIO LOW ISR\n");
    gpio_test_status = GPIO_INPUT_END_TEST;
};

int main (void)
{
    int rc;
	struct metal_cpu *cpu;
	struct metal_interrupt *cpu_intr;
    struct metal_gpio *gpio0;
	unsigned long long timebase = 0;
    int value = 0;

    //Get gpio device handle
    gpio0 = metal_gpio_get_device(0);

    //set gpio 3 as input pin
    metal_gpio_enable_input(gpio0, 3);
    //set gpio 3 as gpio pin
    metal_gpio_disable_pinmux(gpio0, 3);

    gpio3_ic = metal_gpio_interrupt_controller(gpio0);
    if (gpio3_ic == NULL) {
        printf("gpio interrupt controller is null.\n");
        return 4;
    }
    metal_interrupt_init(gpio3_ic);
    //get gpio 3 plic number
    gpio3_irq = metal_gpio_get_interrupt_id(gpio0, 3);
    printf("irq:%d\n", gpio3_irq);

    /* Initialize interrupt handling on the current CPU */
	cpu = metal_cpu_get(metal_cpu_get_current_hartid());
	if(!cpu) {
		printf("Unable to get CPU handle\n");
		return 1;
	}

	cpu_intr = metal_cpu_interrupt_controller(cpu);
	if(!cpu_intr) {
		printf("Unable to get CPU Interrupt handle\n");
		return 2;
	}
	metal_interrupt_init(cpu_intr);

    while(1){
        //get timer frequency
        metal_timer_get_timebase_frequency(0, &timebase);

        for (int i = 0 ; i < timebase;  i++)
            ;
        value = metal_gpio_get_input_pin(gpio0, 3);
        printf("gpio %d: %d\n", 3, value);
        switch(gpio_test_status){
              case GPIO_INPUT_RISING_TEST:
                  printf("in GPIO RISING CONFIG\n");
                  rc = metal_interrupt_register_handler(gpio3_ic, gpio3_irq, gpio3_isr_rising, gpio0);
                  if (rc < 0) {
                      printf("gpio3 interrupt handler registration failed\n");
                      return (rc * -1);
                  }
                  metal_gpio_clear_interrupt(gpio0, 3, METAL_GPIO_INT_MAX);
                  metal_gpio_config_interrupt(gpio0, 3, METAL_GPIO_INT_RISING);
                  if (metal_interrupt_enable(gpio3_ic, gpio3_irq) == -1) {
                      printf("gpio3 interrupt enable failed\n");
                      return 5;
                  }
                  if (metal_interrupt_enable(cpu_intr, 0) == -1) {
                      printf("CPU interrupt enable failed\n");
                      return 6;
                  }
                  break;
              case GPIO_INPUT_FALLING_TEST:
                  printf("in GPIO FALLING CONFIG\n");
                  rc = metal_interrupt_register_handler(gpio3_ic, gpio3_irq, gpio3_isr_falling, gpio0);
                  if (rc < 0) {
                      printf("gpio3 interrupt handler registration failed\n");
                      return (rc * -1);
                  }
                  metal_gpio_clear_interrupt(gpio0, 3, METAL_GPIO_INT_MAX);
                  metal_gpio_config_interrupt(gpio0, 3, METAL_GPIO_INT_FALLING);
                  if (metal_interrupt_enable(gpio3_ic, gpio3_irq) == -1) {
                      printf("gpio3 interrupt enable failed\n");
                      return 5;
                  }
                  if (metal_interrupt_enable(cpu_intr, 0) == -1) {
                      printf("CPU interrupt enable failed\n");
                      return 6;
                  }
                  break;
              case GPIO_INPUT_LEVEL_HIGH_TEST:
                  printf("in GPIO HIGH CONFIG\n");
                  rc = metal_interrupt_register_handler(gpio3_ic, gpio3_irq, gpio3_isr_high, gpio0);
                  if (rc < 0) {
                      printf("gpio3 interrupt handler registration failed\n");
                      return (rc * -1);
                  }
                  metal_gpio_clear_interrupt(gpio0, 3, METAL_GPIO_INT_MAX);
                  metal_gpio_config_interrupt(gpio0, 3, METAL_GPIO_INT_HIGH);
                  if (metal_interrupt_enable(gpio3_ic, gpio3_irq) == -1) {
                      printf("gpio3 interrupt enable failed\n");
                      return 5;
                  }
                  if (metal_interrupt_enable(cpu_intr, 0) == -1) {
                      printf("CPU interrupt enable failed\n");
                      return 6;
                  }
                  break;
              case GPIO_INPUT_LEVEL_LOW_TEST:
                  printf("in GPIO LOW CONFIG\n");
                  rc = metal_interrupt_register_handler(gpio3_ic, gpio3_irq, gpio3_isr_low, gpio0);
                  if (rc < 0) {
                      printf("gpio3 interrupt handler registration failed\n");
                      return (rc * -1);
                  }
                  metal_gpio_clear_interrupt(gpio0, 3, METAL_GPIO_INT_MAX);
                  metal_gpio_config_interrupt(gpio0, 3, METAL_GPIO_INT_LOW);
                  if (metal_interrupt_enable(gpio3_ic, gpio3_irq) == -1) {
                      printf("gpio3 interrupt enable failed\n");
                      return 5;
                  }
                  if (metal_interrupt_enable(cpu_intr, 0) == -1) {
                      printf("CPU interrupt enable failed\n");
                      return 6;
                  }
                  break;
              case GPIO_INPUT_END_TEST:
                  metal_gpio_config_interrupt(gpio0, 3, METAL_GPIO_INT_DISABLE);
                  metal_gpio_clear_interrupt(gpio0, 3, METAL_GPIO_INT_LOW);
                  printf("Finish all GPIO ISR tests\n");
                  return 0;
                  break;
        }

    };

    return 0;
}

