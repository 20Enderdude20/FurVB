#include <stdint.h>
#include "audio.h"


volatile uint8_t* const WAVEDATA1 =		(uint8_t*)0x01000000;
volatile uint8_t* const WAVEDATA2 =		(uint8_t*)0x01000080;
volatile uint8_t* const WAVEDATA3 =		(uint8_t*)0x01000100;
volatile uint8_t* const WAVEDATA4 =		(uint8_t*)0x01000180;
volatile uint8_t* const WAVEDATA5 =		(uint8_t*)0x01000200;
volatile uint8_t* const MODDATA =		(uint8_t*)0x01000280;
volatile SOUNDREG* const SND_REGS =	(SOUNDREG*)0x01000400;