////////////////////////////////////////////////////////////////////////////////////////
// Labyrinthino : an arduino project part of the M2 PFA Univers et particules courses //
// Written by : SI SAID Wassim Abdelraouf, CLAUSTRE Simon                             //
// Supervised by : Professor David CALVET                                             //
//                                                                                    //
// Project started on 10/10/2022 and ended on 11/01/2023 with a presentation of the f-//
// nal prototype in front of a jury for an evaluation that counts for the semester    //
////////////////////////////////////////////////////////////////////////////////////////

//Including libraries
#include <U8g2lib.h>
#include "Wire.h"
#include <EEPROM.h>

//////////// Variables definitions ////////////

//Arduinos IO initialization
#define pinY    A0
#define pinX    A1
#define OLED_RESET -1

//Joystick threshold/sensibility to movement
int sensibilite = 256;

//Player's start position
int PlayerPosI = 1;
int PlayerPosJ = 1;
int PlayerScore = 0;

//Player's goal position
int winningCaseX = 31;
int winningCaseY = 31;

//Loading the best score from the EEPROM
int BestPlayerScore = EEPROM.read(10);

//Maze frame and it's size
unsigned int mazeFrame[33][33];
unsigned int sizeOfmazeFrame = sizeof(mazeFrame) / sizeof(mazeFrame[0]);
unsigned int AdvancementMax = 520;

//Conditions to avoid drawing the same progress bar / Performance fix
bool at25 = false;
bool at50 = false;
bool at75 = false;

//Labyrinthino splash screen XBM in a bianry form
static unsigned char splash[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x63, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x80, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x31, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x81, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x80, 0x3b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x81,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xe0, 0x81, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x01, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x06, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x01,
  0x07, 0x00, 0x00, 0x00, 0x00, 0x06, 0x8f, 0x01, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xf0, 0x00, 0x07, 0x00, 0x00, 0x30, 0x00, 0x06, 0x87, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x06, 0x00, 0x00, 0x30,
  0x00, 0xe6, 0x07, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00,
  0x06, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x06, 0x00, 0x00, 0xf8, 0x00, 0x00,
  0x00, 0xc0, 0x7b, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x3e, 0x00,
  0x00, 0xfc, 0x01, 0x00, 0x00, 0xc0, 0x7f, 0x00, 0x06, 0x00, 0x00, 0x00,
  0x00, 0x3c, 0x3e, 0xe0, 0x1c, 0xdc, 0x01, 0x00, 0x00, 0x00, 0x1e, 0x00,
  0x0e, 0x00, 0xc0, 0x61, 0x70, 0x1c, 0x7e, 0xe3, 0x3e, 0x8c, 0x01, 0x00,
  0x00, 0x00, 0x1c, 0x00, 0x0e, 0xc0, 0xe0, 0x67, 0xf0, 0x0c, 0x6e, 0xc7,
  0x7f, 0x8e, 0x03, 0x00, 0x00, 0x00, 0x1c, 0x00, 0xfc, 0xd9, 0xfc, 0xe7,
  0xfe, 0x0c, 0x6e, 0xc7, 0xe7, 0x0e, 0x03, 0x00, 0x00, 0x00, 0x0c, 0xf8,
  0xfd, 0xdb, 0x7d, 0xee, 0xfe, 0x0c, 0x6e, 0x87, 0xe3, 0x0e, 0x03, 0x00,
  0x00, 0x00, 0x0c, 0xf8, 0xbd, 0xdf, 0x79, 0xce, 0xdc, 0x0d, 0x6c, 0x87,
  0xc3, 0x0e, 0x03, 0x00, 0x00, 0x00, 0x0c, 0xd8, 0x1d, 0xff, 0x71, 0xc0,
  0x98, 0x1d, 0xec, 0x87, 0xc3, 0x9e, 0x03, 0x00, 0x00, 0xe0, 0x0f, 0x9c,
  0x1d, 0xfe, 0xf3, 0xc0, 0x98, 0x1d, 0xec, 0x8f, 0xc3, 0xff, 0x01, 0x00,
  0x00, 0xe0, 0x0f, 0xcc, 0x3f, 0xfe, 0xe3, 0xc0, 0x98, 0x19, 0xee, 0x8f,
  0xc3, 0xff, 0x01, 0x00, 0x00, 0x60, 0x0f, 0xcc, 0x3f, 0xfc, 0xe3, 0xc0,
  0xb9, 0x19, 0xcf, 0xdd, 0x83, 0xe7, 0x00, 0x00, 0x00, 0x60, 0x1f, 0xfc,
  0x3f, 0x0e, 0xcf, 0xc0, 0xb9, 0x39, 0xc7, 0xfd, 0x81, 0x03, 0x00, 0x00,
  0x00, 0xe0, 0x3f, 0xfe, 0xf0, 0x07, 0xcf, 0x80, 0xb9, 0xbb, 0x03, 0xfc,
  0x80, 0x03, 0x00, 0x00, 0x00, 0xe0, 0x7b, 0x7f, 0xf0, 0xc7, 0x0f, 0x00,
  0x30, 0xf3, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x07,
  0xc0, 0xe0, 0x07, 0x00, 0x00, 0xf0, 0x01, 0x00, 0x0c, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xc0, 0x03, 0x00, 0xf0, 0x06, 0x00, 0x00, 0x60, 0x00, 0x00,
  0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x06, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x38, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x06, 0xfc, 0x00, 0x00, 0x03, 0x00,
  0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x06, 0xfe,
  0x01, 0x00, 0x83, 0xc7, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x18, 0x06, 0xfe, 0xc1, 0x01, 0xc0, 0xe7, 0x3d, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x06, 0xb6, 0xf9, 0xfd, 0xc6, 0xe7,
  0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x07, 0xb6,
  0xf9, 0xfc, 0xce, 0x66, 0xb8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xf8, 0x07, 0xf6, 0x79, 0xce, 0xfe, 0xe7, 0xb8, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0xf0, 0x71, 0xce, 0xfc, 0xe7,
  0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0,
  0x30, 0xdc, 0xfc, 0xfb, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x70, 0x70, 0xfc, 0xbe, 0xff, 0xef, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x78, 0x7f, 0x1f,
  0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
  0x00, 0x00, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x6e, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
  0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x20, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x20, 0x90, 0x00, 0x00, 0x20, 0x70, 0x38, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x90, 0x30, 0x00,
  0x20, 0x50, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x20, 0xb0, 0x78, 0x30, 0x20, 0x12, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x20, 0xa0, 0x4c, 0x60, 0x23, 0x12, 0x78, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xe0, 0x04, 0xc0,
  0x21, 0x12, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x40, 0x40, 0x04, 0x40, 0x20, 0x12, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x04, 0xe0, 0x40, 0x32, 0x9a, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x5e, 0xcc, 0xb0,
  0xc1, 0x22, 0x9a, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x43, 0x78, 0x80, 0x81, 0xe3, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x80, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//////////// Game logics ////////////

//Initialization of the display using U8G2 library
U8G2_SH1107_SEEED_128X128_F_SW_I2C display(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);


//Function that reset the arduino and restart the game from software
void(* resetFunc) (void) = 0;


//Function that generates the maze using the Kruskal's algorithm and storing it in the mazeFrame array variable
void mazeGenerator() {

  //initialization of the mazeFrame
  int n = 1;
  for (int  i = 0; i < sizeOfmazeFrame; i++) {
    for (int  j = 0; j < sizeOfmazeFrame; j++) {
      if ( i % 2 == 0 || j % 2 == 0 ) {
        mazeFrame[i][j] = 0;
      } else {
        mazeFrame[i][j] = n;
        n++;
      }
    }
  }

  //Generating the maze hallways by destroying random walls following Kruskal's algorithm rules
  for (int Advancement = 0; Advancement < AdvancementMax;) { //280
    int x = random(1, sizeOfmazeFrame - 1);
    int y = random(1, sizeOfmazeFrame - 1);
    if (x % 2 == 0 && y % 2 == 0) {
      //not filling the (pair,pair) cell's of the array.
    } else {
      //Picking a wall cell to merge two non wall cells and replacing the wall and the adjacent wall's with the lowest number of the 2 non walls cells.
      if (mazeFrame[x][y] == 0) {
        //picking a random dirrection to merge the cells 0 is vertical, 1 is horizontal
        if (random(0, 2) == 0) { //Vertical direction
          breakVerWall(x,y); //breaks vertical wall
          Advancement++; //Incrementing the advancement
        } else { //Horizontal direction
          breakHorWall(x,y); //breaks horizontal wall
          Advancement++; //Incrementing the advancement
        }
      }
    }
    float AdvancementPourcentage = (float(Advancement) / float(AdvancementMax) ) * 100.0; //calculating the advancement pourcetage
    loadingScreenUpdater(AdvancementPourcentage); //updating the loadingScreen with the new advancement
  }

  //Forcing the start cells to have empty adjacent cells to avoid any blocking start
  mazeFrame[1][2] = 1;
  mazeFrame[2][1] = 1;
  //Forcing the end cells to have empty adjacent cells to avoid any blocking end
  mazeFrame[30][31] = 1;
  mazeFrame[31][30] = 1;
}

//function that break walls vertically
void breakVerWall(int x, int y) {
  if (mazeFrame[x][y + 1] != mazeFrame[x][y - 1] && mazeFrame[x][y + 1] != 0 && mazeFrame[x][y - 1] != 0  ) {
    mazeFrame[x][y] = mazeFrame[x][y - 1];
    for (int  i = 0; i < sizeOfmazeFrame; i++) {
      for (int  j = 0; j < sizeOfmazeFrame; j++) {
        if ( mazeFrame[i][j] == mazeFrame[x][y + 1]) {
          mazeFrame[i][j] = mazeFrame[x][y - 1];
        }
      }
    }
  }
}

//function that break walls horizontally
void breakHorWall(int x, int y) {
  if (mazeFrame[x + 1][y] != mazeFrame[x - 1][y] && mazeFrame[x + 1][y ] != 0 && mazeFrame[x - 1][y] != 0 ) {
    mazeFrame[x][y] = mazeFrame[x - 1][y];
    for (int  i = 0; i < sizeOfmazeFrame - 1; i++) {
      for (int  j = 0; j < sizeOfmazeFrame - 1; j++) {
        if ( mazeFrame[i][j] == mazeFrame[x + 1][y]) {
          mazeFrame[i][j] = mazeFrame[x - 1][y];
        }
      }
    }
  }
}

//////////// Screen controll functions ////////////

//Function that draws a splash screen using the splash XBM array and U8G2 library
void splashScreen() {
  //Using U8G2 page buffers to draw on the screen
  display.firstPage();
  do {
    display.drawXBM(0, 0, 128, 128, splash); //Drawing the XBM splash logo
  } while ( display.nextPage() );

  // show splash screen for a 2 seconds
  delay(2000);
}

//Function that shows the loading screen with a progress bar and a text of the best score from the EEPROM
void loadingScreen() {
  //Drawing the progress bar and generating your maze text
  display.clearBuffer();
  display.setFont(u8g2_font_6x13_tr);
  display.drawStr(2, 50, "Generating your maze");
  display.drawFrame(13, 60, 102, 8);

  //Showing the best score under the progress bar
  String bestScore = "Best score is : " + String(BestPlayerScore);
  display.setFont(u8g2_font_6x13_tr);
  display.drawStr(10, 100, bestScore.c_str());
  display.sendBuffer();
}

//Function that shows the updates the loading screen by just drawing box over the existing progree bar layout
void loadingScreenUpdater(float(AdvancementPourcentage)) {
  //Updating on different progress stages according to 25-50-75-100% stages
  if (AdvancementPourcentage == 25 && !at25) {
    display.drawBox(14, 61, 25, 6);
    display.updateDisplay();
    at25 = true; //Updating the condition of 25
  } else if (AdvancementPourcentage == 50 && !at50) {
    display.drawBox(14, 61, 50, 6);
    display.updateDisplay();
    at50 = true; //Updating the condition of 50
  } else if (AdvancementPourcentage == 75 && !at75) {
    display.drawBox(14, 61, 75, 6);
    display.updateDisplay();
    at75 = true; //Updating the condition of 75
  } else if (AdvancementPourcentage == 100) {
    display.drawBox(14, 61, 100, 6);
    display.updateDisplay();
  }
}

//Function that display the winning screen with play again and reset data from EEPROM
void winnerScreen() {
  display.clearBuffer();
  display.setFont(u8g2_font_profont22_tf);
  display.drawStr(20, 50, "YOU WON!");
  display.setFont(u8g2_font_6x13_tr);

  //Showing the game session score
  String Score = "your score : " + String(PlayerScore);
  display.drawStr(22, 62, Score.c_str() );
  //Writing the new high score if the current one is smaller than the previous
  if (PlayerScore < BestPlayerScore) {
    EEPROM.write(10, PlayerScore);
    String bestScore = "You score a new best score !";
    display.drawStr(22, 72, bestScore.c_str() );
    display.setFont(u8g2_font_6x13_tr);
  } else {
    //Showing the best game score
    String bestScore = "best score : " + String(BestPlayerScore);
    display.drawStr(22, 72, bestScore.c_str() );
    display.setFont(u8g2_font_6x13_tr);
  }
  
  //Showing IO inputs to play or reset score
  display.drawStr(8, 100, "play again ? down");
  display.drawStr(8, 115, "clear score ? up");
  display.sendBuffer();


}

//Function that show the game and the 5 surrounding cells according to the player's position
void gameScreen(int PlayerPosI, int PlayerPosJ) {
  //Using the U8G2 to display the gamescreen, we found it 10% better than using full buffer one.
  display.firstPage();
  do {
    display.drawDisc(62, 62, 6, U8G2_DRAW_ALL); //Drawing the player controllable character
    for (int i = PlayerPosI - 2; i < PlayerPosI + 3 ; i++) {
      for (int j = PlayerPosJ - 2; j < PlayerPosJ + 3 ; j++) {
        if (i < 0 || j < 0 || i > sizeOfmazeFrame - 1 || j > sizeOfmazeFrame - 1) {
          //Not showing anything if out of mazeFrame boundary/Game boundary
        } else {
          if (mazeFrame[i][j] == 0) {
            display.drawBox(25 * (i - PlayerPosI + 2) + 1, 25 * (j - PlayerPosJ + 2) + 1, 23, 23);  //Showing a wall of 23 pixels and a safe black line of 1 pixel as delimitation
          }
        }
      }
    }

    //Showing the player's movement count and the position x,y in the mazeFrame in the bottom of the screen
    display.setFont(u8g2_font_squeezed_r6_tr );
    String currentScore = "Moves : " + String(PlayerScore) + "        x:" + String(PlayerPosI) + ", y:" + String(PlayerPosJ);
    display.drawStr(0, 128, currentScore.c_str() );
  } while ( display.nextPage() );
}


//////////// Game control functions ////////////

//Arduino's Setup function
void setup() {
  //setting up a random seed to diversify the mazeGeneration
  randomSeed(analogRead(A7));


  Serial.begin(9600); //Initialization of the comunication of the arduino
  display.begin(); //Initialization of the display using U8G2
  display.setPowerSave(0); //Avoiding any issues by forcing powerSave mode to 0
  splashScreen(); //Showing the splash screen
  loadingScreen(); //Showing the loading screen
  mazeGenerator(); //Generating the maze
  gameScreen(PlayerPosI, PlayerPosJ); //Showing the game screen centred on the player
  Wire.setClock(400000);
}

//Arduino's loop function
void loop() {
  //Checking if the player is not in a win position to consider it's inputs
  if (!isItAWin(PlayerPosI, PlayerPosJ)) { //Is not in win screen
    playerMouvement(InputDirections()); //Registring player's mouvement using playerMouvement function
  } else { //Is in win screen
    if (InputDirections() == 3) { //Move the joystick down (play again without wiping the data)
      resetFunc();
    } else if (InputDirections() == 4) { //Move the joystick up (Wipe data and play again)
      EEPROM.write(10, 255);
      resetFunc();
    }
  }

}

//Function that takes players inputs to initialize a movement
void playerMouvement(int playerInput) {
  //storing temporary player's position
  int tempPlayerPosJ;
  int tempPlayerPosI;
  //Moving to a dirrection according to the player's input direction
  switch (playerInput) {
    case 1:
      tempPlayerPosI = PlayerPosI + 1; //Horizontal move right
      tempPlayerPosJ = PlayerPosJ;
      if (!isItAWall(tempPlayerPosI, tempPlayerPosJ)) { //Checking if it not a wall using bool isItAWall() function
        PlayerPosI = tempPlayerPosI;
        PlayerPosJ = tempPlayerPosJ;
        PlayerScore++; //Incrementing players movement score
        gameScreen(PlayerPosI, PlayerPosJ); //Updating the gameScreen()
        if (isItAWin(PlayerPosI, PlayerPosJ)) { //Checking if it's a win after movement
          winnerScreen();
        }
      }

      break;
    case 2:
      tempPlayerPosI = PlayerPosI - 1; //Horizontal move left
      tempPlayerPosJ = PlayerPosJ;
      if (!isItAWall(tempPlayerPosI, tempPlayerPosJ)) { //Checking if it not a wall using bool isItAWall() function
        PlayerPosI = tempPlayerPosI;
        PlayerPosJ = tempPlayerPosJ;
        PlayerScore++; //Incrementing players movement score
        gameScreen(PlayerPosI, PlayerPosJ); //Updating the gameScreen()
        if (isItAWin(PlayerPosI, PlayerPosJ)) { //Checking if it's a win after movement
          winnerScreen();
        }
      }

      break;
    case 3:
      tempPlayerPosI = PlayerPosI;
      tempPlayerPosJ = PlayerPosJ + 1; //Vertical move down
      if (!isItAWall(tempPlayerPosI, tempPlayerPosJ)) { //Checking if it not a wall using bool isItAWall() function
        PlayerPosI = tempPlayerPosI;
        PlayerPosJ = tempPlayerPosJ;
        PlayerScore++; //Incrementing players movement score
        gameScreen(PlayerPosI, PlayerPosJ); //Updating the gameScreen()
        if (isItAWin(PlayerPosI, PlayerPosJ)) { //Checking if it's a win after movement
          winnerScreen();
        }
      }

      break;
    case 4:
      tempPlayerPosI = PlayerPosI;
      tempPlayerPosJ = PlayerPosJ - 1; //Vertical move up
      if (!isItAWall(tempPlayerPosI, tempPlayerPosJ)) { //Checking if it not a wall using bool isItAWall() function
        PlayerPosI = tempPlayerPosI;
        PlayerPosJ = tempPlayerPosJ;
        PlayerScore++; //Incrementing players movement score
        gameScreen(PlayerPosI, PlayerPosJ); //Updating the gameScreen()
        if (isItAWin(PlayerPosI, PlayerPosJ)) { //Checking if it's a win after movement
          winnerScreen();
        }
      }
      break;
  }
}

//////////// Conditional funtions ////////////

//Function that checks if a cell is a wall and returns true or false
bool isItAWall(int tempPlayerPosI, int tempPlayerPosJ) {
  return (mazeFrame[tempPlayerPosI][tempPlayerPosJ] == 0);
}

//Function that checks if the player is in win cell and returns true or false
bool isItAWin(int tempPlayerPosI, int tempPlayerPosJ) {
  return (tempPlayerPosI == winningCaseX && tempPlayerPosJ == winningCaseY);
}

//////////// Game input drivers ////////////

//Function that works as input driver for the game returning int as directions 1: right, 2: left, 3: down, 4: up , 0: not moving
int InputDirections() {
  int direction = 0;
  if (analogRead(pinX) >= 512 + sensibilite) {
    return 1;
  } else if (analogRead(pinX) <= 512 - sensibilite) {
    return 2;
  } else if (analogRead(pinY) >= 512 + sensibilite) {
    return 3;
  } else if (analogRead(pinY) <= 512 - sensibilite) {
    return 4;
  }
};
