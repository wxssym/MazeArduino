//definition des inputs et outputs
#include <U8g2lib.h>
#include "Wire.h"
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1327.h>

#define pinY    A0
#define pinX    A1
#define swPin    2
int sensibilite = 256;

#define OLED_RESET -1
unsigned int tab32[32][32];

void Generateur() {
  Serial.println("generating maze");
  int n = 1;
  for (int  i = 0; i < 31; i++) {
    for (int  j = 0; j < 31; j++) {

      if ( i % 2 == 0 || j % 2 == 0 ) {
        tab32[i][j] = 0;
      } else {
        tab32[i][j] = n;
        n++;
      }
    }
  }
  for (int A = 0; A < 280;) { //280
    int x = random(1, 30);
    int y = random(1, 30);
    if (x % 2 == 0 && y % 2 == 0) {

    } else {
      if (tab32[x][y] == 0) {
        switch (int axis = random(0, 2)) {
          case 0:
            if (tab32[x][y + 1] != tab32[x][y - 1] && tab32[x][y + 1] != 0 && tab32[x][y - 1] != 0  ) {
              tab32[x][y] = tab32[x][y - 1];
              A++;
              for (int  i = 0; i < 31; i++) {
                for (int  j = 0; j < 31; j++) {
                  if ( tab32[i][j] == tab32[x][y + 1]) {
                    tab32[i][j] = tab32[x][y - 1];
                  }
                }
              }
            }
            break;
          case 1:
            if (tab32[x + 1][y] != tab32[x - 1][y] && tab32[x + 1][y ] != 0 && tab32[x - 1][y] != 0 ) {
              tab32[x][y] = tab32[x - 1][y];
              A++;
              for (int  i = 0; i < 31; i++) {
                for (int  j = 0; j < 31; j++) {
                  if ( tab32[i][j] == tab32[x + 1][y]) {
                    tab32[i][j] = tab32[x - 1][y];
                  }
                }
              }
            }
            break;
        }
      }
    }
    Serial.println(A / 100);
  }
  Serial.println("maze generated");
}


U8G2_SH1107_SEEED_128X128_F_SW_I2C display(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1327_SEEED_96X96_F_SW_I2C display(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1327_SEEED_96X96_1_HW_I2C display(U8G2_R0, A5, A4);
//Adafruit_SSD1327 display(96, 96, &Wire, OLED_RESET, 1000000);
//U8X8_SSD1327_SEEED_96X96_SW_I2C u8x8(A5, A4);

void displaying(int PlayerPosI, int PlayerPosJ) {
  display.clearBuffer();
  display.drawDisc(62, 62, 6, U8G2_DRAW_ALL); // clear the internal memory
  for (int i = PlayerPosI - 3; i < PlayerPosI + 3 ; i++) {
    for (int j = PlayerPosJ - 3; j < PlayerPosJ + 3 ; j++) {
      if (i < 0 || j < 0 || i > 32 || j > 32) {

      } else {
        if (tab32[i][j] == 0) {
          //display.drawBox(25 * (i+1) + 1, 25 * (j+1) + 1, 23, 23);
          display.drawBox(25 * i + 1, 25 * j + 1, 23, 23);
        }
      }
    }
  }
  display.sendBuffer();          // transfer internal memory to the display
}

void setup() {
  int PlayerPosI = 3;
  int PlayerPosJ = 1;
  randomSeed(analogRead(A7));
  printf("hiii");
  Serial.begin(9600);
  display.begin();
  display.setPowerSave(0);
  Generateur();
  //Wire.setClock(400000);
  displaying(PlayerPosI, PlayerPosJ);
}

void loop() {
  int A = InputDirections(analogRead(pinX), analogRead(pinY));
  Serial.println(analogRead(pinX));
}




//Fonction qui retourne la direction choisie par l'utilisateur
int InputDirections( int X, int Y) {
  int direction = 0;

  if (X >= 512 + sensibilite) {
    Serial.println("A droite");
    direction = 1;
  } else if (X <= 512 - sensibilite) {
    Serial.println("A gauche");
    direction = 2;
  } else if (Y >= 512 + sensibilite) {
    Serial.println("en bas");
    direction = 3;
  } else if (Y <= 512 - sensibilite) {
    Serial.println("en haut");
    direction = 4;
  };

  return direction;
};
