// bare-metal functions
// we finally get to write to the registers

#define BG_MAP 0x00020000
#define BRTA 0x0005F824
#define BRTA_DEFAULT 42
#define BRTB_DEFAULT 84
#define BRTC_DEFAULT 0
#define CHAR_TBL 0x00078000
#define DPCTRL 0x0005f822
#define DP_DISP 0b0000000000000010
#define DP_RE 0b0000000100000000
#define DP_SYNCE 0b0000001000000000
#define FRMCYC 0x0005f82e
#define GPLT0 0x0005f860
#define INTCLR 0x0005f804
#define INTENB 0x0005f802
#define INTPND 0x0005f800
#define SDR_A 0b0000000000000100
#define SDR_B 0b0000000000001000
#define SDR_LD 0b0000010000000000
#define SDR_LL 0b0000001000000000
#define SDR_LR 0b0000000100000000
#define SDR_LU 0b0000100000000000
#define TIMER_TCR 0x02000020
#define TIMER_THR 0x0200001C
#define TIMER_TLR 0x02000018
#define VIP 0x00000000
#define VIP_END 0x00080000
#define VSU 0x01000000
#define VSU_END 0x01000800
#define VSU_S1INT 0x01000400
#define VSU_SSTOP 0x01000580
#define VSU_WAVE_0 0x01000000
#define WCR 0x02000024
#define WORLD_BGM_NORMAL 0b0000000000000000
#define WORLD_END 0b0000000001000000
#define WORLD_LON 0b1000000000000000
#define WORLD_RON 0b0100000000000000
#define WORLD_SCX_0 0b0000000000000000
#define WORLD_SCY_0 0b0000000000000000
#define WORLD_TBL 0x0003d800
#define WRAM 0x05000000
#define WRAM_END 0x05010000
#define XPCTRL 0x0005f842
#define XPSTTS 0x0005f840
#define XP_XPEN 0b0000000000000010
#define XP_XPRST 0b0000000000000001


//VSU register offsets
#define o_S1INT 0x00  // Channel 1 Sound Interval Specification Register
#define o_S1LRV 0x04  // Channel 1 Level Setting Register
#define o_S1FQL 0x08  // Channel 1 Frequency Setting Low Register
#define o_S1FQH 0x0C  // Channel 1 Frequency Setting High Register
#define o_S1EV0 0x10  // Channel 1 Envelope Specification Register 0
#define o_S1EV1 0x14  // Channel 1 Envelope Specification Register 1
#define o_S1RAM 0x18  // Channel 1 Base Address Setting Register

//Timer control bits
#define TIMER_CLKSEL 0x10 // Set to 0 for 100us intervals, 1 for 20us
#define TIMER_ZINT 0x8 // Set to 1 to enable the zero interrupt
#define TIMER_STATCLR 0x4 // Set to 1 to clear Z-Stat and acknowledge interrupt
#define TIMER_STAT 0x2 // Set to 1 when the timer counter becomes zero (read-only)
#define TIMER_TENB 0x1 // Set to 1 to enable timer