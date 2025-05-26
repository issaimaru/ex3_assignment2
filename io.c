#include "io.h"	///	uart defined

SYSCTRL* sysctrl = (SYSCTRL*) SYSCTRL_ADDR;
GPIO* gpio = (GPIO* ) GPIO_ADDR;
UART* _uart[2] = { (UART*)UART_0_ADDR, (UART*)UART_1_ADDR };

static int UART_CHAN_ID = 0;

UARTBuf UART_BUF_RX[2], UART_BUF_TX[2];

void __attribute__((constructor)) _io_init(void) {
	//(activate) : baud_period(2), bit_count(8), stop_count(1), parity_type(3)
	_uart[0]->ctrl = 1 | UART_PARAM;
	_uart[1]->ctrl = 1 | UART_PARAM;
}

void __attribute__((destructor)) _io_fini(void) {
	flush_UART();
}

void set_uart_ID(int uidx) {
    UART_CHAN_ID = (uidx != 0);
}

void _io_putch(char ch) {
    UART* uart = _uart[UART_CHAN_ID];
	while (UART_TX_FULL(uart)) {

	} /// while (tx_full == 1);
	uart->data = ch;
}

void _UARTBuf_putch(char ch) {
    UARTBuf* ub_tx = &UART_BUF_TX[UART_CHAN_ID];
    while (ub_tx->full) {} /// wait until (ub_tx->full == 0)
    UART* uart = _uart[UART_CHAN_ID];
    enter_critical_section();
    UARTBuf_write(ub_tx, ch);
    if (!UART_TX_FULL(uart)) { /// if (tx_full == 0);
        uart->data = UARTBuf_read(ub_tx);
    }
    leave_critical_section();
}

char _io_getch(void) {
    UART* uart = _uart[UART_CHAN_ID];
    if (UART_RX_EMPTY(uart)) {
        return ' ';
    } /// while (rx_empty == 1);
    return uart->data;
}

char _UARTBuf_getch(void) {
    UARTBuf* ub_rx = &UART_BUF_RX[UART_CHAN_ID];
    while (!ub_rx->not_empty) {} /// wait until (ub_rx->not_empty == 0)
    enter_critical_section();
    char ch = UARTBuf_read(ub_rx);
    leave_critical_section();
    return ch;
}

void UARTBuf_consume_new_line(UARTBuf* ub) {
    enter_critical_section();
    ub->num_new_lines--;
    leave_critical_section();    
}

static unsigned prv_mstatus = 0, critical_section_depth = 0;

void enter_critical_section() { /// critical_section_depth should be 0 when enter_critical_section is called
    if ((critical_section_depth++) == 0) {
        asm volatile("csrr %[old], mstatus" : [old] "=&r" (prv_mstatus) : );    /// save mstatus bits to prv_mstatus
        asm volatile("csrc mstatus, 0x0008" : : );	///	clear mstatus.mie[3]   : disable machine-interrupt
    } else { set_gpio_error(0); } /// invalid call to enter_critical_section
}
void leave_critical_section() { /// critical_section_depth should be 1 when leave_critical_section_critical_section is called
    if ((--critical_section_depth) == 0) {
        asm volatile("csrw mstatus, %[new]" : : [new] "r" (prv_mstatus));       /// restore mstatus bits from prv_mstatus
    } else { set_gpio_error(1); } /// invalid call to leave_critical_section_critical_section
}

void UARTBuf_reset(UARTBuf* ub) { ub->rp = 0; ub->wp = 0; ub->full = 0; ub->not_empty = 0; ub->num_new_lines = 0; ub->terminate_code = 0; }

#define NXT_PNT(cur_pnt)    (((cur_pnt) == MAX_UART_BUF_SIZE - 1) ? 0 : (cur_pnt) + 1)

static void UARTBuf_update(UARTBuf* ub, int wflag) {
    int nrp = ub->rp;
    int nwp = ub->wp;
    if (!wflag) { nrp = NXT_PNT(nrp); }
    if (wflag) { nwp = NXT_PNT(nwp); }
    ub->not_empty = (wflag) ? 1 : (nrp != nwp);
    ub->full = (!wflag) ? 0 : (nrp == nwp);
    ub->rp = nrp;
    ub->wp = nwp;
}

int UARTBuf_buffered_size(UARTBuf* ub) {
    int dif = ub->wp - ub->rp;
    if (dif < 0) { dif += MAX_UART_BUF_SIZE; }
    return dif;
}

void UARTBuf_write(UARTBuf* ub, char ch) {
    if (!ub->full) {
        ub->buf[ub->wp] = ch;
        UARTBuf_update(ub, 1);	///	wflag = 1;
    }
}

char UARTBuf_read(UARTBuf* ub) {
    char ch = 0;
    if (ub->not_empty) {
        ch = ub->buf[ub->rp];	///	uart <-- uart_buf[rp];
        UARTBuf_update(ub, 0);	///	wflag = 0;
    }
    return ch;
}

char UARTBuf_read_last(UARTBuf* ub) {
    int nwp = ub->wp;
    if (nwp == 0) {
        if (ub->not_empty) {
            nwp = MAX_UART_BUF_SIZE - 1;
        }
    }
    else {
        nwp --;
    }
    return ub->buf[nwp];
}

void flush_UART() {
#if 1
    while (UART_BUF_TX[0].not_empty || !UART_TX_EMPTY(_uart[0]) || UART_BUSY(_uart[0])) {} /// while (tx_empty == 0);
    while (UART_BUF_TX[1].not_empty || !UART_TX_EMPTY(_uart[1]) || UART_BUSY(_uart[1])) {} /// while (tx_empty == 0);
#else
    while (UART_BUF_TX[0].not_empty || !UART_TX_EMPTY(_uart[0])) {} /// while (tx_empty == 0);
	while (UART_BUF_TX[1].not_empty || !UART_TX_EMPTY(_uart[1])) {} /// while (tx_empty == 0);
#endif
}

void (*io_putch)(char) = _io_putch;
char (*io_getch)(void) = _io_getch;

void UARTBuf_reset_all() {
    UARTBuf_reset(&UART_BUF_TX[0]);
    UARTBuf_reset(&UART_BUF_TX[1]);
    UARTBuf_reset(&UART_BUF_RX[0]);
    UARTBuf_reset(&UART_BUF_RX[1]); 
}

void enable_BUTTON_interrupt() {
	asm volatile("csrw mtvec, %[new]" : : [new] "r" (_mhandler));
	sysctrl->irq_enable |= 0x7c;	///	set sysctrl->ext_irq_enable |= 0x0000007c; (enable button[6:2] interrupts)
	asm volatile("csrs mie, %[new]" : : [new] "r" (0x00000800));	///	set meie[11] : machine-external-interrupt-enable
	asm volatile("csrs mstatus, 0x0008" : : );	///	set mie[3]   : machine-interrupt-enable
}

void enable_UART_interrupt() {
    UARTBuf_reset_all();
	io_putch = _UARTBuf_putch;  /// _UARTBuf_putch : interrupt-based putch
    io_getch = _UARTBuf_getch;  /// _UARTBuf_getch : interrupt-based getch
	sysctrl->irq_enable |= 3;	///	set sysctrl->irq_enable |= 0x00000003; (enable uart1/uart0 interrupts)
	asm volatile("csrs mie, %[new]" : : [new] "r" (0x00000800));	///	set meie[11] : machine-external-interrupt-enable
	asm volatile("csrs mstatus, 0x0008" : : );	///	set mie[3]   : machine-interrupt-enable
	asm volatile("csrw mtvec, %[new]" : : [new] "r" (_mhandler));
 }

void disable_UART_interrupt() {
    while (UART_BUSY(_uart[0])) {}
    while (UART_BUSY(_uart[1])) {}
    io_putch = _io_putch;   /// _io_putch : polling-based putch
    io_getch = _io_getch;   /// _io_getch : polling-based getch
	sysctrl->irq_enable &= ~3;	///	clear sysctrl->ext_irq_enable &= ~0x00000003; (disable uart1/uart0 interrupts)
	asm volatile("csrc mstatus, 0x0008" : : );	///	clear mie[3]   : machine-interrupt-enable
    //while (!UART_TX_EMPTY(_uart[0]) || !UART_TX_EMPTY(_uart[1])) {} /// while (tx_empty == 0);
    UARTBuf_reset_all();
}

#if 1
void UARTintr_handler(int uidx) {	///	called from interrupt handler
    UART* uart = _uart[uidx];
    UARTBuf* ub_tx = &UART_BUF_TX[uidx];
    while (ub_tx->not_empty && !UART_TX_FULL(uart)) {	///	while(tx_full == 0)
        uart->data = UARTBuf_read(ub_tx);
        /// ub_tx->not_empty --> 0 can happen
        /// ub_tx->full --> 0 always happens
    }
    UARTBuf* ub_rx = &UART_BUF_RX[uidx];
    while (!ub_rx->full && !UART_RX_EMPTY(uart)) {	///	while(rx_empty == 0)
        char uart_rx = uart->data;
        UARTBuf_write(ub_rx, uart_rx);  ///	ub_rx <-- uart;
        /// ub_rx->full --> 1 can happen
        /// ub_rx->not_empty --> 1 always happens
        if (uart_rx == TERMINATE_CODE) {  /// ctrl-Z
            ub_rx->terminate_code = 1;  /// abort program
        }
        else if (uart_rx == '\n') {
            ub_rx->num_new_lines ++;    /// count up new lines
        }
    }
}

#include <stdio.h>

void ext_intr_handler() {
	unsigned irq = sysctrl->irq_ctrl;
    for (int i = 0; i < 2; ++i) {
        if (irq & (1 << i)) {
            UARTintr_handler(i);
        }
    }
#if 0
    if ((irq >> 2) & 0x1f) {
        int uidx = UART_CHAN_ID;
        UART_CHAN_ID = 0;
        printf("irq:0x%02x\n", irq);
        UART_CHAN_ID = uidx;
    }
#endif
    sysctrl->irq_ctrl = 0x7f;  /// clear all interrupt pending bits
}
#else
void UARTintr_handler(int uart_vec) {	///	called from interrupt handler
	int i;
	for (i = 0; i < 2; ++i) {
		if ((uart_vec >> i) & 1) {	///	interrupt pending at _uart[i]
			UART* uart = _uart[i];
            UARTBuf* ub_tx = &UART_BUF_TX[i];
            UARTBuf* ub_rx = &UART_BUF_RX[i];
			//// uart_status = ((rx_error << 4) | (rx_empty << 3) | (rx_full << 2) | (tx_empty << 1) | (tx_full));
            while (ub_tx->not_empty && !UART_TX_FULL(uart)) {	///	while(tx_full == 0)
                uart->data = UARTBuf_read(ub_tx);
                /// ub_tx->not_empty --> 0 can happen
                /// ub_tx->full --> 0 always happens
            }
            while (!UART_RX_EMPTY(uart)) {	///	while(rx_empty == 0)
                char uart_rx = uart->data;
                UARTBuf_write(ub_rx, uart_rx);  ///	ub_rx <-- uart;
                /// ub_rx->full --> 1 can happen
                /// ub_rx->not_empty --> 1 always happens
                if (uart_rx == TERMINATE_CODE) {  /// ctrl-Z
                    ub_rx->stat = -10000;  /// abort program
                }
                else if (uart_rx == '\n') {
                    ub_rx->stat ++;    /// count up new lines
                }
            }
		}
	}
}

#include <stdio.h>

void ext_intr_handler() {
	unsigned irq = sysctrl->irq_ctrl;
    if (irq & 3) {
        UARTintr_handler(irq & 3);
        sysctrl->irq_ctrl = 3;  /// clear 2 LSBs : UART interrupt bits
    }
    else {
        int uidx = UART_CHAN_ID;
        UART_CHAN_ID = 0;
        printf("irq = 0x%02x\n", irq);
        UART_CHAN_ID = uidx;
        for (unsigned i = 2; i < 32; ++i) {
            if ((irq >> i) & 1) {
                sysctrl->irq_ctrl = (1 << i);
                return;
            }
        }
    }
}
#endif
void mhandler(void)
{
	unsigned mcause;
	asm volatile("csrr %[old], mcause" : [old] "=&r" (mcause) : );

	if (mcause >> 31) {	///	interrupt events : assume machine-mode only
		switch (mcause & 0xff) {
		case 0x7:
            sysctrl->mtimecmp = sysctrl->mtime + mtimer_interval;
            if (timer_interrupt_hook) {
                timer_interrupt_hook();
            }
			break;
		case 0xb: 
			ext_intr_handler();
			break;
		}
	}
	else {
        ///	exceptions events --> exception handlers not implemented
	}
}

void set_gpio_string(U64 val) {
    gpio->dout7SEG[0] = (unsigned) val;
    gpio->dout7SEG[1] = (unsigned) (val >> 32);
}

char convert_char_seg7(int ch) {
    switch (ch) {
        case '0' : return S7_0;
        case '1' : return S7_1;
        case '2' : return S7_2;
        case '3' : return S7_3;
        case '4' : return S7_4;
        case '5' : return S7_5;
        case '6' : return S7_6;
        case '7' : return S7_7;
        case '8' : return S7_8;
        case '9' : return S7_9;
        case 'a' : return S7_a;
        case 'b' : return S7_b;
        case 'c' : return S7_c;
        case 'd' : return S7_d;
        case 'e' : return S7_e;
        case 'f' : return S7_f;
        case 'g' : return S7_g;
        case 'h' : return S7_h;
        case 'i' : return S7_i;
        case 'j' : return S7_j;
        case 'k' : return S7_k;
        case 'l' : return S7_l;
        case 'm' : return S7_m;
        case 'n' : return S7_n;
        case 'o' : return S7_o;
        case 'p' : return S7_p;
        case 'q' : return S7_q;
        case 'r' : return S7_r;
        case 's' : return S7_s;
        case 't' : return S7_t;
        case 'u' : return S7_u;
        case 'v' : return S7_v;
        case 'w' : return S7_w;
        case 'x' : return S7_x;
        case 'y' : return S7_y;
        case 'z' : return S7_z;
        case '-' : return S7_minus;
        case '_' : return S7_under_score;
        case '.' : return S7_period;
        case ' ' : return S7_space;
        case '=' : return S7_equal;
        default  : return S7_space;
    }
}

#define GPIO_STR8(a, b, c, d, e, f, g, h)   ((((U64)convert_char_seg7(a)) << 0x38) | (((U64)convert_char_seg7(b)) << 0x30) | \
                                             (((U64)convert_char_seg7(c)) << 0x28) | (((U64)convert_char_seg7(d)) << 0x20) | \
                                             (((U64)convert_char_seg7(e)) << 0x18) | (((U64)convert_char_seg7(f)) << 0x10) | \
                                             (((U64)convert_char_seg7(g)) << 0x08) | (((U64)convert_char_seg7(h)) << 0x00))

unsigned convert_hex_char(int val) {
    int hex = val & 0xf;
    if (hex <= 9) { return '0' + hex; }
    else          { return 'a' + hex - 10; }
}
unsigned convert_unsigned_hex_str(unsigned short val) {
    return (convert_hex_char(val >> 12) << 24) | (convert_hex_char(val >> 8) << 16) | (convert_hex_char(val >> 4) << 8) | (convert_hex_char(val));
}

void set_gpio_error(unsigned short err_num) {
    unsigned hex_str = convert_unsigned_hex_str(err_num);
    char h3 = (hex_str >> 24) & 0xff;
    char h2 = (hex_str >> 16) & 0xff;
    char h1 = (hex_str >>  8) & 0xff;
    char h0 = (hex_str >>  0) & 0xff;
    set_gpio_string(GPIO_STR8('e', 'r', 'r', '-', h3, h2, h1, h0));
}

int mtimer_interval = 0;//CLK_FREQ >> 4;

void set_mtimer_interval(int msec) {
    mtimer_interval = ((CLK_FREQ >> 4) / 1000) * msec;
}

unsigned mtime_to_microsec(unsigned tm) {
    return (tm << 4) / (CLK_FREQ / 1000000);
}

void enable_timer_interrupt() {
	asm volatile("csrs mie, %[new]" : : [new] "r" (0x00000080));	///	set meie[7] : machine-external-interrupt-enable
	asm volatile("csrs mstatus, 0x0008" : : );	///	set mie[3]   : machine-interrupt-enable
	asm volatile("csrw mtvec, %[new]" : : [new] "r" (_mhandler));
}

void (*timer_interrupt_hook)(void) = 0;

Profiler gprof;
void start_profiler() {
    asm volatile("rdcycle %[old]" : [old] "=&r" (gprof.cycle) : );
    asm volatile("rdinstret %[old]" : [old] "=&r" (gprof.instret) : );
    asm volatile("rdtime %[old]" : [old] "=&r" (gprof.time) : );
}
void end_profiler() {
    unsigned cycle2, instret2, time2;
    asm volatile("rdcycle %[old]" : [old] "=&r" (cycle2) : );
    asm volatile("rdinstret %[old]" : [old] "=&r" (instret2) : );
    asm volatile("rdtime %[old]" : [old] "=&r" (time2) : );
    printf("PROFILE : %d cycles, %d insts, %d usecs\n", cycle2 - gprof.cycle, instret2 - gprof.instret,
        mtime_to_microsec(time2 - gprof.time));
}