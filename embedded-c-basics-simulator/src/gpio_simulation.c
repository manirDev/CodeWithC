#include "stdio.h"
#include "stdint.h"
#include "unistd.h" //for sleep

#define PIN5 (1 << 5)
typedef struct
{
    volatile uint32_t MODER; //mode reg
    volatile uint32_t ODR; //output data reg
    volatile uint32_t IDR; //input data reg
} GPIOType;

//fake gpio port instance in RAM
GPIOType GPIOA = {0};

// Configure pin as output
void gpio_config_output(GPIOType *port, uint32_t pin)
{
    port->MODER |= (1 << (pin * 2));
    printf("[CONFIG] Pin %d set as OUTPUT\n", pin);
}

// Write HIGH or LOW
void gpio_write(GPIOType *port, uint32_t pin, uint8_t state)
{
    if (state)
    {
        port->ODR |= pin;
    }
    else
    {
        port->ODR &= ~pin;
    }
    printf("[WRITE] Pin state => ODR = 0x%08X\n", port->ODR);
}

// Toggle pin
void gpio_toggle(GPIOType *port, uint32_t pin)
{
    port->ODR ^= pin;
    printf("[TOGGLE] ODR after toggle = 0x%08X\n");
}


int main()
{
    printf("=== GPIO Simulation (no hardware) ===\n");

    gpio_config_output(&GPIOA, 5);

    // Simulate blinking
    for (int i = 0; i < 5; i++) {
        gpio_toggle(&GPIOA, PIN5);
        sleep(1); // simulate delay (like delay_ms)
    }

    printf("Simulation complete.\n");
    return 0;
}
