
  .section  .text.init
  .globl _start
_start:
  la sp, stack_bot  /// initialize sp
  la t0, __bss_start
  la t1, _end
  beq t0, t1, 2f
1:
  sw zero, 0(t0)
  addi t0, t0, 4
  bne t0, t1, 1b
2:
  jal disable_UART_interrupt
  jal _io_init
  jal enable_BUTTON_interrupt
  jal main
  jal _io_fini
  wfi
  j _start
  .section .stack,"aw",@nobits
stack_top:
  .skip 0x2000
stack_bot:
