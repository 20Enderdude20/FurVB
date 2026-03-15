#ifndef _LIBGCCVB_AUDIO_H
#define _LIBGCCVB_AUDIO_H


#include <stdint.h>


typedef struct SOUNDREG
{
	// this table is for the most part untested, but looks to be accurate
	//                 |      D7      ||      D6      ||      D5      ||      D4      ||      D3      ||      D2      ||      D1      ||      D0      |
	uint8_t SxINT; //       [----Enable----][--XXXXXXXXXX--][-Interval/??--][--------------------------------Interval Data---------------------------------]
	uint8_t spacer1[3];
	uint8_t SxLRV; //       [---------------------------L Level----------------------------][---------------------------R Level----------------------------]
	uint8_t spacer2[3];
	uint8_t SxFQL; //       [------------------------------------------------------Frequency Low Byte------------------------------------------------------]
	uint8_t spacer3[3];
	uint8_t SxFQH; //       [--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--------------Frequency High Byte-------------]
	uint8_t spacer4[3];
	uint8_t SxEV0; //       [---------------------Initial Envelope Value-------------------][------U/D-----][-----------------Envelope Step----------------]
	uint8_t spacer5[3];
			 //Ch. 1-4 [--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][------R/S-----][----On/Off----]
			 //Ch. 5   [--XXXXXXXXXX--][------E/D-----][----?/Short---][--Mod./Sweep--][--XXXXXXXXXX--][--XXXXXXXXXX--][------R/S-----][----On/Off----]
	uint8_t SxEV1; //Ch. 6  [--XXXXXXXXXX--][----------------------E/D---------------------][--XXXXXXXXXX--][--XXXXXXXXXX--][------R/S-----][----On/Off----]
	uint8_t spacer6[3];
	// Ch. 1-5 only (I believe address is only 3 bits, but may be 4, needs testing)
	uint8_t SxRAM; //       [--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--------------Waveform RAM Address------------]
	uint8_t spacer7[3];
	// Ch. 5 only
	uint8_t S5SWP; //       [------CLK-----][-------------Sweep/Modulation Time------------][------U/D-----][----------------Number of Shifts--------------]
	uint8_t spacer8[35];
} SOUNDREG;

extern volatile uint8_t* const WAVEDATA1;
extern volatile uint8_t* const WAVEDATA2;
extern volatile uint8_t* const WAVEDATA3;
extern volatile uint8_t* const WAVEDATA4;
extern volatile uint8_t* const WAVEDATA5;
extern volatile uint8_t* const MODDATA;
extern volatile SOUNDREG* const SND_REGS;
#define SSTOP				*(uint8_t*)0x01000580

/***** Sound Register Mnemonics *****/
#define	WAVE1	0x00	// Voluntary wave channel #1
#define	WAVE2	0x01	// Voluntary wave channel #2
#define	WAVE3	0x02	// Voluntary wave channel #3
#define	WAVE4	0x03	// Voluntary wave channel #4
#define	SWEEP	0x04	// Sweep/modulation channel
#define	NOISE	0x05	// Pseudorandom noise channel


#endif