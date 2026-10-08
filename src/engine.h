#define MAC_VOL 0
#define MAC_ARP 1
#define MAC_NOISEL 2
#define MAC_WAVE 3
#define MAC_PANL 4
#define MAC_PANR 5
#define MAC_PITCH 6
#define MAC_PHASE 7

#define MAX_STACKDEPTH 13
typedef struct {
	uint32_t PC;
	uint8_t ChannelStackPtr;
	uint32_t ChannelDataStack[MAX_STACKDEPTH];
} ChannelDataRegisters;

typedef struct {
	short note, oldNote, lastIns, pitch, portaSpeed, portaNote;
	short volume, volSpeed, volSpeedTarget, cut, volCut, legatoDelay, legatoTarget, rowDelay, volMax;
	uint16_t wait, portaTemp, baseFreq;
	bool portaSign;
	short vibratoDepth, vibratoRate, vibratoPos, vibratoShape, vibratoFine;
	short tremoloDepth, tremoloRate, tremoloPos;
	short panDepth, panRate, panPos, panSpeed;
	uint8_t arp, arpStage, arpTicks, panL, panR, panRL, panRR, lastVibrato, lastPorta, cutType;
	bool doNote, legato, portaStop, keyOn, keyOff, stopOnOff, releasing;
	bool arpYield, delayLocked, inPorta, scheduledSlideReset, shorthandPorta, wasShorthandPorta, noteOnInhibit, resetArp, sampleOffSet;
	bool wentThroughNote, goneThroughNote;
} ChannelState;
void ChannelState_Init(ChannelState* ch_s) {
	ch_s->wait = 0;
	ch_s->note = -1;
	ch_s->portaTemp = 0;
	ch_s->oldNote = -1;
	ch_s->lastIns = -1;
	ch_s->pitch = 0;
	ch_s->portaSpeed = -1;
	ch_s->portaNote = -1;
	ch_s->volume = 0xf00;
	ch_s->volSpeed = 0;
	ch_s->volSpeedTarget = -1;
	ch_s->cut = -1;
	ch_s->volCut = -1;
	ch_s->legatoDelay = -1;
	ch_s->legatoTarget = 0;
	ch_s->rowDelay = 0;
	ch_s->volMax = 0xfff;
	ch_s->vibratoDepth = 0;
	ch_s->vibratoRate = 0;
	ch_s->vibratoPos = 0;
	ch_s->vibratoShape = 0;
	ch_s->vibratoFine = 15;
	ch_s->tremoloDepth = 0;
	ch_s->tremoloRate = 0;
	ch_s->tremoloPos = 0;
	ch_s->panDepth = 0;
	ch_s->panRate = 0;
	ch_s->panPos = 0;
	ch_s->panSpeed = 0;
	ch_s->arp = 0;
	ch_s->arpStage = -1;
	ch_s->arpTicks = 1;
	ch_s->panL = 255;
	ch_s->panR = 255;
	ch_s->panRL = 0;
	ch_s->panRR = 0;
	ch_s->lastVibrato = 0;
	ch_s->lastPorta = 0;
	ch_s->cutType = 0;
	ch_s->doNote = false;
	ch_s->legato = false;
	ch_s->portaStop = false;
	ch_s->keyOn = false;
	ch_s->keyOff = false;
	ch_s->stopOnOff = false;
	ch_s->releasing = false;
	ch_s->arpYield = false;
	ch_s->delayLocked = false;
	ch_s->inPorta = false;
	ch_s->scheduledSlideReset = false;
	ch_s->shorthandPorta = false;
	ch_s->wasShorthandPorta = false;
	ch_s->noteOnInhibit = false;
	ch_s->resetArp = false;
	ch_s->sampleOffSet = false;
	ch_s->wentThroughNote = false;
	ch_s->goneThroughNote = false;
	return;
}

typedef struct SOUNDREGSHORT
{
	// this table is for the most part untested, but looks to be accurate
	//                 |      D7      ||      D6      ||      D5      ||      D4      ||      D3      ||      D2      ||      D1      ||      D0      |
	u8 SxINT; //       [----Enable----][--XXXXXXXXXX--][-Interval/??--][--------------------------------Interval Data---------------------------------]
	u8 SxLRV; //       [---------------------------L Level----------------------------][---------------------------R Level----------------------------]
	u8 SxFQL; //       [------------------------------------------------------Frequency Low Byte------------------------------------------------------]
	u8 SxFQH; //       [--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--------------Frequency High Byte-------------]
	u8 SxEV0; //       [---------------------Initial Envelope Value-------------------][------U/D-----][-----------------Envelope Step----------------]
			 //Ch. 1-4 [--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][------R/S-----][----On/Off----]
			 //Ch. 5   [--XXXXXXXXXX--][------E/D-----][----?/Short---][--Mod./Sweep--][--XXXXXXXXXX--][--XXXXXXXXXX--][------R/S-----][----On/Off----]
	u8 SxEV1; //Ch. 6  [--XXXXXXXXXX--][----------------------E/D---------------------][--XXXXXXXXXX--][--XXXXXXXXXX--][------R/S-----][----On/Off----]
	// Ch. 1-5 only (I believe address is only 3 bits, but may be 4, needs testing)
	u8 SxRAM; //       [--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--XXXXXXXXXX--][--------------Waveform RAM Address------------]
	// Ch. 5 only
	u8 S5SWP; //       [------CLK-----][-------------Sweep/Modulation Time------------][------U/D-----][----------------Number of Shifts--------------]
} SOUNDREGSHORT;

typedef struct {
	short val[256];
	unsigned char len, delay, speed, loop, rel;
} InstrumentMacro;
typedef struct {
	bool usemodwave;
	char modwave[256];
	InstrumentMacro macro[8];
} Instrument;