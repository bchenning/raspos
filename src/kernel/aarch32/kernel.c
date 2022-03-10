#include <stddef.h>
#include <stdint.h>
 
static uint32_t MMIO_BASE;

static inline int get_board_part_num() {
    uint32_t reg;
    int part_num;

    asm volatile ("mrc p15,0,%0,c0,c0,0" : "=r" (reg));

    return ((reg >> 4) & 0xfff);
}
 
// The MMIO area base address, depends on board type
static inline void mmio_init(int raspi) {
    switch (raspi) {
        case 2:
        case 3:  MMIO_BASE = 0x3f000000; break; // for raspi2 & 3
        case 4:  MMIO_BASE = 0xfe000000; break; // for raspi4
        default: MMIO_BASE = 0x20000000; break;     // for raspi1, raspi zero etc.
    }
}

// Memory-Mapped I/O output
static inline void mmio_write(uint32_t reg, uint32_t data) {
    *(volatile uint32_t*)(MMIO_BASE + reg) = data;
}
 
// Memory-Mapped I/O input
static inline uint32_t mmio_read(uint32_t reg) {
    return *(volatile uint32_t*)(MMIO_BASE + reg);
}
 
// Loop <delay> times in a way that the compiler won't optimize away
static inline void delay(int32_t count) {
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
                 : "=r"(count): [count]"0"(count) : "cc");
}
 
enum {
    // The offsets for reach register.

    // base address for GPIO
    GPIO_BASE = 0x200000,
    GPFSEL0   = (GPIO_BASE + 0x00), // GPIO Function Select Register 0
    GPFSEL1   = (GPIO_BASE + 0x04), // GPIO Function Select Register 1
    GPFSEL2   = (GPIO_BASE + 0x08), // GPIO Function Select Register 2
    GPFSEL3   = (GPIO_BASE + 0x0c), // GPIO Function Select Register 3
    GPFSEL4   = (GPIO_BASE + 0x10), // GPIO Function Select Register 4
    GPFSEL5   = (GPIO_BASE + 0x14), // GPIO Function Select Register 5
    GPSET0    = (GPIO_BASE + 0x1c), // GPIO Pin Output Set Register 0
    GPSET1    = (GPIO_BASE + 0x20), // GPIO Pin Output Set Register 1
    GPCLR0    = (GPIO_BASE + 0x28), // GPIO Pin Output Clear Register 0 
    GPCLR1    = (GPIO_BASE + 0x2c), // GPIO Pin Output Clear Register 1 
    GPLEV0    = (GPIO_BASE + 0x34), // GPIO Pin Level Register 0 
    GPLEV1    = (GPIO_BASE + 0x38), // GPIO Pin Level Register 1 
    GPEDS0    = (GPIO_BASE + 0x40), // GPIO Event Detect Status Register 0 
    GPEDS1    = (GPIO_BASE + 0x44), // GPIO Event Detect Status Register 1 
    GPREN0    = (GPIO_BASE + 0x4c), // GPIO Rising Edge Detect Enable Register 0 
    GPREN1    = (GPIO_BASE + 0x50), // GPIO Rising Edge Detect Enable Register 1 
    GPFEN0    = (GPIO_BASE + 0x58), // GPIO Falling Edge Detect Enable Register 0 
    GPFEN1    = (GPIO_BASE + 0x5c), // GPIO Falling Edge Detect Enable Register 1 
    GPHEN0    = (GPIO_BASE + 0x64), // GPIO High Detect Enable Register 0 
    GPHEN1    = (GPIO_BASE + 0x68), // GPIO High Detect Enable Register 1 
    GPLEN0    = (GPIO_BASE + 0x70), // GPIO Low Detect Enable Register 0 
    GPLEN1    = (GPIO_BASE + 0x74), // GPIO Low Detect Enable Register 1 
    GPAREN0   = (GPIO_BASE + 0x7c), // GPIO Async. rising Edge Detect Enable Register 0 
    GPAREN1   = (GPIO_BASE + 0x80), // GPIO Async. rising Edge Detect Enable Register 1 
    GPAFEN0   = (GPIO_BASE + 0x88), // GPIO Async. falling Edge Detect Enable Register 0
    GPAFEN1   = (GPIO_BASE + 0x8c), // GPIO Async. falling Edge Detect Enable Register 1
    GPPUD     = (GPIO_BASE + 0x94), // GPIO Pin Pull-up/down Enable Register
    GPPUDCLK0 = (GPIO_BASE + 0x98), // GPIO Pull-up/down Clock Register 0
    GPPUDCLK1 = (GPIO_BASE + 0x9c), // GPIO Pull-up/down Clock Register 1
    GPTEST    = (GPIO_BASE + 0xb0), // GPIO Test

    // base address and offsets for UART0.
    UART0_BASE   = 0x201000,
    UART0_DR     = (UART0_BASE + 0x00),
    UART0_RSRECR = (UART0_BASE + 0x04),
    UART0_FR     = (UART0_BASE + 0x18),
    UART0_ILPR   = (UART0_BASE + 0x20),
    UART0_IBRD   = (UART0_BASE + 0x24),
    UART0_FBRD   = (UART0_BASE + 0x28),
    UART0_LCRH   = (UART0_BASE + 0x2C),
    UART0_CR     = (UART0_BASE + 0x30),
    UART0_IFLS   = (UART0_BASE + 0x34),
    UART0_IMSC   = (UART0_BASE + 0x38),
    UART0_RIS    = (UART0_BASE + 0x3C),
    UART0_MIS    = (UART0_BASE + 0x40),
    UART0_ICR    = (UART0_BASE + 0x44),
    UART0_DMACR  = (UART0_BASE + 0x48),
    UART0_ITCR   = (UART0_BASE + 0x80),
    UART0_ITIP   = (UART0_BASE + 0x84),
    UART0_ITOP   = (UART0_BASE + 0x88),
    UART0_TDR    = (UART0_BASE + 0x8C),
    
    // base address and offsets for Mailbox registers
    MBOX_BASE    = 0xB880,
    MBOX_READ    = (MBOX_BASE + 0x00),
    MBOX_STATUS  = (MBOX_BASE + 0x18),
    MBOX_WRITE   = (MBOX_BASE + 0x20),
};
 
// A Mailbox message with set clock rate of PL011 to 3MHz tag
volatile unsigned int  __attribute__((aligned(16))) mbox[9] = {
    9*4, 0, 0x38002, 12, 8, 2, 3000000, 0 ,0
};
 
void uart_init(int raspi) {
    mmio_init(raspi);
    
    // Disable UART0.
    mmio_write(UART0_CR, 0x00000000);
    // Setup the GPIO pin 14 && 15.

    // Disable pull up/down for all GPIO pins & delay for 150 cycles.
    mmio_write(GPPUD, 0x00000000);
    delay(150);
    
    // Disable pull up/down for pin 14,15 & delay for 150 cycles.
    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);

    // Write 0 to GPPUDCLK0 to make it take effect.
    mmio_write(GPPUDCLK0, 0x00000000);

    // Clear pending interrupts.
    mmio_write(UART0_ICR, 0x7FF);

    // Set integer & fractional part of baud rate.
    // Divider = UART_CLOCK/(16 * Baud)
    // Fraction part register = (Fractional part * 64) + 0.5
    // Baud = 115200.

    // For Raspi3 and 4 the UART_CLOCK is system-clock dependent by default.
    // Set it to 3Mhz so that we can consistently set the baud rate
    if (raspi >= 3) {
        // UART_CLOCK = 30000000;
        unsigned int r = (((unsigned int)(&mbox) & ~0xF) | 8);
        // wait until we can talk to the VC
        while ( mmio_read(MBOX_STATUS) & 0x80000000 ) { }
        // send our message to property channel and wait for the response
        mmio_write(MBOX_WRITE, r);
        while ( (mmio_read(MBOX_STATUS) & 0x40000000) || mmio_read(MBOX_READ) != r );
    }

    // Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
    mmio_write(UART0_IBRD, 1);
    // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
    mmio_write(UART0_FBRD, 40);
    
    // Enable FIFO & 8 bit data transmission (1 stop bit, no parity).
    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

    // Mask all interrupts.
    mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
                           (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

    // Enable UART0, receive & transfer part of UART.
    mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}
 
void uart_putc(unsigned char c)
{
    // Wait for UART to become ready to transmit.
    while ( mmio_read(UART0_FR) & (1 << 5) );
    mmio_write(UART0_DR, c);
}
 
unsigned char uart_getc()
{
    // Wait for UART to have received something.
    while ( mmio_read(UART0_FR) & (1 << 4) );
    return mmio_read(UART0_DR);
}
 
void uart_puts(const char* str)
{
    for (size_t i = 0; str[i] != '\0'; i ++)
        uart_putc((unsigned char)str[i]);
}
 
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
 
// arguments for AArch32
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    int raspi;
    int board_part_num = get_board_part_num();
    switch (board_part_num) {
        case 0xb76: raspi = 1; break;
        case 0xc07: raspi = 2; break;
        case 0xd03: raspi = 3; break;
        case 0xd08: raspi = 4; break;
        default:    raspi = 0; break;
    }

    uart_init(raspi);
    uart_puts("Hello, kernel World!\r\n");

    while (1)
        uart_putc(uart_getc());
}
