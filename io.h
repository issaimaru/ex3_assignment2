
#if !defined(_IO_H_)
#define _IO_H_

#if !defined(NATIVE_MODE)

#include "../rv32_io_setting.h"
#define USE_UART

//extern int UART_CHAN_ID;

/**************** io address map ****************/
/// uart_chan_0 : 0x10000000 - 0x10000010 (16 bytes)
/// uart_char_1 : 0x10000040 - 0x10000050 (16 bytes)
/// gpio		: 0x10000080 - 0x10000090 (16 bytes)
/// sysctrl     : 0x10001000 - 0x10001030 (24 bytes)
#define UART_0_ADDR		0x10000000
#define UART_1_ADDR		0x10000040
#define GPIO_ADDR		0x10000080
#define SYSCTRL_ADDR	0x10001000

////////////// memory-mapped IOs //////////////
typedef volatile struct ST_UART {	///	0x10000000, 0x10000040
	char data;				///	0x10000000, 0x10000040 : 8-bit data (read/write)
							/// 3-byte padding due to data alignment
	unsigned ctrl;			///	0x10000004, 0x10000044 : 32-bit data (read/write)
} UART;
extern UART* _uart[2];

typedef volatile struct ST_GPIO {	///	0x10000080
	unsigned int data;			///	input [15:0], output[31:0]
	unsigned int dout7SEG[2];	///	output [63:0]
	unsigned short doutLED;		///	output [15:0]
} GPIO;
extern GPIO* gpio;

typedef volatile struct ST_SYSCTRL {	///	0x10001000
	unsigned int irq_ctrl;				///	0x10001000 : 32-bit data (read/write)
	unsigned int pad0;					/// 0x10001004 : 4-byte padding
	unsigned int irq_enable;			///	0x10001008 : 32-bit data (read/write)
	unsigned int pad1;					///	0x1000100c : 4-byte padding
	unsigned long long mtime;			///	0x10001010 : 64-bit data (read/write) 
	unsigned long long mtimecmp;		///	0x10001020 : 64-bit data (read/write)
} SYSCTRL;
extern SYSCTRL* sysctrl;

/*** SYSCTRL : mtime, interrupt controller
0x10001000 (irq_ctrl):
R : data = (ext_irq_enable & ext_irq_pending);
W : ext_irq_pending &= ~data; // turn-off ext_irq_pending bits
0x10001008 (ext_irq_enable) : R/W
0x10001010 (mtime : 8-bytes) : R-only
0x10001020 (mtimecmp : 8-bytes) : R/W

SYSCTRL FSM:
ext_irq_pending |= intr; (each cycle)
if (divider == 0) { increment mtime; }
divider = (divider + 1) & 0xf;
***/

#define MAX_UART_BUF_SIZE	40//100
typedef volatile struct ST_UARTBuf
{
    char not_empty, full;
    int rp, wp;
	int num_new_lines;
	int terminate_code;
    char buf[MAX_UART_BUF_SIZE];
} UARTBuf;

/// ((rx_error << 4) | (rx_empty << 3) | (rx_full << 2) | (tx_empty << 1) | (tx_full))
#define UART_TX_FULL(uart)		((uart->ctrl >> 0) & 1)
#define UART_TX_EMPTY(uart)		((uart->ctrl >> 1) & 1)
#define UART_RX_FULL(uart)		((uart->ctrl >> 2) & 1)
#define UART_RX_EMPTY(uart) 	((uart->ctrl >> 3) & 1)
#define UART_BUSY(uart) 		((uart->ctrl >> 4) & 1)

extern void set_uart_ID(int uidx);

typedef unsigned long long U64;

void set_gpio_error(unsigned short err_num);

extern void enter_critical_section();
extern void leave_critical_section();

extern UARTBuf UART_BUF_RX[2], UART_BUF_TX[2];

#define TERMINATE_CODE  0x1a

extern void (*io_putch)(char);	///	initialized to _io_putch
extern char (*io_getch)(void);	///	initialized to _io_getch
extern void _io_putch(char ch);
extern void _UARTBuf_putch(char ch);
extern char _io_getch(void);
extern char _UARTBuf_getch(void);

extern void putline(const char *s);

extern void UARTBuf_reset(UARTBuf* ub);
extern void UARTBuf_write(UARTBuf* ub, char ch);
extern char UARTBuf_read(UARTBuf* ub);
extern char UARTBuf_read_last(UARTBuf* ub);
extern int UARTBuf_buffered_size(UARTBuf* ub);
void UARTBuf_consume_new_line(UARTBuf* ub);
extern void enable_UART_interrupt();
extern void enable_BUTTON_interrupt();

extern int mtimer_interval;
extern void set_mtimer_interval(int msec);
extern void enable_timer_interrupt();
extern void (*timer_interrupt_hook)(void);

extern void _mhandler(void);

extern void UARTintr(int uart_vec);	///	called from interrupt handler
extern void flush_UART();

typedef struct ST_Profiler {
	unsigned int cycle;
	unsigned int time;
	unsigned int instret;
} Profiler;
extern Profiler gprof;
extern void start_profiler();
extern void end_profiler();
#else
#define start_profiler()
#define end_profiler()
#endif

#if !defined(NATIVE_MODE)
#define START_PROFILER	\
asm volatile("rdcycle %[old]" : [old] "=&r" (gprof.cycle) : );		\
asm volatile("rdinstret %[old]" : [old] "=&r" (gprof.instret) : );	\
asm volatile("rdtime %[old]" : [old] "=&r" (gprof.time) : );

#define END_PROFILER
#else
#define START_PROFILER
#define END_PROFILER
#endif

#if defined(TEST_WFI_BREAK) && !defined(NATIVE_MODE)
#define WFI_BREAK   	asm volatile("wfi")	///	wfi
#else
#define WFI_BREAK   
#endif

#endif	/// !defined(_IO_H_)

