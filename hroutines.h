#include <stdint.h>
#include "vbdefines.h"

#define TIMER_20u_50Hz 0x3e7
#define TIMER_100u_50Hz 0xc7
extern int main();

void InitVSUIns() {
	*(volatile uint8_t*)(VSU_SSTOP) = 1;
	volatile uint8_t* Wave0Base = (volatile uint8_t*)(VSU_WAVE_0); // Load the default wave (saw)
	uint32_t* wavptr32 = (uint32_t*)DefaultWave; // Unrolled to make wave loading as fast as possible
	uint32_t tmp = wavptr32[0];
	Wave0Base[0x0 << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x1 << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x2 << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x3 << 2] = tmp;
	tmp = wavptr32[1];
	Wave0Base[0x4 << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x5 << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x6 << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x7 << 2] = tmp;
	tmp = wavptr32[2];
	Wave0Base[0x8 << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x9 << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0xa << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0xb << 2] = tmp;
	tmp = wavptr32[3];
	Wave0Base[0xc << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0xd << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0xe << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0xf << 2] = tmp;

	tmp = wavptr32[4];
	Wave0Base[0x10 << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x11 << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x12 << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x13 << 2] = tmp;
	tmp = wavptr32[5];
	Wave0Base[0x14 << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x15 << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x16 << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x17 << 2] = tmp;
	tmp = wavptr32[6];
	Wave0Base[0x18 << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x19 << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x1a << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x1b << 2] = tmp;
	tmp = wavptr32[7];
	Wave0Base[0x1c << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x1d << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x1e << 2] = tmp;
	tmp >>= 8;
	Wave0Base[0x1f << 2] = tmp;


	for (uint8_t chan = 0; chan < 6; chan++) { // Per-channel setup
		volatile uint8_t* ChanRegBase = (volatile uint8_t*)(VSU_S1INT + (chan << 6));
		ChanRegBase[o_S1EV0] = 0; // 0 volume, envelopes disabled
		ChanRegBase[o_S1LRV] = 0xff; // max left volume, max right volume
		ChanRegBase[o_S1RAM] = 0; // channels set to first wave in RAM
		ChanRegBase[o_S1INT] = 0x80; // channels are enabled to produce sound, no auto scheduled deactivation (not a thing in furnace)
	}
	return;


}

void Reset() {
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

