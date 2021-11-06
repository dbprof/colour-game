#include "button.h"

#define BUZZER_PIN 8
#define RED_BUT_PIN 4
#define YELLOW_BUT_PIN 3
#define GREEN_BUT_PIN 2
#define RED_LED_PIN 7
#define YELLOW_LED_PIN 6
#define GREEN_LED_PIN 5

#define COUNTCLICKS 5             //сложность игры или количество нажатий
#define STDELAY 200               //стандартная задержка
#define BIGDELAY 1000             //большая задержка

#define RED_SIGNAL 0
#define YELLOW_SIGNAL 1
#define GREEN_SIGNAL 2
#define WIN_SIGNAL 3
#define FAULT_SIGNAL 4
#define HELLO_SIGNAL 5

button cBUTRED(RED_BUT_PIN);
button cBUTYELLOW(YELLOW_BUT_PIN);
button cBUTGREEN(GREEN_BUT_PIN);

byte gamemode;
byte bCount;
byte myClicks[COUNTCLICKS];
bool isWinner;

void clearallpins(){
  digitalWrite(GREEN_LED_PIN, 0);
  digitalWrite(YELLOW_LED_PIN, 0);
  digitalWrite(RED_LED_PIN, 0);
  digitalWrite(BUZZER_PIN, 0);
  noTone(BUZZER_PIN);
}

void dosignal(byte sygnaltype){
  // 0 - RED_SIGNAL
  // 1 - YELLOW_SIGNAL
  // 2 - GREEN_SIGNAL
  // 3 - WIN_SIGNAL
  // 4 - FAULT_SIGNAL
  // 5 - HELLO_SIGNAL
  switch (sygnaltype) {
    case RED_SIGNAL:
      Serial.println("RED");
      digitalWrite(RED_LED_PIN, 1);
      tone(BUZZER_PIN, 1000);
      delay(STDELAY);
      clearallpins();
      break;
    case YELLOW_SIGNAL:
      Serial.println("YELLOW");
      digitalWrite(YELLOW_LED_PIN, 1);
      tone(BUZZER_PIN, 1300);
      delay(STDELAY);
      clearallpins();
      break;
    case GREEN_SIGNAL:
      Serial.println("GREEN");
      digitalWrite(GREEN_LED_PIN, 1);
      tone(BUZZER_PIN, 1600);
      delay(STDELAY);
      clearallpins();
      break;
    case WIN_SIGNAL:
      Serial.println("WIN");
      digitalWrite(GREEN_LED_PIN, 1);
      tone(BUZZER_PIN, 500 , STDELAY);
      delay(100);
      tone(BUZZER_PIN, 600 , STDELAY+100);
      delay(BIGDELAY);
      clearallpins();
      break;
    case FAULT_SIGNAL:
      Serial.println("FAULT");
      digitalWrite(RED_LED_PIN, 1);
      tone(BUZZER_PIN, 600 , STDELAY);
      delay(100);
      tone(BUZZER_PIN, 500 , STDELAY+100);
      delay(BIGDELAY);
      clearallpins();
      break;
    case HELLO_SIGNAL:
      Serial.println("HELLO");
      dosignal(0);
      dosignal(1);
      dosignal(2);
      dosignal(1);
      dosignal(0);
      clearallpins();
      break;
    default:
      break;
  }
}

void setup(){
  dosignal(HELLO_SIGNAL);
  Serial.begin(9600);
  gamemode = 1; //1 = read, 2 = show
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  clearallpins();
}

void loop() {
  if (gamemode == 1){
    delay(BIGDELAY);
    dosignal(HELLO_SIGNAL);
    delay(BIGDELAY);
    bCount = 0;
    do {
      if (cBUTRED.click()) {
        dosignal(RED_SIGNAL);
        myClicks[bCount] = RED_SIGNAL;
        bCount++;
      }
      if (cBUTYELLOW.click()){
        dosignal(YELLOW_SIGNAL);
        myClicks[bCount] = YELLOW_SIGNAL;
        bCount++;
      }
      if (cBUTGREEN.click()){
        dosignal(GREEN_SIGNAL);
        myClicks[bCount] = GREEN_SIGNAL;
        bCount++;
      }  
    } while (bCount < COUNTCLICKS);
    gamemode = 2;
  }
  else if (gamemode == 2){
    isWinner = true;
    delay(BIGDELAY);
    for (byte i = 0; i < COUNTCLICKS; i++){
      switch (myClicks[i]) {
        case RED_SIGNAL:
          dosignal(RED_SIGNAL);
          delay(BIGDELAY/2);
          break;
        case YELLOW_SIGNAL:
          dosignal(YELLOW_SIGNAL);
          delay(BIGDELAY/2);
          break;
        case GREEN_SIGNAL:
          dosignal(GREEN_SIGNAL);
          delay(BIGDELAY/2);
          break;
      }
      gamemode = 3;
    }
  }
  
  else if (gamemode == 3){
    delay(1000);
    bCount = 0;
    do {
      if (cBUTRED.click()) {
        dosignal(RED_SIGNAL);
        if (myClicks[bCount] != RED_SIGNAL) isWinner = false;
        bCount++;
      }
      if (cBUTYELLOW.click()){
        dosignal(YELLOW_SIGNAL);
        if (myClicks[bCount] != YELLOW_SIGNAL) isWinner = false;
        bCount++;
      }
      if (cBUTGREEN.click()){
        dosignal(GREEN_SIGNAL);
        if (myClicks[bCount] != GREEN_SIGNAL) isWinner = false;
        bCount++;
      }  
    } while (bCount < COUNTCLICKS && isWinner);
    
    if (isWinner) {
      delay(BIGDELAY/2);
      dosignal(WIN_SIGNAL);
      //clearmyClicks
      bCount = 0;
      do {
        myClicks[bCount] = NULL;
        bCount++; 
      } while (bCount < COUNTCLICKS);
      gamemode = 1;
    }
    else {
      delay(BIGDELAY/2);
      dosignal(FAULT_SIGNAL);
      gamemode = 2;
    }
    delay(BIGDELAY);
  }
}
 

 
