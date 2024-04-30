#include <AltSoftSerial.h>
#include <LiquidCrystal.h>

AltSoftSerial ethernetModule;
auto& rfidScanner = Serial;
LiquidCrystal lcd(11, 10, 5, 4, 3, 2);

const char SERVER_OK[] PROGMEM = "ok";
const char SERVER_FORBIDDEN[] PROGMEM = "forbidden";


const int RFID_LEN = 10;
const unsigned long MOTOR_DELAY = 1000;

const unsigned long OPEN_DOOR_DURATION = 3000;
const unsigned long GREEN_LIGHT_DURATION = 2000;
const unsigned long RED_LIGHT_DURATION = 2000;
const unsigned long LCD_PRINTING_DUARTION = 3000;
const unsigned long SERVER_TIME_OUT = 1000;

const int GREEN_LED_PORT = 13;
const int RED_LED_PORT = 12;
const int MOTOR_PORT_1 = 7;
const int MOTOR_PORT_2 = 6;

const char LCD_ACCESS_GRANTED_MSG[] PROGMEM = "Access Granted!";
const char LCD_ACCESS_DENIED_MSG[] PROGMEM = "Access Denied!";

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

bool isValidServerCommand(const char* command) {
  return (strcmp_P(command, SERVER_OK) == 0) || (strcmp_P(command, SERVER_FORBIDDEN) == 0);
}

int readServerCommand(char buffer[], int maxLen, unsigned long timeOut) {
  unsigned long start = millis();
  int counter = 0;

  while (true) {
    if (millis() - start > timeOut)
      break;
    if (counter >= maxLen)
      break;
    if (!ethernetModule.available())
      continue;

    buffer[counter++] = (char)ethernetModule.read();
    if (isValidServerCommand(buffer))
      break;
  }

  return counter;
}

int readRfid(char buffer[]) {
  static int bufferCounter = 0;
  if (rfidScanner.available()) {
    bufferCounter %= RFID_LEN;
    buffer[bufferCounter++] = rfidScanner.read();
  }

  return bufferCounter;
}

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

void sendToServer(const char rfid[]) {
  rfidScanner.print("Sending to server: ");
  rfidScanner.println(rfid);
  ethernetModule.write(rfid);
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
  char command[128] = { 0 };
  int len = readServerCommand(command, 127, SERVER_TIME_OUT);
  if (len) {
    rfidScanner.println("Server Command:");
    rfidScanner.println(command);
  }

  if (!strcmp_P(command, SERVER_OK)) {
    grantAccess(state);
  } else if (!strcmp_P(command, SERVER_FORBIDDEN)) {
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
  ethernetModule.begin(9600);
  rfidScanner.begin(9600);

  pinMode(GREEN_LED_PORT, OUTPUT);
  pinMode(RED_LED_PORT, OUTPUT);

  pinMode(MOTOR_PORT_1, OUTPUT);
  pinMode(MOTOR_PORT_2, OUTPUT);

  lcd.begin(16, 2);
}

void loop() {
  static State currentState;
  static char rfidBuff[11] = { 0 };
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
