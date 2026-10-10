volatile unsigned char* const STDOUT = (unsigned char*) 0x02000030;

int putchar(int ch) {
    *STDOUT = ch;
    return ch;
}

int puts(const char *str) {
    while (*str != 0) {
        *STDOUT = *str;
        str++;
    }
    *STDOUT = '\n';
    return 0;
}

int putsnt(const char *str) {
    while (*str != 0) {
        *STDOUT = *str;
        str++;
    }
    return 0;
}

int putn_ix(int num, bool newline) {
    putsnt("0x");
    for (int i = 28; i >= 0; i -= 4)
        *STDOUT = "0123456789ABCDEF"[(num >> i) & 0xF];
    if (newline) {
        *STDOUT = '\n';
    }
    return 0;
}

int putn_sx(short num, bool newline) {
    putsnt("0x");
    for (int i = 12; i >= 0; i -= 4)
        *STDOUT = "0123456789ABCDEF"[(num >> i) & 0xF];
    if (newline) {
        *STDOUT = '\n';
    }
    return 0;
}

int putn_bx(char num, bool newline) {
    putsnt("0x");
    for (int i = 4; i >= 0; i -= 4)
        *STDOUT = "0123456789ABCDEF"[(num >> i) & 0xF];
    if (newline) {
        *STDOUT = '\n';
    }
    return 0;
}




void pchannelstats(){
        putsnt("\n\n\n\n\n\n\n\n\n\n\n\n");
    	puts("Channel States:");
		puts("\t\t  ch 1 \t ch 2 \t ch 3 \t ch 4 \t ch 5 \t ch 6");
		putsnt("SxINT:\t");
		putn_bx(SND_REGS_WRAM[0].SxINT, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[1].SxINT, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[2].SxINT, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[3].SxINT, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[4].SxINT, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[5].SxINT, true);
		putsnt("SxLRV:\t");
		putn_bx(SND_REGS_WRAM[0].SxLRV, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[1].SxLRV, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[2].SxLRV, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[3].SxLRV, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[4].SxLRV, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[5].SxLRV, true);
		putsnt("SxFQ:\t ");
		putn_sx(SND_REGS_WRAM[0].SxFQL + (SND_REGS_WRAM[0].SxFQH << 8), false);
		putchar('\t');
		putn_sx(SND_REGS_WRAM[1].SxFQL + (SND_REGS_WRAM[1].SxFQH << 8), false);
		putchar('\t');
		putn_sx(SND_REGS_WRAM[2].SxFQL + (SND_REGS_WRAM[2].SxFQH << 8), false);
		putchar('\t');
		putn_sx(SND_REGS_WRAM[3].SxFQL + (SND_REGS_WRAM[3].SxFQH << 8), false);
		putchar('\t');
		putn_sx(SND_REGS_WRAM[4].SxFQL + (SND_REGS_WRAM[4].SxFQH << 8), false);
		putchar('\t');
		putn_sx(SND_REGS_WRAM[5].SxFQL + (SND_REGS_WRAM[5].SxFQH << 8), true);
		putsnt("SxEV0:\t");
		putn_bx(SND_REGS_WRAM[0].SxEV0, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[1].SxEV0, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[2].SxEV0, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[3].SxEV0, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[4].SxEV0, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[5].SxEV0, true);
		putsnt("SxEV1:\t");
		putn_bx(SND_REGS_WRAM[0].SxEV1, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[1].SxEV1, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[2].SxEV1, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[3].SxEV1, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[4].SxEV1, false);
		putsnt("\t  ");
		putn_bx(SND_REGS_WRAM[5].SxEV1, true);
}