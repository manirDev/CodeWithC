#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GPIO_BASE_ADR 0x40020000UL
#define GPIO_ODR (*(volatile uint32_t*)(GPIO_BASE_ADR + 0x14))

//Simulated hardware memory (4KB)
uint32_t fake_memory[1024];

//Convert hardware address to simulated memory index
#define MEM(addr) (fake_memory[(addr - GPIO_BASE_ADR) / 4])

void write_register(uint32_t addr, uint32_t value) 
{
   MEM(addr) = value;
}

uint32_t read_register(uint32_t addr) 
{
   return MEM(addr);
}

int main() {
    printf("====== GPIO Simulation ======\n");
    write_register(GPIO_BASE_ADR + 0x14, 0x20);
    uint32_t odr = read_register(GPIO_BASE_ADR + 0x14);
    printf("GPIO ODR Register Value: 0x%08X\n", odr);

    write_register(GPIO_BASE_ADR + 0x14, odr ^ 0x20);
    odr = read_register(GPIO_BASE_ADR + 0x14);
    printf("GPIO_ODR toggled value: 0x%08X\n", odr);
    
    return 0;
}