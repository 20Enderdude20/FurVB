# Set the project name here
NAME = FurVB

OUTPUTDIR = output
SRCDIRS = src crt0
INCLUDE_DIRS = . src libgccvb crt0 extern/libgccvb/source
TOOLCHAIN_ARCH := v810
ifeq ($(OS),Windows_NT)
TOOLCHAIN_DIR := $(USERPROFILE)/llvm-v810
else
TOOLCHAIN_DIR := $(HOME)/llvm-v810
endif

# Toolchain paths
BINDIR = $(TOOLCHAIN_DIR)/bin
INCDIR = $(TOOLCHAIN_DIR)/include
LIBDIR = $(TOOLCHAIN_DIR)/lib

CC      = $(BINDIR)/clang
LD      = $(BINDIR)/ld.lld
OBJCOPY = $(BINDIR)/llvm-objcopy

# Host shell helpers (Win vs Linux; I shouldn't need two separate makefiles lol)
ifeq ($(OS),Windows_NT)
SHELL := cmd
.SHELLFLAGS := /C
MKDIR_P = if not exist "$1" mkdir "$1"
RM_RF   = if exist "$1" rmdir /s /q "$1"
else
MKDIR_P = mkdir -p "$1"
RM_RF   = rm -rf "$1"
endif

# Common target flags
TARGET  = -target v810-unknown-vb -mcpu=vb

# Compilation flags
CFLAGS  = $(TARGET) -I$(INCDIR) $(addprefix -I,$(INCLUDE_DIRS)) -g -O2 -ffreestanding -Wall -Wextra
ASFLAGS = $(TARGET) -g
LDFLAGS = $(TARGET) --ld-path=$(LD) -L$(LIBDIR) \
          -Tvb.ld -nolibc -flto

# Automatically collect source files
CFILES := $(foreach d,$(SRCDIRS),$(wildcard $(d)/*.c)) \
		  extern/libgccvb/source/audio.c
SFILES := $(foreach d,$(SRCDIRS),$(wildcard $(d)/*.s))

# Object files go in output/
COBJS := $(patsubst %.c,$(OUTPUTDIR)/%.o,$(CFILES))
SOBJS := $(patsubst %.s,$(OUTPUTDIR)/%.o,$(SFILES))
OFILES := $(COBJS) $(SOBJS)

ELFFILE := $(OUTPUTDIR)/$(NAME).elf
VBFILE  := $(OUTPUTDIR)/$(NAME).vb

.PHONY: all clean distclean

all: $(VBFILE)

# Convert ELF to final binary
$(VBFILE): $(ELFFILE) | $(OUTPUTDIR)
	@$(OBJCOPY) -S -O binary $< $@

# Link
$(ELFFILE): $(OFILES) | $(OUTPUTDIR)
	@$(CC) $(OFILES) $(LDFLAGS) -o $@

# Compile C
$(OUTPUTDIR)/%.o: %.c
	@$(call MKDIR_P,$(@D))
	@$(CC) $(CFLAGS) -c $< -o $@

# Assemble .s (interrupt vectors included)
$(OUTPUTDIR)/%.o: %.s
	@$(call MKDIR_P,$(@D))
	@$(CC) $(ASFLAGS) -c $< -o $@

# Ensure output directory exists
$(OUTPUTDIR):
	@$(call MKDIR_P,$(OUTPUTDIR))

clean:
	@$(call RM_RF,$(OUTPUTDIR))

distclean: clean