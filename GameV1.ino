#include "Adafruit_GFX.h"
#include "Adafruit_HT1632.h"


#define HT_DATA 2
#define HT_WR   3
#define HT_CS   4
#define HT_CS2  5

int button = 8;

  
int len = 3;

int dir = 0; /* use 0 for left, 1 for right */
int x = 0;
int prevRY = 0;
int prevLY = 0;
int boundRight = 0;
int boundLeft = 0;
int prevLen = 0;
int start = 1;
int game_speed = 170;
int game = 1;

// use this line for single matrix
Adafruit_HT1632LEDMatrix matrix = Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS);
// use this line for two matrices!
//Adafruit_HT1632LEDMatrix matrix = Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS, HT_CS2);

void setup() {
  Serial.begin(9600);
  matrix.begin(ADA_HT1632_COMMON_16NMOS);
  matrix.fillScreen();
  delay(200);
  matrix.clearScreen();
  matrix.setTextWrap(false);

  pinMode(button, INPUT_PULLUP);
}
/*
void gameover(x, y){
  matrix.setCursor(x, y)
}
*/


void loop() {

  
  

  while(game == 1){
    Serial.println("Start Again");
    while(start == 1){
    
      int rightY = len-2;
      while (digitalRead(button) == LOW){
        if(rightY < 15){
          rightY += 1;
          for(int i = 0; i < len; i++){
            matrix.setPixel(x, rightY - i);
          }
          matrix.clrPixel(x, rightY-len);
          matrix.writeScreen();
          
        } else {
          for(int i = 0; i < 16; i++){
            matrix.clrPixel(x, i);
          }
          rightY = len - 2;
        }
        delay(game_speed);
      }
    
      
      while(digitalRead(button) == HIGH){
      }
      
      
      if (x==0){
        boundRight = rightY;
        boundLeft = rightY - (len-1);
    
        prevLY = boundLeft;
        prevRY = boundRight;
      }
      
      if (x > 0){
        if (prevRY > rightY || prevLY < rightY - (len - 1)){
          
          prevLen = len;
          len -= abs(prevRY - rightY);
          
          if(prevRY > rightY){ /*Overlap on the left*/
            prevRY = rightY;
          }
    
          if(prevLY < rightY - (prevLen - 1)){ /*Overlap on the right*/
            prevLY = rightY - (prevLen - 1);
          }
    
          
    
          
        }
      }
      for(int i = 0; i<16; i++){
        if (i < prevLY or i > prevRY){
          matrix.clrPixel(x, i);
        }
      }
      x++;
      if (x <= 20){
        game_speed -= 7;
      }
      /*prevRY = rightY;
      prevLY = rightY - len - 1;*/
      
      if (x > 23 && len >= 1){
        prevRY = 0;
        prevLY = 0;
        Serial.println("win");
        matrix.clearScreen();
        matrix.setCursor(0, 0);
        matrix.setRotation(1);
        matrix.print("WIN");
        matrix.writeScreen();
        start = 0;
      }
      if (len < 1){
        prevRY = 0;
        prevLY = 0;
        Serial.println("lose");
        matrix.clearScreen();
        matrix.setCursor(0, 0);
        matrix.setRotation(1);
        matrix.print("GG");
        matrix.setCursor(0, 12);
        matrix.print(x-1);
  
        matrix.writeScreen();
        start = 0;
        
      }
      
  }
  while(digitalRead(button)==LOW){
  
  }
  matrix.clearScreen();
  start = 1;
  dir = 0; /* use 0 for left, 1 for right */
  x = -1;
  prevRY = 0;
  prevLY = 0;
  boundRight = 0;
  boundLeft = 0;
  prevLen = 0;
  game_speed = 170;
  len = 3;
  matrix.setRotation(0);
  }
}
