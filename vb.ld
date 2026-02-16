OUTPUT_FORMAT("elf32-v810", "elf32-v810", "elf32-v810")
OUTPUT("a.elf") /* force elf format output */
OUTPUT_ARCH(v810)
TARGET(elf32-v810)
ENTRY(Reset)
SEARCH_DIR(.);

MEMORY {
	ram	(!r): ORIGIN = 0x05000000, LENGTH = 64k /*64k*/
	rom	(rx): ORIGIN = 0x07000000, LENGTH = 16M
}

__text_vma = 0x07000000;
__data_vma = 0x05000000;

SECTIONS
{
  /* Read-only sections, merged into text segment: */
  .text __text_vma : {
    *(.text)
    *(.text.*)
    /* .gnu.warning sections are handled specially by elf32.em.  */
    *(.gnu.warning)
    *(.gnu.linkonce.t*)
  } >rom = 0xFF

  .rodata : {
    *(.rodata)
    *all.rodata*(*)
    *(.roda)
    *(.rodata.*)
    *(.gnu.linkonce.r*)
    /* SORT(CONSTRUCTORS) */
  } >rom = 0xFF

  /* C++ Constructor/Destructor table */
  /*
  .ctors : {
    ___ctors = .;
    KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*crtend(.ctors))
    ___ctors_end = .;
  } >rom = 0xFF

  .dtors : {
    ___dtors = .;
    KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*crtend.o(.dtors))
    ___dtors_end = .;
  } >rom = 0xFF
  */

  /* Stop empty section from causing a compiler error */
  /DISCARD/ : {
    *(.comment)
  } 

  /* Place orphan ROM data here */
  /* . = .; */
  
  /* Ram memory */

  __data_lma = .;
  .data __data_vma : AT(__data_lma) {
	__data_start = .;
    *(.data)
    *(.data.*)
    *(.sdata)
    *(.sdata.*)
    *(.gnu.linkonce.d*)
    /* CONSTRUCTORS */
  } >ram = 0xFF

  __data_end = .;

  .bss : {
    *(.dynbss)
    *(.bss)
    *(.sdata)
    *(.sbss)
    *(COMMON)
  } >ram = 0xFF
  /* These must appear regardless of  .  */


  /* Compute the vector address */
  /* Prevent overlaps with vbvectors */
  __sections_size = SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.data);
  __rom_size = 1 << LOG2CEIL(__sections_size + 0x220);
  __rom_end = ORIGIN(rom) + __rom_size;

  /* Place interupt and reset vector at end of rom */

  .vbvectors 0x07FFFDE0 : AT(__rom_end - 0x220) {
    KEEP (*(.vbvectors))
  } >rom = 0xFF 
}
