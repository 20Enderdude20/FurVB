#include <stdint.h>
#include <stdbool.h>
#include <stdalign.h>
#include "engine.h"
#include "commands.h"
#include "posteval.h"
#include "hroutines.h"

bool LinearPitch = 1;

_Alignas(4) const uint8_t SongStream[] = { // Needs to align for casting
	#embed "example.bin"
};

 bool FCSPtrSize = false; // False is short (< 64KiB), true is long (> 64KiB)
  const uint8_t *PresetInstrument;
  const uint8_t *PresetDelay;
  const uint8_t *PresetVolume;
  const uint8_t *SpeedDialCMD;

 ChannelDataRegisters* ChDataReg[6];
 ChannelState* ChState[6];


uint8_t FCSEngineStatus = 0; // bit 0: Ready when set, bit 1: Stopped when set
	

void InitCommandStream(const uint8_t* binfile) {
	if (*(short*)(binfile + 4) != 6) // Check for 6 channels.
		return;
	if (binfile[6] & 2) // Not doing any BE bs; this is a LE machine
		return;
	FCSPtrSize = binfile[6] & 1; // I severely doubt any song will get this long but you never know!
	// Stack sizes are hard set to MAX_STACKDEPTH. Not going to mess with malloc() if I don't have a good reason.
	PresetDelay = &binfile[8];
	PresetInstrument = &binfile[0x18];
	PresetVolume = &binfile[0x1e];
	SpeedDialCMD = &binfile[0x22];
	for (uint8_t i = 0; i < 6; i++) {
		ChDataReg[i]->ChannelStackPtr = 0;
		if (FCSPtrSize)
			ChDataReg[i]->PC = *(uint32_t*)(binfile + 40 + (i << 2));
		else
			ChDataReg[i]->PC = *(uint16_t*)(binfile + 40 + (i << 1));
		ChannelState_Init(ChState[i]); // Poor man's constructor
	};

	FCSEngineStatus = 1;
	return;
}

void ParseChannelStream(uint8_t ch, const uint8_t* binfile) {
	uint8_t PCInc = 1;
	bool AreWeDone = false;
	while (AreWeDone == false && ((FCSEngineStatus & 2) == 0)) {
		uint32_t tmp_ptr = ChDataReg[ch]->PC;
		uint8_t command_b = binfile[tmp_ptr];
		uint8_t DispatchIndex = (command_b <= 0xb4) ? 0x0 : (command_b - 0xb4);
		AreWeDone = CmdDispatch[DispatchIndex](ch, &binfile[tmp_ptr]);
		if (command_b == 0xd7)
			PCInc = 1 + FullCmdLengths[binfile[tmp_ptr + 1] - CMD_FULL_DISPATCH_OFFSET];
		else if (command_b >= 0xec && command_b <= 0xef)
			PCInc = FullCmdLengths[SpeedDialCMD[command_b - 0xec] - CMD_FULL_DISPATCH_OFFSET];
		else
			PCInc = CmdLengths[DispatchIndex];
		ChDataReg[ch]->PC += PCInc;
	}
}

void ReadCommandStreams(const uint8_t* binfile) {
	if (FCSEngineStatus & 2)
		return;
	for (uint8_t i = 0; i < 6; i++)
		if (ChState[i]->wait > 0)
			ChState[i]->wait--;
		else
			ParseChannelStream(i, binfile);
	for (uint8_t i = 0; i < 6; i++){
		FCSChannelPost(i);
	}
	return;
}


int main() {
	short x = 0;
	// hardware init
	Reset();
	// Set VSU params for furnace default instrument
	InitVSUIns();
	// setup engine
	InitCommandStream(SongStream);
	while (true) // keep spinning
		x += 1;
}
void timer_interrupt() {
	DISABLE_IRQS();
	*(volatile uint8_t*)(TIMER_TCR) = TIMER_ZINT | TIMER_STATCLR; // disable timer & acknowledge
	*(volatile uint8_t*)(TIMER_TCR) = TIMER_ZINT | TIMER_STATCLR | TIMER_TENB; // re-enable
	ReadCommandStreams(SongStream);
	ENABLE_IRQS();
}
	