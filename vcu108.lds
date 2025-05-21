OUTPUT_ARCH( "riscv" )

ENTRY( _start )

SECTIONS
{
  . = MEM_START;

  .text :
  {
    *(.text.init)
  }

  .text : 
  {
    *(.text)
    *(.text.*)
    *(.gnu.linkonce.t.*)
  }

  . = DATA_START;
  .rodata : 
  {
    *(.rdata)
    *(.rodata)
    *(.rodata.*)
    *(.gnu.linkonce.r.*)
  }

  . = ALIGN(0x1000);
  _etext = .;

  .data : 
  {
    *(.data)
    *(.data.*)
    *(.srodata*)
    *(.gnu.linkonce.d.*)
    *(.comment)
  }

  .init_array :
  {
    __init_array_start = .;
    *(.init_array.*)
    *(.init_array)
    __init_array_end = .;
  }

  .fini_array :
  {
    __fini_array_start = .;
    *(.fini_array.*)
    *(.fini_array)
    __fini_array_end = .;
  }

  . = ALIGN(16);
  _edata = .;

  __bss_start = .;
  .bss : 
  {
    *(.bss)
    *(.bss.*)
    *(.sbss*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
  }

  . = ALIGN(0x1000);
  _end = .;

  .stack :
  {
    *(.stack)
  }
}
