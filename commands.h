#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define CMD_FULL_DISPATCH_OFFSET 0x3e // Lowest full command value used relevant for Virtual Boy

extern bool FCSPtrSize;
extern uint8_t PresetDelay[];
extern uint8_t PresetInstrument[];
extern uint8_t PresetVolume[];
extern uint8_t SpeedDialCMD[];

extern ChannelDataRegisters* ChDataReg[];
extern ChannelState ChState[];

typedef bool (*CommandFunc)(uint8_t chan, const uint8_t* param);

 // Full commands first for use in CmdFull
bool CmdSetupEnv(uint8_t chan, const uint8_t* param) {
	return false; // Boolean tells us if the loop these are used in should be done yet
};

bool CmdNoiseLength(uint8_t chan, const uint8_t* param) {
	return false; 
};

bool CmdWaveform(uint8_t chan, const uint8_t* param) {
	return false; 
};

bool CmdSetupSweep(uint8_t chan, const uint8_t* param) {
	return false; 
};

bool CmdSetupMod(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdModWave(uint8_t chan, const uint8_t* param) {
	return false;
};

CommandFunc CmdFullDispatch[] = {
	CmdSetupEnv, CmdNoiseLength, CmdWaveform, CmdSetupSweep, 
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, CmdSetupMod, NULL, NULL, NULL, CmdModWave,
};

const uint8_t FullCmdLengths[] = { // Starting at full command 0x3e
	// 3e 3f 40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	// 4e 4f 50 51 52 53 54 55 56 57 58 59 5a 5b 5c 5d
		2, 3, 3, 2, 2, 2, 2, 2, 2, 2, 3, 3, 2, 2, 2, 2,
	// 5e 5f 60
		2, 2, 2
};
// Normal commands
bool CmdEvalNote(uint8_t chan, const uint8_t* param) {
	return false; // Boolean tells us if the loop these are used in should be done yet
};

bool CmdSetIns(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdPrePorta(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdArpSpeed(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdVibrato(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdVibRange(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdVibShape(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdPitch(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdArpeggio(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdVolume(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdVolSlide(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdPorta(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdLegato(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdVolSlideWT(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdTremolo(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdPanbrello(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdPanSlide(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdPanning(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdCall32(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdOffWait(uint8_t chan, const uint8_t* param) {
	return true;
};

bool CmdFull(uint8_t chan, const uint8_t* param) {
	return CmdFullDispatch[param[0] - CMD_FULL_DISPATCH_OFFSET](chan, param);
};

bool CmdCall16(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdReturn(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdJump(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdTickRate(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdWait16(uint8_t chan, const uint8_t* param) {
	return true;
};

bool CmdWait8(uint8_t chan, const uint8_t* param) {
	return true;
};

bool CmdWait(uint8_t chan, const uint8_t* param) {
	return true;
};

bool CmdStop(uint8_t chan, const uint8_t* param) {
	return true;
};

CommandFunc CmdDispatch[] = {
	CmdEvalNote, CmdSetIns, NULL, NULL, NULL, NULL, 
	NULL, NULL, NULL, CmdPrePorta, CmdArpSpeed, CmdVibrato, 
	CmdVibRange, CmdVibShape, CmdPitch, CmdArpeggio, CmdVolume, CmdVolSlide, 
	CmdPorta, CmdLegato, CmdVolSlideWT, CmdTremolo, CmdPanbrello, CmdPanSlide, 
	CmdPanning, NULL, NULL, NULL, NULL, NULL, 
	CmdCall32, CmdOffWait, CmdFull, CmdCall16, CmdReturn, CmdJump, 
	CmdTickRate, CmdWait16, CmdWait8, CmdWait, CmdStop
};


const uint8_t CmdLengths[] = { // Starting at command 0xb7
// b7 b8 b9 ba bb bc bd be bf c0 c1 c2 c3 c4 c5 c6
	1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
// c7 c8 c9 ca cb cc cd ce cf d0 d1 d2 d3 d4 d5 d6
	2, 2, 3, 2, 5, 2, 2, 2, 3, 4, 1, 1, 1, 5, 5, 1,
// d7 d8 d9 da db dc dd de df e0 e1 e2 e3 e4 e5 e6
	1, 0, 0, 0, 5, 3, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1,
// e7 e8 e9 ea eb ec ed ee ef f0 f1 f2 f3 f4 f5 f6
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// f7 f8 f9 fa fb fc fd fe ff
	1, 1, 1, 1, 1, 1, 1, 1, 1
};

