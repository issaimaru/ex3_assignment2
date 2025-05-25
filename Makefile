SRC = main
SRC_1 = function
#SRC = t4b
SRC_C = $(SRC).c
SRC_C1 = $(SRC_1).c

DIR_CC=/opt/riscv/bin/
CC=$(DIR_CC)/riscv32-unknown-elf-gcc
LD=$(DIR_CC)/riscv32-unknown-elf-ld
DUMP=$(DIR_CC)/riscv32-unknown-elf-objdump
CFLAGS=-O1

CFLAGS_RV=-march=rv32imafd_zicsr
#CFLAGS_RV+= -DTEST_WFI_BREAK
OUT= temp

OUT2= $(SRC)

MEM_START=0x00001000
DATA_START=0x00020000

LDFLAGS= -I. -nostartfiles -static -Wl,--defsym=MEM_START=$(MEM_START),--defsym=DATA_START=$(DATA_START),-T,vcu108.lds
RV_SRC= entry.S printf.c io.c io.h function.c function.h monitor.h monitor.c structs.h mhandler.S ../rv32_io_setting.h

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OUT).out $(OUT).dump a.out
	cp $(OUT).out $(OUT2).out
	cp $(OUT).dump $(OUT2).dump
	../bin/release/rv32.exe $(OUT2).out
	./a.out
	rm $(OUT).out
	rm $(OUT).dump

RTL_V_DIR=C2R_ROOT/RTL_V_TB
MEM_INIT_FILE=$(RTL_V_DIR)/RV32I_ARCH1_step_MemInitFileName.v
rtl_sim: $(MEM_INIT_FILE)
	cd $(RTL_V_DIR) && make

$(OUT).out: $(SRC_C) $(RV_SRC)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS) $(CFLAGS_RV)

$(OUT).dump: $(OUT).out
	$(DUMP) -D $^ > $@

a.out: $(SRC_C)
	gcc $(CFLAGS) -DNATIVE_MODE $(SRC_C) $(SRC_C1) monitor.c -o $@ -lm

clean:
	rm -rf $(OUT).out *.o $(OUT).dump

