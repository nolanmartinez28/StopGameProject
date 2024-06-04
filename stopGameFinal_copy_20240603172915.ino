#include <Adafruit_CircuitPlayground.h>

bool switchState;
volatile bool switchFlag = 0;
volatile bool rightButtFlag = 0;
int lightSpeed;
int level = 0;
int lightVal;
float darkThresh = 20.0;

float midi[127];
int A_four = 440;
int c_major[14] = {50, 60, 64, 65, 67, 69, 70, 71, 72, 74, 76, 77, 79, 81};

int D3 = c_major[0];
int C4 = c_major[1];
int E4 = c_major[2];
int G4 = c_major[4];
int Alo = c_major[5];
int Bb = c_major[6];
int B4 = c_major[7];
int C5 = c_major[8];
int F5 = c_major[11];
int G5 = c_major[12];
int Ahi = c_major[13];

int happySong [3][2] = {
  {F5, 50},
  {G5, 50},
  {Ahi, 50},
};

int sadSong [3][2] = {
  {E4, 200},
  {C4, 200},
  {D3, 400},
};

int finalSong [10][2] = {
  {E4, 200},
  {G4, 100},
  {Alo, 100},
  {G4, 200},

  {Alo, 100},
  {B4, 200},
  {C5, 100},
  {B4, 100},
  {Bb, 100},
  {Alo, 400},
};


void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  CircuitPlayground.clearPixels();
  generateMIDI();
  attachInterrupt(7, switchISR, CHANGE);
  attachInterrupt(5, rightButtISR, RISING);
  switchState = digitalRead(7);
  lightVal = CircuitPlayground.lightSensor();
}

void loop() {
  if(switchFlag) { //when switch is flipped, read switch state and reset switch flag
    delay(5);
    switchState = digitalRead(7);
    switchFlag = 0;
  }

  if(switchState) { //when switchState is true, play homeMenu function
    Serial.println("HOME: Flip switch to start game!");
    homeMenu();
  }

  else {
    switch(level) { //switch case for each level with decreasing delay of pixel illumination (faster speed)
      case 0:
        Serial.print("LEVEL 1: Get ready... ");
        Serial.println("Press right button to stop light in original position");
        CircuitPlayground.setPixelColor(0, 0, 0, 255);
        delay(2000);
        level1(500);
        break;
      case 1:
        Serial.print("LEVEL 2: Get ready...");
        Serial.println("Press right button to stop light in original position");
        CircuitPlayground.setPixelColor(0, 0, 0, 255);
        delay(2000);
        level2(300);
        break;
      case 2:
        Serial.print("LEVEL 3: Get ready...");
        Serial.println("Press right button to stop light in original position");
        CircuitPlayground.setPixelColor(0, 0, 0, 255);
        delay(2000);
        level3(100);
        break;
      case 3:
        Serial.print("LEVEL 4: Get ready...");
        Serial.println("Press right button to stop light in original position");
        CircuitPlayground.setPixelColor(0, 0, 0, 255);
        delay(2000);
        level4(50);
        break;
      default:
        Serial.println("CONGRATULATIONS! You won!!");
        congratulations();
        break;
    }
  }
}


void homeMenu() { //homeMenu function
  level = 0;

  for(int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 255, 255, 255);
    delay(100);
    CircuitPlayground.clearPixels();
    switchState = digitalRead(7);
    if(!switchState) {
      break;
    }
  }
  for(int i = 9; i >= 0; i--) {
    CircuitPlayground.setPixelColor(i, 255, 255, 255);
    delay(100);
    CircuitPlayground.clearPixels();
    switchState = digitalRead(7);
    if(!switchState) {
      break;
    }
  }
}


void level1(int lightSpeed) { //level 1 function
  for(int i = 0; i < 11; i++) {
    if(i == 10) { //to loop around pixels
      i = 0;
    }
    CircuitPlayground.setPixelColor(i, 255, 255, 255);
    delay(lightSpeed);

    if(rightButtFlag && i == 0) { //when stopped on correct pixel
      delay(5);
      Serial.println("Nice! Level 1 complete.");
      CircuitPlayground.setPixelColor(0, 0, 255, 0);
      delay(200);
      CircuitPlayground.clearPixels();
      delay(200);
      CircuitPlayground.setPixelColor(0, 0, 255, 0);
      delay(200);
      CircuitPlayground.clearPixels();
      delay(200);
      CircuitPlayground.setPixelColor(0, 0, 255, 0);
      delay(200);
      CircuitPlayground.clearPixels();
      rightButtFlag = 0;

      lightVal = CircuitPlayground.lightSensor(); //play happy song
      if(lightVal > darkThresh) {
        for(int i = 0; i < sizeof(happySong) / sizeof(happySong[0]); i++) {
        CircuitPlayground.playTone(midi[happySong[i][0]], happySong[i][1]);
        delay(1);
        }
      }
      delay(500);
      level = 1;
      break;
    }
    else if(rightButtFlag && i != 0) { //when stopped on incorrect pixel
      delay(5);
      CircuitPlayground.setPixelColor(i , 255, 0, 0);

      lightVal = CircuitPlayground.lightSensor(); //play sad song
      if(lightVal > darkThresh) {
        for(int i = 0; i < sizeof(sadSong) / sizeof(sadSong[0]); i++) {
        CircuitPlayground.playTone(midi[sadSong[i][0]], sadSong[i][1]);
        delay(1);
        }
      }
      delay(2000);
      rightButtFlag = 0;
    }
    CircuitPlayground.clearPixels();
    switchState = digitalRead(7);
    if(switchState) { //back to homeMenu if switch is flipped during game
      loop();
      break;
    }
  }
}

void level2(int lightSpeed) { //level 2 function
  for(int i = 0; i < 11; i++) {
    if(i == 10) {
      i = 0;
    }
    CircuitPlayground.setPixelColor(i, 255, 255, 255);
    delay(lightSpeed);

    if(rightButtFlag && i == 0) {
      delay(5);
      Serial.println("Nice! Level 2 complete.");
      CircuitPlayground.setPixelColor(0, 0, 255, 0);
      delay(200);
      CircuitPlayground.clearPixels();
      delay(200);
      CircuitPlayground.setPixelColor(0, 0, 255, 0);
      delay(200);
      CircuitPlayground.clearPixels();
      delay(200);
      CircuitPlayground.setPixelColor(0, 0, 255, 0);
      delay(200);
      CircuitPlayground.clearPixels();
      rightButtFlag = 0;

      lightVal = CircuitPlayground.lightSensor(); //play happy song
      if(lightVal > darkThresh) {
        for(int i = 0; i < sizeof(happySong) / sizeof(happySong[0]); i++) {
        CircuitPlayground.playTone(midi[happySong[i][0]], happySong[i][1]);
        delay(1);
        }
      }
      delay(500);
      level = 2;
      break;
    }
    else if(rightButtFlag && i != 0) {
      delay(5);
      CircuitPlayground.setPixelColor(i , 255, 0, 0);

      lightVal = CircuitPlayground.lightSensor(); //play sad song
      if(lightVal > darkThresh) {
        for(int i = 0; i < sizeof(sadSong) / sizeof(sadSong[0]); i++) {
        CircuitPlayground.playTone(midi[sadSong[i][0]], sadSong[i][1]);
        delay(1);
        }
      }
      delay(2000);
      rightButtFlag = 0;
    }
    CircuitPlayground.clearPixels();
    switchState = digitalRead(7);
    if(switchState) {
      loop();
      break;
    }
  }
}

void level3(int lightSpeed) { //level 3 function
  for(int i = 0; i < 11; i++) {
    if(i == 10) {
      i = 0;
    }
    CircuitPlayground.setPixelColor(i, 255, 255, 255);
    delay(lightSpeed);

    if(rightButtFlag && i == 0) {
      delay(5);
      Serial.println("Nice! Level 3 complete.");
      CircuitPlayground.setPixelColor(0, 0, 255, 0);
      delay(200);
      CircuitPlayground.clearPixels();
      delay(200);
      CircuitPlayground.setPixelColor(0, 0, 255, 0);
      delay(200);
      CircuitPlayground.clearPixels();
      delay(200);
      CircuitPlayground.setPixelColor(0, 0, 255, 0);
      delay(200);
      CircuitPlayground.clearPixels();
      rightButtFlag = 0;

      lightVal = CircuitPlayground.lightSensor(); //play happy song
      if(lightVal > darkThresh) {
        for(int i = 0; i < sizeof(happySong) / sizeof(happySong[0]); i++) {
        CircuitPlayground.playTone(midi[happySong[i][0]], happySong[i][1]);
        delay(1);
        }
      }
      delay(500);
      level = 3;
      break;
    }
    else if(rightButtFlag && i != 0) {
      delay(5);
      CircuitPlayground.setPixelColor(i , 255, 0, 0);

      lightVal = CircuitPlayground.lightSensor(); //play sad song
      if(lightVal > darkThresh) {
        for(int i = 0; i < sizeof(sadSong) / sizeof(sadSong[0]); i++) {
        CircuitPlayground.playTone(midi[sadSong[i][0]], sadSong[i][1]);
        delay(1);
        }
      }
      delay(2000);
      rightButtFlag = 0;
    }
    CircuitPlayground.clearPixels();
    switchState = digitalRead(7);
    if(switchState) {
      loop();
      break;
    }
  }
}

void level4(int lightSpeed) { //level 4 function
  for(int i = 0; i < 11; i++) {
    if(i == 10) {
      i = 0;
    }
    CircuitPlayground.setPixelColor(i, 255, 255, 255);
    delay(lightSpeed);

    if(rightButtFlag && i == 0) {
      delay(5);
      Serial.println("Nice! Level 4 complete.");
      CircuitPlayground.setPixelColor(0, 0, 255, 0);
      delay(200);
      CircuitPlayground.clearPixels();
      delay(200);
      CircuitPlayground.setPixelColor(0, 0, 255, 0);
      delay(200);
      CircuitPlayground.clearPixels();
      delay(200);
      CircuitPlayground.setPixelColor(0, 0, 255, 0);
      delay(200);
      CircuitPlayground.clearPixels();
      rightButtFlag = 0;

      lightVal = CircuitPlayground.lightSensor(); //play happy song
      if(lightVal > darkThresh) {
        for(int i = 0; i < sizeof(happySong) / sizeof(happySong[0]); i++) {
        CircuitPlayground.playTone(midi[happySong[i][0]], happySong[i][1]);
        delay(1);
        }
      }
      delay(500);
      level = 4;
      break;
    }
    else if(rightButtFlag && i != 0) {
      delay(5);
      CircuitPlayground.setPixelColor(i , 255, 0, 0);

      lightVal = CircuitPlayground.lightSensor(); //play sad song
      if(lightVal > darkThresh) {
        for(int i = 0; i < sizeof(sadSong) / sizeof(sadSong[0]); i++) {
        CircuitPlayground.playTone(midi[sadSong[i][0]], sadSong[i][1]);
        delay(1);
        }
      }
      delay(2000);
      rightButtFlag = 0;
    }
    CircuitPlayground.clearPixels();
    switchState = digitalRead(7);
    if(switchState) {
      loop();
      break;
    }
  }
}


void congratulations() { //game completion sequence with music and light show
  lightShow(250);
  lightShow(250);
  lightVal = CircuitPlayground.lightSensor(); //play final song
    if(lightVal > darkThresh) {
      for(int i = 0; i < sizeof(finalSong) / sizeof(finalSong[0]); i++) {
        CircuitPlayground.playTone(midi[finalSong[i][0]], finalSong[i][1]);
        delay(1);
      }
    }
}


void lightShow(int delayTime) {
  CircuitPlayground.clearPixels();
  delay(delayTime/7);
  for(int g = 0; g < 10; g += 2) {
    CircuitPlayground.setPixelColor(g, 0x0000FF);
  }
  for(int r = 1; r < 10; r += 2) {
    CircuitPlayground.setPixelColor(r, 0xFF0000);
  }
  delay(delayTime);

  //alternate
  CircuitPlayground.clearPixels();
  delay(delayTime/7);
  for(int g = 1; g < 10; g += 2) {
    CircuitPlayground.setPixelColor(g, 0x0000FF);
  }
  for(int r = 0; r < 10; r += 2) {
    CircuitPlayground.setPixelColor(r, 0x00FF00);
  }
  delay(delayTime);
}


void generateMIDI() {
  for (int x = 0; x < 127; ++x)
  {
    midi[x] = (A_four / 32.0) * pow(2.0, ((x - 9.0) / 12.0));
    Serial.println(midi[x]);
  }
}


void switchISR() {
  switchFlag = 1;
}

void rightButtISR() {
  rightButtFlag = 1;
}
