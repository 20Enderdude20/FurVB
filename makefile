# Set the project name here
NAME = FurVB

OUTPUTDIR = output
TOOLCHAIN_ARCH := v810
TOOLCHAIN_DIR := $(HOME)/llvm-v810

# Toolchain paths
BINDIR = $(TOOLCHAIN_DIR)/bin
INCDIR = $(TOOLCHAIN_DIR)/include
LIBDIR = $(TOOLCHAIN_DIR)/lib

CC      = $(BINDIR)/clang
LD      = $(BINDIR)/ld.lld
OBJCOPY = $(BINDIR)/llvm-objcopy

# Common target flags
TARGET  = -target v810-unknown-vb -mcpu=vb

# Compilation flags
CFLAGS  = $(TARGET) -I$(INCDIR) -g -O2 -ffreestanding -Wall -Wextra
ASFLAGS = $(TARGET) -g
LDFLAGS = $(TARGET) --ld-path=$(LD) -L$(LIBDIR) \
          -Tvb.ld -nolibc -flto

# Automatically collect source files
CFILES := $(wildcard *.c)
SFILES := $(wildcard *.s)

# Object files go in output/
COBJS := $(patsubst %.c,$(OUTPUTDIR)/%.o,$(CFILES))
SOBJS := $(patsubst %.s,$(OUTPUTDIR)/%.o,$(SFILES))
OFILES := $(COBJS) $(SOBJS)

ELFFILE := $(OUTPUTDIR)/$(NAME).elf
VBFILE  := $(OUTPUTDIR)/$(NAME).vb

.PHONY: all clean distclean

all: $(VBFILE)

# Convert ELF to final binary
$(VBFILE): $(ELFFILE)
	@$(OBJCOPY) -S -O binary $< $@

# Link
$(ELFFILE): $(OFILES)
	@$(CC) $(OFILES) $(LDFLAGS) -o $@

# Compile C
$(OUTPUTDIR)/%.o: %.c | $(OUTPUTDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# Assemble .s (interrupt vectors included)
$(OUTPUTDIR)/%.o: %.s | $(OUTPUTDIR)
	@$(CC) $(ASFLAGS) -c $< -o $@

# Ensure output directory exists
$(OUTPUTDIR):
	@mkdir -p $(OUTPUTDIR)

clean:
	@rm -f $(OUTPUTDIR)/*

distclean: clean
