#include "mylib.h"
#include "font.h"

#include "playscreen.h"
#include "screen.h"

#include <debugging.h>
//#include <stdlib.h> // Include this because we are using rand
#include <stdio.h> // Include this header because we are using sprintf



void waitForVblank();
void setPalette(const u16 *p, int size);
void drawImage4(int r, int c, int width, int height, const u16* image);
void setPixel4(int row, int col, u8 index);
void drawRect4(int row, int col, int height, int width, u8 index);
void fillScreen4(u8 color);
void flipPage();
void drawChar4(int row, int col, char ch, u8 index);
void drawString4(int row, int col, char *str, u8 index);
int play();
void drawPlayer();
void drawEnemy();
void delay(int n);

enum GBAState {
	START,
	GAME,
	WIN,
	LOSE
};

typedef struct player {
	int x;
	int y;
	int life;
}Object;

typedef struct enemy {
	int x;
	int y;
}Object2;

Object person = {146, 226, 3};
Object2 creeper1 = {120, -20};
Object2 creeper2 = {120, -36};
Object2 pigman1 = {104, -250};
Object2 skeleton1 = {88, -100};
Object2 spider1 = {68, -380};
Object2 spider2 = {68, -398};
Object2 zombie1 = {47, -5};
Object2 zombie2 = {47, -21};
Object2 zombie3 = {47, -37};
Object2 golem1 = {31, -300};
Object2 wolf1 = {15, -150};

int main(void) {

	

	for (int i = 256; i < PLAYSCREEN_PALETTE_SIZE; i++) {
		PALETTE[i] = playscreen_palette[i];
	}

	REG_DISPCTL = MODE4 | BG2_ENABLE | BUFFER1FLAG;

	enum GBAState state = START;



	while(1) {

		switch(state) {
			case START:
				for (int i = 0; i < SCREEN_PALETTE_SIZE; i++) {
					PALETTE[i] = screen_palette[i];
				}
				drawImage4(0, 0, TITLE_WIDTH, TITLE_HEIGHT, title);	

				person.x = 146;
				person.y = 226;
				person.life = 3;
				creeper1.x = 120;
				creeper1.y = -20;
				creeper2.x = 120;
				creeper2.y = -36;
				pigman1.x = 104;
				pigman1.y = -250;
				skeleton1.x = 88;
				skeleton1.y = -100;
				spider1.x = 68;
				spider1.y = -380;
				spider2.x = 68;
				spider2.y = -398;
				zombie1.x = 47;
				zombie1.y = -5;
				zombie2.x = 47;
				zombie2.y = -21;
				zombie3.x = 47;
				zombie3.y = -37;
				golem1.x = 31;
				golem1.y = -300;
				wolf1.x = 15;
				wolf1.y = -150;

				if(KEY_DOWN_NOW(BUTTON_START)) {
					state = GAME;
					setPalette(playscreen_palette, PLAYSCREEN_PALETTE_SIZE);
					drawImage4(0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT, background);
					flipPage();
				}
				break;

			case GAME:
				state = play();
				break;

			case WIN:
				for (int i = 0; i < SCREEN_PALETTE_SIZE; i++) {
					PALETTE[i] = screen_palette[i];
				}
				drawImage4(0, 0, WIN_WIDTH, WIN_HEIGHT, win);
				if(KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = START;
				}
				break;

			case LOSE:
				for (int i = 0; i < SCREEN_PALETTE_SIZE; i++) {
					PALETTE[i] = screen_palette[i];
				}
				drawImage4(0, 0, LOSE_WIDTH, LOSE_HEIGHT, lose);
				if(KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = START;
				}
				break;

		}
		waitForVblank();
		flipPage();
		
	}
	return 0;
}

int play() {
	drawPlayer();
	while(1) {
		drawImage4(0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT, background);
		//drawEnemy();

		char str[15];
		drawString4(4, 8, "Life: ", PALETTE[7]);
		sprintf (str, "%d", person.life);
		drawString4(4, 45, str, PALETTE[7]);

		if (KEY_DOWN_NOW(BUTTON_LEFT)) {
			person.x = person.x - 1;
			drawPlayer();
			flipPage();
		} else if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
			person.x = person.x + 1;
			drawPlayer();
			flipPage();
		} else if (KEY_DOWN_NOW(BUTTON_UP)) {
			person.y = person.y - 1;
			drawPlayer();
			flipPage();
		} else if (KEY_DOWN_NOW(BUTTON_DOWN)) {
			person.y = person.y + 1;
			drawPlayer();
			flipPage();
		}
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			return START;
		}

		if ((person.y < 226 || person.y >= 0) && (person.x == 0)) {
			return WIN;
		}

		// if ((((person.x <= creeper1.x) && (person.x+14 >= creeper1.x)) && (person.y == creeper1.y+16)) || 
		// (((person.x <= creeper1.x+16) && (person.x+14 >= creeper1.x+16)) && (person.y == creeper1.y+16)) || 
		// (((person.y <= creeper1.y+16) && (person.y+14 >= creeper1.y+16)) && (person.x == creeper1.x+16)) || 
		// (((person.y <= creeper1.y) && (person.y+14 >= creeper1.y)) && (person.x == creeper1.x+16)) || 
		// (((person.y <= creeper1.y+16) && (person.y+14 >= creeper1.y+16)) && (person.x+14 == creeper1.x)) || 
		// (((person.y <= creeper1.y) && (person.y+14 >= creeper1.y)) && (person.x+14 == creeper1.x)) || 
		// (((person.x <= creeper1.x) && (person.x+14 >= creeper1.x)) && (person.y+14 == creeper1.y)) || 			
		// (((person.x <= creeper1.x+16) && (person.x+14 >= creeper1.x+16)) && (person.y+14 == creeper1.y)) ||

		// (((person.x <= creeper2.x) && (person.x+14 >= creeper2.x)) && (person.y == creeper2.y+16)) || 
		// (((person.x <= creeper2.x+16) && (person.x+14 >= creeper2.x+16)) && (person.y == creeper2.y+16)) || 
		// (((person.y <= creeper2.y+16) && (person.x+14 >= creeper2.y+16)) && (person.x == creeper2.x+16)) || 
		// (((person.y <= creeper2.y) && (person.x+14 >= creeper2.y)) && (person.x == creeper2.x+16)) || 
		// (((person.y <= creeper2.y+16) && (person.x+14 >= creeper2.y+16)) && (person.x+14 == creeper2.x)) || 
		// (((person.y <= creeper2.y) && (person.x+14 >= creeper2.y)) && (person.x+14 == creeper2.x)) || 
		// (((person.x <= creeper2.x) && (person.x+14 >= creeper2.x)) && (person.x+14 == creeper2.y)) || 
		// (((person.x <= creeper2.x+16) && (person.x+14 >= creeper2.x+16)) && (person.x+14 == creeper2.y)) ||

		// (((person.x <= pigman1.x) && (person.x+14 >= pigman1.x)) && (person.y == pigman1.y+16)) || 
		// (((person.x <= pigman1.x+16) && (person.x+14 >= pigman1.x+16)) && (person.y == pigman1.y+16)) || 
		// (((person.y <= pigman1.y+16) && (person.x+14 >= pigman1.y+16)) && (person.x == pigman1.x+16)) || 
		// (((person.y <= pigman1.y) && (person.x+14 >= pigman1.y)) && (person.x == pigman1.x+16)) || 
		// (((person.y <= pigman1.y+16) && (person.x+14 >= pigman1.y+16)) && (person.x+14 == pigman1.x)) || 
		// (((person.y <= pigman1.y) && (person.x+14 >= pigman1.y)) && (person.x+14 == pigman1.x)) || 
		// (((person.x <= pigman1.x) && (person.x+14 >= pigman1.x)) && (person.x+14 == pigman1.y)) || 
		// (((person.x <= pigman1.x+16) && (person.x+14 >= pigman1.x+16)) && (person.x+14 == pigman1.y)) ||

		// (((person.x <= skeleton1.x) && (person.x+14 >= skeleton1.x)) && (person.y == skeleton1.y+16)) || 
		// (((person.x <= skeleton1.x+16) && (person.x+14 >= skeleton1.x+16)) && (person.y == skeleton1.y+16)) || 
		// (((person.y <= skeleton1.y+16) && (person.x+14 >= skeleton1.y+16)) && (person.x == skeleton1.x+16)) || 
		// (((person.y <= skeleton1.y) && (person.x+14 >= skeleton1.y)) && (person.x == skeleton1.x+16)) || 
		// (((person.y <= skeleton1.y+16) && (person.x+14 >= skeleton1.y+16)) && (person.x+14 == skeleton1.x)) || 
		// (((person.y <= skeleton1.y) && (person.x+14 >= skeleton1.y)) && (person.x+14 == skeleton1.x)) || 
		// (((person.x <= skeleton1.x) && (person.x+14 >= skeleton1.x)) && (person.x+14 == skeleton1.y)) || 
		// (((person.x <= skeleton1.x+16) && (person.x+14 >= skeleton1.x+16)) && (person.x+14 == skeleton1.y)) ||	

		// (((person.x <= spider1.x) && (person.x+14 >= spider1.x)) && (person.y == spider1.y+16)) || 
		// (((person.x <= spider1.x+18) && (person.x+14 >= spider1.x+18)) && (person.y == spider1.y+16)) || 
		// (((person.y <= spider1.y+16) && (person.x+14 >= spider1.y+16)) && (person.x == spider1.x+18)) || 
		// (((person.y <= spider1.y) && (person.x+14 >= spider1.y)) && (person.x == spider1.x+18)) || 
		// (((person.y <= spider1.y+16) && (person.x+14 >= spider1.y+16)) && (person.x+14 == spider1.x)) || 
		// (((person.y <= spider1.y) && (person.x+14 >= spider1.y)) && (person.x+14 == spider1.x)) || 
		// (((person.x <= spider1.x) && (person.x+14 >= spider1.x)) && (person.x+14 == spider1.y)) || 
		// (((person.x <= spider1.x+18) && (person.x+14 >= spider1.x+18)) && (person.x+14 == spider1.y)) ||	

		// (((person.x <= spider2.x) && (person.x+14 >= spider2.x)) && (person.y == spider2.y+16)) || 
		// (((person.x <= spider2.x+18) && (person.x+14 >= spider2.x+18)) && (person.y == spider2.y+16)) || 
		// (((person.y <= spider2.y+16) && (person.x+14 >= spider2.y+16)) && (person.x == spider2.x+18)) || 
		// (((person.y <= spider2.y) && (person.x+14 >= spider2.y)) && (person.x == spider2.x+18)) || 
		// (((person.y <= spider2.y+16) && (person.x+14 >= spider2.y+16)) && (person.x+14 == spider2.x)) || 
		// (((person.y <= spider2.y) && (person.x+14 >= spider2.y)) && (person.x+14 == spider2.x)) || 
		// (((person.x <= spider2.x) && (person.x+14 >= spider2.x)) && (person.x+14 == spider2.y)) || 
		// (((person.x <= spider2.x+18) && (person.x+14 >= spider2.x+18)) && (person.x+14 == spider2.y)) ||

		// (((person.x <= zombie1.x) && (person.x+14 >= zombie1.x)) && (person.y == zombie1.y+16)) || 
		// (((person.x <= zombie1.x+16) && (person.x+14 >= zombie1.x+16)) && (person.y == zombie1.y+16)) || 
		// (((person.y <= zombie1.y+16) && (person.x+14 >= zombie1.y+16)) && (person.x == zombie1.x+16)) || 
		// (((person.y <= zombie1.y) && (person.x+14 >= zombie1.y)) && (person.x == zombie1.x+16)) || 
		// (((person.y <= zombie1.y+16) && (person.x+14 >= zombie1.y+16)) && (person.x+14 == zombie1.x)) || 
		// (((person.y <= zombie1.y) && (person.x+14 >= zombie1.y)) && (person.x+14 == zombie1.x)) || 
		// (((person.x <= zombie1.x) && (person.x+14 >= zombie1.x)) && (person.x+14 == zombie1.y)) || 
		// (((person.x <= zombie1.x+16) && (person.x+14 >= zombie1.x+16)) && (person.x+14 == zombie1.y)) ||

		// (((person.x <= zombie2.x) && (person.x+14 >= zombie2.x)) && (person.y == zombie2.y+16)) || 
		// (((person.x <= zombie2.x+16) && (person.x+14 >= zombie2.x+16)) && (person.y == zombie2.y+16)) || 
		// (((person.y <= zombie2.y+16) && (person.x+14 >= zombie2.y+16)) && (person.x == zombie2.x+16)) || 
		// (((person.y <= zombie2.y) && (person.x+14 >= zombie2.y)) && (person.x == zombie2.x+16)) || 
		// (((person.y <= zombie2.y+16) && (person.x+14 >= zombie2.y+16)) && (person.x+14 == zombie2.x)) || 
		// (((person.y <= zombie2.y) && (person.x+14 >= zombie2.y)) && (person.x+14 == zombie2.x)) || 
		// (((person.x <= zombie2.x) && (person.x+14 >= zombie2.x)) && (person.x+14 == zombie2.y)) || 
		// (((person.x <= zombie2.x+16) && (person.x+14 >= zombie2.x+16)) && (person.x+14 == zombie2.y)) ||

		// (((person.x <= zombie3.x) && (person.x+14 >= zombie3.x)) && (person.y == zombie3.y+16)) || 
		// (((person.x <= zombie3.x+16) && (person.x+14 >= zombie3.x+16)) && (person.y == zombie3.y+16)) || 
		// (((person.y <= zombie3.y+16) && (person.x+14 >= zombie3.y+16)) && (person.x == zombie3.x+16)) || 
		// (((person.y <= zombie3.y) && (person.x+14 >= zombie3.y)) && (person.x == zombie3.x+16)) || 
		// (((person.y <= zombie3.y+16) && (person.x+14 >= zombie3.y+16)) && (person.x+14 == zombie3.x)) || 
		// (((person.y <= zombie3.y) && (person.x+14 >= zombie3.y)) && (person.x+14 == zombie3.x)) || 
		// (((person.x <= zombie3.x) && (person.x+14 >= zombie3.x)) && (person.x+14 == zombie3.y)) || 
		// (((person.x <= zombie3.x+16) && (person.x+14 >= zombie3.x+16)) && (person.x+14 == zombie3.y)) ||

		// (((person.x <= golem1.x) && (person.x+14 >= golem1.x)) && (person.y == golem1.y+16)) || 
		// (((person.x <= golem1.x+20) && (person.x+14 >= golem1.x+20)) && (person.y == golem1.y+16)) || 
		// (((person.y <= golem1.y+16) && (person.x+14 >= golem1.y+16)) && (person.x == golem1.x+20)) || 
		// (((person.y <= golem1.y) && (person.x+14 >= golem1.y)) && (person.x == golem1.x+20)) || 
		// (((person.y <= golem1.y+16) && (person.x+14 >= golem1.y+16)) && (person.x+14 == golem1.x)) || 
		// (((person.y <= golem1.y) && (person.x+14 >= golem1.y)) && (person.x+14 == golem1.x)) || 
		// (((person.x <= golem1.x) && (person.x+14 >= golem1.x)) && (person.x+14 == golem1.y)) || 
		// (((person.x <= golem1.x+20) && (person.x+14 >= golem1.x+20)) && (person.x+14 == golem1.y)) ||

		// (((person.x <= wolf1.x) && (person.x+14 >= wolf1.x)) && (person.y == wolf1.y+16)) || 
		// (((person.x <= wolf1.x+18) && (person.x+14 >= wolf1.x+18)) && (person.y == wolf1.y+16)) || 
		// (((person.y <= wolf1.y+16) && (person.x+14 >= wolf1.y+16)) && (person.x == wolf1.x+18)) || 
		// (((person.y <= wolf1.y) && (person.x+14 >= wolf1.y)) && (person.x == wolf1.x+18)) || 
		// (((person.y <= wolf1.y+16) && (person.x+14 >= wolf1.y+16)) && (person.x+14 == wolf1.x)) || 
		// (((person.y <= wolf1.y) && (person.x+14 >= wolf1.y)) && (person.x+14 == wolf1.x)) || 
		// (((person.x <= wolf1.x) && (person.x+14 >= wolf1.x)) && (person.x+14 == wolf1.y)) || 
		// (((person.x <= wolf1.x+18) && (person.x+14 >= wolf1.x+18)) && (person.x+14 == wolf1.y))) {
		if(((person.x < creeper1.x + 17) && (person.x > creeper1.x -1) && (person.y < creeper1.y + 17) && (person.y > creeper1.y -1)) ||
		   ((person.x < creeper2.x + 17) && (person.x > creeper2.x -1) && (person.y < creeper2.y + 17) && (person.y > creeper2.y -1)) ||
		   ((person.x < pigman1.x + 17) && (person.x > pigman1.x -1) && (person.y < pigman1.y + 17) && (person.y > pigman1.y -1)) ||
		   ((person.x < spider1.x + 19) && (person.x > spider1.x -1) && (person.y < spider1.y + 17) && (person.y > spider1.y -1)) ||
		   ((person.x < skeleton1.x + 17) && (person.x > skeleton1.x -1) && (person.y < skeleton1.y + 17) && (person.y > skeleton1.y -1)) ||
		   ((person.x < spider2.x + 19) && (person.x > spider2.x -1) && (person.y < spider2.y + 17) && (person.y > spider2.y -1)) ||
		   ((person.x < zombie1.x + 17) && (person.x > zombie1.x -1) && (person.y < zombie1.y + 17) && (person.y > zombie1.y -1)) ||
		   ((person.x < zombie2.x + 17) && (person.x > zombie2.x -1) && (person.y < zombie2.y + 17) && (person.y > zombie2.y -1)) ||
		   ((person.x < zombie3.x + 17) && (person.x > zombie3.x -1) && (person.y < zombie3.y + 17) && (person.y > zombie3.y -1)) ||
		   ((person.x < golem1.x + 21) && (person.x > golem1.x -1) && (person.y < golem1.y + 17) && (person.y > golem1.y -1)) ||
		   ((person.x < wolf1.x + 19) && (person.x > wolf1.x -1) && (person.y < wolf1.y + 17) && (person.y > wolf1.y -1))) {

			person.life -= 1;
			person.x = 146;
			person.y = 226;
			drawPlayer();

			if (person.life == 0) {
				return LOSE;
			} 
		}

		drawEnemy();



		// creeper1.x %= 160;
		// pigman1.x %= 160;
		// skeleton1.x %= 160;
		// spider1.x %= 160;
		// wolf1.x %= 160;
		// golem1.x %= 160;
		// zombie1.x %= 160;

		creeper1.y %= 240;
		creeper2.y %= 240;
		pigman1.y %= 240;
		skeleton1.y %= 240;
		spider1.y %= 240;
		spider2.y %= 240;
		wolf1.y %= 240;
		golem1.y %= 240;
		zombie1.y %= 240;
		zombie2.y %= 240;
		zombie3.y %= 240;

	}
	return 0;
}

void drawPlayer() {
	if (person.x < 0) {
		person.x = 0;
	} else if (person.x > 146) {
		person.x = 146;
	}
	
	if (person.y < 0) {
		person.y = 0;
	} else if (person.y > 226) {
		person.y = 226;
	}
	drawImage4(person.x, person.y, PLAYER1_WIDTH, PLAYER1_HEIGHT, player1);
	flipPage();
	drawImage4(person.x, person.y, PLAYER2_WIDTH, PLAYER2_HEIGHT, player2);
	flipPage();
	drawImage4(person.x, person.y, PLAYER3_WIDTH, PLAYER3_HEIGHT, player3);
	flipPage();
	drawImage4(person.x, person.y, PLAYER4_WIDTH, PLAYER4_HEIGHT, player4);
	flipPage();
	//delay(1);
	drawImage4(person.x, person.y, PLAYER1_WIDTH, PLAYER1_HEIGHT, player1);
	flipPage();
}

void drawEnemy() {
	creeper1.y += 1;
	creeper2.y += 1;
	pigman1.y += 1;
	skeleton1.y += 1;
	spider1.y += 1;
	spider2.y += 1;
	zombie1.y += 1;
	zombie2.y += 1;
	zombie3.y += 1;
	golem1.y += 1;
	wolf1.y += 1;
	delay(1);
	//waitForVblank();

	drawImage4(person.x, person.y, PLAYER1_WIDTH, PLAYER1_HEIGHT, player1);

	drawImage4(creeper1.x, creeper1.y, CREEPER_WIDTH, CREEPER_HEIGHT, creeper);
	drawImage4(creeper2.x, creeper2.y, CREEPER_WIDTH, CREEPER_HEIGHT, creeper);
	drawImage4(pigman1.x, pigman1.y, PIGMAN_WIDTH, PIGMAN_HEIGHT, pigman);
	drawImage4(skeleton1.x, skeleton1.y, SKELETON_WIDTH, SKELETON_HEIGHT, skeleton);
	drawImage4(spider1.x, spider1.y, SPIDER_WIDTH, SPIDER_HEIGHT, spider);
	drawImage4(spider2.x, spider2.y, SPIDER_WIDTH, SPIDER_HEIGHT, spider);
	drawImage4(zombie1.x, zombie1.y, ZOMBIE_WIDTH, ZOMBIE_HEIGHT, zombie);
	drawImage4(zombie2.x, zombie2.y, ZOMBIE_WIDTH, ZOMBIE_HEIGHT, zombie);
	drawImage4(zombie3.x, zombie3.y, ZOMBIE_WIDTH, ZOMBIE_HEIGHT, zombie);
	drawImage4(golem1.x, golem1.y, GOLEM_WIDTH, GOLEM_HEIGHT, golem);
	drawImage4(wolf1.x, wolf1.y, WOLF_WIDTH, WOLF_HEIGHT, wolf);
	flipPage();
}

void delay(int n) {
	volatile int x = 0;
	for (int i = 0; i < n*5000; i++) {
		x++;
	} 	
}