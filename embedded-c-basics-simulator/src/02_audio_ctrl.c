#include "stdio.h"
#include "stdint.h"

// Set SIMULATION=1 to run on PC without MCU
// Set SIMULATION=0 when compiling for actual MCU
#define SIMULATION 1

// Use union + struct to map control register (32-bit)
typedef union
{
    volatile uint32_t ALL;  // full register access
    struct 
    {  
        volatile uint32_t EN       : 1; // Enable audio
        volatile uint32_t MUTE     : 1; // Mute audio
        volatile uint32_t MODE     : 2; // Audio mode (0=I2S,1=PCM,etc)
        volatile uint32_t RESERVED : 28;
    }BIT;
    
}AUDIO_CTRL_REG;

// Map the audio peripheral registers
typedef struct
{
    AUDIO_CTRL_REG CTRL;      // 0x00
    volatile uint32_t STATUS; // 0x04
    volatile uint32_t TXDATA; // 0x08
    volatile uint32_t RXDATA; // 0x0C
}AUDIOType;

#if SIMULATION
// Simulation: use normal variable instead of hardware address
AUDIOType AUDIO_SIM = {0};
#define AUDIO (&AUDIO_SIM)
#else
// MCU: memory-mapped peripheral base address
#define AUDIO_BASE_ADDR 0x40021000UL
#define AUDIO ((AUDIOType *)AUDIO_BASE_ADDR)
#endif

//----------------------
// Helper: print binary
//----------------------
static void print_binary(uint32_t value, int bits)
{
    for (int i = bits - 1; i >= 0; i--) {
        printf("%c", (value & (1 << i)) ? '1' : '0');
        if (i % 4 == 0) printf(" "); // optional: space every 4 bits
    }
    printf("\n");
}

//-------------------------
// Audio Control functions
//-------------------------

// Enable audio peripheral
static void audio_enable(void)
{
    AUDIO->CTRL.BIT.EN = 1;
#if SIMULATION
    printf("[SIM] CTRL.ALL = ");
    print_binary(AUDIO->CTRL.ALL, 32);
#endif
}

// Disable audio peripheral
static void audio_disable(void)
{
    AUDIO->CTRL.BIT.EN = 0;
#if SIMULATION
    printf("[SIM] CTRL.ALL = ");
    print_binary(AUDIO->CTRL.ALL, 32);
#endif
}

// Mute or unmute audio
static void audio_mute(uint8_t mute)
{
    AUDIO->CTRL.BIT.MUTE = (mute != 0) ? 1 : 0;
#if SIMULATION
    printf("[SIM] CTRL.ALL = ");
    print_binary(AUDIO->CTRL.ALL, 32);
#endif
}

// Set audio mode (0=I2S, 1=PCM)
static void audio_set_mode(uint8_t mode)
{
    AUDIO->CTRL.BIT.MODE = mode & 0x03; // mask 2 bits
#if SIMULATION
    printf("[SIM] CTRL.ALL = ");
    print_binary(AUDIO->CTRL.ALL, 32);
#endif
}

// Send a 32-bit audio sample
static void audio_send_sample(uint32_t sample)
{
#if SIMULATION
    //In simulation, just write smaple and set TX flag
    AUDIO->TXDATA = sample;
    AUDIO->STATUS |= (1 << 0);
    printf("[SIM] TXDATA = 0x%08X, STATUS = ", AUDIO->TXDATA);
    print_binary(AUDIO->STATUS, 32);
#else
    //wait until transmit buffer empty(status bit 0 = TXE)
    while (!(AUDIO->STATUS & (1 << 0)));
    AUDIO->TXDATA = sample;
#endif
}

// Receive a 32-bit audio sample
static uint32_t audio_receive_sample(void)
{
#if SIMULATION
    AUDIO->STATUS |= (1 << 1);
    AUDIO->RXDATA = 0xDEADBEEF;
    printf("[SIM] RXDATA ready, STATUS = ");
    print_binary(AUDIO->STATUS, 32);
    return AUDIO->RXDATA;
#else
    // wait until receive buffer is full (status bit 1 = RXF)
    while(!(AUDIO->STATUS & (1 << 1)));
    return AUDIO->RXDATA;
#endif
}

int main(void)
{
    audio_enable();
    audio_set_mode(0);
    audio_send_sample(0x12345678);

    //receive sample
    uint32_t sample = audio_receive_sample();
    printf("Receive sample : 0x%08X\n", sample);

    audio_mute(1);
    audio_disable();

    printf("ALL REG : 0x%08X\n", AUDIO->CTRL.ALL);
    print_binary(AUDIO->CTRL.ALL, 32);
    return 0;
}