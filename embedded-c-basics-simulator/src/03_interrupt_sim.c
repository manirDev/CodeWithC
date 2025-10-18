#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define SIMULATION 1

typedef struct
{
    volatile uint32_t STATUS;
    volatile uint32_t DATA;
}PERIPHType;

PERIPHType PERIPH = {0};

// Status bit definitions
#define PERIPH_DATA_READY (1 << 0)

typedef void (*irq_handler_t)(void);
irq_handler_t DATA_READY_ISR = NULL;

void register_irq_handler(irq_handler_t handler)
{
    DATA_READY_ISR = handler;
}

void simulate_hardware_event(uint32_t sample)
{
    PERIPH.DATA = sample;
    PERIPH.STATUS |= PERIPH_DATA_READY;

    // Call the "interrupt" if handler registered
    if (NULL != DATA_READY_ISR)
    {
        DATA_READY_ISR();
    }
}

void my_data_ready_isr(void)
{
    uint32_t data = PERIPH.DATA;
    printf("[ISR] Data received: 0x%08X\n", data);
    PERIPH.STATUS &= ~PERIPH_DATA_READY;
}

int main(void)
{
    //Register ISR
    register_irq_handler(my_data_ready_isr);

    printf("Starting interrupt simulation...\n");

    // Simulate events every second
    for (uint32_t i = 0; i < 5; i++) {
        uint32_t sample = 0x1000 + i;
        printf("[HW] Sending sample: 0x%08X\n", sample);
        simulate_hardware_event(sample);

        // Sleep to simulate time between interrupts
        sleep(1);
    }

    printf("Simulation done.\n");

    return 0;
}

