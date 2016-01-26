/*
 * Exported with nin10kit v1.1
 * Invocation command was nin10kit -mode4 screen lose.png title.png win.png 
 * Time-stamp: Monday 11/09/2015, 04:28:11
 * 
 * Image Information
 * -----------------
 * lose.png 240@160
 * title.png 240@160
 * win.png 240@160
 * 
 * Quote/Fortune of the Day!
 * -------------------------
 * 
 * All bug reports / feature requests are to be sent to Brandon (bwhitehead0308@gmail.com)
 */

#ifndef SCREEN_H
#define SCREEN_H

extern const unsigned short screen_palette[255];
#define SCREEN_PALETTE_SIZE 255

extern const unsigned short lose[19200];
#define LOSE_SIZE 19200
#define LOSE_WIDTH 240
#define LOSE_HEIGHT 160

extern const unsigned short title[19200];
#define TITLE_SIZE 19200
#define TITLE_WIDTH 240
#define TITLE_HEIGHT 160

extern const unsigned short win[19200];
#define WIN_SIZE 19200
#define WIN_WIDTH 240
#define WIN_HEIGHT 160

#endif

