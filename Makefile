# SHELL := /bin/bash

bin = ./bin
src =./src
test = ./test
mipsCase = $(src)/testcase
mipsBinOut = $(bin)/caseBin
mipsAssemblyOut = $(test)/caseAssembly
testcases=$(basename $(wildcard $(mipsCase)/*))

CC = g++
CPPFLAGS = -std=c++11 -W -Wall

MIPS_CC = mips-linux-gnu-gcc
MIPS_OBJCOPY = mips-linux-gnu-objcopy
MIPS_OBJDUMP = mips-linux-gnu-objdump
MIPS_CPPFLAGS = -W -Wall -O3 -fno-builtin -march=mips1 -mfp32
MIPS_LDFLAGS = -nostdlib -Wl,-melf32btsmip -march=mips1 -nostartfiles -mno-check-zero-division -Wl,--gpsize=0 -static -Wl,-Bstatic -Wl,--build-id=none
MIPS_Linker = $(src)/linker.ld

#https://ftp.gnu.org/old-gnu/Manuals/make-3.79.1/html_chapter/make_10.html#SEC101
#https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html
.PHONY testbench: $(testcases)

# for rebuild
clean:
	rm ./bin -rf
	rm ./test -rf

makedir:
	mkdir -p $(bin)
	mkdir -p $(test)
	mkdir -p $(mipsBinOut)
	mkdir -p $(mipsAssemblyOut)


# below are for simulator
simList=Register.sim.o Memory.sim.o Simulator.sim.o main.sim.o

simulator: makedir $(simList)
	$(CC) $(CPPFLAGS) $(addprefix $(bin)/,$(simList)) -o bin/mips_simulator

main.sim.o: $(src)/main.cpp
	$(CC) $(CPPFLAGS) -c $< -std=c++11 -o $(bin)/$@

%.sim.o: $(src)/%.cpp $(src)/%.hpp
	$(CC) $(CPPFLAGS) -c $< -std=c++11 -o $(bin)/$@

# below are for testbench
testbench: makedir $(testcases)
	echo $(testcases)
	cp $(src)/mips_testbench $(bin)/mips_testbench

$(testcase):
	echo $@
	$(MIPS_CC) $(MIPS_CPPFLAGS) -c $@.s -o $@.mips.o
	$(MIPS_CC) $(MIPS_CPPFLAGS) $(MIPS_LDFLAGS) -T $(MIPS_Linker) $@.s -o $@.mips.elf
	$(MIPS_OBJCOPY) -O binary --only-section=.text $@.mips.elf $(mipsBinOut)/$(nodir $@).mips.bin
	$(MIPS_OBJDUMP) -j .text -D $@.mips.elf > $(mipsAssemblyOut)/$(nodir $@).mips.s



#old code
%.mips.o: %.c
	$(MIPS_CC) $(MIPS_CPPFLAGS) -c $< -o $@

%.mips.o: %.s
	$(MIPS_CC) $(MIPS_CPPFLAGS) -c $< -o $@

%.mips.elf: %.mips.o
	$(MIPS_CC) $(MIPS_CPPFLAGS) $(MIPS_LDFLAGS) -T $(MIPS_Linker) $< -o $@

%.mips.bin: %.mips.elf
	$(MIPS_OBJCOPY) -O binary --only-section=.text $< $@

%.mips.s : %.mips.elf
	$(MIPS_OBJDUMP) -j .text -D $< > $@

# bin:
# 	g++ -std=c++11 iotest/write.cpp -o ./iotest/a.out
# 	./iotest/a.out
# 	rm ./iotest/a.out
