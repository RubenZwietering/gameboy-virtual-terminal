# https://www.gnu.org/software/make/manual/make.html

PROG:=vt

GDIR:=gfx
ODIR:=obj
DDIR:=dep
BDIR:=bin
IDIR:=inc
LDIR:=lib
SDIR:=src

# https://rgbds.gbdev.io/docs/master/rgbasm.1
ASM:=rgbasm
AFLAGS:=-Wall -I $(IDIR) -D DEBUG -D WOZMON_OPTIMIZED
CC:=sdcc
CFLAGS:=--std=c23 -msm83 --asm=rgbds -S
LINK:=rgblink
LFLAGS:=-p 0xff
FIX:=rgbfix
FFLAGS:=-cvO -p 0
GFX:=rgbgfx
GFLAGS:=
EMU?=sameboy
EFLAGS?=
HW?=
CABLE?=

ifeq ($(HW),gbc)
	HW:=cgb
endif

ifeq ($(EMU),sameboy)
	ifeq ($(HW),dmg)
		EFLAGS:=$(EFLAGS) --model dmg
	else ifeq ($(HW),cgb)
		EFLAGS:=$(EFLAGS) --model cgb
	endif
	ifneq ($(CABLE),)
$(warning Warning: Sameboy SDL version does not support link cable emulation.)
	endif
else ifeq ($(EMU),bgb)
	ifeq ($(HW),dmg)
		EFLAGS:=$(EFLAGS) --setting SystemMode=0
	else ifeq ($(HW),cgb)
		EFLAGS:=$(EFLAGS) --setting SystemMode=1
	endif
	ifneq ($(CABLE),) # default for bgb 127.0.0.1:8765 or 127.0.0.1
		# or --listen
		EFLAGS:=$(EFLAGS) --connect $(CABLE)
	endif
else ifeq ($(EMU),emulicious)
	ifeq ($(HW),dmg)
		EFLAGS:=$(EFLAGS) -set System=GAME_BOY
	else ifeq ($(HW),cgb)
		EFLAGS:=$(EFLAGS) -set System=GAME_BOY_COLOR
	endif
	ifneq ($(CABLE),) # default for emulicious 127.0.0.1:5887 or 127.0.0.1
		EFLAGS:=$(EFLAGS) -link $(wordlist 1,3,$(subst :, -linkport ,$(CABLE)))
	endif
endif

ASRCS:=							\
	char.z80					\
	bcd.z80						\
	c0_control_code.z80			\
	c1_control_code.z80			\
	crash.z80					\
	cursor.z80					\
	div.z80						\
	input.z80					\
	joypad.z80					\
	memory.z80					\
	mul.z80						\
	print.z80					\
	screen.z80					\
	serial.z80					\
	start.z80					\
	vblank.z80					\
	wozmon.z80					\
	# $(wildcard $(SDIR)/*.z80)
CSRCS:=							\
	main.c						\
	# $(wildcard $(SDIR)/*.c)
GSRCS:=							\
	sys.png						\
	# $(wildcard $(GDIR)/*.png)

OBJS:=$(patsubst %.z80,$(ODIR)/%.o,$(ASRCS)) $(patsubst %,$(ODIR)/%.o,$(CSRCS))
DEPS:=$(patsubst $(ODIR)/%.o,$(DDIR)/%.d,$(OBJS))
GFXS:=$(patsubst %.png,$(BDIR)/%.2bpp,$(GSRCS))

OUT:=$(BDIR)/$(PROG).gb
SYM:=$(BDIR)/$(PROG).sym

all: $(ODIR) $(DDIR) $(BDIR) $(OUT)

$(OUT): $(GFXS) $(OBJS)
	$(LINK) $(LFLAGS) -n $(SYM) -o $@ $(OBJS)
	$(FIX) $(FFLAGS) $@

-include $(DEPS)

$(ODIR)/%.o: $(SDIR)/%.z80 makefile
	$(ASM) $(AFLAGS) -M $(DDIR)/$(*F).d -o $@ $<

$(ODIR)/%.c.o: $(ODIR)/%.c.z80 makefile
	$(ASM) $(AFLAGS) -M $(DDIR)/$(*F).c.d -o $@ $<

.SECONDARY: $(patsubst %,$(ODIR)/%.z80,$(_CSRCS))

$(ODIR)/%.c.z80: $(SDIR)/%.c
	$(CC) $(CFLAGS) -o $@ $^

$(BDIR)/%.2bpp: $(GDIR)/%.png
	$(GFX) $(GFLAGS) -o $@ $^

$(ODIR) $(DDIR) $(BDIR):
	mkdir -p $@

.PHONY: all run flash clean

run: all
	$(EMU) $(EFLAGS) $(OUT)

flash: all
	flashgbx --mode dmg --action flash-rom $(OUT)

clean:
	-test -d "$(ODIR)" && rm -f ./$(ODIR)/* && rmdir ./$(ODIR)
	-test -d "$(DDIR)" && rm -f ./$(DDIR)/* && rmdir ./$(DDIR)
	-test -d "$(BDIR)" && rm -f ./$(BDIR)/* && rmdir ./$(BDIR)
