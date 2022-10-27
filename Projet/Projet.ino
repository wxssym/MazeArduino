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

void tableau48() {
  for (int  i = 0; i < 31; i++) {
    for (int  j = 0; j < 31; j++) {
      if ( i % 2 == 0 || j % 2 == 0 ) {
        tab32[i][j] = 0;
      } else {
        tab32[i][j] = random(1, 16);
      }
    }
  }
}


U8G2_SSD1327_SEEED_96X96_F_SW_I2C display(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1327_SEEED_96X96_1_HW_I2C display(U8G2_R0, A5, A4);
//Adafruit_SSD1327 display(96, 96, &Wire, OLED_RESET, 1000000);
//U8X8_SSD1327_SEEED_96X96_SW_I2C u8x8(A5, A4);

void displaying() {
  display.clearBuffer();          // clear the internal memory
  for (int  i = 0; i < 31; i++) {
    for (int  j = 0; j < 31; j++) {
      if (tab32[i][j] == 0) {
        display.drawBox(i * 3, j * 3, 3, 3);
      };
    };
  };
  display.drawPixel(4,4);
  display.sendBuffer();          // transfer internal memory to the display
};


void setup() {
  printf("hiii");
  Serial.begin(9600);
  display.begin();
  display.setPowerSave(0);
  tableau48();
  Wire.setClock(400000);
  displaying();
};

void loop() {
  int A = InputDirections(analogRead(pinX), analogRead(pinY));
  Serial.println(analogRead(pinX));
};




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
