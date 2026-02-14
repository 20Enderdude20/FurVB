#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "tables.h"
#include "special.h"
#include "vbdefines.h"

extern bool FCSPtrSize;
extern bool LinearPitch;
extern ChannelDataRegisters* ChDataReg[6];
extern ChannelState* ChState[6];


uint16_t GetNoteRegVal(uint8_t note, uint16_t* N_Tbl, const uint8_t* O_Tbl, const uint8_t* S_Tbl) {
	uint8_t octaveshift = O_Tbl[(note >> 2)];
	uint32_t regval = 0;
	if (note < 72)
		regval = N_Tbl[note - S_Tbl[6 - octaveshift]] << octaveshift;
	else
		regval = N_Tbl[note - S_Tbl[octaveshift + 6]] >> octaveshift;

	regval = (regval > 0xffff) ? 0xffff : regval;

	return (uint16_t)regval;
}
uint16_t LinearFreqInterpolate(uint16_t basenote, char pitch1, char pitch2, uint16_t arp, bool Arpfixed, uint16_t* N_Tbl) {
	uint16_t nbase = (basenote << 7) + pitch1 + pitch2;
	uint16_t nupper;
	uint16_t rdelta;
	uint16_t regval;
	if (!Arpfixed)
		nbase += arp << 7;
	else
		nbase = (arp << 7) + pitch1 + pitch2;
	uint8_t factor = 0x7f - (nbase & 0x7f); // Why (128 - factor)? You'll see in the if statement
	// interpolation format: 0bNNNNNNNNN.nnnnnnn
	// N: coarse note selection (by semitone, selects which notes in table to interpolate between)
	// n: fine pitch selection (128th of semitone)
	regval = GetNoteRegVal(nbase >> 7, N_Tbl, OctaveTable, SubTable); //aka nlower
	if (factor != 0x7f) {	// Get the two points to interpolate between
		nupper = GetNoteRegVal((nbase >> 7) + 1, N_Tbl, OctaveTable, SubTable);
		rdelta = regval - nupper; // divider values at this point in func: lower note is higher value, and positive delta is desired

		rdelta = Mpyhw(factor, rdelta); //rdelta *= factor 

		rdelta >>= 7; //rdelta /= 128
	}
	regval += rdelta;

	return (regval > 2047) ?
		1 :
		(2048 - regval);

};
short GetVibVal(uint8_t chan) {
	uint8_t pos = ChState[chan]->vibratoPos;
	uint8_t depth = ChState[chan]->vibratoDepth;
	short tmp = 0;
	short tmp2 = 0;
	if (depth == 0)
		return 0;
	switch (ChState[chan]->vibratoShape) {
		case 0:
			tmp = ((pos & 0x1f) < 0x10) ? VibTable[depth][pos & 0x0f] : VibTable[depth][0xf - (pos & 0xf)];
			tmp = ((pos & 0x3f) < 0x20) ? tmp : -tmp;
			break;
		case 1:
			tmp = ((pos & 0x1f) < 0x10) ? VibTable[depth][pos & 0x0f] : VibTable[depth][0xf - (pos & 0xf)];
			tmp = ((pos & 0x3f) < 0x20) ? tmp : 0;
			break;
		case 2:
			tmp = ((pos & 0x1f) < 0x10) ? VibTable[depth][pos & 0x0f] : VibTable[depth][0xf - (pos & 0xf)];
			tmp = ((pos & 0x3f) < 0x20) ? -tmp : 0;
			break;
		case 3:
		case 4:
		case 5:
		case 6:
			tmp = ((pos & 0x3f) < 0x20) ? VibTable[depth][0x0f] : -VibTable[depth][0x0f];
			break;
		case 7:
		case 8:
			tmp = ((pos & 0x3f) < 0x20) ? VibTable[depth][0x0f] : 0;
			break;
		case 9:
			tmp = ((pos & 0x3f) < 0x20) ? -VibTable[depth][0x0f] : 0;
			break;
		case 10:
			tmp = ((pos & 0x3f) < 0x20) ? VibTable[depth][(pos & 0x1f) > 1] : VibTable[depth][0xf - ((pos & 0x1f) > 1)];
			break;
		case 11:
			tmp = ((pos & 0x3f) < 0x20) ? -VibTable[depth][(pos & 0x1f) > 1] : -VibTable[depth][0xf - ((pos & 0x1f) > 1)];
			break;
		default:
			tmp = ((pos & 0x1f) < 0x10) ? VibTable[depth][pos & 0x0f] : VibTable[depth][0xf - (pos & 0xf)];
			tmp = ((pos & 0x3f) < 0x20) ? tmp : -tmp;
			break;
	}
	if (ChState[chan]->vibratoFine != 0xf){ // 0xf -> +- (0xf + 1) >> 4 semitone range is what tables are normalized to (default)
		tmp2 = ChState[chan]->vibratoFine + 1;
		tmp = Mpyhw(tmp2, tmp); // tmp *= tmp2

		tmp = tmp >> 4;
	}
		return tmp;

}

void FCSChannelPost(uint8_t chan) {
	volatile uint8_t *ChanRegBase = (volatile uint8_t*)(VSU_S1INT + (chan << 6));
	uint8_t tmp;
	// Volume first
	if (ChState[chan]->volSpeed != 0) {
		int16_t vol = ChState[chan]->volume + ChState[chan]->volSpeed;
		if (ChState[chan]->volSpeedTarget != -1) {
			if ((vol >= ChState[chan]->volSpeedTarget) && (ChState[chan]->volSpeed > 0)
				|| (vol <= ChState[chan]->volSpeedTarget) && (ChState[chan]->volSpeed < 0)) {
				vol = ChState[chan]->volSpeedTarget;
				ChState[chan]->volSpeed = 0;
			}
		}

		if (vol < 0)
			vol = 0;
		if (vol > ChState[chan]->volMax)
			vol = ChState[chan]->volMax;

		ChState[chan]->volume = vol;
	}
	// Write to volume register
	ChanRegBase[o_S1EV0] = (ChanRegBase[o_S1EV0] & 0xf) | ((ChState[chan]->volume >> 4) & 0xf0);

	// Pitch next
	uint16_t regvalue;

	if (ChState[chan]->vibratoDepth != 0) {
		ChState[chan]->vibratoPos = ChState[chan]->vibratoPos + ChState[chan]->vibratoRate & 0x3f;
	}

	if (LinearPitch)
		regvalue = LinearFreqInterpolate(ChState[chan]->note, ChState[chan]->pitch, GetVibVal(chan), 0, false, NoteTable);
	else {
		regvalue = GetNoteRegVal(ChState[chan]->note, NoteTable, OctaveTable, SubTable) + ChState[chan]->pitch + GetVibVal(chan);
		regvalue = ((short)regvalue < 0) ? 0 : regvalue;
		regvalue = ((short)regvalue > 2047) ? 1 : 2048 - regvalue;
	}
	// Write to frequency registers
	ChanRegBase[o_S1FQL] = (uint8_t)(regvalue & 0xff);
	ChanRegBase[o_S1FQH] = (uint8_t)((regvalue >> 8) & 0x07);

	return;
};






