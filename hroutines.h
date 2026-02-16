#include <stdint.h>
#include "vbdefines.h"
#include "audio.h"

#define TIMER_20u_50Hz 0x3e7
#define TIMER_100u_50Hz 0xc7
extern int main();
extern void InitCommandStream(const uint8_t* binfile);
extern const uint8_t SongStream[];

void InitVSUIns() {
	SSTOP = 1;
	// Load the default wave (saw)
	uint32_t* wavptr32 = (uint32_t*)DefaultWave; // Unrolled to make wave loading as fast as possible
	uint32_t tmp = wavptr32[0];
	WAVEDATA1[0x0 << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x1 << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x2 << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x3 << 2] = tmp;
	tmp = wavptr32[1];
	WAVEDATA1[0x4 << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x5 << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x6 << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x7 << 2] = tmp;
	tmp = wavptr32[2];
	WAVEDATA1[0x8 << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x9 << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0xa << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0xb << 2] = tmp;
	tmp = wavptr32[3];
	WAVEDATA1[0xc << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0xd << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0xe << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0xf << 2] = tmp;

	tmp = wavptr32[4];
	WAVEDATA1[0x10 << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x11 << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x12 << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x13 << 2] = tmp;
	tmp = wavptr32[5];
	WAVEDATA1[0x14 << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x15 << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x16 << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x17 << 2] = tmp;
	tmp = wavptr32[6];
	WAVEDATA1[0x18 << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x19 << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x1a << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x1b << 2] = tmp;
	tmp = wavptr32[7];
	WAVEDATA1[0x1c << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x1d << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x1e << 2] = tmp;
	tmp >>= 8;
	WAVEDATA1[0x1f << 2] = tmp;


	for (uint8_t chan = 0; chan < 6; chan++) { // Per-channel setup
		SND_REGS[chan].SxEV0 = 0x00; // 0 volume, envelopes disabled
		SND_REGS[chan].SxLRV = 0xff; // max left volume, max right volume
		SND_REGS[chan].SxRAM = 0; // channels set to first wave in RAM
		SND_REGS[chan].SxINT = 0x80; // channels are enabled to produce sound, no auto scheduled deactivation (not a thing in furnace)
	}
	return;


}

void Reset() {

	__asm__(
	"    movhi   hi(__data_lma), r0, r4\n\t"
    "    movea   lo(__data_lma), r4, r4\n\t"
    "    movhi   hi(__data_end), r0, r5\n\t"
    "    movea   lo(__data_end), r5, r5\n\t"
    "    movhi   hi(__data_start), r0, r6\n\t"
    "    movea   lo(__data_start), r6, r6\n\t"

    /* initialize data */
    "    jr  3f\n\t"
    "2:\n\t"
    "    ld.b    0[r4], r7\n\t"
    "    st.b    r7, 0[r6]\n\t"
    "    add 1,r4\n\t"
    "    add 1,r6\n\t"
    "3:\n\t"
    "    cmp r5,r6\n\t"
    "    blt 2b\n\t"
		"movhi   hi(__gp), r0, sp\n\t"  
		"movea   lo(__gp), sp, gp"      
	);

	*(volatile uint8_t*)(VSU_SSTOP) = 1; // Early mute
	for (uint16_t i = 0xffff; i != 0; i--) {
		// Spinning for WRAM to warm up
	}
	for (uint32_t i = VSU; i < VSU_END; i+=2) // VSU RAM init
		*(volatile uint16_t*)(i) = 0;

	for (uint32_t i = WRAM; i < WRAM_END; i += 2) // WRAM init
		*(volatile uint16_t*)(i) = 0;

	for (uint32_t i = VIP; i < VIP_END; i += 2) // VIP RAM init
		*(volatile uint16_t*)(i) = 0;

	*(volatile uint8_t*)(FRMCYC) = 0; // 0 frame delay

	uint32_t tmp1 = 0x3DC00;
	uint32_t tmp2 = tmp1 + 510;
	for (uint8_t i = 0x80; i != 0; i--) { // Column tables (based off of GuyPerfect's routine in assembly)
		*(volatile uint16_t*)(tmp1) = (uint8_t)ColumnTableData[0x80 - i]; // Start of table, up
		*(volatile uint16_t*)(tmp1 + 512) = (uint8_t)ColumnTableData[0x80 - i];
		*(volatile uint16_t*)(tmp2) = (uint8_t)ColumnTableData[0x80 - i]; // End of table, down
		*(volatile uint16_t*)(tmp2 + 512) = (uint8_t)ColumnTableData[0x80 - i];
		tmp1 += 2;
		tmp2 -= 2;
	}

	// Turn on display

	*(volatile uint16_t*)(XPCTRL) = XP_XPEN | XP_XPRST;
	*(volatile uint16_t*)(DPCTRL) = DP_SYNCE | DP_RE | DP_DISP;

	// No palette to set for now...

	// Wait state

	*(volatile uint8_t*)(WCR) = 0b11;

	// Acknowledge pending IRQs

	*(volatile uint8_t*)(INTCLR) = *(volatile uint8_t*)(INTPND);

	*(volatile uint8_t*)(INTENB) = 0; // Disable all VIP IRQs

	// setup engine
	InitCommandStream(SongStream);

	ENABLE_IRQS();
	__asm__("ldsr r0, psw");

	InitVSUIns(); // set VSU WRAM and registers for furnace default state

	// Setup the timer

	*(volatile uint8_t*)(TIMER_TLR) = TIMER_100u_50Hz & 0xff;
	*(volatile uint8_t*)(TIMER_THR) = TIMER_100u_50Hz >> 8;
	*(volatile uint8_t*)(TIMER_TCR) = TIMER_ZINT | TIMER_STATCLR | TIMER_TENB; // 100 us
	main();
	return;
}

