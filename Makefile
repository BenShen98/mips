SHELL := /bin/bash

bin = ./bin
src =./src
test = ./test
mipsCase = $(src)/testcase
mipsBinOut = $(bin)/caseBin
mipsAssemblyOut = $(test)/caseAssembly
testcases=$(basename $(wildcard $(mipsCase)/*.s))
caseFile = $(bin)/case.csv


CC = g++
CPPFLAGS = -std=c++11 -W -Wall

.NOTPARALLEL: $(testcases)

#for cross compile on mac (with brew version)
#brew install FiloSottile/musl-cross/musl-cross --without-x86_64 --with-mips
#https://blog.filippo.io/easy-windows-and-linux-cross-compilers-for-macos/
# MIPS_CC = mips-linux-musl-gcc
# MIPS_OBJCOPY = mips-linux-musl-objcopy
# MIPS_OBJDUMP = mips-linux-musl-objdump

# for cross compile on Linux
MIPS_CC = mips-linux-gnu-gcc
MIPS_OBJCOPY = mips-linux-gnu-objcopy
MIPS_OBJDUMP = mips-linux-gnu-objdump

MIPS_CPPFLAGS = -W -Wall -O3 -fno-builtin -march=mips1 -mfp32
MIPS_LDFLAGS = -nostdlib -Wl,-melf32btsmip -march=mips1 -nostartfiles -mno-check-zero-division -Wl,--gpsize=0 -static -Wl,-Bstatic -Wl,--build-id=none --entry=0000000010000000
MIPS_Linker = $(src)/linker.ld

#https://ftp.gnu.org/old-gnu/Manuals/make-3.79.1/html_chapter/make_10.html#SEC101
#https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html
.PHONY testbench: $(testcases)

#short cut for debug
all: clean simulator testbench

# for rebuild
clean:
	rm -rf ./bin
	rm -rf ./test

makedir:
	mkdir -p $(bin)
	mkdir -p $(test)
	mkdir -p $(mipsBinOut)
	mkdir -p $(mipsAssemblyOut)
	mkdir -p $(test)/output


# below are for simulator
simList=Register.sim.o Memory.sim.o Simulator.sim.o main.sim.o

simulator: makedir $(simList)
	$(CC) $(CPPFLAGS) $(addprefix $(bin)/,$(simList)) -o bin/mips_simulator

main.sim.o: $(src)/main.cpp
	$(CC) $(CPPFLAGS) -c $< -std=c++11 -o $(bin)/$@

%.sim.o: $(src)/%.cpp $(src)/%.hpp
	$(CC) $(CPPFLAGS) -c $< -std=c++11 -o $(bin)/$@

#below are code to test single function
#code copied from $(testcases)
%.debug:
	./bin/mips_simulator $(mipsBinOut)/$(basename $@).mips.bin

Test:
	./bin/mips_testbench ./bin/mips_simulator

%.test:
	echo "Compile test case $(mipsCase)/$(basename $@)"
	$(MIPS_CC) $(MIPS_CPPFLAGS) -c $(mipsCase)/$(basename $@).s -o $(mipsCase)/$(basename $@).mips.o
	$(MIPS_CC) $(MIPS_CPPFLAGS) $(MIPS_LDFLAGS) -T $(MIPS_Linker) $(mipsCase)/$(basename $@).s -o $(mipsCase)/$(basename $@).mips.elf
	$(MIPS_OBJCOPY) -O binary --only-section=.text $(mipsCase)/$(basename $@).mips.elf $(mipsBinOut)/$(basename $@).mips.bin
	$(MIPS_OBJDUMP) -j .text -D $(mipsCase)/$(basename $@).mips.elf > $(mipsAssemblyOut)/$(basename $@).mips.s
	rm $(mipsCase)/$(basename $@).mips.o
	rm $(mipsCase)/$(basename $@).mips.elf

	@echo "***********************************************************************"
	@echo "***** below are the source code from $(mipsCase)/$(basename $@).s *****"
	@echo "***********************************************************************"
	@cat $(mipsCase)/$(basename $@).s

	@echo "****************************************************************************************"
	@echo "***** below are the instruction code from $(mipsAssemblyOut)/$(basename $@).mips.s *****"
	@echo "****************************************************************************************"
	@cat $(mipsAssemblyOut)/$(basename $@).mips.s

	@echo "***************************************************************************"
	@echo "***** running testcase from  $(mipsAssemblyOut)/$(basename $@).mips.s *****"
	@echo "***************************************************************************"


	./bin/mips_simulator $(mipsBinOut)/$(basename $@).mips.bin
	@echo "***** Simulator returned $$? *****"
	@echo "**********************************"




# below are for testbench

clearCase:
	rm -f $(caseFile)


testbench: clearCase makedir $(testcases)

	# rm -f $(bin)/case.csv
	# echo $(testcases)
	# for basenamePath in $(testcase); do \
	# 	 head -1 $(basenamePath).s | tr -d " #" >> $(bin)/csv ; \
	# done

	cp $(src)/mips_testbench $(bin)/mips_testbench

$(testcases):
	$(MIPS_CC) $(MIPS_CPPFLAGS) -c $@.s -o $@.mips.o
	$(MIPS_CC) $(MIPS_CPPFLAGS) $(MIPS_LDFLAGS) -T $(MIPS_Linker) $@.s -o $@.mips.elf
	$(MIPS_OBJCOPY) -O binary --only-section=.text $@.mips.elf $(mipsBinOut)/$(notdir $@).mips.bin
	$(MIPS_OBJDUMP) -j .text -D $@.mips.elf > $(mipsAssemblyOut)/$(notdir $@).mips.s
	echo -n $(notdir $@) >> $(caseFile)
	echo -n , >> $(caseFile)
	head -1 $@.s | tr -d '\#' >> $(caseFile)
	rm $@.mips.o
	rm $@.mips.elf




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
