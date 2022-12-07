//definition des inputs et outputs
#include <U8g2lib.h>
#include "Wire.h"
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1327.h>

#define pinY    A0
#define pinX    A1
#define swPin    2
int sensibilite = 256;
int PlayerPosI = 1;
int PlayerPosJ = 1;

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
        if (random(0, 2) == 0) {
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
        } else {
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
        }
      }
    }
    Serial.println(A / 100);
  }
  Serial.println("maze generated");
}


U8G2_SH1107_SEEED_128X128_F_SW_I2C display(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

void displaying(int PlayerPosI, int PlayerPosJ) {
  display.clearBuffer();
  display.drawDisc(62, 62, 6, U8G2_DRAW_ALL); // clear the internal memory
  for (int i = PlayerPosI - 2; i < PlayerPosI + 3 ; i++) {
    for (int j = PlayerPosJ - 2; j < PlayerPosJ + 3 ; j++) {
      if (i < 0 || j < 0 || i > 32 || j > 32) {

      } else {
        if (tab32[i][j] == 0) {
          //display.drawBox(25 * (i+1) + 1, 25 * (j+1) + 1, 23, 23);
          display.drawBox(25 * (i - PlayerPosI + 2) + 1, 25 * (j - PlayerPosJ + 2) + 1, 23, 23);
        }
      }
    }
  }
  display.sendBuffer();          // transfer internal memory to the display
}


void setup() {

  randomSeed(analogRead(A7));
  Serial.begin(9600);
  display.begin();
  display.setPowerSave(0);
  Generateur();
  Wire.setClock(400000);
  displaying(PlayerPosI, PlayerPosJ);
}

void loop() {
  //  int A = InputDirections(analogRead(pinX), analogRead(pinY));
  //  Serial.println(A);
  playerMouvement(InputDirections());
}

void playerMouvement(int playerInput) {
  int tempPlayerPosJ;
  int tempPlayerPosI;
  switch (playerInput) {
    case 1:
      tempPlayerPosI = PlayerPosI + 1;
      tempPlayerPosJ = PlayerPosJ;
      if (!isItAWall(tempPlayerPosI, tempPlayerPosJ)) {
        PlayerPosI = tempPlayerPosI;
        PlayerPosJ = tempPlayerPosJ;
        Serial.println("en droite");
        displaying(PlayerPosI, PlayerPosJ);
      }
      break;
    case 2:
      tempPlayerPosI = PlayerPosI - 1;
      tempPlayerPosJ = PlayerPosJ;
      if (!isItAWall(tempPlayerPosI, tempPlayerPosJ)) {
        PlayerPosI = tempPlayerPosI;
        PlayerPosJ = tempPlayerPosJ;
        Serial.println("en gauche");
        displaying(PlayerPosI, PlayerPosJ);

      }
      break;
    case 3:
      tempPlayerPosI = PlayerPosI;
      tempPlayerPosJ = PlayerPosJ + 1;
      if (!isItAWall(tempPlayerPosI, tempPlayerPosJ)) {
        PlayerPosI = tempPlayerPosI;
        PlayerPosJ = tempPlayerPosJ;
        Serial.println("en bas");
        displaying(PlayerPosI, PlayerPosJ);
      }
      break;
    case 4:
      tempPlayerPosI = PlayerPosI;
      tempPlayerPosJ = PlayerPosJ - 1;
      if (!isItAWall(tempPlayerPosI, tempPlayerPosJ)) {
        PlayerPosI = tempPlayerPosI;
        PlayerPosJ = tempPlayerPosJ;
        Serial.println("en haut");
        displaying(PlayerPosI, PlayerPosJ);
      }
      break;
  }
}

bool isItAWall(int tempPlayerPosI, int tempPlayerPosJ) {
  if (tab32[tempPlayerPosI][tempPlayerPosJ] == 0) {
    return true;
  } else {
    return false;
  }
}

//Fonction qui retourne la direction choisie par l'utilisateur
int InputDirections() {
  int direction = 0;
  if (analogRead(pinX) >= 512 + sensibilite) {
    //Serial.println("A droite");
    direction = 1;
  } else if (analogRead(pinX) <= 512 - sensibilite) {
    //Serial.println("A gauche");
    direction = 2;
  } else if (analogRead(pinY) >= 512 + sensibilite) {
    //Serial.println("en bas");
    direction = 3;
  } else if (analogRead(pinY) <= 512 - sensibilite) {
    //Serial.println("en haut");
    direction = 4;
  }
  Serial.println(direction);
  return direction;
};
