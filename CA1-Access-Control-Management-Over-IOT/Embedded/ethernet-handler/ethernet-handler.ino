#include <EtherCard.h>
#include <AltSoftSerial.h>

AltSoftSerial rfidModule;
auto& logTerminal = Serial;

const int BAUD_RATE = 9600;
const int RFID_LENGHT = 10;

const char SERVER_AUTHENTICATION_PATH[] PROGMEM = "/authentication";
const char RFID_TEMPLATE[] = "{ \"rfid\": \"%s\" }";
const int RFID_MSG_LEN = 16;

const char SERVER_OK[] PROGMEM = "HTTP/1.1 200 OK";
const char SERVER_UNAUTHORIZED[] PROGMEM = "HTTP/1.1 401 Unauthorized";

const char OK_CALLBACK[] = "ok";
const char FORBIDDEN_CALLBACK[] = "forbidden";

const byte ip[] = { 192, 168, 2, 2 };
const byte gateway[] = { 192, 168, 2, 1 };
const byte mac[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };

const byte serverIp[] = { 192, 168, 2, 1 };
const char website[] PROGMEM = "127.0.0.1";

byte Ethernet::buffer[700];
Stash stash;

unsigned long timer;

struct ServerResponse {
  enum class Status {
    ok,
    forbidden,
    badRequest
  } status;
};

bool readRfid(char rfidBuffer[]) {
  if (!rfidModule.available())
    return false;

  rfidModule.readBytes(rfidBuffer, RFID_LENGHT);
  return true;
}

byte httpPostSend(const char* payload, const char* path, const char* website) {
  byte sd = stash.create();
  stash.print(payload);
  stash.save();

  Stash::prepare(
    PSTR(
      "POST $F HTTP/1.1"
      "\r\n"
      "Host: $F"
      "\r\n"
      "Accept: */*"
      "\r\n"
      "Content-Length: $D"
      "\r\n"
      "Content-Type: application/json"
      "\r\n" "\r\n"
      "$H"),
    path, website, stash.size(), sd);

  return ether.tcpSend();
}

byte sendToServer(const char rfid[]) {
  logTerminal.print("Sending rfid to server: ");
  logTerminal.println(rfid);

  char msg[RFID_MSG_LEN];
  sprintf(msg, RFID_TEMPLATE, rfid);

  logTerminal.print("Sending message to server: ");
  logTerminal.println(msg);

  return httpPostSend(msg, SERVER_AUTHENTICATION_PATH, website);
}

ServerResponse handleServerResponse(const char* message) {
  logTerminal.println("Received packet: ");
  logTerminal.println(message);

  if (!strncasecmp_P(message, SERVER_OK, strlen_P(SERVER_OK)))
    return { ServerResponse::Status::ok };
  if (!strncasecmp_P(message, SERVER_UNAUTHORIZED, strlen_P(SERVER_UNAUTHORIZED)))
    return { ServerResponse::Status::forbidden };

  return { ServerResponse::Status::badRequest };
}

void callback(const ServerResponse& serverResponse) {
  if (serverResponse.status == ServerResponse::Status::ok) {
    logTerminal.print("Calling back: ");
    logTerminal.println(OK_CALLBACK);

    rfidModule.print(OK_CALLBACK);
  }

  else if (serverResponse.status == ServerResponse::Status::forbidden) {
    logTerminal.print("Calling back: ");
    logTerminal.println(FORBIDDEN_CALLBACK);

    rfidModule.print(FORBIDDEN_CALLBACK);
  }

  else if (serverResponse.status == ServerResponse::Status::badRequest) {
    logTerminal.print("Bad Request");
  }
}

void setup() {
  rfidModule.begin(BAUD_RATE);
  logTerminal.begin(BAUD_RATE);

  ether.begin(sizeof Ethernet::buffer, mac, SS);
  ether.staticSetup(ip, gateway);

  ether.copyIp(ether.hisip, serverIp);
}

void loop() {
  char rfid[RFID_LENGHT] = { 0 };
  static byte session;

  ether.packetLoop(ether.packetReceive());
  if (readRfid(rfid)) {
    session = sendToServer(rfid);
  }
  const char* serverResponsePacket = ether.tcpReply(session);
  if (serverResponsePacket != nullptr) {
    ServerResponse serverResponse = handleServerResponse(serverResponsePacket);
    callback(serverResponse);
  }
}
