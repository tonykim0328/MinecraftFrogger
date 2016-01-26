#include "mylib.h"

u16 *videoBuffer = (u16 *) 0x6000000;
u16 __key_curr = 0, __key_prev = 0;


void waitForVblank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void drawImage4(int r, int c, int width, int height, const u16* image) {
	int i;
	for (i = 0; i < height; i++) {
		DMA[3].src = &image[OFFSET(i, 0, width / 2)];
		DMA[3].dst = videoBuffer + OFFSET(r + i, c, WIDTH)/2;
		DMA[3].cnt = (width/2) | DMA_ON;
	}
}


void setPixel4(int row, int col, u8 index) {
	int whichPixel = OFFSET(row, col, 240);
	int whichShort = whichPixel/2;
	if (col & 1) {
		videoBuffer[whichShort] = (videoBuffer[whichShort] & 0x00FF) | (index << 8);
	} else {
		videoBuffer[whichShort] = (videoBuffer[whichShort] & 0xFF00) | (index);
	}
}

void drawRect4(int row, int col, int height, int width, u8 index) {
	volatile u16 color = index | (index << 8);
	int i;
	for(i = 0; i < height; i++) {
		DMA[3].src = &color;
		DMA[3].dst = videoBuffer + OFFSET(row + i, col, WIDTH)/2;	
		DMA[3].cnt = (width/2) | DMA_ON | DMA_SOURCE_FIXED;
	}
}

void fillScreen4(u8 index) {
	volatile u16 color = index | (index << 8);
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = ((WIDTH * HEIGHT) / 2) | DMA_ON | DMA_SOURCE_FIXED;
}

void flipPage() {
	if(REG_DISPCTL & BUFFER1FLAG) {
		// We have been displaying BUFFER1
		REG_DISPCTL &= ~BUFFER1FLAG;
		videoBuffer = BUFFER1;
	} else {
		// WE have been displaying BUFFER0
		REG_DISPCTL |= BUFFER1FLAG;
		videoBuffer = BUFFER0;
	}
}

void setPalette(const u16 *p, int size){
	int i;
	for(i=0;i<size;i++)
	{
		DMA[3].src = &p[i];
		DMA[3].dst = &PALETTE[i];
		DMA[3].cnt = size | DMA_ON| DMA_SOURCE_FIXED;
	}
}