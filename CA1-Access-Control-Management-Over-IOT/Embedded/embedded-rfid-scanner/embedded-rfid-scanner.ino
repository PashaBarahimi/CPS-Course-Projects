#include <AltSoftSerial.h>

AltSoftSerial rfidScanner;
auto& server = Serial;

const char* CONFIRMATION_MSG = "ok";
const int RFID_LEN = 12;
const unsigned long MOTOR_DELAY = 1000;
const unsigned long OPEN_DOOR_TIME = 30000;

const int GREEN_LED_PORT = 13;
const int RED_LED_PORT = 12;
const int MOTOR_PORT_1 = 7;
const int MOTOR_PORT_2 = 6;

struct State {
  bool isDoorOpen;
  unsigned long lastTimeDoorOpened;

  bool isGreenLightOn;
  unsigned long lastTimeGreenLightTurnedOn;

  bool isRedLightOn;
  unsigned long lastTimeRedLightTurnedOn;
};


void openDoor() {
  digitalWrite(MOTOR_PORT_1, HIGH);
  digitalWrite(MOTOR_PORT_2, LOW);
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

  openDoor();
  state.isDoorOpen = true;
  state.lastTimeDoorOpened = millis();
}

void denyAccess(State& state) {
  digitalWrite(RED_LED_PORT, HIGH);
  state.isRedLightOn = true;
  state.lastTimeRedLightTurnedOn = millis();
}

void executeServerCommand(State& state) {
  while(!server.available())
    ;
  
  char buffer[128] = {0};
  server.readBytes(buffer, 128);
  if (!strcmp(buffer, CONFIRMATION_MSG)) {
    rfidScanner.println("Access Granted!");
    grantAccess(state);
  }
  else {
    rfidScanner.println("Access denied!");
    denyAccess(state);
  }
}


void setup() {
  Serial.begin(9600);
  rfidScanner.begin(9600);

  pinMode(GREEN_LED_PORT, OUTPUT);
  pinMode(RED_LED_PORT, OUTPUT);

  pinMode(MOTOR_PORT_1, OUTPUT);
  pinMode(MOTOR_PORT_2, OUTPUT);
}

void loop() {
  static State currentState = {0};
  static char rfidBuff[13] = {0};
  static bool isSentToServer = false;

  int rfidLen = readRfid(rfidBuff);
  
  if (rfidLen != RFID_LEN)
    isSentToServer = false;
  if (!isSentToServer && rfidLen == RFID_LEN) {
    sendToServer(rfidBuff);
    isSentToServer = true;
    executeServerCommand(state);
  }
}
