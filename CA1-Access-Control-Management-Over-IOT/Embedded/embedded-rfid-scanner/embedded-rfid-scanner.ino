#include <AltSoftSerial.h>
#include <LiquidCrystal.h>

AltSoftSerial rfidScanner;
auto& server = Serial;
LiquidCrystal lcd(11, 10, 5, 4, 3, 2);

const char* CONFIRMATION_MSG = "ok";
const int RFID_LEN = 10;
const unsigned long MOTOR_DELAY = 1000;

const unsigned long OPEN_DOOR_DURATION = 3000;
const unsigned long GREEN_LIGHT_DURATION = 2000;
const unsigned long RED_LIGHT_DURATION = 2000;
const unsigned long LCD_PRINTING_DUARTION = 3000;

const int GREEN_LED_PORT = 13;
const int RED_LED_PORT = 12;
const int MOTOR_PORT_1 = 7;
const int MOTOR_PORT_2 = 6;

const char* LCD_ACCESS_GRANTED_MSG = "Access Granted!";
const char* LCD_ACCESS_DENIED_MSG = "Access Denied!";

struct State {
  bool isDoorOpen = false;
  unsigned long lastTimeDoorOpened;

  bool isGreenLightOn = false;
  unsigned long lastTimeGreenLightTurnedOn;

  bool isRedLightOn = false;
  unsigned long lastTimeRedLightTurnedOn;

  bool isLcdClear = true;
  unsigned long lastTimeLcdPrinted;
};


void openDoor(State& state) {
  if (state.isDoorOpen)
    return;
  
  digitalWrite(MOTOR_PORT_1, HIGH);
  digitalWrite(MOTOR_PORT_2, LOW);
  delay(MOTOR_DELAY);
  digitalWrite(MOTOR_PORT_1, LOW);
  digitalWrite(MOTOR_PORT_2, LOW);
}

void closeDoor(State& state) {
  if (!state.isDoorOpen)
    return;
  
  digitalWrite(MOTOR_PORT_1, LOW);
  digitalWrite(MOTOR_PORT_2, HIGH);
  delay(MOTOR_DELAY);
  digitalWrite(MOTOR_PORT_1, LOW);
  digitalWrite(MOTOR_PORT_2, LOW);
}

int readRfid(char buffer[]) {
  static int bufferCounter = 0;

  if (rfidScanner.available()) {
    bufferCounter %= RFID_LEN;
    buffer[bufferCounter++] = rfidScanner.read();
  }

  return bufferCounter;
}

void sendToServer(const char rfid[]) {
  server.write(rfid);
}

void grantAccess(State& state) {
  digitalWrite(GREEN_LED_PORT, HIGH);
  state.isGreenLightOn = true;
  state.lastTimeGreenLightTurnedOn = millis();

  openDoor(state);
  state.isDoorOpen = true;
  state.lastTimeDoorOpened = millis();

  lcd.clear();
  lcd.print(LCD_ACCESS_GRANTED_MSG);
  state.isLcdClear = false;
  state.lastTimeLcdPrinted = millis();
}

void denyAccess(State& state) {
  digitalWrite(RED_LED_PORT, HIGH);
  state.isRedLightOn = true;
  state.lastTimeRedLightTurnedOn = millis();

  closeDoor(state);
  state.isDoorOpen = false;

  lcd.clear();
  lcd.print(LCD_ACCESS_DENIED_MSG);
  state.isLcdClear = false;
  state.lastTimeLcdPrinted = millis();
}

void executeServerCommand(State& state) {
  while(!server.available())
    ;
  
  char buffer[128] = {0};
  server.readBytes(buffer, 128);
  if (!strcmp(buffer, CONFIRMATION_MSG)) {
    grantAccess(state);
  }
  else {
    denyAccess(state);
  }
}

void stabalize(State& state) {
  const unsigned long currentTime = millis();

  if (state.isDoorOpen)
    if (currentTime - state.lastTimeDoorOpened >= OPEN_DOOR_DURATION) {
      closeDoor(state);
      state.isDoorOpen = false;
    }
  
  if (state.isGreenLightOn)
    if (currentTime - state.lastTimeGreenLightTurnedOn >= GREEN_LIGHT_DURATION) {
      digitalWrite(GREEN_LED_PORT, LOW);
      state.isGreenLightOn = false;
    }
  
  if (state.isRedLightOn)
    if (currentTime - state.lastTimeRedLightTurnedOn >= RED_LIGHT_DURATION) {
      digitalWrite(RED_LED_PORT, LOW);
      state.isRedLightOn = false;
    }

  if (!state.isLcdClear)
    if (currentTime - state.lastTimeLcdPrinted >= LCD_PRINTING_DUARTION) {
      lcd.clear();
      state.isLcdClear = true;
    }
}


void setup() {
  Serial.begin(9600);
  rfidScanner.begin(9600);

  pinMode(GREEN_LED_PORT, OUTPUT);
  pinMode(RED_LED_PORT, OUTPUT);

  pinMode(MOTOR_PORT_1, OUTPUT);
  pinMode(MOTOR_PORT_2, OUTPUT);

  lcd.begin(16, 2);
}

void loop() {
  static State currentState;
  static char rfidBuff[13] = {0};
  static bool isSentToServer = false;

  int rfidLen = readRfid(rfidBuff);
  
  if (rfidLen != RFID_LEN)
    isSentToServer = false;
  if (!isSentToServer && rfidLen == RFID_LEN) {
    sendToServer(rfidBuff);
    isSentToServer = true;
    executeServerCommand(currentState);
  }

  stabalize(currentState);
}
