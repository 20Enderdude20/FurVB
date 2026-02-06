#include <stdint.h>
#include <stdbool.h>
#include <stdalign.h>
#include "engine.h"
#include "commands.h"

_Alignas(4) const uint8_t SongStream[] = { // Needs to align for casting
	#embed "example.bin"
};

 bool FCSPtrSize = false; // False is short (< 64KiB), true is long (> 64KiB)
 uint8_t PresetDelay[16];
 uint8_t PresetInstrument[6];
 uint8_t PresetVolume[6];
 uint8_t SpeedDialCMD[4];

 ChannelDataRegisters* ChDataReg[6];
 ChannelState ChState[6];


uint8_t FCSEngineStatus = 0; // bit 0: Ready when set, bit 1: Stopped when set
	

void InitCommandStream(const uint8_t* binfile) {
	if (*(short*)(binfile + 4) != 6) // Check for 6 channels.
		return;
	if (binfile[6] & 2) // Not doing any BE bs; this is a LE machine
		return;
	FCSPtrSize = binfile[6] & 1; // I severely doubt any song will get this long but you never know!
	// Stack sizes are hard set to MAX_STACKDEPTH. Not going to mess with malloc() if I don't have a good reason.
	for (uint8_t i = 0; i < 6; i++) {
		ChDataReg[i]->ChannelStackPtr = 0;
		if (FCSPtrSize)
			ChDataReg[i]->PC = *(uint32_t*)(binfile + 40 + (i << 2));
		else
			ChDataReg[i]->PC = *(uint16_t*)(binfile + 40 + (i << 1));
		ChannelState_Init(&ChState[i]); // Poor man's constructor
	};

	FCSEngineStatus = 0b01;
	return;
}

void ParseChannelStream(uint8_t ch, const uint8_t* binfile) {
	uint16_t ushort_param = 0;
	uint8_t PCInc = 1;
	bool AreWeDone = false;
	while (AreWeDone == false) {
		uint32_t tmp_ptr = ChDataReg[ch]->PC;
		uint8_t command_b = binfile[tmp_ptr];
		uint8_t DispatchIndex = (command_b <= 0xb7) ? 0x0 : (command_b - 0xb7);
		AreWeDone = CmdDispatch[DispatchIndex](ch, &binfile[tmp_ptr]);
		if (command_b != 0xd7)
			PCInc = CmdLengths[DispatchIndex];
		else
			PCInc = FullCmdLengths[binfile[tmp_ptr + 1] - CMD_FULL_DISPATCH_OFFSET];
		ChDataReg[ch]->PC += PCInc;
	}
}

void ReadCommandStreams(const uint8_t* binfile) {
	for (uint8_t i = 0; i < 6; i++)
		ParseChannelStream(i, binfile);
	return;
}


int main() {
	//setup engine
	InitCommandStream(SongStream);
}
void timer_interrupt() {
	ReadCommandStreams(SongStream);
}
	