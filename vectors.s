/*======================================================
 * Macros - GNU AS equivalents of ISAS macros
 *======================================================*/

.macro push reg
    add -4, sp
    st.w \reg, 0[sp]
.endm

.macro pop reg
    ld.w 0[sp], \reg
    add 4, sp
.endm

.macro mcall label
    push r31
    jal \label
    pop r31
.endm

.macro ret
    jmp [r31]
.endm

.macro movw value, reg
    movhi hi(\value), r0, \reg
    movea lo(\value), \reg, \reg
.endm

.macro jump label
    movw \label, r30
    jmp [r30]
.endm

.equ INTCLR,            0x0005f804
.equ INTPND,            0x0005f800

/*--------------------------------------------------------*/
/* Interrupt Vectors Section */
/* This section is placed by the linker at the end of ROM */
/* Offsets are relative to section start */
/*--------------------------------------------------------*/
.section .vbvectors, "ax", @progbits

        /* ROM title - offset 0x00 (0xFFFFFDE0 absolute) */
        .ascii "FURVB"

        /* reserved - offset 0x10 */
        .org 0x14
        .byte 0x00, 0x00, 0x00, 0x00, 0x00

        /* dev code - offset 0x19 */
        .ascii "BF"

        /* game code - offset 0x1B */
        .ascii "2104"

        /* ROM vers - offset 0x1F */
        .byte 0x00

/*-------------------------------------------------------------------------------*/
/* IRQ Vectors - starting at offset 0x20 (0xFFFFFE00 absolute) */

        .org 0x20
        /* Key Interrupt - offset 0x20 */
        reti

        .org 0x30
        /* Timer Interrupt - offset 0x30 */
        push r30
        movhi   hi(timer_interrupt), r0, r30
        movea   lo(timer_interrupt), r30, r30
        jmp [r30]

        .org 0x40
        /* Expansion Port Interrupt - offset 0x40 */
        reti

        .org 0x50
        /* Link Port Interrupt - offset 0x50 */
        reti

        .org 0x60
        /* VIP Interrupt - offset 0x60 */
        push r6
        push r7
        movw    INTPND, r6
        movw    INTCLR, r7
        ld.h    0[r6], r6
        st.h    r6, 0[r7]
        pop r7
        pop r6
        reti

        /* Pad to reset vector at offset 0x210 (0xFFFFFFF0 absolute) */
        .org 0x210

/* Reset Vector - offset 0x210 */
        movhi   hi(Reset), r0, r30
        movea   lo(Reset), r30, r30
        jmp [r30]
        nop  /* pad to 16 bytes total ROM size alignment */
        nop
        nop