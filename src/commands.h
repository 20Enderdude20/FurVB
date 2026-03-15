#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "audio.h"

#define CMD_FULL_DISPATCH_OFFSET 0x3e // Lowest full command value used relevant for Virtual Boy

extern bool FCSPtrSize;
extern uint8_t FCSEngineStatus;
extern const uint8_t* PresetInstrument;
extern const uint8_t* PresetDelay;
extern const uint8_t* PresetVolume;
extern const uint8_t* SpeedDialCMD;

extern ChannelDataRegisters ChDataReg[];
extern ChannelState ChState[];

typedef bool (*CommandFunc)(uint8_t chan, const uint8_t* param);

 // Full commands first for use in CmdFull
bool CmdSetupEnv(uint8_t chan, const uint8_t* param) {
	return false; // Boolean tells us if the loop these are used in should be done yet
};

bool CmdNoiseLength(uint8_t chan, const uint8_t* param) {
	 //channel 6 (noise)
	SND_REGS[5].SxEV1 = (param[1] & 0x7) << 4;
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

const CommandFunc CmdFullDispatch[] = {
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
	ChState[chan].baseFreq = param[0] << 7;
	ChState[chan].portaTemp = 0;
	ChState[chan].keyOn = true;
	ChState[chan].keyOff = false;
	return false; // Boolean tells us if the loop these are used in should be done yet
};

bool CmdNoteOff(uint8_t chan, const uint8_t* param) {
	ChState[chan].keyOn = false;
	ChState[chan].keyOff = true;
	return false;
};

bool CmdNoteOffEnv(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdEnvRel(uint8_t chan, const uint8_t* param) {
	return false;
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
	ChState[chan].vibratoRate = (param[1] & 0xf0) >> 4;

	if ((param[1] & 0xf) == 0)
		ChState[chan].vibratoRate = 0;
	else
		ChState[chan].vibratoDepth = (param[1] & 0xf) - 1;

	return false;
};

bool CmdVibRange(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdVibShape(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdPitch(uint8_t chan, const uint8_t* param) {
	ChState[chan].pitch = (char)param[1];
	return false;
};

bool CmdArpeggio(uint8_t chan, const uint8_t* param) {
	ChState[chan].arp = param[1];
	return false;
};

bool CmdVolume(uint8_t chan, const uint8_t* param) {
	ChState[chan].volume = param[1] << 8;
	return false;
};

bool CmdVolSlide(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdPorta(uint8_t chan, const uint8_t* param) {
	ChState[chan].portaNote = param[1] << 7; // Target
	if (ChState[chan].portaNote > ChState[chan].baseFreq)
	{
		ChState[chan].portaSign = false; // positive frequency sweep
		ChState[chan].portaSpeed = param[2];
		}
	else
	{
		ChState[chan].portaSign = true; // negative frequency sweep
		ChState[chan].portaSpeed = -param[2];
		}
	ChState[chan].inPorta = true;
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
	ChDataReg[chan].ChannelStackPtr += 1;
	ChDataReg[chan].ChannelDataStack[ChDataReg[chan].ChannelStackPtr] = ChDataReg[chan].PC + 5; // size of call32 is 5
	ChDataReg[chan].PC = param[1] | (param[2] << 8) | (param[3] << 16) | (param[4] << 24);
	return false;
};

bool CmdOffWait(uint8_t chan, const uint8_t* param) {
	ChState[chan].keyOn = false;
	ChState[chan].keyOff = true;
	return true;
};

bool CmdFull(uint8_t chan, const uint8_t* param) {
	return CmdFullDispatch[param[0] - CMD_FULL_DISPATCH_OFFSET](chan, param + 1);
};

bool CmdCall16(uint8_t chan, const uint8_t* param) {
	ChDataReg[chan].ChannelStackPtr += 1;
	ChDataReg[chan].ChannelDataStack[ChDataReg[chan].ChannelStackPtr] = ChDataReg[chan].PC + 3; // size of call16 is 3
	ChDataReg[chan].PC = param[1] | (param[2] << 8);
	return false;
};

bool CmdReturn(uint8_t chan, const uint8_t* param) {
	ChDataReg[chan].PC = ChDataReg[chan].ChannelDataStack[ChDataReg[chan].ChannelStackPtr];
	ChDataReg[chan].ChannelStackPtr -= 1;
	return false;
};

bool CmdJump(uint8_t chan, const uint8_t* param) {
	if (FCSPtrSize) // 32-bit jump
		ChDataReg[chan].PC = param[1] | (param[2] << 8) | (param[3] << 16) | (param[4] << 24);
	else // 16-bit jump
		ChDataReg[chan].PC = param[1] | (param[2] << 8);
	return false;
};

bool CmdTickRate(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdWait16(uint8_t chan, const uint8_t* param) {
	ChState[chan].wait = param[1] | (param[2] << 8);
	return true;
};

bool CmdWait8(uint8_t chan, const uint8_t* param) {
	ChState[chan].wait = param[1];
	return true;
};

bool CmdWait(uint8_t chan, const uint8_t* param) {
	ChState[chan].wait = 1;
	return true;
};

bool CmdStop(uint8_t chan, const uint8_t* param) {
	SSTOP = 1;
	FCSEngineStatus |= 2;
	return true;
};

bool CmdPIns(uint8_t chan, const uint8_t* param) {
	return false;
};

bool CmdPVol(uint8_t chan, const uint8_t* param) {
	ChState[chan].volume = PresetVolume[param[0] - 0xe6] << 8;
	return false;
};

bool CmdSpeedDial(uint8_t chan, const uint8_t* param) {
	return CmdFullDispatch[SpeedDialCMD[param[0] - 0xec] - CMD_FULL_DISPATCH_OFFSET](chan, param);
};

bool CmdPDelay(uint8_t chan, const uint8_t* param) {
	ChState[chan].wait = PresetDelay[param[0] - 0xf0];
	return true;
};

static CommandFunc CmdDispatch[] = {
	CmdEvalNote, CmdNoteOff, CmdNoteOffEnv, CmdEnvRel, CmdSetIns, NULL, 
	NULL, NULL, NULL, NULL, NULL, NULL, CmdPrePorta, CmdArpSpeed, CmdVibrato, 
	CmdVibRange, CmdVibShape, CmdPitch, CmdArpeggio, CmdVolume, CmdVolSlide, 
	CmdPorta, CmdLegato, CmdVolSlideWT, CmdTremolo, CmdPanbrello, CmdPanSlide, 
	CmdPanning, NULL, NULL, NULL, NULL, NULL, 
	CmdCall32, CmdOffWait, CmdFull, CmdCall16, CmdReturn, CmdJump, 
	CmdTickRate, CmdWait16, CmdWait8, CmdWait, CmdStop,
	CmdPIns, CmdPIns, CmdPIns, CmdPIns, CmdPIns, CmdPIns,
	CmdPVol, CmdPVol, CmdPVol, CmdPVol, CmdPVol, CmdPVol,
	CmdSpeedDial, CmdSpeedDial, CmdSpeedDial, CmdSpeedDial, CmdPDelay, CmdPDelay,
	CmdPDelay, CmdPDelay, CmdPDelay, CmdPDelay, CmdPDelay, CmdPDelay, 
	CmdPDelay, CmdPDelay, CmdPDelay, CmdPDelay, CmdPDelay, CmdPDelay, 
	CmdPDelay, CmdPDelay
};


const uint8_t CmdLengths[] = { // Starting at command 0xb4
// b4 b5 b6
	1, 1, 1,
// b7 b8 b9 ba bb bc bd be bf c0 c1 c2 c3 c4 c5 c6
	1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
// c7 c8 c9 ca cb cc cd ce cf d0 d1 d2 d3 d4 d5 d6
	2, 2, 3, 2, 5, 2, 2, 2, 3, 4, 1, 1, 1, 5, 5, 1,
// d7 d8 d9 da db dc dd de df e0 e1 e2 e3 e4 e5 e6
	1, 0, 0, 0, 5, 3, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, // 0 for calls, returns since the PC is changed by them already, and stop to prevent overflow. 
// e7 e8 e9 ea eb ec ed ee ef f0 f1 f2 f3 f4 f5 f6
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// f7 f8 f9 fa fb fc fd fe ff
	1, 1, 1, 1, 1, 1, 1, 1, 1
};

